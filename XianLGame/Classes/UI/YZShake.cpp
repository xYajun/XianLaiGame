#include "YZShake.h"  

namespace YZ
{

Shake::Shake() 
	: _strength_x(0)
	, _strength_y(0)
	, _initial_x(0)
	, _initial_y(0)  
{  
}  

Shake* Shake::create(float d, float strength)  
{  
	// call other construction method with twice the same strength  
	return create( d, strength, strength );  
}  

Shake* Shake::create(float duration, float strength_x, float strength_y)  
{  
	Shake *p_action = new Shake();  
	p_action->initWithDuration(duration, strength_x, strength_y);  
	p_action->autorelease();  

	return p_action;  
}  

bool Shake::initWithDuration(float duration, float strength_x, float strength_y)  
{  
	if (CCActionInterval::initWithDuration(duration))  
	{  
		_strength_x = strength_x;  
		_strength_y = strength_y;  

		return true;  
	}  

	return false;  
}  

// Helper function. I included it here so that you can compile the whole file  
// it returns a random value between min and max included  
float fgRangeRand( float min, float max )  
{  
	float rnd = ((float)rand()/(float)RAND_MAX);  
	return rnd*(max-min)+min;  
}  

void Shake::update(float time)  
{  
	float randx = fgRangeRand( -_strength_x, _strength_x );  
	float randy = fgRangeRand( -_strength_y, _strength_y );  

	// move the target to a shaked position  
	_target->setPosition(Vec2(_initial_x + randx,  
		_initial_y + randy));  
}  

Shake* Shake::clone(void) const  
{  
	auto a = new Shake();  
	a->initWithDuration(_duration, _strength_x, _strength_y);  
	a->autorelease();  
	return a;  
}  

Shake* Shake::reverse() const  
{  
	return Shake::create(_duration, -_strength_x, -_strength_y);  
}  

void Shake::startWithTarget(Node *target)  
{  
	CCActionInterval::startWithTarget(target);  

	// save the initial position  
	_initial_x = target->getPosition().x;  
	_initial_y = target->getPosition().y;  
}  

void Shake::stop(void)  
{  
	// Action is done, reset clip position  
	_target->setPosition(Vec2( _initial_x, _initial_y ) );  

	CCActionInterval::stop();  
}


}