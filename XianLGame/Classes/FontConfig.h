#ifndef __FontConfig_H__
#define __FontConfig_H__

#include "cocos2d.h"
USING_NS_CC;

static const char* LOADING = "res/loading.png";

extern Color3B colorGold;          //金色
extern Color3B colorOrange;        //橙色
extern Color3B colorWhite;         //白色
extern Color3B colorRed;           //红色
extern Color3B colorBlack;         //黑色
extern Color3B colorGray;          //灰色

extern std::string gFontFile;       //字体文件名称

extern TTFConfig gFontConfig_30;    //房间名称30号字
extern TTFConfig gFontConfig_28;    //房间名称28号字
extern TTFConfig gFontConfig_25;    //房间名称25号字
extern TTFConfig gFontConfig_22;    //普通信息22号字
extern TTFConfig gFontConfig_20;    //玩家信息20号字
extern TTFConfig gFontConfig_18;    //房间名称18号字
extern TTFConfig gFontConfig_16;    //玩家信息16号字
extern TTFConfig gFontConfig_14;    //玩家信息14号字
extern TTFConfig gFontConfig_12;    //玩家信息12号字
extern TTFConfig gFontConfig_10;    //玩家信息10号字

Label* createLabel(const TTFConfig& ttfConfig, const std::string& text, const Color3B& color = colorWhite);

Label* createLabel(const std::string& text, float fontSize);
#endif	//__FontConfig_H__
