#ifndef __PERSONAL_LAYER_H__
#define __PERSONAL_LAYER_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "YZNetExport.h"
#include "PlatformDefine.h"



using namespace cocostudio;
using namespace cocos2d::ui;
USING_NS_CC;

// --------- btn tag
#define  BTN_PERSONAL 10001


// btn Heads
#define LOCAL_HEAD_PIC_TAG 100000

// btn edit nick layer
#define  BTN_NICK_CONFIRM 1051
#define  BTN_PERSONALLAYER_EXIT 1052
#define  BTN_SAVE 1053

// -------- image tag
#define  BTN_SEL_GIRL 2001
#define  BTN_SEL_BOY 2002
#define  IMG_SEX_GIRL 2003
#define  IMG_SEX_BOY 2004

// ---------- layer tag
#define PERSONALLAYER 30001



class PersonalLayer : public Layer,public EditBoxDelegate
{
    
public:
    
    Widget* _uiPersonalLayer;
    
    ImageView* img_sexgirl;
    ImageView* img_sexboy;
    
    int tag_current;
    int tag_last;
    
    ListView* _headList;
    
    Text* label_name;
    EditBox* tf_nickname;
    
    Button* btn_head;
    Button* btn_save;
    
    int _headImgId;
    
    char _regNick[32];
    
    bool _isBoy;
    
public:
    
    static cocos2d::Layer* createLayer();
    virtual bool init();
    
private:
    void initPanel_Personal();
    
    void setPersonalHeadPic();
    void reSetRoleSex(bool isboy);									//设置性别
    
    bool modifyRoleInforSelector(YZSocketMessage* socketMessage);
    
    void onBtnClicked(Ref * pSender,Widget::TouchEventType type);
    void onBtnSelClicked(Ref * pSender,Widget::TouchEventType type);
    void headBtnClicked(Ref * pSender,Widget::TouchEventType type);
    void onHeadPicClicked(Ref * pSender,Widget::TouchEventType type);
    
public:
    
    void editBoxReturn(EditBox* editBox);
    
    void editBoxTextChanged(EditBox* editBox, const std::string& text);
    
    CREATE_FUNC(PersonalLayer);
    
};

#endif 
