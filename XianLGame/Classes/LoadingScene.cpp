#include "LoadingScene.h"
#include "SimpleAudioEngine.h"
#include "GlobalUnits.h"
#include "PlatformDefine.h"
#include "GamePrompt.h"
USING_NS_CC;

Scene* LoadingScene::createScene(int uNameID , int bDeskIndex , bool bAutoCreate)
{
    auto scene = Scene::create();
    auto layer = LoadingScene::createLayer(uNameID, bDeskIndex, bAutoCreate);
    scene->addChild(layer);
    return scene;
}

LoadingScene* LoadingScene::createLayer(int uNameID , int bDeskIndex , bool bAutoCreate)
{
    LoadingScene *pRet = new LoadingScene();
    pRet->uNameID = uNameID;
    pRet->bDeskIndex = bDeskIndex;
    pRet->bAutoCreate = bAutoCreate;
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return nullptr;
    }
}

bool LoadingScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    
	auto winsize=Director::getInstance()->getWinSize();
	
    Label * label = Label::createWithTTF("正在初始化桌子...", "fonts/DroidSansFallback.ttf", 25);

    Sprite* sp = Sprite::create("erMaJiang/loading/MjLoading1.png");
    sp->setPosition(Vec2(winsize.width - sp->getContentSize().width - label->getContentSize().width , sp->getContentSize().height));
    this->addChild(sp , 1);
    
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label->setPosition(sp->getPosition() + Vec2(sp->getContentSize().width/1.5f, -sp->getContentSize().height/4));
    this->addChild(label);
    
    Animation* pAnimation = Animation::create();
    for (int i = 1; i <= 8; i++)
    {
        char fileName[60];
        sprintf(fileName, "erMaJiang/loading/MjLoading%d.png", i);
        pAnimation->addSpriteFrameWithFile(fileName);
    }
    pAnimation->setDelayPerUnit(1.5f / 8);
    pAnimation->setRestoreOriginalFrame(true);
    Animate* pAnimate = Animate::create(pAnimation);
    sp->runAction(RepeatForever::create(pAnimate));
    
    this->runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create([=]{
        this->actionEnd();
    }), NULL));
    
    return true;
}


void LoadingScene::actionEnd()
{
    bool ret = GameCreator()->startGameClient(this->uNameID, this->bDeskIndex, this->bAutoCreate);
    if (!ret)
    {
        GamePromptLayer::create()->showPrompt(GBKToUtf8("出错..."));
    }
    else
    {
        //auto pDesk = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(item, "Image_desk"));
        //pDesk->loadTexture("platform/desk_res/comon_desk2.png");
        
    }
}
