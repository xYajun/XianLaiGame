#include "ChatLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalUnits.h"
#include "SimpleAudioEngine.h"
#include "PlatformDefine.h"


using namespace cocostudio;
using namespace cocos2d::ui;
USING_NS_CC;

using namespace CocosDenshion;


bool ChatLayer::init()
{
	if ( !Layer::init() )
	{
		return false;
	}

	Size winSize = Director::getInstance()->getWinSize();
	_uiChatLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ChatLayer/ChatLayer_1.json");
	_uiChatLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
	_uiChatLayer->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	addChild(_uiChatLayer);

	panel_face = (Layout*)Helper::seekWidgetByName(_uiChatLayer,"Panel_face");
	panel_face->setTag(PANEL_FACE);
	panel_talk = (Layout*)Helper::seekWidgetByName(_uiChatLayer,"Panel_word");
	panel_talk->setTag(PANEL_TALK);

	tag_current = PANEL_FACE;
	tag_last = tag_current;

	// -- btn
	Button* btn_face = (Button*)(Helper::seekWidgetByName(_uiChatLayer,"btn_face"));
	btn_face->addTouchEventListener(CC_CALLBACK_2(ChatLayer::onBtnClicked,this));
	btn_face->setTag(BTN_FACE);

	Button* btn_talk = (Button*)(Helper::seekWidgetByName(_uiChatLayer,"btn_word"));
	btn_talk->addTouchEventListener(CC_CALLBACK_2(ChatLayer::onBtnClicked,this));
	btn_talk->setTag(BTN_TALK);

	Button* btn_close = (Button*)(Helper::seekWidgetByName(_uiChatLayer,"btn_close"));
	btn_close->addTouchEventListener(CC_CALLBACK_2(ChatLayer::onBtnClicked,this));
	btn_close->setTag(BTN_CLOSE);

	Button* btn_send = (Button*)(Helper::seekWidgetByName(_uiChatLayer,"btn_send"));
	btn_send->addTouchEventListener(CC_CALLBACK_2(ChatLayer::onBtnClicked,this));
	btn_send->setTag(BTN_SEND);

	for (int i = 1; i < 29; i++)
	{
		std::string name = String::createWithFormat("face_%d",i)->_string;
		Button* btn_face = (Button*)(Helper::seekWidgetByName(_uiChatLayer,name));
		btn_face->addTouchEventListener(CC_CALLBACK_2(ChatLayer::onFaceBtnClicked,this));
		btn_face->setTag(BTN_FACE_TAG + i);
	}

	// ----------------------
	listview_talk = static_cast<ListView*>(Helper::seekWidgetByName(_uiChatLayer, "listview_talk"));
	Layout* pTalkItem = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("ChatLayer/ChatCell.json"));
	listview_talk->setItemModel(pTalkItem);

	showTalks();

	return true;
}

void ChatLayer::onImageClicked(Ref * pSender,Widget::TouchEventType type)
{
}

void ChatLayer::onBtnClicked(Ref * pSender,Widget::TouchEventType type)
{
	
	Button *btn = (Button*)pSender;
	if(type==Widget::TouchEventType::ENDED && btn)
	{
		YZAudioEngine::getInstance()->playEffect(GAME_SOUND_CLICK);
		log("tag = %d", btn->getTag());
		switch (btn->getTag())
		{
		case BTN_FACE:
			{
				tag_current = PANEL_FACE;
				if (tag_last != PANEL_FACE)
				{
					_uiChatLayer->getChildByTag(tag_last)->setVisible(false);
					panel_face->setVisible(true);
					tag_last = tag_current;
				}
			}
			break;
		case BTN_TALK:
			{
				tag_current = PANEL_TALK;
				if (tag_last != PANEL_TALK)
				{
					_uiChatLayer->getChildByTag(tag_last)->setVisible(false);
					panel_talk->setVisible(true);
					tag_last = tag_current;
				}
			}
			break;
		case BTN_CLOSE:
			{
				removeFromParentAndCleanup(true);
			}
			break;
		case BTN_SEND:
			{
			}
			break;
		default:
			break;
		}
	}
}


void ChatLayer::onFaceBtnClickedCB(const std::function<void(int id)>& onSendChatFace)
{
	_onSendChatFace = onSendChatFace;
}

void ChatLayer::onFaceBtnClicked(Ref * pSender,Widget::TouchEventType type)
{

	Button *btn = (Button*)pSender;
	if(type==Widget::TouchEventType::ENDED && btn)
	{
		log("tag = %d", btn->getTag());
		int faceid = btn->getTag() - BTN_FACE_TAG;
		_onSendChatFace(faceid);
		removeFromParentAndCleanup(true);
	}
}


void ChatLayer::onTalkImgClickedCB(const std::function<void(int id)>& onSendChatTalk)
{
	_onSendChatTalk = onSendChatTalk;
}

void ChatLayer::onTalkImgClicked(Ref * pSender,Widget::TouchEventType type)
{

	ImageView *img = (ImageView*)pSender;
	if(type==Widget::TouchEventType::ENDED && img)
	{
		YZAudioEngine::getInstance()->playEffect(GAME_SOUND_CLICK);
		log("tag = %d", img->getTag());
		int wordid = img->getTag() - BTN_TALK_TAG;
		_onSendChatTalk(wordid);
		removeFromParentAndCleanup(true);
	}
}

void ChatLayer::showTalks()
{
	listview_talk->removeAllItems();
	listview_talk->refreshView();
	for (int i=1;i<13;i++)
	{
		listview_talk->pushBackDefaultItem();
		Widget* pModelItem = listview_talk->getItem(listview_talk->getItems().size() - 1);

		//ͼƬ;
		char name[60];
		memset(name, 0, sizeof(name));
		sprintf(name, "ChatLayer/chat_res/chat_word%02d%s",i,".png");
		ImageView* img_word = static_cast<ImageView*>(Helper::seekWidgetByName(pModelItem, "img_word"));
		img_word->loadTexture(name);
		img_word->setTouchEnabled(true);

		img_word->addTouchEventListener(CC_CALLBACK_2(ChatLayer::onTalkImgClicked,this));
		img_word->setTag(BTN_TALK_TAG + i);
	}
	listview_talk->refreshView();
}

