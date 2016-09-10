#include "FontConfig.h"

Color3B colorGold(255, 215, 72);                               //金色
Color3B colorOrange(255, 165, 0);                              //橙色
Color3B colorWhite(255, 255, 255);                             //白色
Color3B colorRed(255, 0, 0);                                   //红色
Color3B colorBlack(0, 0, 0);                                   //黑色
Color3B colorGray(128, 128, 128);                              //灰色

std::string gFontFile("fonts/DroidSansFallback.ttf");           //字体文件名称

TTFConfig gFontConfig_30(gFontFile.c_str(), 30);    //房间名称30号字
TTFConfig gFontConfig_28(gFontFile.c_str(), 28);    //房间名称28号字
TTFConfig gFontConfig_25(gFontFile.c_str(), 25);    //房间名称25号字
TTFConfig gFontConfig_22(gFontFile.c_str(), 22);    //普通信息22号字
TTFConfig gFontConfig_20(gFontFile.c_str(), 20);    //玩家信息20号字
TTFConfig gFontConfig_18(gFontFile.c_str(), 18);    //房间名称18号字
TTFConfig gFontConfig_16(gFontFile.c_str(), 16);    //玩家信息16号字
TTFConfig gFontConfig_14(gFontFile.c_str(), 14);    //玩家信息14号字
TTFConfig gFontConfig_12(gFontFile.c_str(), 12);    //玩家信息12号字
TTFConfig gFontConfig_10(gFontFile.c_str(), 10);    //玩家信息10号字

Label* createLabel(const TTFConfig& ttfConfig, const std::string& text, const Color3B& color /*= colorWhite*/)
{
    Label* label = Label::createWithTTF(ttfConfig, text);
    label->setColor(color);
    return label;
}

Label* createLabel(const std::string& text, float fontSize)
{
    Label* label = nullptr;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    label = Label::createWithSystemFont(text, "Microsoft Yahei", fontSize);
#else
    label = Label::createWithSystemFont(text, "DroidSansFallback", fontSize);
#endif
    return label;
}