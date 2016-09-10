#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;

using namespace ui;
using namespace cocostudio;


class LoadingScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(int uNameID , int bDeskIndex , bool bAutoCreate);

    static LoadingScene* createLayer(int uNameID , int bDeskIndex , bool bAutoCreate);
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	

    // implement the "static create()" method manually
    CREATE_FUNC(LoadingScene);

private: 
	void actionEnd();

    int uNameID;
    int bDeskIndex;
    bool bAutoCreate;
};

#endif // __WELCOME_SCENE_H__
