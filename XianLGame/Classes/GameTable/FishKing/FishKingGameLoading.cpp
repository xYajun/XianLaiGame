#include "FishKingGameLoading.h"
#include "FishKingGameTableUI.h"

namespace FishKing
{
	static const char*	LAYER_BG				=	"fishKing/common/bg.jpg";
	static const char*	NET_PLIST_PATH			=	"fishKing/bullet/netImg.plist";
	static const char*	NET_IMAGE_PATH			=	"fishKing/bullet/netImg.png";
	static const char*	FISH_IMAGE_PATH			=   "fishKing/fishAnimation/fishImg.pvr.ccz";
	static const char*	FISH_PLIST_PATH			=   "fishKing/fishAnimation/fishImg.plist";
	static const char*	FISH_PATH				=   "fishKing/fishAnimation/bigFishAnimation.ExportJson";
	static const char*	WATER_PLIST_PATH		=	"fishKing/Scene/water.plist";
	static const char*	WATER_IMAGE_PATH		=	"fishKing/Scene/water.pvr.ccz";


	FishKingGameLoading::FishKingGameLoading()
		: fileNum (0)
		, _loading(true)
	{
	}

	FishKingGameLoading::~FishKingGameLoading()
	{
	}

	FishKingGameLoading* FishKingGameLoading::create(bool loading)
	{
		FishKingGameLoading* pRet = new FishKingGameLoading();
		if (nullptr != pRet && pRet->init(loading))
		{
			pRet->autorelease();
			return pRet;
		}
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}

	bool FishKingGameLoading::init(bool loading)
	{
		if (!YZLayer::init())
		{
			return false;
		}

		setBackGroundImage(LAYER_BG);
		auto winSize = Director::getInstance()->getWinSize();

		_loading = loading;

		return true;
	}

	void FishKingGameLoading::loadRes()
	{
		if(_loading)
		{
			Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGBA4444);
			Director::getInstance()->getTextureCache()->addImageAsync(FISH_IMAGE_PATH, CC_CALLBACK_1(FishKingGameLoading::loadingTextureCallback, this, FISH_PLIST_PATH));
			Director::getInstance()->getTextureCache()->addImageAsync(WATER_IMAGE_PATH, CC_CALLBACK_1(FishKingGameLoading::loadingTextureCallback, this, WATER_PLIST_PATH));
			Director::getInstance()->getTextureCache()->addImageAsync(NET_IMAGE_PATH, CC_CALLBACK_1(FishKingGameLoading::loadingTextureCallback, this, NET_PLIST_PATH));

			ArmatureDataManager::getInstance()->addArmatureFileInfoAsync(FISH_PATH, this, schedule_selector(FishKingGameLoading::loadingCallback));
		}		
	}

	void FishKingGameLoading::loadingTextureCallback(Texture2D* textureData, std::string plist)
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist , textureData);

		loadingCallback(0.0f);
	}

	void FishKingGameLoading::loadingCallback(float dt)
	{
		fileNum++;
		if (fileNum >= 4)
		{
			if (nullptr != onCloseCallBack)
			{
				onCloseCallBack();
			}
			this->removeFromParent();
		}
	}	
}