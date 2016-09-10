#include "ErMahjongGamePlayer.h"
#include "ErMahjongGameTableUI.h"
#include "ErMahjongGameTableUI.h"

#include "YZCommon/YZConverCode.h"
#include "FontConfig.h"
#include "YZUIExport.h"
#include "../../UI/HttpSprite.h"
#include "PlayerInfoLayer.h"

namespace ErMahjong
{
	const char* Layout_Json_File = "erMaJiang/MaJiangPlayer.json";
	const char* Player_Normal_M  = "erMaJiang/DisplayPicture/man.png";
	const char* Player_Normal_W  = "erMaJiang/DisplayPicture/woman.png";
	const char* Player_Auto      = "erMaJiang/DisplayPicture/btn_autoOutCard2.png";
	
	//////////////////////////////////////////////////////////////////////////
	const char* File_Win_Num     = "ErMahjong/game/fonts/win_number.fnt";
	const char* File_Lose_Num    = "ErMahjong/game/fonts/lose_number.fnt";
    
    const char* Direct_N    = "erMaJiang/banker_3.png";
    const char* Direct_S    = "erMaJiang/banker_1.png";
    const char* Direct_W    = "erMaJiang/banker_2.png";
    const char* Direct_Banker = "erMaJiang/head_banker.png";

	//////////////////////////////////////////////////////////////////////////
	PlayerUI* PlayerUI::create(LLONG userId, INT score)
	{
		PlayerUI* player = new PlayerUI();
		if(player->init(userId , score))
		{
			player->ignoreAnchorPointForPosition(false);
			player->autorelease();
			return player;
		}
		CC_SAFE_DELETE(player);
		return nullptr;
	}
	
	PlayerUI::PlayerUI()
		: _ivHead(nullptr)
		, _textName(nullptr)
		, _textMoney(nullptr)
		, _userId(INVALID_USER_ID)
		, _score(0)
		, _tableUI(nullptr)
	{
	
	}

	PlayerUI::~PlayerUI()
	{
	}

	bool PlayerUI::init(LLONG userId, INT score)
	{
		if(!YZLayer::init())
		{
			return false;
		}

		_userId = userId;
        _score = score;

		auto layer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(Layout_Json_File);
		this->addChild(layer);
		loadComponet(layer);

		Size size = layer->getContentSize();
		this->setContentSize(size);
		layer->setAnchorPoint(Vec2::ZERO);
		layer->setPosition(Vec2::ZERO);

		return true;
	}

	void PlayerUI::setUserId(LLONG userId)
	{
		_userId = userId;
	}

	void PlayerUI::setUserName(const std::string& name)
	{
		CCAssert(nullptr != _textName, "nullptr == _textName");
		if(nullptr == _textName) return;

		_name = name;
		if (!name.empty())
		{
			char tmp[128] = {0};
			sprintf(tmp, "%s", name.c_str());
			_textName->setString(GBKToUtf8(tmp));
            
            if (_textName->getContentSize().width > _panelName->getContentSize().width) {
                _textName->setPosition(Vec2(_panelName->getContentSize().width, _panelName->getContentSize().height/2));
                _textName->runAction(RepeatForever::create(Sequence::createWithTwoActions(MoveTo::create(10.0f, Vec2(- _textName->getContentSize().width, _panelName->getContentSize().height/2)), CallFunc::create([=]{
                    _textName->setPosition(Vec2(_panelName->getContentSize().width, _panelName->getContentSize().height/2));
                }))));
            }else{
                _textName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                _textName->setPosition(_panelName->getContentSize()/2);
            }
        }
		else
		{
			_textName->setString("");
		}
	}

    string PlayerUI::getUserName(){
        return _name;
    }
    
	void PlayerUI::changeUserScore(INT score)
	{
        _score += score;
        
        char tmp[50] = {0};
        sprintf(tmp, "%d", _score);
        _textMoney->setString(GBKToUtf8(tmp));
	}

	void PlayerUI::setUserCardCount(int count)
	{
		
	}
    
	void PlayerUI::setAutoHead(bool isAuto)
	{
		
	}
    
     
	void PlayerUI::setHead(UserInfoStruct* userInfo)
	{
		CCAssert(nullptr != _ivHead, "nullptr == _ivHead");
		if(nullptr == _ivHead) return;

        _ivHead->setVisible(false);
        std::string headUrl = userInfo->szHeadUrl;
        auto httpSp = HttpSprite::create(headUrl, userInfo->bBoy ? Player_Normal_M : Player_Normal_W);
        httpSp->setImgSize( Size(_ivHead->getContentSize().width * _ivHead->getScaleX(), _ivHead->getContentSize().height * _ivHead->getScaleY()));
        httpSp->setPosition(_ivHead->getPosition());
        _ivHead->getParent()->addChild(httpSp , 1);
        
        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(PlayerUI::onTouchBegan, this);
        listener->onTouchEnded = CC_CALLBACK_2(PlayerUI::onTouchEnded, this);
        listener->setSwallowTouches(true);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, httpSp);
        
        _userId = userInfo->dwUserID;
        _userInfo = userInfo;
	}
    
    bool PlayerUI::getIsBoy(){
        return _isBoy;
    }

	void PlayerUI::setBanker(bool ibanker)
	{
		_imgBanker->setVisible(ibanker);
	}


	void PlayerUI::showMoney(bool visible)
	{
		return;
		CCAssert(nullptr != _textMoney, "nullptr == _textMoney");
		if(nullptr == _textMoney) return;

		_textMoney->setVisible(visible);
	}

	void PlayerUI::showCard(bool visible)
	{
		
	}

	void PlayerUI::showTingCard(bool visible)
	{
		_imgTing->setVisible(visible);
	}

	void PlayerUI::setTableUI(cocos2d::Node* tableUI)
	{
		_tableUI = tableUI;
	}


	bool PlayerUI::onTouchBegan(Touch *touch, Event *unused_event)
	{
		if(_userId == INVALID_USER_ID || _tableUI == nullptr)
		{
			return false;
		}

		Rect rect(0, 0, this->getContentSize().width, this->getContentSize().height);
		Vec2 pos = this->convertToNodeSpace(touch->getLocation());
		if(rect.containsPoint(pos))
		{
			return true;
		}
		return	false;
	}

	void PlayerUI::onTouchEnded(Touch *touch, Event *unused_event)
	{
		if(_tableUI == nullptr)	return;

		Size winSize = Director::getInstance()->getWinSize();

		Vec2 pos = _tableUI->convertToNodeSpace(this->getPosition());
        
        PlayerInfo pInfo;
        pInfo.bBoy = _userInfo->bBoy;
        pInfo.nickName = _userInfo->nickName;
        pInfo.dwUserID = _userInfo->dwUserID;
        pInfo.dwUserIP = _userInfo->dwUserIP;
        pInfo.isHall = false;
        pInfo.headUrl = _userInfo->szHeadUrl;
        auto layer = PlayerInfoLayer::createLayer(pInfo);
        this->getParent()->addChild(layer, 9999);
    }

	void PlayerUI::loadComponet(ui::Widget* widget)
	{
		_ivHead      = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(widget, "Image_head"));

		_textName    = dynamic_cast<Text*>(ui::Helper::seekWidgetByName(widget, "Label_name"));
		_textMoney   = dynamic_cast<Text*>(ui::Helper::seekWidgetByName(widget, "Label_money"));
		_imgBanker   = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(widget, "Image_bank"));
		//_imgTing  = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(widget, "Image_ting"));
        
        _panelName  =dynamic_cast<Layout*>(ui::Helper::seekWidgetByName(widget, "panel_name"));
        
		_textName->setString("");
		_textMoney->setString("0");
        _imgBanker->setVisible(false);
		
        this->changeUserScore(0);
	}
}
