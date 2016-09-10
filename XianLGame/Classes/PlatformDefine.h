#ifndef __PlatformDefine_H__
#define __PlatformDefine_H__

#include "YZNetExport.h"

// WinSize 
#define Win_Size			Director::getInstance()->getWinSize()
#define Win_w				Director::getInstance()->getWinSize().width
#define Win_h				Director::getInstance()->getWinSize().height

// 本地数据保存配置
//////////////////////////////////////////////////////////////////////////
static const char* GAME_SOUND_BUTTON			= "platform/sound/sound_button.mp3";       //点击弹出按钮音效
static const char* GAME_SOUND_CLOSE				= "platform/sound/sound_close.mp3";        //点击关闭按钮音效
static const char* GAME_SOUND_CLICK				= "platform/sound/sound_click.mp3";        //点击音效
static const char* GAME_BACKGROUND_MUSIC		= "platform/sound/background.mp3";         //背景音乐
static const char* GAME_BACKGROUND_MUSICYL		= "platform/sound/bgm1.mp3";         //背景音乐


// 本地数据保存配置
//////////////////////////////////////////////////////////////////////////
static const char* SOUND_VALUE_TEXT		= "sound";
static const char* MUSIC_VALUE_TEXT		= "music";
static const char* USERNAME_TEXT		= "username";
static const char* PASSWORD_TEXT		= "password";
static const char* SAVE_TEXT			= "save";
static const char* GUEST_LOGIN_TEXT		= "guest";
static const char* GUEST_USERNAME_TEXT	= "guest_username";
static const char* GUEST_PASSWORD_TEXT	= "guest_password";	
static const char* GUEST_LOGIN_TIMES	= "guest_login_times";
//////////////////////////////////////////////////////////////////////////
#define INVALID_SOUND_VALUE		-1
#define MAX_SOUND_VALUE			100
#define MIN_SOUND_VALUE			0

#define INVALID_MUSIC_VALUE		-1
#define MAX__MUSIC_VALUE		100
#define MIN_MUSIC_VALUE			0
//////////////////////////////////////////////////////////////////////////
//static const char* LOADING				= "platform/loading.png";

//////////////////////////////////////////////////////////////////////////

class MoneyChangeNotify
{
public:
	virtual ~MoneyChangeNotify() {}
	virtual void walletChanged(LLONG money) = 0;
	virtual void bankChanged(LLONG money) {}
};

#endif	//__PlatformDefine_H__
                                                  
