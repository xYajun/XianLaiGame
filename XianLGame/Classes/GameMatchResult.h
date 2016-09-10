#ifndef __GameMatchResult_h__
#define __GameMatchResult_h__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "StoreLayer.h"
#include "YZNetExport.h"
#include "YZUIExport.h"
#include <vector>

USING_NS_CC;

using namespace cocostudio;
using namespace ui;
using namespace YZ;

class GameMatchResult: public YZLayer
{
private:
	int  _rank;
public:
	static Layer* createMatchRet(int rank);
	static GameMatchResult* create(int rank) ;
public:
	GameMatchResult(int rank);
	virtual ~GameMatchResult();

	virtual bool init();
	virtual void onExit() override;
private:
	void menuClickedCallback(Ref* pRef,  Widget::TouchEventType touchtype);
};

#endif // GameMatchResult