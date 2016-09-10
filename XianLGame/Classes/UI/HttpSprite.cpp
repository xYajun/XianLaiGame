//
//  HttpSprite.cpp
//  WechatTest
//
//  Created by Dada on 16/7/21.
//
//

#include "HttpSprite.h"
#include "../Util/JzLog.h"

HttpSprite::HttpSprite(void)
{
    
}


HttpSprite* HttpSprite::create(std::string url , std::string normal)
{
    HttpSprite *sprite = new  HttpSprite();
    
    if (sprite && sprite->initWithFile(normal))
    {
        sprite->init();
        sprite->autorelease();
        sprite->GetHttpImg(sprite, url, 1);
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

bool HttpSprite::init(){
    _clickSpriteFunc = nullptr;
    
    m_iHeadWidth = this->getContentSize().width;
    m_iHeadHeight = this->getContentSize().height;
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(HttpSprite::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(HttpSprite::onTouchEnded, this);
    listener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}


void HttpSprite::GetHttpImg(Ref* obj, string imgurl,int number)
{
    HttpRequest* request = new HttpRequest();
    // required fields
    request->setUrl(imgurl.c_str());
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(obj, httpresponse_selector(HttpSprite::onHttpRequestRptImg));
    // optional fields
    char thisnumber[10] = "";
    sprintf(thisnumber, "%d",number);
    request->setTag(thisnumber);
    HttpClient::getInstance()->send(request);
    request->release();
}

void HttpSprite::onHttpRequestRptImg(HttpClient *sender, HttpResponse *response)
{

    JzDebug("请求微信头像....");
    
    char c_tag[20]= "";
    sprintf(c_tag, "%s",response->getHttpRequest()->getTag());
    CCLOG("%s completed", response->getHttpRequest()->getTag());
    string str_tag = c_tag;
    if (!response)
    {
        jzError("请求微信头像失败...");
        return;
    }
    // You can get original request type from: response->request->reqType
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        CCLOG("%s completed", response->getHttpRequest()->getTag());
    }
    
    if (!response->isSucceed())
    {
        CCLOG("response failed");
        CCLOG("error buffer: %s", response->getErrorBuffer());
        jzError("请求微信头像失败.. error buffer %s", response->getErrorBuffer());

        return;
    }
    
    vector<char> *buffer = response->getResponseData();
    
    //create image
    Image* img = new Image;
    img->initWithImageData((unsigned char*)buffer->data(),buffer->size());
    
    //create texture
    texture = new Texture2D();
    bool isImg = texture->initWithImage(img);
    
    if (isImg) {
        SpriteFrame* sprite_frame = cocos2d::SpriteFrame::createWithTexture(texture,
                                                                            Rect(0,0,texture->getContentSize().width, texture->getContentSize().height));
        this->setSpriteFrame(sprite_frame);
        this->resetImgSize(m_iHeadWidth, m_iHeadHeight);
        texture->release();
    }
    
    img->release();
    
    //已下是自己封装的PublicDoFunc异步调用主线程执行换图操作的部分，可以自己去封装
    //SEL_CallFuncN pfnCallback =callfuncN_selector(View_Room::upPlayerHead);
    //CCNode* nd = CCNode::create();
    //buffer->clear();
    //nd->setTag(CCString::create(str_tag)->intValue());
    //PublicDoFunc::toDoFuncN(LAYER_ROOM, pfnCallback,nd);
}

void HttpSprite::setImgSize(Size imgSize){
    JzDebug("设置头像尺寸...  宽 : %02f  高 : %02f " , imgSize.width , imgSize.height);
    m_iHeadWidth = imgSize.width;
    m_iHeadHeight = imgSize.height;
    resetImgSize(m_iHeadWidth , m_iHeadHeight);
}

void HttpSprite::resetImgSize(float oldhead_w,float oldhead_h)
{
    JzDebug("重置头像尺寸...  宽 : %02f  高 : %02f " , oldhead_w , oldhead_h);

    float s_X=oldhead_w/this->getContentSize().width;
    float s_Y=oldhead_h/this->getContentSize().height;
    this->setScaleX(s_X);
    this->setScaleY(s_Y);
}

void HttpSprite::CreateImg(CCSprite* tagSprite,CCSprite* oldSprite,CCTexture2D* texture,int newTag)
{
    /*
    int oldw =oldSprite->getContentSize().width;
    int oldh =oldSprite->getContentSize().height;
    if(texture!=NULL)
    {
        CCSprite* sprite = CCSprite::createWithTexture(texture);
        //按原比例设置
        setImgSize(oldw,oldh,sprite);
        sprite->setPosition(oldSprite->getPosition());
        tagSprite->addChild(sprite,1,newTag);
        texture->release();
    }*/
}

bool HttpSprite::onTouchBegan(Touch *touch, Event *unused_event)
{
    Rect rect(0, 0, this->getContentSize().width, this->getContentSize().height);
    Vec2 pos = this->convertToNodeSpace(touch->getLocation());
    if(rect.containsPoint(pos))
    {
        return true;
    }
    return	false;
}

void HttpSprite::onTouchEnded(Touch *touch, Event *unused_event)
{
    Size winSize = Director::getInstance()->getWinSize();
    
    if (_clickSpriteFunc != nullptr) {
        _clickSpriteFunc();
    }
}

void HttpSprite::onClickSpriteCallBack(std::function<void ()> callfunc){
    _clickSpriteFunc = callfunc;
}

HttpSprite::~HttpSprite(void)
{
    _clickSpriteFunc = nullptr;
}