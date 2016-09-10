//
//此类为实现精灵震动效果或者震屏效果
//

#ifndef __libCommon_HNShake__  
#define __libCommon_HNShake__  

#include "cocos2d.h"  

using namespace cocos2d;  

namespace YZ
{
	class Shake : public ActionInterval  
	{  
	public:  
		Shake();  

		/* Create the action with a time and a strength (same in x and y)  
		*  产生震动效果的初始化函数参数,两个方向相同  
		*  @param d 震动持续的时间  
		*  @param strength 震动的幅度  
		*/
		static Shake* create(float d, float strength);  

		// Create the action with a time and strengths (different in x and y)  
		// 产生震动效果的初始化函数,两个方向值不一样  
		static Shake* create(float d, float strength_x, float strength_y);  
		bool initWithDuration(float d, float strength_x, float strength_y);  

		//以下都是重写父类抽象类的函数(必须重写)  
		virtual Shake* clone() const override;  
		virtual Shake* reverse(void) const override;  
		virtual void startWithTarget(Node *target) override;  
		virtual void update(float time) override;  
		virtual void stop(void);  

	protected:  
		// Initial position of the shaked node  
		// 精灵的位置  
		float _initial_x, _initial_y;  
		// Strength of the action  
		// 抖动的幅度  
		float _strength_x, _strength_y;  
	};

}

#endif //__libCommon_HNShake__ 
