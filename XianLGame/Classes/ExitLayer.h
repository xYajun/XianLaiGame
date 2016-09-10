#ifndef ExitLayer_h__
#define ExitLayer_h__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"


#include "network/HttpClient.h"
#include "YZNetExport.h"


USING_NS_CC;

class ExitLayer : public YZLayer
{

public:
	ExitLayer();
	virtual ~ExitLayer();

public:
	virtual bool init() override;  
private:
	void menuEventCallBack(Ref* pSender, Widget::TouchEventType type);
public:
	void showPrompt();
	CREATE_FUNC(ExitLayer);
};


#endif // ExitLayer_h__
