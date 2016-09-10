#include "ErMahjongCardListBoard.h"
#include "ErMahjongGameTableUI.h"
#include "Util/JzLog.h"
#include <algorithm>

using namespace YZ;

/*
 * 玩家出牌操作面板
 * Tag用来记录牌是否被点击
 * Name用来记录牌是否被提起
 */

#define MJ_BOTTOM_W     58          //下方麻将宽度
#define MJ_BOTTOM_H     84          //下方麻将高度

#define MJ_RIGHT_W      24    
#define MJ_RIGHT_H      59

#define MJ_TOP_W        50
#define MJ_TOP_H        64

#define MJ_LEFT_W       24
#define MJ_LEFT_H       59

#define MJ_W_MOVE_SPEED 350

#define ErName_Up        "up"
#define ErName_Down      "down"

#define ERUP_OFFSET				25.0f

#define ErTag_Touched    1      //可以点击的牌
#define ErTag_GangCard   2      //杠牌
#define ErTag_GangDCard  3      //杠牌中堆在上面的牌
#define ErTag_AnGuangCard 6     //暗杠
#define ErTag_AnGuangDCard  7     //杠牌中堆在上面的牌

#define ErTag_PengCard   4      //碰牌
#define ErTag_ChiCard    5      //吃牌


#define ErZorder_Card_Began 1
#define ErZorder_Touch      100


static float send_card_time		=	0.5f;

namespace ErMahjong
{
    CardListBoard* CardListBoard::create(bool isMySelf, BYTE seatNo)
    {
        CardListBoard* action = new CardListBoard();
        if(action->init(isMySelf, seatNo))
        {
            action->autorelease();
            return action;
        }
        CC_SAFE_DELETE(action);
        return nullptr;
    }
    
    bool CardListBoard::init(bool isMySelf, BYTE seatNo)
    {
        if(!YZLayer::init())
        {
            return false;
        }
        this->ignoreAnchorPointForPosition(false);
        this->setAnchorPoint(Vec2(0.5f, 0.5f));
        
        setMJBoardDirection(static_cast<ErMahjong::MJPostion>(seatNo));
        this->initBoards();
        
        _isMySelf = isMySelf;
        _seatNo = seatNo;
        
        outCardLine = NULL;
        
        if(_isMySelf)
        {
            auto listener = EventListenerTouchOneByOne::create();
            listener->onTouchBegan = CC_CALLBACK_2(CardListBoard::onTouchBegan, this);
            listener->onTouchMoved = CC_CALLBACK_2(CardListBoard::onTouchMoved, this);
            listener->onTouchEnded = CC_CALLBACK_2(CardListBoard::onTouchEnded, this);
            listener->setSwallowTouches(true);
            this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        }
                
        return true;
    }

    void CardListBoard::initBoards(){
    
        Size size = Director::getInstance()->getWinSize();
        
        switch (_direction) {
            case ErMahjong::MJPostion::Bottom :
            {
                this->setContentSize(Size(size.width , MJ_BOTTOM_H));

                for (int i = 0; i < MAX_COUNT; i++) {
                    
                    if (i == MAX_COUNT - 1) {
                        _cardPoint[i] = Vec2(MJ_BOTTOM_W/2 + MJ_BOTTOM_W * (i+1) + MJ_BOTTOM_W * 1.5f, this->getPositionY());
                    }else{
                        _cardPoint[i] = Vec2(MJ_BOTTOM_W/2 + MJ_BOTTOM_W * (i+1) + MJ_BOTTOM_W, this->getPositionY());
                    }
                    
                    if (i % 3 == 0) {
                        int index = i /3 ;
                        _actionPoint[index] = Vec2(MJ_BOTTOM_W * (i+1) + MJ_BOTTOM_W / 2 * index , this->getPositionY());
                    }
                    
                }
            }
                break;
                
            case ErMahjong::MJPostion::Left :
            {
                for (int i = 0; i < MAX_COUNT; i++) {
                    
                    if (i == MAX_COUNT - 1) {
                        _cardPoint[i] = Vec2(this->getPositionX(),size.height * 0.85f - MJ_LEFT_H/2 * (i + 1) - MJ_LEFT_H/2);
                    }else{
                        _cardPoint[i] = Vec2(this->getPositionX(),size.height * 0.85f - MJ_LEFT_H/2 * (i + 1));
                    }
            
                    if (i % 3 == 0) {
                        int index = i /3 ;
                        _actionPoint[index] = Vec2(this->getPositionX(), size.height * 0.88f - MJ_LEFT_H/2 * (i + 1));
                    }
                }
            
                this->setContentSize(Size(MJ_LEFT_W , size.height));
            }
                break;
                
            case ErMahjong::MJPostion::Top :
            {
                for (int i = 0; i < MAX_COUNT; i++) {
                    
                    if (i == MAX_COUNT - 1) {
                        _cardPoint[i] = Vec2(size.width * 0.94f - MJ_TOP_W * (i + 1) - MJ_TOP_W / 2, size.height/2 - 60);
                    }else{
                        _cardPoint[i] = Vec2(size.width * 0.94f - MJ_TOP_W * (i + 1), size.height/2 - 60);
                    }
                    
                    if (i % 3 == 0) {
                        int index = i /3 ;
                        _actionPoint[index] = Vec2(size.width * 0.93f - MJ_TOP_W * (i + 1), this->getPositionY());
                    }
                }
                //this->setContentSize(Size(size.width , MJ_TOP_H));
            }
                break;
                
            case ErMahjong::MJPostion::Right :
            {
                for (int i = 0; i < MAX_COUNT; i++) {
                    if (i == MAX_COUNT - 1) {
                        _cardPoint[i] = Vec2(this->getPositionX(),size.height * 0.13f +  MJ_RIGHT_H /2 * (i + 1) + MJ_RIGHT_H/2);
                    }else{
                        _cardPoint[i] = Vec2(this->getPositionX(),size.height * 0.13f +  MJ_RIGHT_H /2 * (i + 1));
                    }
                    
                    if (i % 3 == 0) {
                        int index = i /3 ;
                        _actionPoint[index] = Vec2(this->getPositionX(), size.height * 0.07f +  MJ_RIGHT_H /2 * (i + 1));
                    }
                    
                }
                this->setContentSize(Size(MJ_RIGHT_W , size.height));
                
            }
                break;
        }
        
    }
    
    void CardListBoard::addCards(const std::vector<BYTE>& values)
    {
        if(values.empty())
        {
            return;
        }
        
        unschedule(schedule_selector(CardListBoard::scheduleRun));
        _undoList.clear();
        
        _undoList = values;
        schedule(schedule_selector(CardListBoard::scheduleRun), send_card_time);
    }
    
    
    void CardListBoard::scheduleRun(float delta)
    {
        if (_undoList.empty())
        {
            unschedule(schedule_selector(CardListBoard::scheduleRun));
            _initCardFinishFunc(_direction);
        }
        else
        {
            
            int showCount = 4;
            
            if(showCount > _undoList.size())
            {
                showCount = (int)_undoList.size();
            }
            
            for (size_t i = 0 ; i < showCount; i++) {
                addCard(_undoList[0]);
                _undoList.erase(_undoList.begin());
            }
        }
    }
    
    void CardListBoard::addOneCard(BYTE value){

        setCanOutCard(true);
        addCard(value);
    }
    
    int getRand(int start,int end)
    {
        float i = CCRANDOM_0_1()*(end-start+1)+start;  //产生一个从start到end间的随机数
        return (int)i;
    }
    
    void CardListBoard::addCard(BYTE value)
    {
        MahjongCard* card = this->createMahjongCard(value);
       
        if(card != nullptr)
        {
            addChild(card);
            _cardsPtr.push_back(card);
        }
    }
    
    void CardListBoard::addAllCardOneTimes(std::vector<BYTE> cards){
        for (int i = 0; i < cards.size(); i++) {
            this->addCard(cards.at(i));
        }
    }
    
    void CardListBoard::setCaiShenCard(BYTE card){
        _caiShenCard = card;
    }
    
    MahjongCard * CardListBoard::createMahjongCard(BYTE value){
        MahjongCard* card = MahjongCard::create(value,_direction);
        if(card != nullptr)
        {
            if(_direction == MJPostion::Bottom || _direction == MJPostion::Top){
                card->setCardScaleValue(_isMySelf ? MJ_BOTTOM_W / card->getContentSize().width : MJ_TOP_W / card->getContentSize().width);
            }else{
                card->setCardScaleValue(1.0f);
                //card->setScale(MJ_RIGHT_H / card->getContentSize().height);
            }
            card->setAnchorPoint(Vec2::ZERO);
            card->setPosition(_cardPoint[_cardsPtr.size()]);
            
            if (_direction == MJPostion::Right) {
                card->setLocalZOrder(MAX_COUNT - (int)_cardsPtr.size());
            }else{
                card->setLocalZOrder((int)_cardsPtr.size());
            }

            card->setCaiShenCardValue(_caiShenCard);
            card->setName(ErName_Down);
            card->setTag(ErTag_Touched);
        }
        return card;
    }
    
    
    void CardListBoard::checkAllCardPoints(){
        
        if (_outCardIndex == -1) {
            CCLOG("Error... Player out card index is -1...");
            return;
        }
        
        if (_outCardIndex == MAX_COUNT - 1) {
            CCLOG("玩家打出的是最后摸的牌...");
            _outCardIndex = -1;
            return;
        }
        
        
        MahjongCard * lastCard = _cardsPtr.back();
        int cardNeed2Index = -1;
        
        if (_isMySelf) {
            
            if (lastCard->getCardValue() == _caiShenCard) {
                cardNeed2Index = _cardStartIndex;
            }else{
                for (int i = _cardStartIndex; i < _cardsPtr.size() - 1; i++) {
                    MahjongCard * tmpCard = _cardsPtr[i];
                    if (lastCard->afterMJCard(tmpCard)) {
                        cardNeed2Index = i;
                        break;
                    }
                }
            }
        }else{
            
            cardNeed2Index = _outCardIndex;
            
            if (_outCardIndex < _cardStartIndex) {
                cardNeed2Index = _cardStartIndex;
            }
            
        }
        
        if (cardNeed2Index == -1) {
            cardNeed2Index = MAX_COUNT - 2;
            //CCLOG("Error 2 find card need to index...");
            //return;
        }
        
        CCLOG("检查所有牌的位置  出牌位置 ： %d   牌需要去的位置 : %d" , _outCardIndex , cardNeed2Index);
        
        
        if (_isMySelf) {
            if (_outCardIndex == cardNeed2Index  && cardNeed2Index != MAX_COUNT - 2) {
                MahjongCard * lastCard = _cardsPtr.back();
                this->cardMove2Index(lastCard, cardNeed2Index);
            }else{
                if (cardNeed2Index == MAX_COUNT - 2) {
                    JzDebug("需要移动到最后面...");
                    for (int i =  cardNeed2Index ; i >= _outCardIndex; i--) {
                        
                        MahjongCard * tmpCard = _cardsPtr[i];
                        
                        Vec2 toPoint = _cardPoint[i];
                        int length = abs(tmpCard->getPositionX() - toPoint.x);
                        
                        tmpCard->runAction(Sequence::create(MoveTo::create(length/MJ_W_MOVE_SPEED, toPoint),NULL));
                    }
                    
                }else if(cardNeed2Index == _cardStartIndex){
                    
                    JzDebug("出牌位置 : %d..   需要去的位置 : %d..", _outCardIndex , cardNeed2Index);

                    _needMoveCount = abs(_outCardIndex - cardNeed2Index);
                    
                    JzDebug("需要移动到的数量 : %d.." ,_needMoveCount );
                    
                    auto fun = CallFunc::create([=]{
                        _needMoveCount --;
                        if (_needMoveCount <= 0) {
                            
                            MahjongCard * lastCard = _cardsPtr.back();
                            Vec2 toPoint = _cardPoint[cardNeed2Index];
                            //int length = abs(_cardPoint[MAX_COUNT - 1].x - toPoint.x);
                            lastCard->runAction(Sequence::create(MoveTo::create(0.1f,lastCard->getPosition() + Vec2(0, MJ_BOTTOM_H)),MoveTo::create(0.8f, toPoint + Vec2(0, MJ_BOTTOM_H)),MoveTo::create(0.1f, toPoint),CallFunc::create([=]{
                                _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, _cardsPtr.back());
                                _cardsPtr.pop_back();
                                
                            }),NULL));
                        }
                    });
                    
                    for (int i =  _outCardIndex - 1 ; i >= _cardStartIndex ; i--) {
                        
                        MahjongCard * tmpCard = _cardsPtr[i];
                        
                        Vec2 toPoint = _cardPoint[i + 1];
                        int length = abs(tmpCard->getPositionX() - toPoint.x);
                        
                        tmpCard->runAction(Sequence::create(MoveTo::create(length/MJ_W_MOVE_SPEED, toPoint),fun,NULL));
                    }
                } else{
                    
                    if(_outCardIndex < cardNeed2Index){
                        
                        _needMoveCount = abs(cardNeed2Index - _outCardIndex);
                        
                        JzDebug("出牌位置在需要位置的前方....   需要移动%d个麻将牌....", _needMoveCount);
                        
                        JzDebug("出牌位置 : %d..   需要去的位置 : %d..", _outCardIndex , cardNeed2Index);
                        
                        auto fun = CallFunc::create([=]{
                            _needMoveCount --;
                            if (_needMoveCount == 0) {
                                MahjongCard * lastCard = _cardsPtr.back();
                                this->cardMove2Index(lastCard, cardNeed2Index);
                            }
                        });
                        
                        for (int i =  cardNeed2Index - 1 ; i >= _outCardIndex ; i--) {
                            
                            MahjongCard * tmpCard = _cardsPtr[i];
                            
                            Vec2 toPoint = _cardPoint[i];
                            int length = abs(tmpCard->getPositionX() - toPoint.x);
                            
                            tmpCard->runAction(Sequence::create(MoveTo::create(length/MJ_W_MOVE_SPEED, toPoint),fun,NULL));
                        }
                        
                    }else{
                        //如果空位在需要的后方  从needIndex开始 到 空 Index - 1 都向后移动一个位置
                        //需要移动的位置
                        
                        _needMoveCount = abs(_outCardIndex - cardNeed2Index) ;
                        JzDebug("出牌位置在需要位置的后方....   需要移动%d个麻将牌....", _needMoveCount);
                        
                        JzDebug("出牌位置 : %d..   需要去的位置 : %d..", _outCardIndex , cardNeed2Index);

                        auto fun = CallFunc::create([=]{
                            _needMoveCount --;
                            if (_needMoveCount == 0) {
                                
                                MahjongCard * lastCard = _cardsPtr.back();
                                this->cardMove2Index(lastCard, cardNeed2Index);
                            }
                        });
                        
                        for (int i =  _outCardIndex - 1; i >= cardNeed2Index; i--) {
                            
                            MahjongCard * tmpCard = _cardsPtr[i];
                            
                            Vec2 toPoint = _cardPoint[i + 1];
                            int length = abs(tmpCard->getPositionX() - toPoint.x);
                            
                            tmpCard->runAction(Sequence::create(MoveTo::create(length/MJ_W_MOVE_SPEED, toPoint),fun,NULL));
                        }
                    }
                }
            }
            
            
        }else{
            MahjongCard * lastCard = _cardsPtr.back();
            this->cardMove2Index(lastCard, cardNeed2Index);
        }
     
        
        CCLOG("===================================================");
    }
    
    void CardListBoard::cardMove2Index(MahjongCard* card ,int index){
        Vec2 toPoint = _cardPoint[index];
        int length = abs(_cardPoint[MAX_COUNT - 1].x - toPoint.x);
        
        Vec2 offset;
        
        switch (_direction) {
            case MJPostion::Bottom:
                offset = Vec2(0 , MJ_BOTTOM_H);
                break;
            case MJPostion::Right:
                offset = Vec2( - MJ_RIGHT_W , 0);
                break;
            case MJPostion::Top:
                offset = Vec2(0 , - MJ_BOTTOM_H);
                break;
            case MJPostion::Left:
                offset = Vec2(MJ_RIGHT_W , 0);
                break;
        }
        
        card->runAction(Sequence::create(MoveTo::create(0.1f, card->getPosition() + offset),MoveTo::create(0.7f, toPoint + offset),MoveTo::create(0.1f, toPoint),CallFunc::create([=]{
            _cardsPtr.insert(_cardsPtr.begin() + index, card);
            _cardsPtr.pop_back();
            //this->resetAllCardPoint();
        }),NULL));
    }
    
    void CardListBoard::setMJBoardDirection(MJPostion direction){
        _direction = direction;
    }
    
    
    bool CardListBoard::onTouchBegan(Touch* touch, Event *event)
    {
        Vec2 pos = this->convertToNodeSpace(touch->getLocation());
        
        Rect rect(0,0, this->getContentSize().width, this->getContentSize().height);
        
        if(rect.containsPoint(pos))
        {

            int nowTouchIndex = touchCheck(pos);
            if(nowTouchIndex >= 0 && nowTouchIndex < MAX_COUNT){
                
                if (_upCardIndex != -1) {
                    this->downCard(_cardsPtr[_upCardIndex]);
                }

                this->upCard(_cardsPtr[nowTouchIndex]);
              
                _upCardIndex = nowTouchIndex;
                _outCardIndex  = nowTouchIndex;
                
                return true;
            }
            
            return false;
        }
        else
        {
            if (_upCardIndex != -1) {
                this->downCard(_cardsPtr[_upCardIndex]);
            }
            _upCardIndex = -1;
            return false;
        }
    }
    
    void CardListBoard::onTouchMoved(Touch* touch, Event *event)
    {
        Vec2 pos = this->convertToNodeSpace(touch->getLocation());
        _cardsPtr[_outCardIndex]->setPosition(pos);
    }
    
    void CardListBoard::onTouchEnded(Touch* touch, Event *event)
    {
        Vec2 pos = this->convertToNodeSpace(touch->getLocation());

        if (pos.y > 100 && _canOut) {
            
            setCanOutCard(false);
            
            _upCardIndex = -1;

            BYTE outCardValue = _cardsPtr[_outCardIndex]->getCardValue();

            if (_outCardFunc!= nullptr) {
                _outCardFunc(outCardValue);
            }
        }else{
            _cardsPtr[_outCardIndex]->setPosition(_cardPoint[_outCardIndex] + Vec2(0, ERUP_OFFSET));
            
            this->downCard(_cardsPtr[_upCardIndex]);
            
            _upCardIndex = -1;
        }
    }

    void CardListBoard::playerOutCard(BYTE cardValue){
        _outCardIndex = getRand(_cardStartIndex, _cardsPtr.size() - 1);
        _cardsPtr[_outCardIndex]->setCardValue(cardValue);
        this->outCard(cardValue);
    }
    
    int  CardListBoard::getCardCount(MahjongCard* card){
        int count = 0;
        for (int i = 0; i< _cardsPtr.size(); i++) {
            if (_cardsPtr.at(i)->getCardValue() == card->getCardValue()) {
                count++;
            }
        }
        return count;
    }
 
    
    BYTE CardListBoard::getGuangCardValue(){
        
        for (int i = 0; i< _cardsPtr.size(); i++) {
            int cardCount = getCardCount(_cardsPtr.at(i));
            JzDebug("cardCount : %d" , cardCount);
            if (cardCount > 3) {
                return _cardsPtr.at(i)->getCardValue();
            }
            /*
            if (((_cardsPtr[i]->getCardValue()==_cardsPtr[i+1]->getCardValue()))&&((_cardsPtr[i]->getCardValue()==_cardsPtr[i+2]->getCardValue()))&&((_cardsPtr[i]->getCardValue()==_cardsPtr[i+3]->getCardValue())))
            {
                return _cardsPtr[i]->getCardValue();
            }*/
        }
        
        return 0x00;
    }

    
    void CardListBoard::outCard(BYTE cardValue){
        
        CCLOG("===================================================");
        
        CCLOG("出牌...");
        
        if (_outCardIndex < 0 || _outCardIndex > MAX_COUNT) {
            _outCardIndex = this->getCardIndex(cardValue);
        }
        
        if (_outCardIndex == -1) {
            JzLog::shareLog()->log(kLogLevelError, "出牌错误。。 牌值 ：%d , 索引 : %d" , cardValue , _outCardIndex);
            return;
        }
        
        MahjongCard * outCard = _cardsPtr[_outCardIndex];
        
        _outCardValue.seatNo = _seatNo;
        _outCardValue.cardValue = outCard->getCardValue();
        _outCardValue.cardPoint = this->getPosition();
        
        MahjongCard * lastCard = _cardsPtr.back();
        
        lastCard->setLocalZOrder(outCard->getLocalZOrder());
        
        vector< MahjongCard*>::iterator k = remove(_cardsPtr.begin(),_cardsPtr.end(),outCard);
        
        _cardsPtr.erase(k);
        
        outCard->removeFromParentAndCleanup(true);
        
        if (_callfunc != nullptr) {
            _callfunc(this);
        }
        
        this->checkAllCardPoints();
    }
    
    int CardListBoard::touchCheck(const Vec2& pos)
    {
        int touchIndex = -1;
        
        for(int i = 0; i < _cardsPtr.size(); i++)
        {
            Rect rect = _cardsPtr[i]->getBoundingBox();
            if(rect.containsPoint(pos))
            {
                //判断麻将是否可以点击
                MahjongCard * card = _cardsPtr[i];
                if (card -> getTag() == ErTag_Touched) {
                    touchIndex = i;
                    break;
                }
            }
        }
        return touchIndex;
    }
    
    void CardListBoard::upCard(ErMahjong::MahjongCard *card){
        if(card->getName().compare(ErName_Down) == 0)
        {
            card->setName(ErName_Up);
            card->stopAllActions();
            card->runAction(MoveTo::create(0.05f, Vec2(card->getPositionX(), ERUP_OFFSET)));
        }

    }
    
    void CardListBoard::downCard(ErMahjong::MahjongCard *card){
        if(card != NULL && card->getName().compare(ErName_Up) == 0)
        {
            card->setName(ErName_Down);
            card->stopAllActions();
            card->runAction(MoveTo::create(0.05f, Vec2(card->getPositionX(), 0)));
            //card->runAction(Sequence::create(DelayTime::create(0.01f), MoveTo::create(0.05f, Vec2(card->getPositionX(), 0)), nullptr));
        }
    }
    
    
    int CardListBoard::getCardIndex(BYTE card){
        for (int i = 0; i < _cardsPtr.size(); i++) {
            if (_cardsPtr[i]->getTag()== ErTag_Touched && _cardsPtr[i]->getCardValue() == card) {
                return i;
            }
        }
        return -1;
    }
    
    
    int CardListBoard::getPengCardIndex(BYTE card){
        for (int i = 0; i < _cardStartIndex; i++) {
            
            if (_cardsPtr[i]->getTag()== ErTag_PengCard && _cardsPtr[i]->getCardValue() == card) {
                return i;
            }
        }
        return -1;
    }

    
    
    Vec2 CardListBoard::getTwoCardLength(){
    
        Vec2 length;
        
        switch (_direction) {
            case MJPostion::Bottom:
                length = Vec2(- MJ_BOTTOM_W / 4, 0);
                break;
            case MJPostion::Right:
                length = Vec2(0, -MJ_RIGHT_H/4);
                break;
            case MJPostion::Top:
                length = Vec2(MJ_TOP_W/4, 0);
                break;
            case MJPostion::Left:
                length = Vec2(0, MJ_LEFT_H/4);
                break;
        }
    
        return length;
    }
    
    bool CardListBoard::checkIsPengCard(BYTE card){
        return this->getPengCardIndex(card) != -1;
    }
    
    
    std::vector<int>  CardListBoard::getCardValueAllIndex(BYTE cardValue){
        std::vector<int> indexs;
        indexs.clear();
        for (int i = 0; i<_cardsPtr.size(); i++) {
            if (_cardsPtr.at(i)->getCardValue() == cardValue) {
                indexs.push_back(i);
            }
        }
        
        return indexs;
    }
    
    void CardListBoard::doUserOperateCard(bool bDarkGuang ,ActionType actionType , BYTE card){

        JzLog::shareLog()->log(kLogLevelDebug, "====================玩家动作处理开始=========================");
        
        JzLog::shareLog()->log(kLogLevelDebug, "收到玩家动作结果信息... 动作类型 : %d  动作牌值 : %d " ,actionType , card );
        if (_isMySelf) {
            switch (actionType) {
                case ActionType::WIK_PENG:  //碰
                {
                    JzLog::shareLog()->log(kLogLevelDebug, "我的自己的碰牌操作...");
                    //同类型的牌的位置
                    int startIndex = getCardIndex(card);
                    
                    if (startIndex == -1) {
                        CCLOG("Not Find Peng Card...");
                        return;
                    }
                    
                    MahjongCard * card1 = _cardsPtr[startIndex];
                    MahjongCard * card2 = _cardsPtr[startIndex + 1];
                    MahjongCard * card3 = createMahjongCard(card);
                    
                    
                    card1->setCardValue(card);
                    card2->setCardValue(card);
                    card3->setCardValue(card);
                    
                    card1->showCardWinTexture();
                    card2->showCardWinTexture();
                    card3->showCardWinTexture();
                    
                    card1->setTag(ErTag_PengCard);
                    card2->setTag(ErTag_PengCard);
                    card3->setTag(ErTag_PengCard);
                    
                    //card3 --> 0  card1 --> 1  card2 --> 2
                    
                    vector< MahjongCard*>::iterator k1 = remove(_cardsPtr.begin(),_cardsPtr.end(),card1);
                    
                    _cardsPtr.erase(k1);
                    
                    vector< MahjongCard*>::iterator k2 = remove(_cardsPtr.begin(),_cardsPtr.end(),card2);
                    
                    _cardsPtr.erase(k2);
                    
                    _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card1);
                    _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card2);
                    _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card3);
                    addChild(card3);
                    
                    _cardStartIndex += 3;
                    this->resetAllCardPoint();
                }
                    break;
                    
                case ActionType::WIK_GANG:  //杠
                {
                    
                    JzLog::shareLog()->log(kLogLevelDebug, "我的自己的杠牌操作...");
                    //这里要先处理碰牌
      
                    if (bDarkGuang) {
                        //如果是暗杠  1.首先判断是否4连  ， 否则最后摸到的牌则为第四张
                        std::vector<int> cardIndexs = this->getCardValueAllIndex(card);
                        
                        if (cardIndexs.size() != 4) {
                            jzError("没有找到可以暗杠的牌... 牌值为 : %d" , card);
                        }
                        
                        MahjongCard * card1 = _cardsPtr[cardIndexs.at(0)];
                        MahjongCard * card2 = _cardsPtr[cardIndexs.at(1)];
                        MahjongCard * card3 = _cardsPtr[cardIndexs.at(2)];
                        MahjongCard * card4 = _cardsPtr[cardIndexs.at(4)];
                        
                        card1->setCardValue(card);
                        card2->setCardValue(card);
                        card3->setCardValue(card);
                        
                        card1->setCardAnGuangTexture();
                        card2->setCardAnGuangTexture();
                        card3->setCardAnGuangTexture();
                        
                        card1->setTag(ErTag_AnGuangCard);
                        card2->setTag(ErTag_AnGuangDCard);
                        card3->setTag(ErTag_AnGuangCard);
                        
                        vector< MahjongCard*>::iterator k1 = remove(_cardsPtr.begin(),_cardsPtr.end(),card1);
                        
                        _cardsPtr.erase(k1);
                        
                        vector< MahjongCard*>::iterator k2 = remove(_cardsPtr.begin(),_cardsPtr.end(),card2);
                        
                        _cardsPtr.erase(k2);
                        
                        vector< MahjongCard*>::iterator k3 = remove(_cardsPtr.begin(),_cardsPtr.end(),card3);
                        
                        _cardsPtr.erase(k3);
                        
                        vector< MahjongCard*>::iterator k4 = remove(_cardsPtr.begin(),_cardsPtr.end(),card4);
                        
                        _cardsPtr.erase(k4);
                        
                        card4->removeFromParentAndCleanup(true);
                        
                        _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card3);
                        _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card2);
                        _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card1);
                        
                        JzLog::shareLog()->log(kLogLevelDebug, "杠牌后，玩家牌的数量 ： %d " , _cardsPtr.size());
                    }else{
                        int startIndex = this->getPengCardIndex(card);
                        
                        if (startIndex != -1) {
                            JzLog::shareLog()->log(kLogLevelDebug, "碰杠操作...");
                            
                            MahjongCard * cardMid = _cardsPtr[startIndex + 1];
                            cardMid->setTag(ErTag_GangDCard);
                            
                            int checkIndex = getCardIndex(card);
                            
                            if (checkIndex != -1) {
                                
                                MahjongCard * card1 = _cardsPtr[checkIndex];
                                
                                vector< MahjongCard*>::iterator k1 = remove(_cardsPtr.begin(),_cardsPtr.end(),card1);
                                
                                _cardsPtr.erase(k1);
                                
                                card1->removeFromParentAndCleanup(true);
                            }
                            
                        }else{
                           //如果是普通杠并且不是碰杠， 则一定是别人打出的杠
                            std::vector<int> cardIndexs = this->getCardValueAllIndex(card);
                            
                            if (cardIndexs.size() != 3) {
                                jzError("没有找到可以拉杠的牌... 牌值为 : %d" , card);
                            }
                            
                            MahjongCard * card1 = _cardsPtr[cardIndexs.at(0)];
                            MahjongCard * card2 = _cardsPtr[cardIndexs.at(1)];
                            MahjongCard * card3 = _cardsPtr[cardIndexs.at(2)];
                            
                            card1->setCardValue(card);
                            card2->setCardValue(card);
                            card3->setCardValue(card);
                            
                            card1->showCardWinTexture();
                            card2->showCardWinTexture();
                            card3->showCardWinTexture();
                            
                            card1->setTag(ErTag_GangCard);
                            card2->setTag(ErTag_GangDCard);
                            card3->setTag(ErTag_GangCard);
                            
                            vector< MahjongCard*>::iterator k1 = remove(_cardsPtr.begin(),_cardsPtr.end(),card1);
                            
                            _cardsPtr.erase(k1);
                            
                            vector< MahjongCard*>::iterator k2 = remove(_cardsPtr.begin(),_cardsPtr.end(),card2);
                            
                            _cardsPtr.erase(k2);
                            
                            vector< MahjongCard*>::iterator k3 = remove(_cardsPtr.begin(),_cardsPtr.end(),card3);
                            
                            _cardsPtr.erase(k3);
                            
                            _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card3);
                            _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card2);
                            _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card1);
                            
                            JzLog::shareLog()->log(kLogLevelDebug, "杠牌后，玩家牌的数量 ： %d " , _cardsPtr.size());
                        }
                    }
                    
                    _cardStartIndex += 3;
                    
                    this->resetAllCardPoint();
                    
                    _gangActionEndFunc(_seatNo,card);
                }
                    break;
                    
                case ActionType::WIK_LEFT:  //左吃
                {
                    
                    BYTE cbValue=(card&MASK_VALUE);
                    
                    if (cbValue > 7) {
                        CCLOG("右吃... 牌的值大于7...");
                        return;
                    }
                    
                    BYTE rightCard = card + 0x01;
                    BYTE rightRightCard = rightCard + 0x01;
                    
                    int rightIndex = getCardIndex(rightCard);
                    int rightRightIndex = getCardIndex(rightRightCard);
                    
                    if (rightIndex == -1 || rightRightIndex == -1) {
                        CCLOG("找不到右边可以吃的牌...  rightIndex: %d ...  rightRightIndex : %d " , rightIndex , rightRightIndex);
                        return;
                    }
                    
                    MahjongCard * card1 = _cardsPtr[rightIndex];
                    MahjongCard * card2 = _cardsPtr[rightRightIndex];
                    MahjongCard * card3 = createMahjongCard(card);
                    
                    card1->showCardWinTexture();
                    card2->showCardWinTexture();
                    card3->showCardWinTexture();
                    
                    card1->setTag(ErTag_ChiCard);
                    card2->setTag(ErTag_ChiCard);
                    card3->setTag(ErTag_ChiCard);
                    
                    //card3 --> 0  card1 --> 1  card2 --> 2
                    
                    vector< MahjongCard*>::iterator k1 = remove(_cardsPtr.begin(),_cardsPtr.end(),card1);
                    
                    _cardsPtr.erase(k1);
                    
                    vector< MahjongCard*>::iterator k2 = remove(_cardsPtr.begin(),_cardsPtr.end(),card2);
                    
                    _cardsPtr.erase(k2);
                    
                    _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card2);
                    _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card1);
                    _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card3);
                    
                    addChild(card3);
                    
                    _cardStartIndex += 3;
                    
                    this->resetAllCardPoint();
                }
                    break;
                case ActionType::WIK_CENTER:  //中吃
                {
                    //同类型的牌的位置
                    
                    BYTE cbValue=(card&MASK_VALUE);
                    
                    if (cbValue < 2 || cbValue > 8) {
                        CCLOG("中吃... 牌的值小于2...");
                        return;
                    }
                    
                    BYTE leftCard = card - 0x01;
                    BYTE rightCard = card + 0x01;
                    
                    
                    int leftIndex = getCardIndex(leftCard);
                    int rightIndex = getCardIndex(rightCard);
                    
                    if (leftIndex == -1 || rightIndex == -1) {
                        CCLOG("找不到左边或者右边可以吃的牌...  leftIndex: %d ...  rightIndex : %d " , leftIndex , rightIndex);
                        return;
                    }
                    
                    MahjongCard * card1 = _cardsPtr[leftIndex];
                    MahjongCard * card2 = _cardsPtr[rightIndex];
                    MahjongCard * card3 = createMahjongCard(card);
                    
                    card1->showCardWinTexture();
                    card2->showCardWinTexture();
                    card3->showCardWinTexture();
                    
                    card1->setTag(ErTag_ChiCard);
                    card2->setTag(ErTag_ChiCard);
                    card3->setTag(ErTag_ChiCard);
                    
                    //card3 --> 0  card1 --> 1  card2 --> 2
                    
                    vector< MahjongCard*>::iterator k1 = remove(_cardsPtr.begin(),_cardsPtr.end(),card1);
                    
                    _cardsPtr.erase(k1);
                    
                    vector< MahjongCard*>::iterator k2 = remove(_cardsPtr.begin(),_cardsPtr.end(),card2);
                    
                    _cardsPtr.erase(k2);
                    
                    _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card2);
                    _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card3);
                    _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card1);
                    addChild(card3);
                    
                    _cardStartIndex += 3;
                    
                    this->resetAllCardPoint();
                }
                    break;
                    
                case ActionType::WIK_RIGHT:  //右吃
                {
                    //同类型的牌的位置
                    
                    //同类型的牌的位置
                    
                    BYTE cbValue=(card&MASK_VALUE);
                    
                    if (cbValue < 3) {
                        CCLOG("左吃... 牌的值小于3...");
                        return;
                    }
                    
                    BYTE leftCard = card - 0x01;
                    BYTE leftLeftCard = leftCard - 0x01;
                    
                    
                    int leftIndex = getCardIndex(leftCard);
                    int leftLeftIndex = getCardIndex(leftLeftCard);
                    
                    if (leftIndex == -1 || leftLeftIndex == -1) {
                        CCLOG("找不到左边可以吃的牌...  leftIndex: %d ...  leftLeftIndex : %d " , leftIndex , leftLeftIndex);
                        return;
                    }
                    
                    MahjongCard * card1 = _cardsPtr[leftIndex];
                    MahjongCard * card2 = _cardsPtr[leftLeftIndex];
                    
                    MahjongCard * card3 = createMahjongCard(card);
                    
                    card1->showCardWinTexture();
                    card2->showCardWinTexture();
                    card3->showCardWinTexture();
                    
                    card1->setTag(ErTag_ChiCard);
                    card2->setTag(ErTag_ChiCard);
                    card3->setTag(ErTag_ChiCard);
                    
                    //card3 --> 0  card1 --> 1  card2 --> 2
                    
                    vector< MahjongCard*>::iterator k1 = remove(_cardsPtr.begin(),_cardsPtr.end(),card1);
                    
                    _cardsPtr.erase(k1);
                    
                    vector< MahjongCard*>::iterator k2 = remove(_cardsPtr.begin(),_cardsPtr.end(),card2);
                    
                    _cardsPtr.erase(k2);
                    
                    _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card3);
                    _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card1);
                    _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card2);
                    addChild(card3);
                    
                    _cardStartIndex += 3;
                    
                    this->resetAllCardPoint();
                }
                    break;
            }
            
            _restrictCard = card;
            setCanOutCard(true);
        }else{
            switch (actionType) {
                case ActionType::WIK_PENG:  //碰
                {
                    MahjongCard * card1 = _cardsPtr[_cardStartIndex];
                    MahjongCard * card2 = _cardsPtr[_cardStartIndex+1];
                    MahjongCard * card3 = createMahjongCard(card);
                    
                    card1->setCardValue(card);
                    card2->setCardValue(card);
                    card3->setCardValue(card);
                    
                    card1->setTag(ErTag_PengCard);
                    card2->setTag(ErTag_PengCard);
                    card3->setTag(ErTag_PengCard);
                    //card3 --> 0  card1 --> 1  card2 --> 2
                    _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card3);
                    addChild(card3);
                    
                    card1->showCardWinTexture();
                    card2->showCardWinTexture();
                    card3->showCardWinTexture();
                    
                    _cardStartIndex += 3;
                    
                    this->resetAllCardPoint();
                }
                    break;
                    
                case ActionType::WIK_GANG:  //杠
                {
                    JzLog::shareLog()->log(kLogLevelDebug, "收到其他玩家的杠牌动作。。 CardValue : %d  是否暗杠 : %d" , card , bDarkGuang);

                    if (!bDarkGuang) {
                        int startIndex = this->getPengCardIndex(card);
                        if (startIndex != -1) {
                            
                            JzLog::shareLog()->log(kLogLevelDebug, "玩家的杠牌动作判断结果为碰杠....");
                            
                            //表示是碰杠
                            MahjongCard * cardMid = _cardsPtr[startIndex + 1];
                            cardMid->setTag(ErTag_GangDCard);
                            
                            //删除最后的牌
                            MahjongCard * card1 = _cardsPtr.back();
                            
                            _cardsPtr.pop_back();
                            
                            card1->removeFromParentAndCleanup(true);
                            
                        }else{
                            
                            JzLog::shareLog()->log(kLogLevelDebug, "玩家的杠牌动作判断结果为明杠....");
                            
                            //表示是明杠
                            MahjongCard * card1 = _cardsPtr[_cardStartIndex];
                            MahjongCard * card2 = _cardsPtr[_cardStartIndex+1];
                            MahjongCard * card3 = _cardsPtr[_cardStartIndex+2];
                            
                            card1->setCardValue(card);
                            card2->setCardValue(card);
                            card3->setCardValue(card);
                            
                            card1->setTag(ErTag_GangCard);
                            card2->setTag(ErTag_GangDCard);
                            card3->setTag(ErTag_GangCard);
                            //card3 --> 0  card1 --> 1  card2 --> 2
                            
                            card1->showCardWinTexture();
                            card2->showCardWinTexture();
                            card3->showCardWinTexture();
                        
                            _cardStartIndex += 3;
                            
                        }
                    }else{
                        
                        JzLog::shareLog()->log(kLogLevelDebug, "玩家的杠牌动作判断结果为暗杠....");
                        
                        //暗杠
                        MahjongCard * card1 = _cardsPtr[_cardStartIndex];
                        MahjongCard * card2 = _cardsPtr[_cardStartIndex+1];
                        MahjongCard * card3 = _cardsPtr[_cardStartIndex+2];
                        
                        card1->setTag(ErTag_AnGuangCard);
                        card2->setTag(ErTag_AnGuangDCard);
                        card3->setTag(ErTag_AnGuangCard);
                        
                        card1->setCardValue(card);
                        card2->setCardValue(card);
                        card3->setCardValue(card);
                        
                        card1->setCardAnGuangTexture();
                        card2->setCardAnGuangTexture();
                        card3->setCardAnGuangTexture();
                        
                        //删除最后的牌
                        MahjongCard * card4 = _cardsPtr.back();
                        
                        _cardsPtr.pop_back();
                        
                        card4->removeFromParentAndCleanup(true);
                        
                        _cardStartIndex += 3;
                        
                    }
                    
                    
                    this->resetAllCardPoint();
                    
                }
                    break;
                    
                case ActionType::WIK_LEFT:  //左吃
                {
                    //同类型的牌的位置
                    
                    BYTE leftCard = card + 0x01;
                    BYTE leftLeftCard = leftCard + 0x01;
                    
                    MahjongCard * card1 = _cardsPtr[_cardStartIndex];
                    MahjongCard * card2 = _cardsPtr[_cardStartIndex+1];
                    MahjongCard * card3 = createMahjongCard(card);
                    
                    card1->setCardValue(leftLeftCard);
                    card2->setCardValue(leftCard);
                    card3->setCardValue(card);
                
                    card1->setTag(ErTag_ChiCard);
                    card2->setTag(ErTag_ChiCard);
                    card3->setTag(ErTag_ChiCard);
                    
                    _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex + 2 , card3);
                    addChild(card3);
                    
                    
                    card1->showCardWinTexture();
                    card2->showCardWinTexture();
                    card3->showCardWinTexture();
                    
                    
                    _cardStartIndex += 3;
                    
                    this->resetAllCardPoint();
                }
                    break;
                case ActionType::WIK_CENTER:  //中吃
                {
                    BYTE leftCard = card - 0x01;
                    BYTE rightCard = card + 0x01;
                    
                    MahjongCard * card1 = _cardsPtr[_cardStartIndex];
                    MahjongCard * card2 = _cardsPtr[_cardStartIndex+1];
                    MahjongCard * card3 = createMahjongCard(card);
                    
                    card1->setCardValue(leftCard);
                    card2->setCardValue(rightCard);
                    card3->setCardValue(card);

                    
                    card1->setTag(ErTag_ChiCard);
                    card2->setTag(ErTag_ChiCard);
                    card3->setTag(ErTag_ChiCard);
                    
                    _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex + 1, card3);
                    addChild(card3);
                    
                    
                    card1->showCardWinTexture();
                    card2->showCardWinTexture();
                    card3->showCardWinTexture();
                    
                    _cardStartIndex += 3;
                    
                    this->resetAllCardPoint();
                }
                    break;
                    
                case ActionType::WIK_RIGHT:  //右吃
                {
                    
                    BYTE rightCard = card - 0x01;
                    BYTE rightRightCard = rightCard - 0x01;
                    
                    MahjongCard * card1 = _cardsPtr[_cardStartIndex];
                    MahjongCard * card2 = _cardsPtr[_cardStartIndex+1];
                    MahjongCard * card3 = createMahjongCard(card);
                    
                    card1->setCardValue(rightCard);
                    card2->setCardValue(rightRightCard);
                    card3->setCardValue(card);
                    
                    card1->setTag(ErTag_ChiCard);
                    card2->setTag(ErTag_ChiCard);
                    card3->setTag(ErTag_ChiCard);
                    
                    _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card3);
                    
                    addChild(card3);
                    
                    card1->showCardWinTexture();
                    card2->showCardWinTexture();
                    card3->showCardWinTexture();

                    _cardStartIndex += 3;
                    
                    this->resetAllCardPoint();
                }
                    break;
            }
        }
        
        JzLog::shareLog()->log(kLogLevelDebug, "====================玩家动作处理结束=========================");
    }
    
    void CardListBoard::addGuangCard(MahjongCard * card){
        JzLog::shareLog()->log(kLogLevelDebug, "玩家一张杠牌。。。");
        
        MahjongCard * gCard = createMahjongCard(card->getCardValue());
        gCard->setCardValue(card->getCardValue());
        gCard->showCardWinTexture();
        switch (_direction) {
            case Bottom:
                gCard->setPosition(0 , 15);
                break;
            case Top:
                gCard->setScale(1.0f);
                gCard->setPosition(0 , 20);
                break;
            case Left:
                gCard->setPosition(0, 10);
                break;
            case Right:
                gCard->setPosition(0, 10);
                break;
        }
        
        
        card->addChild(gCard);
    }
    
    void CardListBoard::addAnGuangCard(MahjongCard * card){
        JzLog::shareLog()->log(kLogLevelDebug, "玩家一张暗杠牌。。。");
        
        _guangActionCard = createMahjongCard(card->getCardValue());
        _guangActionCard->setCardValue(card->getCardValue());
        
        if (_isMySelf) {
            _guangActionCard->showCardWinTexture();
        }else{
            _guangActionCard->setCardAnGuangTexture();
        }
        
        switch (_direction) {
            case Bottom:
                _guangActionCard->setPosition(card->getContentSize().width/2, 15);
                break;
            case Top:
                _guangActionCard->setPosition(card->getContentSize().width/2 , 15);
                break;
            case Left:
                _guangActionCard->setPosition(0, 10);
                break;
            case Right:
                _guangActionCard->setPosition(0, 10);
                break;
        }
        
      
       
        card->addChild(_guangActionCard);
    }
    
    void CardListBoard::resetAllCardPoint(){
        
        JzLog::shareLog()->log(kLogLevelDebug, "玩家完成动作...重置所有麻将的位置。。。。");
        
        for (int i = 0; i < _cardsPtr.size(); i++) {
            
            if (i< _cardStartIndex) {
                
                Point p = _cardPoint[i];
                
                if (_cardsPtr[i]->getTag() == ErTag_GangDCard) {
                    _cardsPtr[i]->setTag(ErTag_GangCard);
                    this->addGuangCard(_cardsPtr[i]);
                }
                
                if (_cardsPtr[i]->getTag() == ErTag_AnGuangDCard) {
                    _cardsPtr[i]->setTag(ErTag_AnGuangCard);
                    this->addAnGuangCard(_cardsPtr[i]);
                }
                
                _cardsPtr[i]->setPosition(p);
                
                if (_direction == MJPostion::Right) {
                    _cardsPtr[i] -> setLocalZOrder(MAX_COUNT - i);
                }else if(_direction == MJPostion::Left){
                    _cardsPtr[i] -> setLocalZOrder(i);
                }
                
                if (_direction == MJPostion::Top) {
                    _cardsPtr[i]->setScale(0.8f);
                }
            }else{
                _cardsPtr[i]->setPosition(_cardPoint[i] );
            }
        }
    
        //_cardsPtr[_cardsPtr.size()-1]->removeFromParentAndCleanup(true);
        //_cardsPtr.pop_back();
    }

    void CardListBoard::createUserActionCard(bool bDarkGuang ,ActionType actionType , BYTE card){
        
        switch (actionType) {
            case ActionType::WIK_PENG:  //碰
            {
                MahjongCard * card1 = createMahjongCard(card);
                MahjongCard * card2 = createMahjongCard(card);
                MahjongCard * card3 = createMahjongCard(card);
                
                card1->setCardValue(card);
                card2->setCardValue(card);
                card3->setCardValue(card);
                
                card1->showCardWinTexture();
                card2->showCardWinTexture();
                card3->showCardWinTexture();
                
                card1->setTag(ErTag_PengCard);
                card2->setTag(ErTag_PengCard);
                card3->setTag(ErTag_PengCard);
                
                _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card1);
                _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card2);
                _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card3);
                addChild(card1);
                addChild(card2);
                addChild(card3);
                _cardStartIndex += 3;
            }
                break;
                
            case ActionType::WIK_GANG:  //杠
            {
                if (!bDarkGuang) {
                    MahjongCard * card1 = createMahjongCard(card);
                    MahjongCard * card2 = createMahjongCard(card);
                    MahjongCard * card3 = createMahjongCard(card);
                    
                    card1->setCardValue(card);
                    card2->setCardValue(card);
                    card3->setCardValue(card);
                    
                    card1->showCardWinTexture();
                    card2->showCardWinTexture();
                    card3->showCardWinTexture();
                    
                    card1->setTag(ErTag_GangCard);
                    card2->setTag(ErTag_GangDCard);
                    card3->setTag(ErTag_GangCard);
                    //card3 --> 0  card1 --> 1  card2 --> 2
                    
                    addChild(card1);
                    addChild(card2);
                    addChild(card3);
                    _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card1);
                    _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card2);
                    _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card3);
                }else{
                    
                    MahjongCard * card1 = createMahjongCard(card);
                    MahjongCard * card2 = createMahjongCard(card);
                    MahjongCard * card3 = createMahjongCard(card);
                    
                    card1->setCardAnGuangTexture();
                    card2->setCardAnGuangTexture();
                    card3->setCardAnGuangTexture();
                    
                    card1->setTag(ErTag_GangCard);
                    card2->setTag(ErTag_GangDCard);
                    card3->setTag(ErTag_GangCard);
                    //card3 --> 0  card1 --> 1  card2 --> 2
                    addChild(card1);
                    addChild(card2);
                    addChild(card3);
                    _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card1);
                    _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card2);
                    _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card3);
                }
                
                _cardStartIndex += 3;
            }
                break;
                
            case ActionType::WIK_LEFT:  //左吃
            {
                //同类型的牌的位置
                
                BYTE leftCard = card - 0x01;
                BYTE leftLeftCard = leftCard - 0x01;
                
                MahjongCard * card1 = createMahjongCard(card);
                MahjongCard * card2 = createMahjongCard(leftCard);
                MahjongCard * card3 = createMahjongCard(leftLeftCard);
                
                card1->setCardValue(leftLeftCard);
                card2->setCardValue(leftCard);
                card3->setCardValue(card);
                
                card1->showCardWinTexture();
                card2->showCardWinTexture();
                card3->showCardWinTexture();
                
                card1->setTag(ErTag_ChiCard);
                card2->setTag(ErTag_ChiCard);
                card3->setTag(ErTag_ChiCard);
                
                _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card3);
                _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card2);
                _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card1);
                
                addChild(card1);
                addChild(card2);
                addChild(card3);
                
                _cardStartIndex += 3;
                
            }
                break;
            case ActionType::WIK_CENTER:  //中吃
            {
                BYTE leftCard = card - 0x01;
                BYTE rightCard = card + 0x01;
                
                MahjongCard * card1 = createMahjongCard(leftCard);
                MahjongCard * card2 = createMahjongCard(card);
                MahjongCard * card3 = createMahjongCard(rightCard);
                
                card1->setCardValue(leftCard);
                card2->setCardValue(rightCard);
                card3->setCardValue(card);
                
                card1->showCardWinTexture();
                card2->showCardWinTexture();
                card3->showCardWinTexture();
                
                card1->setTag(ErTag_ChiCard);
                card2->setTag(ErTag_ChiCard);
                card3->setTag(ErTag_ChiCard);
                
                _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card3);
                _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card2);
                _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card1);

                addChild(card1);
                addChild(card2);
                addChild(card3);
                
                _cardStartIndex += 3;
                
            }
                break;
                
            case ActionType::WIK_RIGHT:  //右吃
            {
                
                BYTE rightCard = card + 0x01;
                BYTE rightRightCard = rightCard + 0x01;
                
                MahjongCard * card1 = createMahjongCard(rightRightCard);
                MahjongCard * card2 = createMahjongCard(rightCard);
                MahjongCard * card3 = createMahjongCard(card);
                
                card1->setCardValue(rightCard);
                card2->setCardValue(rightRightCard);
                card3->setCardValue(card);
                
                card1->showCardWinTexture();
                card2->showCardWinTexture();
                card3->showCardWinTexture();
                
                card1->setTag(ErTag_ChiCard);
                card2->setTag(ErTag_ChiCard);
                card3->setTag(ErTag_ChiCard);
                
                _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card3);
                _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card2);
                _cardsPtr.insert(_cardsPtr.begin() + _cardStartIndex, card1);

                
                addChild(card1);
                addChild(card2);
                addChild(card3);
                
                _cardStartIndex += 3;
                
            }
                break;
        }
    }
    
    
    void CardListBoard::addAllActionCards(std::vector<CMD_WeaveItem> weaveItems){
        
        for (int i = 0; i < weaveItems.size(); i++) {
            CMD_WeaveItem item = weaveItems.at(i);
            
            this->createUserActionCard(false, (ActionType)item.cbWeaveKind, item.cbCenterCard);
        }
        
        this->resetAllCardPoint();
    }
    
    
    
    void CardListBoard::showAllWinCard(bool isWin , BYTE winCardValue ,const std::vector<BYTE> values){
        int index = 0;
        
        for (int i=0; i< values.size(); i++) {
            if (values[i] == _caiShenCard) {
                
            }
        }
        
        for (int i = _cardStartIndex; i < _cardsPtr.size(); i++) {
            
            if (index >= values.size()) {
                break;
            }
            
            MahjongCard * card = _cardsPtr[i];
            
            card->setCardValue(values.at(index));
            card->showCardWinTexture();
            
            index++;
        }
        
        if (isWin) {
            MahjongCard * winCard = createMahjongCard(winCardValue);
            
            
            winCard->showCardWinTexture();
            
            addChild(winCard);
            
            _cardsPtr.push_back(winCard);
            
            this->resetAllCardPoint();
        }
    }
    
    bool CardListBoard::getCanOutCard()
    {
        return _canOut;
    }
    
    void CardListBoard::setCanOutCard(bool iCan)
    {
        
        if (_isMySelf) {
            
            if (outCardLine == NULL) {
                Size size = Director::getInstance()->getWinSize();
                outCardLine = Sprite::create("erMaJiang/tuopai.png");
                outCardLine->setPosition(Vec2(size.width/2, 100));
                outCardLine->setVisible(false);
                this->addChild(outCardLine, 100);
            }
            
            outCardLine->setVisible(iCan);
        }
        
        _canOut=iCan;
    }
    
    //显示暗杠牌
    void CardListBoard::showDarkGuangCard(BYTE cardValue){
        
        if (_guangActionCard == NULL || _guangActionCard == nullptr) {
            return;
        }
        
        _guangActionCard->setCardValue(cardValue);
        _guangActionCard->showCardWinTexture();
        
        _guangActionCard = nullptr;
    }

    
    MJOutCard CardListBoard::getCanOutCardValue()
    {
        return _outCardValue;
    }
    
    void CardListBoard::setCallFunction(std::function<void(cocos2d::Node*)> callfunc,std::function<void(MJPostion _dir)> finishfunc,std::function<void (BYTE cardValue)> outCardFunc)
    {
        _callfunc = callfunc;
        _initCardFinishFunc = finishfunc;
        _outCardFunc = outCardFunc;
    }
    
    void CardListBoard::setGangActionEndCallFunc(std::function<void(BYTE seatNo , BYTE cardValue)> callfunc){
        _gangActionEndFunc = callfunc;
    }

    
    void CardListBoard::clear(){
        _cardStartIndex = 0;
        this->removeAllChildren();
        _canOut = false;
        _cardsPtr.clear();
        _cardStartIndex = 0;
        _caiShenCard = 0;
        _restrictCard = 0x00;
        outCardLine= NULL;
    }
    
    CardListBoard::CardListBoard()
    :_callfunc(nullptr),
    _initCardFinishFunc(nullptr),
    _upCardIndex(-1),
    _canOut(false),
    _cardStartIndex(0),
    _isOuting(false),
    _caiShenCard(0),
    _restrictCard(0x00),
    outCardLine(NULL),
    _gangActionEndFunc(nullptr)
    {
    }
    
    CardListBoard::~CardListBoard()
    {        
        _cardsPtr.clear();
        _callfunc = nullptr;
        _initCardFinishFunc = nullptr;
        _undoList.clear();
    }
}
