//
//  RecordLayer.cpp
//  PDKGame
//
//  Created by Dada on 16/7/12.
//
//

#include "RecordLayer.h"
#include "ShareLayer.h"

static const int GAMEPROMPT_LAYER_ZORDER	= 100000000;		// Â≠êËäÇÁÇπÂºπÂá∫Ê°ÜÂ±ÇÁ∫ß

static const int GAMEPROMPT_LAYER_TAG		= 100000000;		// Â≠êËäÇÁÇπÂºπÂá∫Ê°ÜÂ±ÇÁ∫ß

static const char * RECORD_LAYER = "platform/openRoom/Common/RecordLayer.json";     //ÊâìÂºÄËÆ∞ÂΩï

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "jni/JniHelper.h"
#include <android/log.h>

#if 1
#define  LOG_TAG    "JniHelper"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#else
#define  LOGD(...)
#endif

#endif

RecordLayer::RecordLayer()
{
    
}

RecordLayer::~RecordLayer()
{
    
}

bool RecordLayer::init()
{
    if ( !YZLayer::init() )
    {
        return false;
    }
    m_list_count=0;
    memset(&m_item_record, 0, sizeof(m_item_record));
    Size winSize = Director::getInstance()->getVisibleSize();
    _recordLayout = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile(RECORD_LAYER));
    _recordLayout->setAnchorPoint(Vec2(0.5f, 0.5f));
    _recordLayout->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
    addChild(_recordLayout);
    
    listView_record =(ListView*)Helper::seekWidgetByName(_recordLayout, "ListView_record");
    Layout* pModelItem = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("platform/openRoom/Common/RecordCell.json"));
    listView_record->setItemModel(pModelItem);
    
    auto bg = (ImageView*)Helper::seekWidgetByName(_recordLayout, "Image_bg");
    bg->setScale(winSize.width / bg->getContentSize().width , winSize.height / bg->getContentSize().height);
    bg->setAnchorPoint(Vec2(0.5f, 0.5f));
    bg->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
    bg->setVisible(true);
    
    auto img_top = (ImageView*)Helper::seekWidgetByName(_recordLayout, "Image_top");
    img_top->setAnchorPoint(Vec2(0.5f, 0.5f));
    img_top->setPosition(Vec2(winSize.width/2 , winSize.height - img_top->getContentSize().height/2));
    img_top->setVisible(false);
    
    auto btn_share = (Button*)Helper::seekWidgetByName(_recordLayout, "btn_share");
    btn_share->setTag(RRCORDLAYER_BTNSHARE_TAG);
    btn_share->setPosition(Vec2(btn_share->getPositionX(), img_top->getPositionY()));
    btn_share->addTouchEventListener(CC_CALLBACK_2(RecordLayer::menuEventCallBack, this));
    
    auto btn_exit = (Button*)Helper::seekWidgetByName(_recordLayout, "btn_exit");
    btn_exit->setTag(RRCORDLAYER_BTNEXIT_TAG);
    btn_exit->setPosition(Vec2(btn_exit->getPositionX(), img_top->getPositionY()));
    btn_exit->addTouchEventListener(CC_CALLBACK_2(RecordLayer::menuEventCallBack, this));
    
    MSG_GM_C_QUERY_GAMERECORD_PDK record_list;
    record_list.dwRoomID=RoomLogic()->getSelectedRoom()->uRoomID;
    record_list.dwUserID=PlatformLogic()->getInstance()->loginResult.dwUserID;
    record_list.dwDeskIndex=0;
    
    YZRoomLogic::getInstance()->sendData(MDM_GR_ROOM, ASS_GR_QUERY_GAMERECORD_PDK, &record_list, sizeof(record_list));
    YZRoomLogic::getInstance()->addEventSelector(MDM_GR_ROOM, ASS_GR_QUERY_GAMERECORD_PDK, YZ_SOCKET_CALLBACK(RecordLayer::recordrankingEvent, this));
    
    return true;
}

bool RecordLayer::recordrankingEvent(YZSocketMessage* socketMessage)
{
    UINT bAssistantID = socketMessage->messageHead.bAssistantID;
    UINT bHandleCode = socketMessage->messageHead.bHandleCode;
    if (bHandleCode==0)
    {
        MSG_GM_S_QUERY_GAMERECORD_PDK* record_list = (MSG_GM_S_QUERY_GAMERECORD_PDK*)socketMessage->object;
        
        m_list_count++;
        strcpy(m_item_record.szVirRoomID,record_list->szVirRoomID);
        strcpy(m_item_record.szEndTime,record_list->szEndTime);
        m_item_record.dwRoundNo = record_list->dwRoundNo;
        m_item_record.rec.wUserCount= record_list->rec.wUserCount;
        for (int i=0; i<record_list->rec.wUserCount; i++)
        {
            m_item_record.rec.item[i].dwUserID  =record_list->rec.item[i].dwUserID;
            strcpy(m_item_record.rec.item[i].sNickName,record_list->rec.item[i].sNickName);
            m_item_record.rec.item[i].i64ChangePoint=record_list->rec.item[i].i64ChangePoint;
            m_item_record.rec.item[i].i64SrcMoney=record_list->rec.item[i].i64SrcMoney;
        }
        m_items_record.push_back(m_item_record);
    }
    else if (bHandleCode==1)
    {
        load_record();
    }
    return true;
}

void RecordLayer::load_record()
{
    listView_record->removeAllItems();
    listView_record->refreshView();
    std::vector<StatusItem_Record>& items = getItems_record();
    std::vector<StatusItem_Record>::iterator itor = items.begin();
    std::vector<StatusItem_Record>::iterator itor2 = items.begin();
    int count=0;
    LLONG totalScore=0;
    for (int i=0; i<m_list_count; i++)
    {
        for (int j=0; j<itor->rec.wUserCount; j++)
        {
            if (itor->rec.item[j].dwUserID==PlatformLogic()->loginResult.dwUserID)
            {
                log("%d",PlatformLogic()->loginResult.dwUserID);
                totalScore=totalScore+itor->rec.item[j].i64ChangePoint;
                break;
            }
        }
        itor++;
    }
    itor = items.begin();

    char buttfer[60];
    auto Label_score = (Text*)Helper::seekWidgetByName(_recordLayout, "Label_score");
    sprintf(buttfer,"%s%lld","总积分:",totalScore);
    Label_score->setString(buttfer);
    while (&items && itor != items.end() && count <10)
    {
        count++;
        listView_record->pushBackDefaultItem();
        Widget* pModelItem = listView_record->getItem(listView_record->getItems().size() - 1);
        pModelItem->setUserData(&(*itor));
        char name[128];
        ImageView* Image_result = static_cast<ImageView*>(Helper::seekWidgetByName(pModelItem, "Image_result"));
        Text* Label_time = static_cast<Text*>(Helper::seekWidgetByName(pModelItem, "Label_time"));
        Label_time->setString(itor->szEndTime);
        Text* Label_roomNu = static_cast<Text*>(Helper::seekWidgetByName(pModelItem, "Label_roomNu"));
        sprintf(name, "%s%s","房号:",itor->szVirRoomID);
        Label_roomNu->setString(GBKToUtf8(name));
        Text* Label_playNu = static_cast<Text*>(Helper::seekWidgetByName(pModelItem, "Label_playNu"));
        sprintf(name, "%s%d%s","第",itor->dwRoundNo,"局");
        Label_playNu->setString(GBKToUtf8(name));
        for (int i=0; i<itor->rec.wUserCount; i++)
        {
            if (i >= 4) {
                break;
            }

            if (itor->rec.item[i].dwUserID==PlatformLogic()->loginResult.dwUserID)
            {
                if (itor->rec.item[i].i64ChangePoint>=0)
                {
                    Image_result->loadTexture("platform/openRoom/Common/record_bg/win.png");
                }
                else
                {
                    Image_result->loadTexture("platform/openRoom/Common/record_bg/lose.png");
                }
            }
            char str[60];
            sprintf(name, "Panel_player%d",i+1);
            Layout* Panel_player = static_cast<Layout*>(Helper::seekWidgetByName(pModelItem, name));
            Text* Label_name = static_cast<Text*>(Helper::seekWidgetByName(Panel_player, "Label_name"));
            Label_name->setString(GBKToUtf8(itor->rec.item[i].sNickName));
            Text* Label_uid = static_cast<Text*>(Helper::seekWidgetByName(Panel_player, "Label_uid"));
            sprintf(str, "%d",itor->rec.item[i].dwUserID);
            Label_uid->setString(str);
            Text* Label_currentScore = static_cast<Text*>(Helper::seekWidgetByName(Panel_player, "Label_currentScore"));
            sprintf(str, "%lld",itor->rec.item[i].i64ChangePoint);
            Label_currentScore->setString(str);
            Text* Label_totalScore = static_cast<Text*>(Helper::seekWidgetByName(Panel_player, "Label_totalScore"));
            LLONG score=0;
            
            for (int j=0; j<m_list_count; j++)
            {
                if(strcmp(itor->szVirRoomID, itor2->szVirRoomID) == 0)
                {
                    for (int z=0; z<itor->rec.wUserCount; z++)
                    {
                        if (itor2->rec.item[z].dwUserID==itor->rec.item[i].dwUserID)
                        {
                            score+=itor2->rec.item[z].i64ChangePoint;
                        }
                    }

                }
                itor2++;
            }
            itor2= items.begin();
            sprintf(str,"%s%lld","累计:",score);
            Label_totalScore->setString(str);
        }
        itor++;
    }
    listView_record->refreshView();

}



void RecordLayer::menuEventCallBack(Ref* pSender, Widget::TouchEventType type)
{
    if (Widget::TouchEventType::ENDED != type) return;
    auto btn=(Button*)pSender;
    switch (btn->getTag())
    {
        case RRCORDLAYER_BTNSHARE_TAG:
        {
            utils::captureScreen(CC_CALLBACK_2(RecordLayer::screenShot, this), "/mnt/sdcard/share.png");
        }
            break;
        case RRCORDLAYER_BTNEXIT_TAG:
            removeFromParentAndCleanup(true);
            break;
        default:
            break;
    }
}

void RecordLayer::showPrompt()
{
    Node* root = Director::getInstance()->getRunningScene();
    CCAssert(nullptr != root, "root is null");
    
    root->addChild(this, GAMEPROMPT_LAYER_ZORDER, GAMEPROMPT_LAYER_TAG);
}



void RecordLayer::screenShot(bool b, const std::string& name){
    //Ωÿ∆¡≥…π¶£¨ µ˜”√Œ¢–≈∑÷œÌ
    CCLOG("success %s, file name:%s", b?"true":"false", name.c_str());
    
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

        JniMethodInfo minfo;

        bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/dada/AppActivity","shareImg", "()V");
    
        if (isHave)
        {
            minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
        }
    #endif
}
