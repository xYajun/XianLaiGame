#ifndef __Game_Queue_Wait_H__
#define __Game_Queue_Wait_H__

#include "YZUIExport.h"
#include "YZNetExport.h"

class GameQueueWait: public YZRoomEventLayer
{
public:
	static Scene* createScene();
	static GameQueueWait* create();
	virtual bool init();
	GameQueueWait();
	virtual ~GameQueueWait();
	void stop();
	void updateWord();
	virtual void onQueueUserSitMessage(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& users);
protected:
	void menuClickedCallback(Ref* pRef);
private:
	Sprite* _gameIcon;
	Sprite* _hintSp;
	Node* _loadNode;
	MenuItemImage* _menuItem;
	bool _joined;
	int _flag;
};

#endif // !__Game_Queue_Wait_H__
