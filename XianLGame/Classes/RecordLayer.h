//
//  RecordLayer.hpp
//  PDKGame
//
//  Created by Dada on 16/7/12.
//
//

#ifndef RecordLayer_h
#define RecordLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"


#include "network/HttpClient.h"
#include "YZNetExport.h"

#define RRCORDLAYER_BTNSHARE_TAG 1001
#define RRCORDLAYER_BTNEXIT_TAG  1002


USING_NS_CC;
class Statues_gameRecord_item
{
public:
    int dwUserID;
    long long  i64SrcPoint;
    long long  i64ChangePoint;
    long long  i64SrcMoney;
    long long  i64ChangeMoney;
    long long  i64ChangeTax;
    char	 sNickName[50];
};
class Statues_gameRecord
{
public:
    short  wUserCount;
    Statues_gameRecord_item  item[18];
};

class StatusItem_Record
{
public:
    int dwRoomID;
    int dwDeskIndex;
    int dwRoundNo;
    int dwGameRounds;
    char szEndTime[50];
    char  szVirRoomID[50];
    Statues_gameRecord  rec;
};
class RecordLayer : public YZLayer
{
    
public:
    RecordLayer();
    virtual ~RecordLayer();
    
public:
    virtual bool init() override;
private:
    bool recordrankingEvent(YZSocketMessage* socketMessage);
    
    void load_record();
    void menuEventCallBack(Ref* pSender, Widget::TouchEventType type);
    
    
    
    void screenShot(bool b, const std::string& name);
    
    ui::Widget* _recordLayout;
    ui::ListView * listView_record;
    StatusItem_Record	m_item_record;
    int m_list_count;
    std::vector<StatusItem_Record> m_items_record;
    
public:
    void showPrompt();
    
    std::vector<StatusItem_Record>& getItems_record(){ return m_items_record; };
    CREATE_FUNC(RecordLayer);
};



#endif /* RecordLayer_hpp */
