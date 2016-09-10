#ifndef __YZ_FishKing_GAMEUSERMESSAGE_LAYER_H__
#define __YZ_FishKing_GAMEUSERMESSAGE_LAYER_H__

#include "cocos2d.h"
#include "YZNetExport.h"
#include "YZUIExport.h"

USING_NS_CC;

using namespace YZ;

namespace FishKing
{
	class FishKingUserMessageBox : public YZDialogBase
	{
		Rect _userDataBoxRect;
	public:
		bool init(cocos2d::Node* parent, INT userID, LLONG userMoney);
		static FishKingUserMessageBox* createMessageBox(cocos2d::Node* parent, INT userID, LLONG userMoney);
	private:
		Layout* _loader;
	};
}

#endif // __GAMEUSERMESSAGE_LAYER_H__
