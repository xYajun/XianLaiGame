#ifndef GamePrompt_h__
#define GamePrompt_h__

#include "YZUIExport.h"

#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

using namespace ui;
using namespace cocostudio;

class GamePromptLayer : public YZLayer
{
	CCRect _userRect;

private:
	Text*			_Label_Prompt;
	Button*         _Button_OK;
	Layout*			_layout;

public:
	CREATE_FUNC(GamePromptLayer);

	GamePromptLayer();
	virtual ~GamePromptLayer();

public:
	virtual bool init() override;  

	void showPrompt(const std::string& prompt);

	void closePrompt();

	void setCallBack(std::function<void ()> sure);

private:
	void menuEventCallBack(Ref* pSender, Widget::TouchEventType type);

	void setPrompt(const std::string& prompt);

	void setPrompt(const std::string& prompt, const std::string& font, int size);

private:
	std::function<void ()> _sure;

};

#endif // GamePrompt_h__
