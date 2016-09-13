#ifndef _ErMahjong_Game_Table_UI_H_
#define _ErMahjong_Game_Table_UI_H_

#include "ErMahjongGameTableUICallback.h"
#include "ErMahjongGamePlayer.h"
#include "YZNetExport.h"
#include "cocos2d.h"
#include "ErMahjongGameLogic.h"

namespace ErMahjong
{
	//
	class GameTableLogic;
	class GameLogic;
	class CardOutList;
	class CardListBoard;
	
	class GameTableUI : public YZ::YZGameUIBase , public GameTableUICallback
	{
	public:
		static GameTableUI* create(BYTE bDeskIndex, bool bAutoCreate);
		virtual bool init(BYTE bDeskIndex, bool bAutoCreate);
		virtual void onEnter();
		virtual void onExit();
	
	public:
		virtual void addUser(BYTE seatNo, bool bMe) override;
		virtual void removeUser(BYTE seatNo, bool bMe) override;

		virtual void setUserName(BYTE seatNo, const std::string& name) override;
		virtual void showUserMoney(BYTE seatNo, bool visible) override;
		virtual void setUserMoney(BYTE seatNo, INT money) override;
		virtual void setUserHead(BYTE seatNo) override;

		virtual void showGameShaiZi(BYTE cbSiceCount[2]);
		virtual void showUserReady(BYTE seatNo,bool visible) override;
		virtual void setBanker(BYTE seatNo,bool visible) override;
		virtual void showTingCard(BYTE seatNo,bool visible) override;
		virtual void showReadyBtn( bool visible) override;
		
        virtual void showActionBtn(BYTE seatNo , BYTE actionWik , BYTE card) override;
        virtual void showActionCard(MJActionData data , ActionType actionType) override;

		virtual void showMahjongLayout(BYTE card ,  bool visible)override;
        virtual void setTureCaiShenCard(BYTE card)override;
		virtual void setWaitTime(BYTE seatNo, BYTE time, bool visible,int statues) override;

		virtual void showAutoOutCardBtn(bool visible) override;
		virtual void setAutoOutCardBtn(bool visible) override;
		virtual void showCAutoOutCardBtn(bool visible)override;

        virtual void showUserHandCard(BYTE seatNo, const std::vector<BYTE> values)override;
		
        virtual void showAllUserCard(BYTE seatNo,std::vector<BYTE> values , BYTE bankerCard)override;
        
        virtual void playerBackGame(BackGameData data) override;
        
        virtual void outCardResultError(BYTE seatNo)override;
        
        virtual void showGameEnd()override;

		virtual void setUpCardList(BYTE seatNo, const std::vector<BYTE>& upCards) override;
		virtual void getUpCardList(BYTE seatNo, std::vector<BYTE>* upCards) override;

		virtual void showBtnType(int actCount,BYTE statues)override;
		virtual void hideBtnType()override;

		virtual void setChiPaiLayout(int type,BYTE cardValues)override;
		//virtual void showUserReslutInfor(bool iWin,BYTE seatNo,bool iBanker,char name[61],LLONG iDiFen,int iLianZhuang,WzType wzType,WzHuType wzHuType, tagMaiDing::MdInfo iDiPoint,LLONG iTotalPoint)override;
		virtual void showThinkAct(int type)override;
		virtual void showActionResult(USHORT type,BYTE count,BYTE cardValues,bool iVisible)override;
		virtual void showOutCard(BYTE seatNo,BYTE cardValues)override;
		virtual void showUserHandCardOut(BYTE seatNo)override;
        virtual void setGameZMCount(int zmCount) override;  //设置游戏的扎码数

        //玩家得到一张牌
        virtual void showPlayerGetCard(BYTE seatNo , BYTE value)override;
        virtual void setGameBanker(BYTE seatNo)override;  //设置游戏的庄家
        virtual void setGamePlayerPoint(BYTE seatNo)override;  //设置游戏的庄家
        virtual void clearDesk() override;
		virtual void leaveDesk() override;
        
        virtual void showUserTrustee(BYTE seatNo ,bool bTrustee)override;  //显示玩家托管操作
        
        virtual void doGameEndResp(MJGameEndData data, std::map<BYTE, std::vector<BYTE>> actionCardInfoMap  , std::map<BYTE , std::vector<BYTE>> cardInfoMap)override;
        
        virtual void showUserChatFaceAnimate(BYTE seatNo, int _faceId) override;
        virtual void showUserChatTalkMsg(BYTE seatNo, std::string msg) override;
        
        virtual BYTE getPlayerGuangCard(BYTE seatNO) override;

        virtual void isLastGame(int nowGameCount , int totalGameCount)override;

        virtual void showUserAskDisbandRoom(bool isShow ) override;

        virtual void showListenerCard(bool isShow, std::vector<BYTE> cards) override;
        
        virtual void showUserVoice(BYTE seatNo, std::string voiceName , int voiceTime) override;
        
        virtual void onGameDisConnected()override;
        
        void GameStart();
        
        static GameTableUI * _instance;
        static GameTableUI* getInstance();
        
        void uploadFileEnd(std::string fileName);
        void downloadFileEnd(std::string fileName);
        
        void voiceStart();
        
        void update(float dt);
    protected:
		GameTableUI();
		virtual ~GameTableUI();
		void loadLayout();
		bool isValidSeat(BYTE seatNo);
		void callEverySecond(float delta);
		void startTimer();
		void stopTimer();
		void initParams();

		void showCardType(int type,BYTE cardValues);
		void btnClickEventCallback(cocos2d::Ref* pSender, ui::Widget::TouchEventType touchtype);

        void btnChatClickEventCallback(cocos2d::Ref* pSender, ui::Widget::TouchEventType touchtype);
        void btnChatItemClickEventCallback(cocos2d::Ref* pSender, ui::Widget::TouchEventType touchtype);
        void disbandClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);

        
		void addOutCardList(BYTE seatNo);
		void addHandCardList(BYTE seatNo);
		void autoOutCallFunction(cocos2d::Node* pNode);
        void initCardFinish(MJPostion _dir);        //所有玩家的麻将初始化完成
        void outCardFinish(Vec2 point);
        void gangActionEnd(BYTE seatNo , BYTE cardValue);
        
        
        void playerOutCardResp(BYTE card);
        
        void checkActionBtnShow(BYTE actionType , BYTE cardValue);
        void showActionAnimation(BYTE seatNo , ActionType aType);
        void playActionSound(BYTE seatNo , ActionType aType);
        void hideAllAcitonBtn();
        void showCaiShenCard(BYTE card,bool visible);
        
        void showGamePanelBtns(bool isVisable);
        void showResultLayout(bool isVisable);
        void showChatLayout(bool isVisable);
        void showSettingLayout(bool isVisable);
        
        void bgSliderEvent(cocos2d::Ref *pSender, SliderEventType type);
        void soundSliderEvent(cocos2d::Ref *pSender, SliderEventType type);

        void showChatMsg(std::string msg);
        
        void showVoiceLayer(bool isShow);
        
        void changeImageTexture(ImageView * image , BYTE cardValue);
        
        Vec2 getAnimatePoint(BYTE actionSeatNo);
                
    private:
		GameTableLogic*  _tableLogic;
		GameLogic        _gameLogic;
		ui::Widget* _tableWidget;
        ui::Widget* _chatWidget;
        ui::Widget* _settingWidget;
		PlayerUI* _players[PLAYER_COUNT];
		Button*  _btnReady;
		Button* _btnRight;
        ui::ListView* _chatListView;
        std::vector<ui::Text> chatMsgs;
        
        std::map<int , ui::Widget *> chatWidget;
        
		Button*  _btnAction[3];
		Button*  _btnVoice;
        Button*  _btnSet;
        Button*  _btnChat;
        
		ImageView*  _cancleAutobg;
		Button*  _btnType[6];
		Button*  _btnChiType[3];
		ImageView* _imgChiType[3];
		ImageView* _imgReady[PLAYER_COUNT];
		ui::TextAtlas* _timerText;
		Layout* _panelAction[5];
        Layout* _leftPanel;
        Button * _btnLeft;
        EditBox* _editChatMsg;
        
        std::string _fontName;
        Size _fontConSize;
        int _fontSize;

		CardListBoard* _cardListBoard[PLAYER_COUNT];
		CardOutList*   _cardOutList[PLAYER_COUNT];
		Layout* _PanelChi;
		int _currentStatues;
		BYTE _index;
		BYTE outCard[5][11];
        
		void showRightPanel(bool isShow);
        
        BYTE _bankerCard;
        BYTE _bankerSeatNo;
        BYTE _caiShenCard;
        int _lastOutCartSeatNo;
        bool waitingForAnGuang;
        bool _leftShow;
        
        BYTE bDarkGuangSeatNo;  //暗杠发生的位置
        BYTE bDarkGuangCard;    //暗杠的牌
        
        Sprite * outIndexSp;
        Layout * disbandRoomLayout;
        Layout * listenerCardLayout;
        ImageView * voiceLayout;

        Text* label_roomNum;
        Text* label_niaoNum;
        Text* label_GameTimes;
        bool _isLastGame;
        int m_iNowGameCount;
        
        Button* _btnMusic;
        Button* _btnSound;
        ui::Slider * bgSlider;
        ui::Slider * soundSlider;
    private:
        //结算
        void initResultLayer();
        
        ImageView * gameMsgBar;
        Text* label_msg;
        
        void showGameMsgBar(bool isShow);
        
        void msgBar(float dt);
        
        ui::Widget* _resultWidget;
        
        std::vector<ImageView *> _listenerCardVec;      //听牌列表

        std::vector<ImageView *> _voiceProgressVec;      //录音进度
        
        string getCardTextureFileName(BYTE cardValue, bool isAction);
        
        void touchVoiceEvent(Ref *pSender, Widget::TouchEventType type);
        
        void updateProgress(float Dt);
        
        std::vector<Node *>  clearNodes;
        
        bool m_bHaveGangAction;       //是否有杠牌动作
        
        bool m_bGangActionEnd;         //杠牌动作是否结束
        
        BYTE gangActionSeatNo;
        
        std::vector<BYTE> gangActionCard;
        
        void voiceEnd();
        
        std::string myVoiceName;
        
        float voiceTime;
        
        int playerCount;
        
        int voiceShowIndex;
        float oneVoiceTime;
        
        int nowShowZmIndex;
        
        void showNextZmImage(std::vector<BYTE> cards,std::vector<ImageView *> showCards);
        
        void hideZmLayers();
    };
}

#endif
