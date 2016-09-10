#include "YZSocketLogic.h"
#include "YZSocketThread.h"
#include "YZSocketMessageDelegate.h"
#include "YZSocketProtocolData.h"
#include "YZNetProtocol/YZProtocolExport.h"
#include "YZCommon/YZLog.h"
#include "YZCommon/YZCommonMarco.h"

namespace YZ
{
	YZSocketLogic::YZSocketLogic(ISocketMessageDelegate* delegate, const std::string& tag) : _delegate(delegate)
	{
		_socketThread = new YZSocketThread(this);
		_socketThread->setTag(tag);
		_selectorQueue = new YZSocketMessageSelectorHandler();
		auto scheduler = Director::getInstance()->getScheduler();
		scheduler->schedule(schedule_selector(YZSocketLogic::socketDataDispatch), this, 0.0f, true);
	}

	YZSocketLogic::~YZSocketLogic(void)
	{
		auto scheduler = Director::getInstance()->getScheduler();
		scheduler->unschedule(schedule_selector(YZSocketLogic::socketDataDispatch), this);

		YZ_SAFE_DELETE(_selectorQueue);

		YZ_SAFE_DELETE(_socketThread);
	}

	bool YZSocketLogic::init() 
	{
		return true;
	}

	void YZSocketLogic::resetData()
	{
		_selectorQueue->clear();
	}

	bool YZSocketLogic::openWithIp(const CHAR* ip, INT port)
	{
		return _socketThread->openWithIp(ip, port);
	}

	bool YZSocketLogic::openWithHost(const CHAR* host, INT port)
	{
		return _socketThread->openWithHost(host, port);
	}

	bool YZSocketLogic::close()
	{		
		resetData();
		return _socketThread->close();
	}

	bool YZSocketLogic::connected() const
	{ 
		return _socketThread->connected(); 
	}

	INT YZSocketLogic::send(UINT MainID, UINT AssistantID, UINT CheckCode, void* object, INT objectSize)
	{	
		YZSocketProtocolData protocolData;
		protocolData.createPackage(MainID, AssistantID, CheckCode, object, objectSize);
		if (MainID==103&&(AssistantID==8))
		{
			for (int i = 0; i < objectSize; i++)
			{
				YZLOG("--------------------------------%02x",protocolData.getPackage()[i]);
			}
		}
		return _socketThread->send(protocolData.getPackage(), protocolData.getPackageSize());
	}

	void YZSocketLogic::addEventSelector(UINT MainID, UINT AssistantID, SEL_SocketMessage selector)
	{
		CCAssert(nullptr != selector, "selector is nullptr");
		if (nullptr != selector)
		{
			CHAR messageKey[16] = {0};
			sprintf(messageKey, SELECTER_KEY_FORMAT, MainID, AssistantID);
			_selectorQueue->addSelector(messageKey, selector);
		}
	}

	void YZSocketLogic::removeEventSelector(UINT MainID, UINT AssistantID)
	{
		CHAR messageKey[16] = {0};
		sprintf(messageKey, SELECTER_KEY_FORMAT, MainID, AssistantID);
		_selectorQueue->removeSelector(messageKey);
	}

	void YZSocketLogic::heartBeat(UINT MainID, UINT AssistantID, UINT CheckCode)
	{
		YZSocketProtocolData protocolData;
		protocolData.createPackage(MainID, AssistantID, CheckCode);
		int ret = _socketThread->send(protocolData.getPackage(), protocolData.getPackageSize());
		if (ret >= 0)
		{	
			//YZLOG_WARNING("%s >> heartBeat", _socketThread->getTag().c_str());
		}
	}

	void YZSocketLogic::onReadComplete()
	{
		Director::getInstance()->getScheduler()->resumeTarget(this);
	}

	void YZSocketLogic::socketDataDispatch(float dt)
	{
		_socketThread->transform([this](YZSocketMessage* socketMessage)
		{
			assert(nullptr != _delegate);
			switch (socketMessage->socketStatus)
			{
			case SocketStatus_UNKNOW:
				break;
			case SocketStatus_CONNECT:
				break;
			case SocketStatus_OUTTIME:
				_delegate->onConnected(false, SocketStatus_OUTTIME);
				break;
			case SocketStatus_STCCESS:
				{

				}
				break;
			case SocketStatus_RECV:
				{
					bool ret = _selectorQueue->executeSelector(socketMessage->strKey, socketMessage);
					if (!ret)
					{
						_delegate->onSocketMessage(socketMessage);
					}
				}
				break;
			case SocketStatus_DISCONNECT:
				_selectorQueue->clear();
				_delegate->onDisConnect();
				break;
			case SocketStatus_ERROR:
				_selectorQueue->clear();
				_delegate->onConnected(false, SocketStatus_ERROR);
				break;
			default:
				break;
			}
		});
		Director::getInstance()->getScheduler()->pauseTarget(this);
	}
}
