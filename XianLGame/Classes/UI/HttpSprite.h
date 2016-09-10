//
//  HttpSprite.hpp
//  WechatTest
//
//  Created by Dada on 16/7/21.
//
//

#ifndef HttpSprite_h
#define HttpSprite_h

#include "cocos2d.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
using namespace std;
using namespace cocos2d::network;
USING_NS_CC;

class HttpSprite : public cocos2d::Sprite
{
public:
    
    HttpSprite();
    ~HttpSprite();
    
    virtual bool init();
    
    static HttpSprite* create(std::string url , std::string normal);
    
    //获取网络图片，需要地址
    static void GetHttpImg(Ref* obj, string imgurl,int number);
    
    void onHttpRequestRptImg(HttpClient *sender, HttpResponse *response);
    
    //按原来默认头像比例设置新图片大小
    void resetImgSize(float oldhead_w,float oldhead_h);
    
    void setImgSize(Size imgSize);
    
    //创建图片覆盖原有图片
    static void CreateImg(Sprite* tagSprite,Sprite* oldSprite,Texture2D* texture,int newTag);
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    
    void onClickSpriteCallBack(std::function<void()> callfunc);
    
private:
    Texture2D* texture;
    
    float m_iHeadWidth;
    float m_iHeadHeight;
    
    std::function<void()> _clickSpriteFunc;
};
#endif /* HttpSprite_hpp */
