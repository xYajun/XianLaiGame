#include "YZSocketThread.h"
//#include <condition_variable>
#include "YZSocket.h"
#include "YZSocketMessage.h"
#include "YZSocketProtocolData.h"
#include "YZSocketEventDelegate.h"
#include "YZCommon/YZLog.h"
#include "YZCommon/YZCommonMarco.h"

namespace YZ
{
static const INT TCP_RECV_BUFFER_SIZE = 100*1024;

static const INT TCP_SEND_BUFFER_SIZE = 16400;

YZSocketThread::YZSocketThread(ISocketEventDelegate* delegate) 
	: _threadExit(false)
	, _connected(false)
	, _recvThread(nullptr)
	, _delegate(delegate)
{
	memset(_readBuffer, 0x0, sizeof(_readBuffer));
	_socket = new YZSocket();
	_socketDataCacheQueue = new YZSocketDataCacheQueue();	
	_socketMessageQueue = new YZSocketMessageQueue();
}

YZSocketThread::~YZSocketThread(void)
{
	_threadExit = true;
	_socket->close();
	_socket->shutdown();
	if (_recvThread->joinable())
	{
		_recvThread->join();
	}

	for (auto iter = _socketMessageQueue->begin(); iter != _socketMessageQueue->end();)
	{
		YZSocketMessage* message = *iter;
		YZSocketMessage::releaseMessage(message);
		iter = _socketMessageQueue->erase(iter);
	}
	_socketMessageQueue->clear();
	YZ_SAFE_DELETE(_socketMessageQueue);

	YZ_SAFE_DELETE(_socketDataCacheQueue);
	YZ_SAFE_DELETE(_recvThread);
	YZ_SAFE_DELETE(_socket);
}

bool YZSocketThread::openWithIp(const CHAR* ip, INT port)
{
	assert(ip != nullptr);
	if (!_connected)
	{
		_socket->setIP(ip);
		_socket->setPort(port);
		_recvThread = new std::thread(std::bind(&YZSocketThread::onSocketReadThread, this));
	}
	return true;
}

bool YZSocketThread::openWithHost(const CHAR* host, INT port)
{
	assert(host != nullptr);
	if (!_connected)
	{
		_socket->setHost(host);
		_socket->setPort(port);
		_recvThread = new std::thread(std::bind(&YZSocketThread::onSocketReadThread, this));
	}
	return true;
}

bool YZSocketThread::initSocket()
{
	bool ret = false;
	do
	{
		//_ips = YZSocket::getIpAddress("www.boss666.com");
		//ips = YZSocket::getIpAddress("localhost");

		// create socket
		if (!_socket->create())
		{
			break;
		}

		// set recv buffer
		_socket->setSoRcvbuf(TCP_RECV_BUFFER_SIZE);

		// set send buffer
		_socket->setSoSendbuf(TCP_SEND_BUFFER_SIZE);

		INT err = _socket->connect();

		if (TCP_CONNECT_OUTTIME == err)
		{
			YZSocketMessage* SocketMessage = YZSocketMessage::getMessage();
			SocketMessage->socketStatus = SocketStatus_OUTTIME;
			{
				std::lock_guard <std::mutex> autoLock(_dataMutex);
				_socketMessageQueue->push_back(SocketMessage);
			}
			_delegate->onReadComplete();
			break;
		}
        
		if (TCP_CONNECT_ERROR == err)
		{
			YZSocketMessage* SocketMessage = YZSocketMessage::getMessage();
			SocketMessage->socketStatus = SocketStatus_ERROR;
			{
				std::lock_guard <std::mutex> autoLock(_dataMutex);
				_socketMessageQueue->push_back(SocketMessage);
			}
			_delegate->onReadComplete();
			break;
		}
		_connected = true;
		_threadExit = false;
		ret = true;
	} while (0);

	return ret;
}

void YZSocketThread::clear()
{
	_connected = false;
	_threadExit = true;
	_socketDataCacheQueue->clear();
}

bool YZSocketThread::close()
{
	if (_connected)
	{
		_connected = false;
		_threadExit = true;	
		_socket->close();
		_socket->shutdown();
		if (_recvThread->joinable())
		{
			_recvThread->join();
		}
		return true;
	}
	return false;
}

INT YZSocketThread::send(char* object, INT objectSize)
{
	if (_connected)
	{
		return _socket->send(object, objectSize);
	}
	YZLOG_ERROR ("error: network not connected");
	return -1;
}

void YZSocketThread::onSocketReadThread()
{
	if (!initSocket())
	{
		return;
	}

	//std::this_thread::sleep_for(std::chrono::milliseconds(50));

	YZLOG_ERROR ("%s >> onSocketThread >> begin.", _tag.c_str());

	timeval timeOut, timeout_copy;
	timeOut.tv_sec = 0;
	timeOut.tv_usec = 16000;
		
	while (true)
	{
		if (_threadExit) break;

		timeout_copy = timeOut;
        
		INT nready = _socket->select(&timeout_copy);
		// outtime
		if (0 == nready)
		{
			continue;
		}

		// network error
		if (nready < 0)
		{
			YZLOG_ERROR("%s >> %s", _tag.c_str(), strerror(errno));

			YZSocketMessage* SocketMessage = YZSocketMessage::getMessage();
			SocketMessage->socketStatus = SocketStatus_DISCONNECT;
			{
				std::lock_guard <std::mutex> autoLock(_dataMutex);
				_socketMessageQueue->push_back(SocketMessage);
			}
			_delegate->onReadComplete();
			break;
		}
        
		INT recvSize = 0;
		recvSize = _socket->recv(_readBuffer, sizeof(_readBuffer));
        
		// server close
		if (0 == recvSize)	
		{
			YZLOG_ERROR("%s >> server close >> %s",  _tag.c_str(), strerror(errno));
			YZSocketMessage* SocketMessage = YZSocketMessage::getMessage();
			SocketMessage->socketStatus = SocketStatus_DISCONNECT;
			{
				std::lock_guard <std::mutex> autoLock(_dataMutex);
				_socketMessageQueue->push_back(SocketMessage);
			}
			_delegate->onReadComplete();
			break;
		}
        
		if (recvSize > 0)
        {
            onRead(_readBuffer, recvSize);
        } 
	}
	
	clear();
	
	YZLOG_ERROR ("%s >> onSocketThread >> end.", _tag.c_str());
}

void YZSocketThread::onRead(const CHAR* buffer, INT recvSize)
{
	// cache network data
	_socketDataCacheQueue->push(_readBuffer, recvSize);

	const UINT NetMessageHeadSize = sizeof(NetMessageHead);

	NetMessageHead* pMessageHead = nullptr;
	
	UINT messageSize = (UINT)_socketDataCacheQueue->size();
	if (messageSize >= NetMessageHeadSize)
	{
		do
		{
			pMessageHead = (NetMessageHead*) _socketDataCacheQueue->front();
			if (nullptr != pMessageHead && messageSize >= pMessageHead->uMessageSize)
			{
				//YZLOG_INFO("%s >> MainID = [%lu] AssistantID = [%lu] MessageSize = [%lu]", _tag.c_str(), pMessageHead->bMainID, pMessageHead->bAssistantID, pMessageHead->uMessageSize);

				CHAR* pData = _socketDataCacheQueue->front() + NetMessageHeadSize;
				YZSocketMessage* SocketMessage = YZSocketMessage::getMessage();
				SocketMessage->setMessage(pMessageHead, pData, pMessageHead->uMessageSize - NetMessageHeadSize);
				SocketMessage->socketStatus = SocketStatus_RECV;
				_socketDataCacheQueue->pop(pMessageHead->uMessageSize);

				// dispatch network data
				{
					std::lock_guard <std::mutex> autoLock(_dataMutex);
					_socketMessageQueue->push_back(SocketMessage);
					_delegate->onReadComplete();
				}
			}
			else
			{
				YZLOG_WARNING("%s >> not a complete data packet [messageSize = %lu, pMessageHead->uMessageSize = %lu]",
					_tag.c_str(), messageSize, pMessageHead->uMessageSize);
			}
			messageSize = (UINT)_socketDataCacheQueue->size();
			//YZLOG_INFO("%s >> messageSize ---> end = [%lu]", _tag.c_str(), messageSize);
		} while(messageSize >= pMessageHead->uMessageSize);
	}
}

void YZSocketThread::transform(std::function<void(YZSocketMessage* socketMessage)> func)
{
	int queueSize = 0;
	do 
	{	
		YZSocketMessage* socketMessage = nullptr;
		{
			std::lock_guard < std::mutex > autoLock(_dataMutex);
			queueSize = _socketMessageQueue->size();
			if (queueSize > 0)
			{
				socketMessage = _socketMessageQueue->front();
				_socketMessageQueue->pop_front();
			}
		}
		if (nullptr != socketMessage)
		{
			func(socketMessage);
			YZSocketMessage::releaseMessage(socketMessage);
		}
	} while (queueSize > 0);
	Director::getInstance()->getScheduler()->pauseTarget(this);
}

}
