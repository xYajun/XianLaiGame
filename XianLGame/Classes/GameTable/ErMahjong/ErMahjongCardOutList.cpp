#include "ErMahjongCardOutList.h"
#include "ErMahjongCard.h"
#include "ErMahjongGameTableUI.h"

namespace ErMahjong
{

#define ErX_OUT_OFFSET 29.0f

#define MJ_BOTTOM_W     58          //下方麻将宽度
#define MJ_BOTTOM_H     84          //下方麻将高度
    
#define MJ_RIGHT_W      44
#define MJ_RIGHT_H      37
    
#define MJ_TOP_W        50
#define MJ_TOP_H        50
    
#define MJ_LEFT_W       44
#define MJ_LEFT_H       37
    
#define MJ_BOTTOM_SCALE    0.6f
#define MJ_TOP_SCALE    0.6f
#define MJ_LEFT_SCALE 0.8f
    
#define OUT_CARD_KUANG 8785
#define OUT_CARD_ZHISHI 8786
    
    
//////////////////////////////////////////////////////////////////////////

	CardOutList* CardOutList::create(BYTE seatNo)
	{
		CardOutList* cardList = new CardOutList();
		if(cardList->init(seatNo))
		{
			cardList->autorelease();
			return cardList;
		}
		CC_SAFE_DELETE(cardList);
		return nullptr;
	}

	bool CardOutList::init(BYTE seatNo)
	{
		if(!YZLayer::init())
		{
			return false;
		}	
		this->ignoreAnchorPointForPosition(false);

        setMJBoardDirection(static_cast<ErMahjong::MJPostion>(seatNo));
        
        cardSps = *new Vector<Sprite *>();
        
        this->initBoards();
        
		return true;
	}

    void CardOutList::setMJBoardDirection(MJPostion direction){
        _direction = direction;
    }
    
    void CardOutList::initBoards(){
    
        Size size = Director::getInstance()->getWinSize();
        
        switch (_direction) {
            case ErMahjong::MJPostion::Bottom :
            {
                this->setContentSize(Size(size.width , MJ_BOTTOM_H));
                for (int i = 0; i < MAX_COUNT * 2; i++) {
                    
                    int hang = i >= MAX_COUNT ? 1 : 0;
                    int index = i >= MAX_COUNT ? i - MAX_COUNT : i;
                    
                    _cardPoint[i] = Vec2(size.width * 0.3f + MJ_BOTTOM_W * MJ_BOTTOM_SCALE * (index+1), this->getPositionY() + MJ_BOTTOM_H/2 + MJ_BOTTOM_H/1.5f * hang);
                    /*
                    //FOR TEST...
                    Sprite * sp = Sprite::create("erMaJiang/outCard/buttom_top/w_1.png");
                    sp->setPosition(_cardPoint[i]);
                    sp->setScale(MJ_BOTTOM_SCALE);
                    this->addChild(sp,999);
                     */
                }
            }
                break;
                
            case ErMahjong::MJPostion::Left :
            {
                int oneCount = 11;
                
                for (int i = 0; i < oneCount * 2; i++) {
                    
                    int hang = i >= oneCount ? 1 : 0;
                    int index = i >= oneCount ? i - oneCount : i;
                    
                    _cardPoint[i] = Vec2(this->getPositionX() + MJ_LEFT_W * hang ,size.height * 0.8f - MJ_LEFT_H * MJ_LEFT_SCALE * (index + 1));
                    /*
                    //FOR TEST...
                    Sprite * sp = Sprite::create("erMaJiang/outCard/left/w_1.png");
                    sp->setPosition(_cardPoint[i]);
                    sp->setScale(MJ_LEFT_SCALE);
                    this->addChild(sp,999);
                     */
                }
                this->setContentSize(Size(MJ_LEFT_W , size.height));
            }
                break;
                
            case ErMahjong::MJPostion::Top :
            {
                for (int i = 0; i < MAX_COUNT *2; i++) {
                    
                    int hang = i >= MAX_COUNT ? 1 : 0;
                    int index = i >= MAX_COUNT ? i - MAX_COUNT : i;
                    
                    _cardPoint[i] = Vec2(size.width * 0.83f - MJ_TOP_W * MJ_TOP_SCALE * (index + 1), this->getPositionY() - MJ_TOP_H * hang );
                    
                    /*
                    //FOR TEST...
                    Sprite * sp = Sprite::create("erMaJiang/outCard/buttom_top/w_1.png");
                    sp->setPosition(_cardPoint[i]);
                    sp->setScale(MJ_TOP_SCALE);
                    sp->setColor(Color3B::GREEN);
                    this->addChild(sp,999);
                     */
                }
                this->setContentSize(Size(size.width , MJ_TOP_H));
            }
                break;
                
            case ErMahjong::MJPostion::Right :
            {
                int oneCount = 11;
                
                for (int i = 0; i < oneCount * 2; i++) {
                    int hang = i >= oneCount ? 1 : 0;
                    int index = i >= oneCount ? i - oneCount : i;
                    
                     _cardPoint[i] = Vec2(this->getPositionX() + MJ_RIGHT_W * 1.5f - MJ_RIGHT_W* hang ,size.height * 0.2f + MJ_LEFT_H * MJ_LEFT_SCALE * (index + 1));
                    /*
                    //FOR TEST...
                    Sprite * sp = Sprite::create("erMaJiang/outCard/right/w_1.png");
                    sp->setPosition(_cardPoint[i]);
                    sp->setScale(MJ_LEFT_SCALE);
                    this->addChild(sp,999);
                     */
                }
                this->setContentSize(Size(MJ_RIGHT_W , size.height));
                
            }
                break;
        }
        
        /*FOR TEST...
        Vec2 outCardPoint = _cardPoint[0];
        
        switch (_direction) {
            case MJPostion::Bottom:
                outCardPoint = Vec2(size.width/2, _cardPoint[0].y);
                //outCard->setScale(MJ_BOTTOM_SCALE);
                break;
            case MJPostion::Right:
                outCardPoint = Vec2(_cardPoint[0].x , size.height/2);
                //outCard->setScale(MJ_LEFT_SCALE);
                break;
            case MJPostion::Top:
                outCardPoint = Vec2(_cardPoint[7].x, _cardPoint[0].y);
                //outCard->setScale(MJ_TOP_SCALE);
                break;
            case MJPostion::Left:
                outCardPoint = Vec2(_cardPoint[0].x , size.height/2);
                //outCard->setScale(MJ_LEFT_SCALE);
                break;
        }
        
        Sprite* outCard = Sprite::create("erMaJiang/outCard/right/w_1.png");
        outCard->setPosition(outCardPoint);
        outCard->setScale(1.5f);
        outCard->setColor(Color3B::BLUE);
        this->addChild(outCard,999);
     
         */
    }
    
    void CardOutList::setCallFunction(std::function<void(cocos2d::Vec2)> _outCardFinish){
        _outCardEndFunc = _outCardFinish;
    }
    
    void CardOutList::addCard2OutList(MJOutCard card){
        
        _cardsPtr.push_back(card.cardValue);
        
        CCLOG("在出牌列表添加一张牌  : %d" , card.cardValue);
        
        string fileName = this->getCardTextureFileName(card.cardValue, _direction);
        Sprite* outCard = Sprite::create(fileName);
        outCard->setPosition(card.cardPoint);
        outCard->setScale(1.5f);
        this->addChild(outCard,2);
        
        auto outIndexSp = Sprite::create("erMaJiang/anim/pointer/pointer_1.png");
        outIndexSp->setPosition(outCard->getContentSize().width/2 , outCard->getContentSize().height);
        outIndexSp->setTag(OUT_CARD_ZHISHI);
        outIndexSp->setVisible(true);
        outIndexSp->setScale(0.01f);
        outCard->addChild(outIndexSp,20);
        
        Animation* pAnimation = Animation::create();
        for (int i = 1; i <= 6; i++)
        {
            char strImagePath[128] = { 0 };
            sprintf(strImagePath, "erMaJiang/anim/pointer/pointer_%d.png", i);
            pAnimation->addSpriteFrameWithFile(strImagePath);
        }
        pAnimation->setDelayPerUnit(1.0f / 10.0f);
        pAnimation->setRestoreOriginalFrame(true);
        
        Animate* pAnimate = Animate::create(pAnimation);
        outIndexSp->runAction(RepeatForever::create(pAnimate));
        outIndexSp->runAction(Sequence::createWithTwoActions(DelayTime::create(1.7f) , CallFunc::create([=]{
            outIndexSp->setScale(1.0f);
        })));
        
        string showFileName = this->getCardTextureFileName(card.cardValue, MJPostion::Bottom);
        outCard->setTexture(showFileName);
        
        Sprite* kuang  = Sprite::create("erMaJiang/show_card_bg.png");
        kuang->setScale(outCard->getContentSize().width / kuang->getContentSize().width + 0.1f, outCard->getContentSize().height / kuang->getContentSize().height + 0.1f);
        kuang->setPosition(outCard->getContentSize().width/2 , outCard->getContentSize().height/2);
        kuang->setTag(OUT_CARD_KUANG);
        outCard->addChild(kuang,-1);
        
        cardSps.pushBack(outCard);
        
        Size size = Director::getInstance()->getWinSize();
        
        Vec2 outCardPoint;
        
        switch (_direction) {
            case MJPostion::Bottom:
                outCardPoint = Vec2(size.width/2, _cardPoint[0].y);
                break;
            case MJPostion::Right:
                outCardPoint = Vec2(_cardPoint[0].x - outCard->getContentSize().height/2, size.height/2);
                break;
            case MJPostion::Top:
                outCardPoint = Vec2(_cardPoint[7].x, _cardPoint[0].y - outCard->getContentSize().height/2);
                break;
            case MJPostion::Left:
                outCardPoint = Vec2(_cardPoint[0].x + outCard->getContentSize().height/2, size.height/2);
                break;
        }
        outCard->runAction(Sequence::create(MoveTo::create(0.1f, outCardPoint),DelayTime::create(1.5f),CallFunc::create([=]{
            string fileName = this->getCardTextureFileName(card.cardValue, _direction);
            outCard->setTexture(fileName);
            outCard->setScale(_direction == MJPostion::Bottom ? MJ_BOTTOM_SCALE : _direction == MJPostion::Right ? MJ_LEFT_SCALE : _direction == MJPostion::Top ? MJ_TOP_SCALE : MJ_LEFT_SCALE);
            
            outCard->setRotation(0);
            
            outCard->removeChildByTag(OUT_CARD_KUANG);
            
            /*
            if (_outCardEndFunc != nullptr) {
                _outCardEndFunc(outCard->convertToWorldSpace(Vec2::ZERO) + outCard->getContentSize()/2);
            }
             */
            
            /*
            if(kuang != NULL)
             
            if (zhishiSp!= NULL) {
                zhishiSp->setVisible(true);
            }*/
            
        }),MoveTo::create(0.3f,
                          _cardPoint[_cardsPtr.size()]),NULL));
    }
    
    void CardOutList::addAllCard2OutListOneTimes(std::vector<BYTE> cards){
        
        if (cards.size() == 0) {
            return;
        }
        
        for (int i = 0; i < cards.size(); i++) {
            _cardsPtr.push_back(cards.at(i));
            
            string fileName = this->getCardTextureFileName(cards.at(i), _direction);
            
            auto card = Sprite::create(fileName);
            card->setPosition(_cardPoint[_cardsPtr.size()]);
            card->setScale(_direction == MJPostion::Bottom ? MJ_BOTTOM_SCALE : _direction == MJPostion::Right ? MJ_LEFT_SCALE : _direction == MJPostion::Top ? MJ_TOP_SCALE : MJ_LEFT_SCALE);
            this->addChild(card,2);
        }
    }
    
    void CardOutList::clear(){
        this->removeAllChildren();
        _cardsPtr.clear();
    }
    
    void CardOutList::removeLastCard(){
        Sprite * sp = cardSps.back();
        sp->removeFromParentAndCleanup(true);
        
        _cardsPtr.pop_back();
    }
    
    void CardOutList::removeLastCardZhiShiSp(){
        
        CCLOG("cardSps size : %lu" , cardSps.size());
        
        if (cardSps.size() == 0) {
            return;
        }
        
        Sprite * sp = cardSps.back();
        sp->removeAllChildren();
    }
    
    /*
    void CardOutList::addCard2OutListEnd(bool isRemove){
        
        if (_lastOutCard == NULL || _lastOutCard == nullptr) {
            return;
        }
        
        if (isRemove) {
            _lastOutCard->removeFromParentAndCleanup(true);
            _cardsPtr.pop_back();
        }else{
            _lastOutCard->setScale(_direction == MJPostion::Bottom ? MJ_BOTTOM_SCALE : _direction == MJPostion::Right ? MJ_LEFT_SCALE : _direction == MJPostion::Top ? MJ_TOP_SCALE : MJ_LEFT_SCALE);
            
            _lastOutCard->runAction(MoveTo::create(0.3f,_cardPoint[_cardsPtr.size()]));
        }
        
        _lastOutCard = nullptr;
    }*/
    
    
	CardOutList::CardOutList()
	{
        _lastOutCard = nullptr;
        _outCardEndFunc = nullptr;
	}

	CardOutList::~CardOutList()
	{
        
        cardSps.clear();
        _cardsPtr.clear();
    }
    
    string CardOutList::getCardTextureFileName(BYTE cardValue,MJPostion pos)
    {
        BYTE cbColor=(cardValue&MASK_COLOR)>>4;
        BYTE cbValue=(cardValue&MASK_VALUE);
        

        char filename[60];
        switch (pos)
        {
            case ErMahjong::Bottom:
            {
                switch (cbColor)
                {
                    case MJColor::Wan:
                        sprintf(filename,"erMaJiang/outCard/buttom_top/w_%d.png",cbValue);
                        break;
                    case MJColor::Tiao:
                        sprintf(filename,"erMaJiang/outCard/buttom_top/tiao_%d.png",cbValue);
                        break;
                    case MJColor::Bing:
                        sprintf(filename,"erMaJiang/outCard/buttom_top/tong_%d.png",cbValue);
                        break;
                    case MJColor::Feng:
                        sprintf(filename,"erMaJiang/outCard/buttom_top/feng_%d.png",cbValue);
                        break;
                }
            }
                break;
            case ErMahjong::Right:
            {
                switch (cbColor)
                {
                    case MJColor::Wan:
                        sprintf(filename,"erMaJiang/outCard/right/w_%d.png",cbValue);
                        break;
                    case MJColor::Tiao:
                        sprintf(filename,"erMaJiang/outCard/right/tiao_%d.png",cbValue);
                        break;
                    case MJColor::Bing:
                        sprintf(filename,"erMaJiang/outCard/right/tong_%d.png",cbValue);
                        break;
                    case MJColor::Feng:
                        sprintf(filename,"erMaJiang/outCard/right/feng_%d.png",cbValue);
                        break;
                }
            }                break;
            case ErMahjong::Top:
            {
                switch (cbColor)
                {
                    case MJColor::Wan:
                        sprintf(filename,"erMaJiang/outCard/buttom_top/w_%d.png",cbValue);
                        break;
                    case MJColor::Tiao:
                        sprintf(filename,"erMaJiang/outCard/buttom_top/tiao_%d.png",cbValue);
                        break;
                    case MJColor::Bing:
                        sprintf(filename,"erMaJiang/outCard/buttom_top/tong_%d.png",cbValue);
                        break;
                    case MJColor::Feng:
                        sprintf(filename,"erMaJiang/outCard/buttom_top/feng_%d.png",cbValue);
                        break;
                }
            }
                break;
            case ErMahjong::Left:
            {
                switch (cbColor)
                {
                    case MJColor::Wan:
                        sprintf(filename,"erMaJiang/outCard/left/w_%d.png",cbValue);
                        break;
                    case MJColor::Tiao:
                        sprintf(filename,"erMaJiang/outCard/left/tiao_%d.png",cbValue);
                        break;
                    case MJColor::Bing:
                        sprintf(filename,"erMaJiang/outCard/left/tong_%d.png",cbValue);
                        break;
                    case MJColor::Feng:
                        sprintf(filename,"erMaJiang/outCard/left/feng_%d.png",cbValue);
                        break;
                }
            }
                break;
        }
        
        return filename;
    }
    
    
}
