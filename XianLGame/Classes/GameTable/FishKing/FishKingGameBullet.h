#ifndef __YZ_FishKing_GameBullet_H__
#define __YZ_FishKing_GameBullet_H__

#include "YZNetExport.h"
#include "YZUIExport.h"
#include "cocos2d.h"
#include "FishKingMessageHead.h"
#include "FishKingGameFishManager.h"
USING_NS_CC;

namespace FishKing
{
	class GamePlayer;

	class GameBullet: public YZ::YZSprite
	{
	public:
		typedef std::function<void (INT fishId , INT bulletId , BulletKind bulletKind , INT  bulletMuriple)> HitFishCallBack;
		HitFishCallBack  onHitFishCallBack;
	public:
		~GameBullet();
		static GameBullet* create(const std::string & file, GamePlayer* player, GameFishManager* fishManager, BulletKind bulletKind, INT bulletId, INT bulletMulriple, FLOAT net_radius, FLOAT speed);
		void runStartAction(float radians);
		//子弹运动
		virtual void update(float dt);
		//子弹和鱼碰撞检测
		virtual void collisionDetection();
		//子弹爆炸撒网
		void bulletExplosion();	

	protected:
		GameBullet(GamePlayer* cannonUI, GameFishManager* fishManager, BulletKind bulletKind, INT bulletId, INT bulletMulriple, FLOAT net_radius, FLOAT speed);
	protected:
		CC_SYNTHESIZE(FLOAT,		_speed, Bullet_speed);				//子弹速度
		CC_SYNTHESIZE(FLOAT,		_radians, Bullet_Radians);			//子弹弧度
		CC_SYNTHESIZE(FLOAT,		_netRadius, Net_Radians);			//渔网弧度
		CC_SYNTHESIZE(INT,			_bullet_id, Bullet_id);				//子弹ID
		CC_SYNTHESIZE(INT,			_bullet_mulriple, Bullet_mulriple);	//子弹倍数
		CC_SYNTHESIZE(BulletKind,	_bullet_kind, Bullet_Kind);			//子弹类型
		CC_SYNTHESIZE(INT,			_lockFishId, LockFish_id);			//锁定的鱼的id

		Size						_WinSize;
		bool						_isDoUpdateFUNC;				//因为pc是每秒30帧，手游60帧，所以控制2帧动作一次，为了跟pc同步
	protected:
		GameFishManager*		_fishManager;
		GamePlayer*				_player;

	};
}

#endif // __FishKing_GameBullet_H__
