#ifndef __YZ_FishKing_FishFactory_H__
#define __YZ_FishKing_FishFactory_H__

#include "YZNetExport.h"
#include "YZUIExport.h"
#include "FishKingMessageHead.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <string>

using namespace ui;
using namespace std;
using namespace cocostudio;

USING_NS_CC;

namespace FishKing
{
	class Fish : public YZ::YZSprite
	{
		struct FishSpirteInfo
		{
			std::string name;
			BYTE	count;
			FLOAT	time;
		};

		FishSpirteInfo _fishName[26];

	public:
		typedef std::function<void (int fishId)> MoveOutCallBack;
		MoveOutCallBack  onMoveOutCallBack;

		typedef std::function<void (INT fishId)> UnLockCallBack;
		UnLockCallBack  onUnLockCallBack;
	public:
		static Fish* create(int fishId, FishKind fishKind, FishKind fishKind_ex[5]);
		virtual bool init();

		//设置鱼帧动画图片信息
		void onRender();
		//创建鱼游动帧动画
		Animation* addFishData(FishSpirteInfo fish, BYTE kind);

	public:
		Fish(int fishId, FishKind fishKind, FishKind fishKind_ex[5]);
		~Fish();

	public:
		//创建普通鱼（类型0-25）
		void createNormalFish(BYTE fishKind);
		//创建三角大三元鱼
		void createDaSanYuanFish(BYTE fishKind);
		//创建直线大三元鱼
		void createZXDaSanYuanFish(BYTE fishKind);
		//创建四角大四喜鱼
		void createDaSixiFish(BYTE fishKind);		
		//创建直线大四喜鱼
		void createZXDaSiXiFish(BYTE fishKind);
		//创建红色鱼王
		void createRedKingFish(BYTE fishKind);
		//创建带底盘的鱼王
		void createPlatekingFish(BYTE fishKind);
		//播放鱼儿死亡动画
		void playFishDieAction();

	public:		
		virtual void setPosition(const Vec2& pos) override;
		virtual void update(float dt) override;

	public:
		//让鱼消失
		void setHide();
		//重置属性
		void refreshParams();
		//设置鱼的游动路径
		void setFishTraceList(std::vector<FPointAngle> fishTraceList, bool isUseFishTtrace);
		//让鱼游动起来
		void playFishAction(std::vector<Vec2>* fishTrace, TraceType traceType, FLOAT speed);
		//鱼按照鱼阵路径运动
		void playFishActionByTraceList();
		//设置鱼的游动动作(直线/贝塞尔曲线)
		FiniteTimeAction* getActionInThreePoint(Vec2 first, Vec2 second, Vec2 third, TraceType actionType = TRACE_LINEAR);
		//让鱼潮中的鱼按要求停止前进
		void setFishStop(std::vector<FPointAngle>::size_type stop_index, std::vector<FPointAngle>::size_type stop_count);
		//获取辅助鱼类型
		FishKind getFishKindEx();

	private:
		Size						_winSize;
		FiniteTimeAction*			_fishMoveAction;				//鱼运动的动作
		std::vector<FPointAngle>	_fishTraceList;					//5种场景鱼阵客户端计算的鱼轨迹
		UINT						_FishTraceIndex;				//鱼行进到哪一个坐标点
		bool						_isDoUpdateFUNC;				//因为pc是每秒30帧，手游60帧，所以控制2帧动作一次，为了跟pc同步
		Vec2						_oldPosition;					//上一次控制的坐标
		Vec2						_desPosition;					//本次的坐标

		std::vector<FPointAngle>::size_type		_stop_index;
		std::vector<FPointAngle>::size_type		_stop_count;
		std::vector<FPointAngle>::size_type		_current_stop_count;

	private:
		CC_SYNTHESIZE(INT,			_fishID, FishID);				//鱼的id编号
		CC_SYNTHESIZE(FishKind,		_fishKind, FishKind);			//主鱼的类型
		CC_SYNTHESIZE(Rect,			_fishRect, FishRect);			//鱼的矩形
		CC_SYNTHESIZE(Size,			_fishSize, FishSize);			//鱼的SIZE
		CC_SYNTHESIZE(FLOAT,		_fishSpeed, FishSpeed);			//服务端发来的鱼游动速度
		CC_SYNTHESIZE(bool,			_isTouch, CanTouch);			//该鱼还能不能被子弹打到
		CC_SYNTHESIZE(bool,			_isPause, PauseAction);			//鱼是否暂停游动动画
		CC_SYNTHESIZE(bool,			_isUseFishTrace, UseFishTrace);	//是否使用服务端位置控制
		CC_SYNTHESIZE(bool,			_isSwitch, SwitchScene);		//是否正在切换场景
		CC_SYNTHESIZE(ImageView*,	_gameBG, GameBG);				//切换的游戏背景图，用来在场景切换时和鱼碰撞消除
		CC_SYNTHESIZE(bool,			_isLock, FishIsLock);			//是否已经被锁定

	public:
		FishKind					_fishKind_ex[5];				//辅助的鱼类型
	};

}

#endif // __FishKing_FishFactory_H__
