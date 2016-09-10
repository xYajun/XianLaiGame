#include "YZPlatformEventLayer.h"

#include "YZPlatformLogic.h"

namespace YZ {

YZPlatformEventLayer::YZPlatformEventLayer(void)
{
}

YZPlatformEventLayer::~YZPlatformEventLayer(void)
{
}

void YZPlatformEventLayer::onEnter()
{
	YZLayer::onEnter();
	PlatformLogic()->addObserver(this);
}

void YZPlatformEventLayer::onExit()
{
	YZLayer::onExit();
	PlatformLogic()->removeObserver(this);
}


void YZPlatformEventLayer::sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize)
{	
	PlatformLogic()->sendData(MainID, AssistantID, object, objectSize);
}

void YZPlatformEventLayer::sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize, SEL_SocketMessage selector)
{
	PlatformLogic()->sendData(MainID, AssistantID, object, objectSize, selector);
}

void YZPlatformEventLayer::addEventSelector(UINT MainID, UINT AssistantID, SEL_SocketMessage selector)
{
	PlatformLogic()->addEventSelector(MainID, AssistantID, selector);
}

void YZPlatformEventLayer::removeEventSelector(UINT MainID, UINT AssistantID)
{
	PlatformLogic()->removeEventSelector(MainID, AssistantID);
}

void YZPlatformEventLayer::platformLogin(const std::string& name, const std::string& password, UINT uNameID,const std::string szUID ,const std::string hearUrl)
{
    //ss
	MSG_GP_S_LogonByNameStruct logonByName;
	logonByName.uRoomVer = 4;
	//logonByName.uNameID  = uNameID;
	strcpy(logonByName.TML_SN, "EQ4gG6vEUL06ajaGn4EAuXDa662vaeeqL6UdoOQatxuujAlnqovO6VndvXT4Tv0l4a28XGoDxqde4El6XUAXLXe66lg2o6gQN4tlOgeAoV6gulE2jTNneUulE");

	strcpy(logonByName.szName, name.c_str());

	strcpy(logonByName.szMD5Pass, password.c_str());
	logonByName.gsqPs = 5471;
	strcpy(logonByName.szMathineCode, "000266-703028-103FA5-C05024-006BBE-007F50");
	strcpy(logonByName.szCPUID, "612826255");
	strcpy(logonByName.szHardID, "2222222");
	strcpy(logonByName.szIDcardNo, "123456789");
	strcpy(logonByName.szMathineCode, "123456789");
	logonByName.iUserID = -1;
	strcpy(logonByName.szIDcardNo, "*");
	strcpy(logonByName.szMobileVCode, "*");
	strcpy(logonByName.szUID, szUID.c_str());
    strcpy(logonByName.szHeadUrl, hearUrl.c_str());
    
	sendData(MDM_GP_LOGON, ASS_GP_LOGON_BY_NAME, &logonByName, sizeof(MSG_GP_S_LogonByNameStruct));
} 

void YZPlatformEventLayer::platformCheckLogin( const std::string& name )
{
	MSG_GP_S_User_Verification loginName;
	strcpy(loginName.szAccount, name.c_str());
	sendData(MDM_GP_REGISTER, ASS_GP_USER_NAME_VERIFICTAION, &loginName, sizeof(MSG_GP_S_User_Verification));
}
void YZPlatformEventLayer::platformRegister(bool isFastRegister, UINT uid, const std::string& name, const std::string& password, const std::string& nickname, const std::string& oldpawd)
{
	if (isFastRegister)
	{
		// 不存在，则进行游客快速注册
		MSG_GP_S_VistorRegister vistorRegister;
		strcpy(vistorRegister.szMD5Pass, password.c_str());
		PlatformLogic()->sendData(MDM_GP_REGISTER, ASS_GP_VISITOR_REGISTER, &vistorRegister, sizeof(MSG_GP_S_VistorRegister));
	}
	else if (uid > 0)
	{
		//游客绑定
		MSG_GP_S_VisitorBind visitorBind = {0};
		visitorBind.dwUserID = uid;
		strcpy(visitorBind.szAccount, name.c_str());
		strcpy(visitorBind.szOldPass, oldpawd.c_str());
		strcpy(visitorBind.szNewPass, password.c_str());
		strcpy(visitorBind.szNick, nickname.c_str());	
		PlatformLogic()->sendData(MDM_GP_REGISTER, ASS_GP_VISITOR_BIND, &visitorBind, sizeof(MSG_GP_S_VisitorBind));;
	}
	else
	{
		// 用户注册
		MSG_GP_S_UserRegister userRegister = {0};
		strcpy(userRegister.szAccount, name.c_str());
		strcpy(userRegister.szMD5Pass, password.c_str());
		strcpy(userRegister.szNick, nickname.c_str());	
		strcpy(userRegister.szPass, oldpawd.c_str());
		PlatformLogic()->sendData(MDM_GP_REGISTER, ASS_GP_USER_REGISTER, &userRegister, sizeof(MSG_GP_S_UserRegister));;
	}
}

void YZPlatformEventLayer::requestRoomListInfo(UINT uKindID, UINT uNameID)
{
	MSG_GP_SR_GetRoomStruct GetRoomStruct;
	GetRoomStruct.uKindID = uKindID;
	GetRoomStruct.uNameID = uNameID;
	sendData(MDM_GP_LIST, ASS_GP_LIST_ROOM,&GetRoomStruct, sizeof(GetRoomStruct));
}


}