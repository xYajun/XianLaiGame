#include "SaveBoxLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalUnits.h"
#include "YZNetExport.h"
#include "HallLayer.h"
#include "GamePrompt.h"


using namespace cocostudio;
using namespace cocos2d::ui;
USING_NS_CC;


SaveBoxLayer* SaveBoxLayer::createLayer(MoneyChangeNotify* delegate,bool isOpen)
{
	SaveBoxLayer *pRet = new SaveBoxLayer(); 
	if (pRet && pRet->init(isOpen)) 
	{ 
		pRet->setChangeDelegate(delegate);
		pRet->autorelease(); 
		return pRet; 
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}
}

bool SaveBoxLayer::init(bool isOpen)
{
	if ( !Layer::init() )
	{
		return false;
	}
	_isGetNick=false;
	if (isOpen)
	{
		openSaveBoxLayer();
		return true;
	}
	Size winSize = Director::getInstance()->getWinSize();
	auto uiSaveBoxLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("SaveBox/SaveBox_1.json");
	uiSaveBoxLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
	uiSaveBoxLayer->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	addChild(uiSaveBoxLayer);

	auto btn_ok = (Button*)(Helper::seekWidgetByName(uiSaveBoxLayer,"btn_ok"));
	btn_ok->addTouchEventListener(CC_CALLBACK_2(SaveBoxLayer::onFcBtnClicked,this));
	btn_ok->setTag(SAVEBOX_BTN_OK);
	auto btn_cancle = (Button*)(Helper::seekWidgetByName(uiSaveBoxLayer,"btn_cancle"));
	btn_cancle->addTouchEventListener(CC_CALLBACK_2(SaveBoxLayer::onFcBtnClicked,this));
	btn_cancle->setTag(SAVEBOX_BTN_CANCLE);
	auto btn_close = (Button*)(Helper::seekWidgetByName(uiSaveBoxLayer,"btn_close"));
	btn_close->addTouchEventListener(CC_CALLBACK_2(SaveBoxLayer::onFcBtnClicked,this));
	btn_close->setTag(SAVEBOX_BTN_EXIT);

	tf_pwd=(TextField*)(Helper::seekWidgetByName(uiSaveBoxLayer,"tf_pwd"));

	return true;
}

void SaveBoxLayer::openSaveBoxLayer()
{
	Size winSize = Director::getInstance()->getWinSize();
	//panel
	auto uiSaveBoxLayer2 = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("SaveBox/SaveBox_2.json");
	uiSaveBoxLayer2->setAnchorPoint(Vec2(0.5f, 0.5f));
	uiSaveBoxLayer2->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	addChild(uiSaveBoxLayer2);
	Panel_bank= (Layout*)Helper::seekWidgetByName(uiSaveBoxLayer2,"Panel_bank");
	Panel_bank->setTag(SAVEBOX_PANEL_BANK);
	Panel_bank->setVisible(true);
	Panel_give = (Layout*)Helper::seekWidgetByName(uiSaveBoxLayer2,"Panel_give");
	Panel_give->setTag(SAVEBOX_PANEL_GIVE);
	Panel_give->setVisible(false);
	Panel_modifyPwd = (Layout*)Helper::seekWidgetByName(uiSaveBoxLayer2,"Panel_modifyPwd");
	Panel_modifyPwd->setTag(SAVEBOX_PANEL_PWD);
	Panel_modifyPwd->setVisible(false);


	// -- btn
	btn_bank = (Button*)(Helper::seekWidgetByName(uiSaveBoxLayer2,"btn_bank"));
	btn_bank->addTouchEventListener(CC_CALLBACK_2(SaveBoxLayer::onBtnClicked,this));
	btn_bank->setTag(SAVEBOX_BTN_BANK);
	btn_bank->setBright(false);
	btn_bank->setEnabled(false);
	btn_bank->setTouchEnabled(false); 


	btn_give = (Button*)(Helper::seekWidgetByName(uiSaveBoxLayer2,"btn_give"));
	btn_give->addTouchEventListener(CC_CALLBACK_2(SaveBoxLayer::onBtnClicked,this));
	btn_give->setTag(SAVEBOX_BTN_GIVE);

	btn_modifyPwd = (Button*)(Helper::seekWidgetByName(uiSaveBoxLayer2,"btn_modifyPwd"));
	btn_modifyPwd->addTouchEventListener(CC_CALLBACK_2(SaveBoxLayer::onBtnClicked,this));
	btn_modifyPwd->setTag(SAVEBOX_BTN_PWD);


	Button * btn_close = (Button*)(Helper::seekWidgetByName(uiSaveBoxLayer2,"btn_close"));
	btn_close->setPressedActionEnabled(true);
	btn_close->addTouchEventListener(CC_CALLBACK_2(SaveBoxLayer::onFcBtnClicked,this));
	btn_close->setTag(SAVEBOX_BTN_EXIT);

	initPanelBank();
	initPanelGive();
	initPanelPwd();
}

void SaveBoxLayer::initPanelBank()
{ 
	label_bankMoney = (Text*)(Helper::seekWidgetByName(Panel_bank,"Label_bankMoney"));
	label_walltMoney = (Text*)(Helper::seekWidgetByName(Panel_bank,"Label_walltMoney"));
	tf_inputMoney = (TextField*)(Helper::seekWidgetByName(Panel_bank,"tf_inputMoney"));
	tf_inputMoney->setString("0");
	
	auto btn_cqType1 = (Button*)(Helper::seekWidgetByName(Panel_bank,"btn_cqType1"));
	btn_cqType1->setTag(SAVEBOX_BTN_CQTYPE_1);
	btn_cqType1->addTouchEventListener(CC_CALLBACK_2(SaveBoxLayer::onFcBtnClicked,this));
	auto btn_cqType2 = (Button*)(Helper::seekWidgetByName(Panel_bank,"btn_cqType2"));
	btn_cqType2->setTag(SAVEBOX_BTN_CQTYPE_2);
	btn_cqType2->addTouchEventListener(CC_CALLBACK_2(SaveBoxLayer::onFcBtnClicked,this));
	auto btn_cqType3 = (Button*)(Helper::seekWidgetByName(Panel_bank,"btn_cqType3"));
	btn_cqType3->setTag(SAVEBOX_BTN_CQTYPE_3);
	btn_cqType3->addTouchEventListener(CC_CALLBACK_2(SaveBoxLayer::onFcBtnClicked,this));
	auto btn_cqType4 = (Button*)(Helper::seekWidgetByName(Panel_bank,"btn_cqType4"));
	btn_cqType4->setTag(SAVEBOX_BTN_CQTYPE_4);
	btn_cqType4->addTouchEventListener(CC_CALLBACK_2(SaveBoxLayer::onFcBtnClicked,this));
	auto btn_allTake = (Button*)(Helper::seekWidgetByName(Panel_bank,"btn_allTake"));
	btn_allTake->setTag(SAVEBOX_BTN_ALLTAKE);
	btn_allTake->addTouchEventListener(CC_CALLBACK_2(SaveBoxLayer::onFcBtnClicked,this));
	auto btn_allStore = (Button*)(Helper::seekWidgetByName(Panel_bank,"btn_allStore"));
	btn_allStore->setTag(SAVEBOX_BTN_ALLSTORE);
	btn_allStore->addTouchEventListener(CC_CALLBACK_2(SaveBoxLayer::onFcBtnClicked,this));
	auto btn_store = (Button*)(Helper::seekWidgetByName(Panel_bank,"btn_store"));
	btn_store->setTag(SAVEBOX_BTN_STORE);
	btn_store->addTouchEventListener(CC_CALLBACK_2(SaveBoxLayer::onFcBtnClicked,this));
	auto btn_take = (Button*)(Helper::seekWidgetByName(Panel_bank,"btn_take"));
	btn_take->setTag(SAVEBOX_BTN_TAKE);
	btn_take->addTouchEventListener(CC_CALLBACK_2(SaveBoxLayer::onFcBtnClicked,this));
}
void SaveBoxLayer::initPanelGive()
{
	label_nick = (Text*)(Helper::seekWidgetByName(Panel_give,"Label_nick"));
	tf_id = (TextField*)(Helper::seekWidgetByName(Panel_give,"tf_id"));
	tf_giveMoney = (TextField*)(Helper::seekWidgetByName(Panel_give,"tf_giveMoney"));
	tf_giveMoney->setString("0");
	auto btn_gType1 = (Button*)(Helper::seekWidgetByName(Panel_give,"btn_gType1"));
	btn_gType1->setTag(SAVEBOX_BTN_GTYPE_1);
	btn_gType1->addTouchEventListener(CC_CALLBACK_2(SaveBoxLayer::onFcBtnClicked,this));
	auto btn_gType2 = (Button*)(Helper::seekWidgetByName(Panel_give,"btn_gType2"));
	btn_gType2->setTag(SAVEBOX_BTN_GTYPE_2);
	btn_gType2->addTouchEventListener(CC_CALLBACK_2(SaveBoxLayer::onFcBtnClicked,this));
	auto btn_gType3 = (Button*)(Helper::seekWidgetByName(Panel_give,"btn_gType3"));
	btn_gType3->setTag(SAVEBOX_BTN_GTYPE_3);
	btn_gType3->addTouchEventListener(CC_CALLBACK_2(SaveBoxLayer::onFcBtnClicked,this));
	auto btn_gType4 = (Button*)(Helper::seekWidgetByName(Panel_give,"btn_gType4"));
	btn_gType4->setTag(SAVEBOX_BTN_GTYPE_4);
	btn_gType4->addTouchEventListener(CC_CALLBACK_2(SaveBoxLayer::onFcBtnClicked,this));
	auto btn_getNick = (Button*)(Helper::seekWidgetByName(Panel_give,"btn_getNick"));
	btn_getNick->setTag(SAVEBOX_BTN_GETNICK);
	btn_getNick->addTouchEventListener(CC_CALLBACK_2(SaveBoxLayer::onFcBtnClicked,this));
	auto btn_giveMoney = (Button*)(Helper::seekWidgetByName(Panel_give,"btn_giveMoney"));
	btn_giveMoney->setTag(SAVEBOX_BTN_GIVEMONEY);
	btn_giveMoney->addTouchEventListener(CC_CALLBACK_2(SaveBoxLayer::onFcBtnClicked,this));
	
}
void SaveBoxLayer::initPanelPwd()
{
	tf_oldPwd = (TextField*)(Helper::seekWidgetByName(Panel_modifyPwd,"tf_oldPwd"));
	tf_newPwd = (TextField*)(Helper::seekWidgetByName(Panel_modifyPwd,"tf_newPwd"));
	tf_confirmPwd = (TextField*)(Helper::seekWidgetByName(Panel_modifyPwd,"tf_confirmPwd"));
	auto btn_modifyPwd = (Button*)(Helper::seekWidgetByName(Panel_modifyPwd,"btn_modifyPwd"));
	btn_modifyPwd->setTag(SAVEBOX_BTN_MODIFY);
	btn_modifyPwd->addTouchEventListener(CC_CALLBACK_2(SaveBoxLayer::onFcBtnClicked,this));
}


void SaveBoxLayer::onBtnClicked(Ref * pSender,Widget::TouchEventType type)
{
	Button *btn = (Button*)pSender;
	if(type==Widget::TouchEventType::ENDED && btn)
	{
		YZAudioEngine::getInstance()->playEffect(GAME_SOUND_CLICK);
		log("tag = %d", btn->getTag());
		btn->setBright(false);
		btn->setEnabled(false);
	    btn->setTouchEnabled(false); 

		switch (btn->getTag())
		{
		case SAVEBOX_BTN_BANK:
			{
				Panel_bank->setVisible(true);
				Panel_give->setVisible(false);
				Panel_modifyPwd->setVisible(false);
				btn_give->setBright(true);
				btn_give->setEnabled(true);
				btn_give->setTouchEnabled(true);
				btn_modifyPwd->setBright(true);
				btn_modifyPwd->setEnabled(true);
				btn_modifyPwd->setTouchEnabled(true);
			}
			break;
		case SAVEBOX_BTN_GIVE:
			{
				Panel_bank->setVisible(false);
				Panel_give->setVisible(true);
				Panel_modifyPwd->setVisible(false);
				btn_bank->setBright(true);
				btn_bank->setEnabled(true);
				btn_bank->setTouchEnabled(true);
				btn_modifyPwd->setBright(true);
				btn_modifyPwd->setEnabled(true);
				btn_modifyPwd->setTouchEnabled(true);
			}
			break;
		case SAVEBOX_BTN_PWD:
			{
				Panel_bank->setVisible(false);
				Panel_give->setVisible(false);
				Panel_modifyPwd->setVisible(true);
				btn_bank->setBright(true);
				btn_bank->setEnabled(true);
				btn_bank->setTouchEnabled(true);
				btn_give->setBright(true);
				btn_give->setEnabled(true);
				btn_give->setTouchEnabled(true);
			}
			break;
		default:
			break;
		}
	}
}
void SaveBoxLayer::onFcBtnClicked( Ref * pSender,Widget::TouchEventType type )
{
	Button *btn = (Button*)pSender;
	if(type==Widget::TouchEventType::ENDED && btn)
	{
		YZAudioEngine::getInstance()->playEffect(GAME_SOUND_CLICK);
		switch (btn->getTag())
		{
		case SAVEBOX_BTN_OK:
			{
				std::string str_pwd = tf_pwd->getStringValue();
				if (str_pwd.length()==0)
				{
					GamePromptLayer::create()->showPrompt(GBKToUtf8("密码不能为空。")); break;
				}
				if (str_pwd.length()<6)
				{
					GamePromptLayer::create()->showPrompt(GBKToUtf8("密码长度至少为6位。")); break;
				}
				//openSaveBoxLayer();
				//PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_OPEN, nullptr, 0, YZ_SOCKET_CALLBACK(SaveBoxLayer::openSaveBoxEventBack, this));
			    auto userMD5Pwd=MD5_CTX::MD5String(str_pwd);
				TMSG_GP_BankOpenReq user_openSaveBox;
				strcpy(user_openSaveBox.szMD5Pass, userMD5Pwd.c_str());
				PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_OPEN	, &user_openSaveBox, sizeof(user_openSaveBox));
				PlatformLogic()->addEventSelector(MDM_GP_BANK, ASS_GP_BANK_OPEN, YZ_SOCKET_CALLBACK(SaveBoxLayer::openSaveBoxEventBack, this));
			}
			break;
		case SAVEBOX_BTN_CANCLE:
			{
				removeFromParentAndCleanup(true);
			}
			break;
		case SAVEBOX_BTN_EXIT:
			{
				removeFromParentAndCleanup(true);
			}
			break;
		case SAVEBOX_BTN_CQTYPE_1:
			{
				char name[60];
				int mon= atoi(tf_inputMoney->getStringValue().c_str());
				mon+=1000000;
				sprintf(name,"%d",mon);
				tf_inputMoney->setString(name);
			}
			break;
		case SAVEBOX_BTN_CQTYPE_2:
			{
				char name[60];
				int mon= atoi(tf_inputMoney->getStringValue().c_str());
				mon+=5000000;
				sprintf(name,"%d",mon);
				tf_inputMoney->setString(name);
			}
			break;
		case SAVEBOX_BTN_CQTYPE_3:
			{
				char name[60];
				int mon= atoi(tf_inputMoney->getStringValue().c_str());
				mon+=10000000;
				sprintf(name,"%d",mon);
				tf_inputMoney->setString(name);
			}
			break;
		case SAVEBOX_BTN_CQTYPE_4:
			{
				char name[60];
				int mon= atoi(tf_inputMoney->getStringValue().c_str());
				mon+=50000000;
				sprintf(name,"%d",mon);
				tf_inputMoney->setString(name);
			}
			break;
		case SAVEBOX_BTN_ALLTAKE:
			{
				char str[60];
				sprintf(str,"%lld",PlatformLogic()->loginResult.i64Money);
				tf_inputMoney->setString(str);
			}
			break;
		case SAVEBOX_BTN_ALLSTORE:
			{
				tf_inputMoney->setString("0");
			}
			break;
		case SAVEBOX_BTN_TAKE:
			{
				std::string str_pwd = tf_pwd->getStringValue();
				int mon= atoi(tf_inputMoney->getStringValue().c_str());
				if (mon>_bankMoney)
				{
					GamePromptLayer::create()->showPrompt(GBKToUtf8("银行余额不足。"));
					break;
				}
				TMSG_GP_BankCheck BankCheck = { 0 };
				BankCheck.operate_type = 1;
				BankCheck.user_id = PlatformLogic()->loginResult.dwUserID;
				BankCheck.game_id = 0;
				BankCheck.money = mon;
				strcpy(BankCheck.szMD5Pass, MD5_CTX::MD5String(str_pwd).c_str());
				PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_CHECK, &BankCheck, sizeof(BankCheck),
					YZ_SOCKET_CALLBACK(SaveBoxLayer::bankerEventSelector, this));
			}
			break;
		case SAVEBOX_BTN_STORE:
			{
				int mon= atoi(tf_inputMoney->getStringValue().c_str());
				if (mon<5000)
				{
					GamePromptLayer::create()->showPrompt(GBKToUtf8("存入保险箱的金额至少为5000金币。"));
					break;
				}
				if (PlatformLogic()->loginResult.i64Money < mon)
				{
					GamePromptLayer::create()->showPrompt(GBKToUtf8("账号余额不足。"));
					break;
				}
				TMSG_GP_BankCheck BankCheck = { 0 };
				BankCheck.operate_type = 2;
				BankCheck.user_id = PlatformLogic()->loginResult.dwUserID;
				BankCheck.game_id = 0;
				BankCheck.money =  mon;
				strcpy(BankCheck.szMD5Pass, MD5_CTX::MD5String("1111").c_str());
				PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_CHECK, &BankCheck, sizeof(BankCheck),
					YZ_SOCKET_CALLBACK(SaveBoxLayer::bankerEventSelector, this));
			}
			break;
		case SAVEBOX_BTN_GTYPE_1:
			{
				char name[60];
				int mon= atoi(tf_giveMoney->getStringValue().c_str());
				mon+=1000000;
				sprintf(name,"%d",mon);
				tf_giveMoney->setString(name);
			}
			break;
		case SAVEBOX_BTN_GTYPE_2:
			{
				char name[60];
				int mon= atoi(tf_giveMoney->getStringValue().c_str());
				mon+=5000000;
				sprintf(name,"%d",mon);
				tf_giveMoney->setString(name);
			}
			break;
		case SAVEBOX_BTN_GTYPE_3:
			{
				char name[60];
				int mon= atoi(tf_giveMoney->getStringValue().c_str());
				mon+=10000000;
				sprintf(name,"%d",mon);
				tf_giveMoney->setString(name);
			}
			break;
		case SAVEBOX_BTN_GTYPE_4:
			{
				char name[60];
				int mon= atoi(tf_giveMoney->getStringValue().c_str());
				mon+=50000000;
				sprintf(name,"%d",mon);
				tf_giveMoney->setString(name);
			}
			break;
		case SAVEBOX_BTN_GETNICK:
			{
				std::string str_id = tf_id->getStringValue();
				if (str_id.length()==0)
				{
					GamePromptLayer::create()->showPrompt(GBKToUtf8("玩家ID不能为空。")); break;
				}
				int pId= atoi(tf_id->getStringValue().c_str());
				MSG_GP_QueryUserInfoByUid requstUserInfo_byUid;
				memset(&requstUserInfo_byUid,0,sizeof(MSG_GP_QueryUserInfoByUid));
				requstUserInfo_byUid.dwUserID =  pId;	
				PlatformLogic()->sendData(MDM_GP_USERINFO, ASS_GP_USERINFO_QUERYINFO_BYUID, &requstUserInfo_byUid, sizeof(requstUserInfo_byUid), YZ_SOCKET_CALLBACK(SaveBoxLayer::showUserNickEventSelector, this));
			}
			break;
		case SAVEBOX_BTN_GIVEMONEY:
			{
				if (!_isGetNick)
				{
					GamePromptLayer::create()->showPrompt(GBKToUtf8("玩家不存在。")); break;
				}
				std::string str_pwd = tf_pwd->getStringValue();
				TMSG_GP_BankTransfer BankTransfer;
				BankTransfer.UserID= PlatformLogic()->loginResult.dwUserID;
				BankTransfer.destUserID=atoi(tf_id->getStringValue().c_str());
				BankTransfer.i64Money=atoi(tf_giveMoney->getStringValue().c_str());
				BankTransfer.bUseDestID=true;
				strcpy(BankTransfer.szMD5Pass, MD5_CTX::MD5String(str_pwd).c_str());
				PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_TRANSFER	, &BankTransfer, sizeof(BankTransfer),
					YZ_SOCKET_CALLBACK(SaveBoxLayer::bankerForwardSelector, this));
			}
			break;
		case SAVEBOX_BTN_MODIFY:
			{
				std::string str_oldpw = tf_oldPwd->getStringValue();
				if (str_oldpw.empty())
				{
					GamePromptLayer::create()->showPrompt(GBKToUtf8("原始密码不能为空。")); break;
				}
				/*if (tf_oldPwd->getStringLength() > tf_oldPwd->getMaxLength())
				{
					GamePromptLayer::create()->showPrompt(GBKToUtf8("密码长度超过最大限制。")); break;
				}*/

				std::string str_newpw = tf_newPwd->getStringValue();
				if (str_newpw.empty())
				{
					GamePromptLayer::create()->showPrompt(GBKToUtf8("新密码不能为空。")); break;
				}
				/*if (tf_newPwd->getStringLength() > tf_newPwd->getMaxLength())
				{
					GamePromptLayer::create()->showPrompt(GBKToUtf8("新密码长度超过最大限制。")); break;
				}
*/
				std::string str_confirmpw = tf_confirmPwd->getStringValue();
				if (str_confirmpw.empty())
				{
					GamePromptLayer::create()->showPrompt(GBKToUtf8("确认密码不能为空。")); break;
				}
				/*if (tf_confirmPwd->getStringLength() > tf_confirmPwd->getMaxLength())
				{
					GamePromptLayer::create()->showPrompt(GBKToUtf8("确认密码长度超过最大限制。")); break;
				}*/

				if (strcmp(str_oldpw.c_str(),LoadStringFromXML("save_password").c_str()) != 0)
				{
					GamePromptLayer::create()->showPrompt(GBKToUtf8("原始密码错误。")); break;
				}
				if (strcmp(str_newpw.c_str(),str_confirmpw.c_str()) != 0)
				{
					GamePromptLayer::create()->showPrompt(GBKToUtf8("密码与确认密码不同。")); break;
				}

				auto userMD5Pwd = MD5_CTX::MD5String(str_newpw);
				auto userMD5OldPwd = MD5_CTX::MD5String(str_oldpw);

				TMSG_GP_BankChPwd ChPassword;
				ChPassword.user_id = PlatformLogic()->loginResult.dwUserID;
				strcpy(ChPassword.MD5Pass_new, userMD5Pwd.c_str());
				strcpy(ChPassword.MD5Pass_old, userMD5OldPwd.c_str());
				PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_CHPWD, &ChPassword, sizeof(ChPassword));
				PlatformLogic()->addEventSelector(MDM_GP_BANK, ASS_GP_BANK_CHPWD, YZ_SOCKET_CALLBACK(SaveBoxLayer::modifyPasswordSelector, this));
			}
			break;
		default:
			break;
		}
	}
}


void SaveBoxLayer::openSaveBox(Node* parent, Vec2 vec, int zorder, int tag)
{
	CCAssert(nullptr != parent, "parent is nullptr");

	Size winSize = Director::getInstance()->getWinSize();
	this->setPosition(vec);
	parent->addChild(this, zorder, tag);
	parent->runAction(MoveBy::create(0.3f, Vec2(-winSize.width, 0)));
}

void SaveBoxLayer::setChangeDelegate(MoneyChangeNotify* delegate)
{
	_delegate = delegate;
}


void SaveBoxLayer::refreshGold(int change)
{

}


bool SaveBoxLayer::isDigitStr( const char* cstr )
{
	if (NULL == cstr || cstr[0] == 0)  
	{  
		return false;  
	}  

	int len = strlen(cstr);  
	int pos = 0;  
	if (cstr[0] == '-' || cstr[0] == '+')  
	{  
		if (len <= 1)   
		{  
			return false;  
		}  

		pos++;          
	}  

	while (pos < len)  
	{  
		if (cstr[pos] < '0' || cstr[pos] > '9')  
		{  
			return false;  
		}  
		pos++;  
	}  

	return true;  
}

bool SaveBoxLayer::openSaveBoxEventBack( YZSocketMessage* socketMessage )
{
	
	if (HC_GP_BANK_OPEN_SUC == socketMessage->messageHead.bHandleCode)	// 服务端已接受
	{
		openSaveBoxLayer();
		auto halllayer = (HallLayer*)this->getParent();
		halllayer->setOpenSave(true);
		
		TMSG_GP_BankInfo* bankInfor = (TMSG_GP_BankInfo*)socketMessage->object;
		auto _wallet_money = dynamic_cast<Text*>(Helper::seekWidgetByName(Panel_bank, "Label_walltMoney"));
		auto _bank_money = dynamic_cast<Text*>(Helper::seekWidgetByName(Panel_bank, "Label_bankMoney"));
		TMSG_GP_BankInfo * bankInfo = reinterpret_cast<TMSG_GP_BankInfo*>(socketMessage->object);
				
		_bankMoney = bankInfo->i64Bank;
		_walletMoney = PlatformLogic()->loginResult.i64Money;
		char buffer[128];
		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "%lld", _walletMoney);
		if (nullptr != _wallet_money)
			_wallet_money->setString(buffer);

		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "%lld", _bankMoney);
		if (nullptr != _bank_money)
			_bank_money->setString(GBKToUtf8(buffer));
	}
	else if (HC_GP_BANK_OPEN_ERR == socketMessage->messageHead.bHandleCode)	// 服务端未能接受
	{
		GamePromptLayer::create()->showPrompt(GBKToUtf8("打开钱框错误。"));
	}
	else if(HC_GP_BANK_OPEN_ERR_PSW == socketMessage->messageHead.bHandleCode)	// 服务端未能接受
	{
		GamePromptLayer::create()->showPrompt(GBKToUtf8("打开钱框错误。"));
	}
	return true;
}

bool SaveBoxLayer::bankerEventSelector( YZSocketMessage* socketMessage )
{
	UINT bAssistantID = socketMessage->messageHead.bAssistantID;
	UINT bHandleCode = socketMessage->messageHead.bHandleCode;

	if (ASS_GP_BANK_CHECK == bAssistantID)
	{
		switch (bHandleCode)
		{
		case HC_GP_BANK_CHECK_SUC://					0			//操作成功;
			{
				// 广播过来;
				CCAssert(sizeof (TMSG_GP_BankCheck) == socketMessage->objectSize, "TMSG_GP_BankCheck size is error.");
				if (sizeof (TMSG_GP_BankCheck) != socketMessage->objectSize) return true;

				TMSG_GP_BankCheck *BankCheck = reinterpret_cast<TMSG_GP_BankCheck*>(socketMessage->object);

				std::string promptInfo;
				// 取钱;
				if (1 == BankCheck->operate_type)
				{
					_bankMoney -= BankCheck->money;
					_walletMoney += BankCheck->money;
					PlatformLogic()->loginResult.i64Bank = _bankMoney;
					PlatformLogic()->loginResult.i64Money = _walletMoney;
	
					auto halllayer = (HallLayer*)this->getParent();
					halllayer->walletChanged(PlatformLogic()->loginResult.i64Money);
				}

				// 存钱;
				if (2 == BankCheck->operate_type)
				{
					_bankMoney += BankCheck->money;
					_walletMoney -= BankCheck->money;
					PlatformLogic()->loginResult.i64Bank = _bankMoney;
					PlatformLogic()->loginResult.i64Money = _walletMoney;
				
					auto halllayer = (HallLayer*)this->getParent();
					halllayer->walletChanged(PlatformLogic()->loginResult.i64Money);
				}


				auto _wallet_money = dynamic_cast<Text*>(Helper::seekWidgetByName(Panel_bank, "Label_walltMoney"));
				auto _bank_money = dynamic_cast<Text*>(Helper::seekWidgetByName(Panel_bank, "Label_bankMoney"));
				TMSG_GP_BankInfo * bankInfo = reinterpret_cast<TMSG_GP_BankInfo*>(socketMessage->object);
				char buffer[128];
				memset(buffer, 0, sizeof(buffer));
				sprintf(buffer, "%lld", _walletMoney);
				if (nullptr != _wallet_money)
					_wallet_money->setString(buffer);

				memset(buffer, 0, sizeof(buffer));
				sprintf(buffer, "%lld", _bankMoney);
				if (nullptr != _bank_money)
					_bank_money->setString(GBKToUtf8(buffer));
			}
			break;
		case HC_GP_BANK_CHECK_NO_USER://				1			//没有找到用户;
			GamePromptLayer::create()->showPrompt(GBKToUtf8("没有找到用户。"));
			break;

		case HC_GP_BANK_CHECK_ERR_PWD://				2			//错误密码;
			GamePromptLayer::create()->showPrompt(GBKToUtf8("错误密码。"));
			break;

		case HC_GP_BANK_CHECK_NOT_MONEY://				4			//余额不足;
			GamePromptLayer::create()->showPrompt(GBKToUtf8("余额不足。"));
			break;
		default:
			break;
		}
	}
	return true;
}

bool SaveBoxLayer::modifyPasswordSelector(YZSocketMessage* socketMessage)
{
	if (HC_GP_BANK_CHPWD_SUC == socketMessage->messageHead.bHandleCode)	// 服务端已接受
	{
		auto prompt = GamePromptLayer::create();
		prompt->showPrompt(GBKToUtf8("密码修改成功。"));
		prompt->setCallBack([&]()
		{
			// 修改成功关闭密码修改界面
			removeFromParentAndCleanup(true);
		});

	}
	else if (HC_GP_BANK_CHPWD_ERR_PWD == socketMessage->messageHead.bHandleCode)	// 服务端未能接受
	{
		GamePromptLayer::create()->showPrompt(GBKToUtf8("密码修改失败。"));
	}
	
	return true;
}

bool SaveBoxLayer::showUserNickEventSelector(YZSocketMessage* socketMessage)
{
	MSG_GP_QueryUserInfoByUidResult* data = (MSG_GP_QueryUserInfoByUidResult*)socketMessage->object;
	if(data->dwUserID==0)
	{
		_isGetNick=false;
		label_nick->setString(GBKToUtf8("用户昵称不存在"));
	}
	else
	{
		_isGetNick=true;
		label_nick->setString(GBKToUtf8(data->szNickName));
	}
	
	return true;
}

bool SaveBoxLayer::bankerForwardSelector( YZSocketMessage* socketMessage )
{
	UINT bAssistantID = socketMessage->messageHead.bAssistantID;
	UINT bHandleCode = socketMessage->messageHead.bHandleCode;
	if (ASS_GP_BANK_TRANSFER == bAssistantID)
	{
		auto prompt = GamePromptLayer::create();
		switch (bHandleCode)
		{
		case HC_GP_BANK_TRANSFER_ERROR:
			{
				prompt->showPrompt(GBKToUtf8("转帐错误。"));
			}
			break;
		case HC_GP_BANK_TRANSFER_SUC:
			{
				TMSG_GP_BankTransfer * transferInfo = reinterpret_cast<TMSG_GP_BankTransfer*>(socketMessage->object);
				prompt->showPrompt(GBKToUtf8("转帐成功。"));
			}
			break;
		case HC_GP_BANK_TRANSFER_NO_DEST:
			{
				prompt->showPrompt(GBKToUtf8("转帐目标不存在。"));
			}
			break;
		case HC_GP_BANK_TRANSFER_PASSWORD_ERROR:
			{
				prompt->showPrompt(GBKToUtf8("转帐密码错误。"));
			}
			break;
		case HC_GP_BANK_TRANSFER_TOTAL_LESS:
			{
				prompt->showPrompt(GBKToUtf8("银行总额太小，不够资格。"));
			}
			break;
		case HC_GP_BANK_TRANSFER_TOO_LESS:
			{
				prompt->showPrompt(GBKToUtf8("单笔转帐数目太少。"));
			}
			break;
		case HC_GP_BANK_TRANSFER_MULTIPLE:
			{
				prompt->showPrompt(GBKToUtf8("单笔转帐数目必须是某数的倍数。"));
			}
			break;
		case HC_GP_BANK_TRANSFER_NOT_ENOUGH:
			{
				prompt->showPrompt(GBKToUtf8("银行金额不够本次转帐。"));
			}
			break;
		case HC_GP_BANK_TRANSFER_TOO_MANY_TIME:
			{
				prompt->showPrompt(GBKToUtf8("当天转账的次数太多了。"));
			}
			break;
		default:
			break;
		}
	}
	return true;
}


