#ifndef __FishKing_LOADING_LAYER_H__
#define __FishKing_LOADING_LAYER_H__

#include "YZNetExport.h"
#include "YZUIExport.h"
#include "cocos2d.h"

USING_NS_CC;

namespace FishKing
{
	class FishKingGameLoading : public YZLayer
	{
		struct FishSpirteInfo
		{
			std::string name;
			BYTE	count;
			FLOAT	time;
		};

		FishSpirteInfo _fishName[26];

		BYTE  fileNum;

	public:
		typedef std::function<void ()> CloseCallBack;
		CloseCallBack  onCloseCallBack;
	public:
		static FishKingGameLoading* create(bool loading);
		void loadingTextureCallback(Texture2D* textureData, std::string plist);
		void loadingCallback(float dt);
		void loadRes();
	protected:
		FishKingGameLoading();
		virtual ~FishKingGameLoading();

	private:
		bool init(bool loading);
		bool _loading;
	};
}
#endif // __FishKing_LOADING_LAYER_H__
