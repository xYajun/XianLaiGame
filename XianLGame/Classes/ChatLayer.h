#ifndef MessageLayer_h__
#define MessageLayer_h__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "YZNetExport.h"

USING_NS_CC;

using namespace ui;
using namespace cocostudio;

#define  PANEL_FACE 1001
#define  PANEL_TALK 1002

// btn tag
#define  BTN_FACE 2001
#define  BTN_TALK 2002
#define  BTN_CLOSE 2003
#define  BTN_SEND 2004

#define  BTN_FACE_TAG 3000
#define  BTN_TALK_TAG 4000


class ChatLayer : public Layer
{
    
private:
    Widget* _uiChatLayer;
    Layout* panel_face;
    Layout* panel_talk;
    
    int tag_current;
    int tag_last;
    
    ListView* listview_talk;
    
    std::function<void(int)> _onSendChatFace;
    std::function<void(int)> _onSendChatTalk;
    
public:
    virtual bool init();
    
    void showTalks();
    
    void onImageClicked(Ref * pSender,Widget::TouchEventType type);
    void onBtnClicked(Ref * pSender,Widget::TouchEventType type);
    
    // 设置表情按钮回调函数
    void onFaceBtnClicked(Ref * pSender,Widget::TouchEventType type);
    void onFaceBtnClickedCB(const std::function<void(int id)>& onSendChatFace);
    // 设置常用语音回调函数
    void onTalkImgClicked(Ref * pSender,Widget::TouchEventType type);
    void onTalkImgClickedCB(const std::function<void(int id)>& onSendChatTalk);
    
    CREATE_FUNC(ChatLayer);
    
};


#endif // FriendLayer_h__
