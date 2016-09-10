#ifndef __ErMahjong_Game_Table_UI_Callback_H__
#define __ErMahjong_Game_Table_UI_Callback_H__

#include "YZNetExport.h"
#include "ErMahjongMessageHead.h"


namespace ErMahjong
{
	class GameTableUICallback
	{
	public:
		virtual void addUser(BYTE seatNo, bool bMe) = 0;
		virtual void removeUser(BYTE seatNo, bool bMe) = 0;

		virtual void setUserName(BYTE seatNo, const std::string& name) = 0;
		virtual void showUserMoney(BYTE seatNo, bool visible) = 0;
		virtual void setUserMoney(BYTE seatNo, INT money) = 0;
		virtual void setUserHead(BYTE seatNo) = 0;

		virtual void showUserReady(BYTE seatNo,bool visible) = 0;
		virtual void setBanker(BYTE seatNo,bool visible)=0;
		virtual void showTingCard(BYTE seatNo,bool visible)=0;

		virtual void showReadyBtn( bool visible)=0;
		virtual void showActionBtn(BYTE seatNo ,BYTE actionWik , BYTE card)=0;
        virtual void showActionCard(MJActionData data,ActionType actionType) = 0;
                
        virtual void showMahjongLayout(BYTE card ,  bool visible) = 0;
        virtual void setTureCaiShenCard(BYTE card) = 0;

		virtual void setWaitTime(BYTE seatNo, BYTE time, bool visible,int statues)=0;

		virtual void showAutoOutCardBtn(bool visible) = 0;
		virtual void setAutoOutCardBtn(bool visible) = 0;
		virtual void showCAutoOutCardBtn(bool visible) = 0;
        
		virtual void setUpCardList(BYTE seatNo, const std::vector<BYTE>& upCards) = 0;
	    virtual void getUpCardList(BYTE seatNo, std::vector<BYTE>* upCards) = 0;

		virtual void showBtnType(int actCount,BYTE statues)=0;
		virtual void hideBtnType()=0;

		virtual void setChiPaiLayout(int type,BYTE cardValues)=0;
		//virtual void showUserReslutInfor(bool iWin,BYTE seatNo,bool iBanker,char name[61],LLONG iDiFen,int iLianZhuang,WzType wzType,WzHuType wzHuType, tagMaiDing::MdInfo iDiPoint,LLONG iTotalPoint)=0;
		virtual void showThinkAct(int type)=0;
		virtual void showActionResult(USHORT type,BYTE count,BYTE cardValues,bool iVisible)=0;
		virtual void showOutCard(BYTE seatNo,BYTE cardValues)=0;
        virtual void showUserHandCard(BYTE seatNo, const std::vector<BYTE> values)=0;
		virtual void showUserHandCardOut(BYTE seatNo)=0;

        virtual void showUserVoice(BYTE seatNo, std::string voiceName , int voiceTime) =0;

        virtual void onGameDisConnected()=0;
        
        virtual void showPlayerGetCard(BYTE seatNo , BYTE value) = 0;
        virtual void setGameBanker(BYTE seatNo) = 0;
        
        virtual void showUserAskDisbandRoom(bool isShow) = 0;

        virtual void setGamePlayerPoint(BYTE seatNo) = 0;  //设置游戏的庄家

        virtual void setGameZMCount(int zmCount) = 0;  //设置游戏的扎码数
        
        virtual void outCardResultError(BYTE seatNo) = 0;
        
        virtual void showAllUserCard(BYTE seatNo,std::vector<BYTE> values , BYTE bankerCard) = 0;
        
        virtual void playerBackGame(BackGameData data)= 0;
        
        virtual void showGameEnd() = 0;
        
		virtual void clearDesk() = 0;
		virtual void leaveDesk() = 0;

        virtual void showUserTrustee(BYTE seatNo , bool bTrustee)=0;
        
        virtual void isLastGame(int nowGameCount , int totalGameCount) = 0;

        virtual void doGameEndResp(MJGameEndData data , std::map<BYTE, std::vector<BYTE>> actionCardInfoMap  , std::map<BYTE , std::vector<BYTE>> cardInfoMap) = 0;
        
        virtual void showListenerCard(bool isShow, std::vector<BYTE> cards) = 0;
		//=================================================//
		virtual void showGameShaiZi(BYTE cbSiceCount[2])=0;
                
        virtual void showUserChatFaceAnimate(BYTE seatNo, int _faceId) = 0;
        virtual void showUserChatTalkMsg(BYTE seatNo, std::string msg) = 0;

        virtual BYTE getPlayerGuangCard(BYTE seatNO) = 0;
	};
}


#endif