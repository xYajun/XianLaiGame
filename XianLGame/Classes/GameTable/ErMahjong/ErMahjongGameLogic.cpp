#include "ErMahjongGameLogic.h"
#include "YZNetExport.h"

#define ACTION_NO				0				//√ª”–∂Ø◊˜
#define ACTION_CHI				1				//≥‘≈∆∂Ø◊˜
#define ACTION_PENG				2				//≈ˆ≈∆∂Ø◊˜
#define ACTION_KAN				3				//ø≤≈∆≈∆∂Ø◊˜
#define ACTION_AN_GANG			4				//∞µ∏‹∂Ø◊˜
#define ACTION_BU_GANG			5				//≤π∏‹∂Ø◊˜
#define ACTION_MING_GANG		6				//√˜∏‹∂Ø◊˜
#define TYPE_JIANG_PAI			7				//Ω´≈∆¿‡–Õ
#define TYPE_JINGDIAO_JIANG		8				//æ´µˆΩ´≈∆¿‡–Õ
#define TYPE_SHUN_ZI			9				//À≥◊”¿‡–Õ
#define TYPE_AN_KE				10				//∞µøÃ¿‡–Õ
#define ACTION_QIANG_CHI		11				//«¿≥‘∂Ø◊˜
#define ACTION_TING				12				//Ã˝∂Ø◊˜
#define ACTION_HU				13				//∫˝∂Ø◊˜
namespace ErMahjong
{
	//ππ‘Ï∫Ø ˝
	GameLogic::GameLogic(void)
	{
		
	}
	//Œˆ‘Ï∫Ø ˝
	GameLogic::~GameLogic(void)
	{
    }
    
    
    //Õ®π˝≈∆÷µ∑µªÿ≈∆µƒµ„ ˝
	BYTE GameLogic::GetDian(BYTE byPs)
	{
		return byPs % 10;
	}

	//ªÒ»°¡Ω∏ˆÀÊª˙µƒ ˝÷µµƒ…´◊”
	//@param byDice0 µ⁄“ªø≈…´◊”,byDice1µ⁄∂˛ø≈…´◊”
	//@return ¡Ω∏ˆ˜ª◊” ˝÷µ∫Õ
	BYTE GameLogic::CastDice(BYTE& byDice0,BYTE& byDice1)
	{
		//::srand(GetTickCount());

		::sleep(50);
		byDice0 = rand()%6 + 1;

		::sleep(50);
		byDice1 = rand()%6 + 1;

		return byDice0 + byDice1;
	}

	//…æ≥˝“ª’≈≈∆
	//@param byCard“™…æ≥˝µƒ≈∆,bySrc[]≈∆‘¥,iSrcNums≈∆‘¥ ˝¡ø
	//@return  «∑Ò≥…π¶…æ≥˝,‘⁄≈∆‘¥÷–√ª”–’“µΩbyCardª·µº÷¬∑µªÿfalse
	bool GameLogic::RemoveOneCard(BYTE byCard,BYTE bySrc[],int iSrcNums)
	{
		int i = 0;

		bool bFind = false;

		for (; i < iSrcNums; ++i)
		{
			if (byCard == bySrc[i])
			{
				bySrc[i] = 0;
				bFind = true;
				break;
			}
		}

		if (i < iSrcNums)
		{
			for (; i < iSrcNums - 1; ++i)
			{
				bySrc[i] = bySrc[i + 1];
			}
		}

		return bFind;
	}

	//≈≈–Ú
	//@param byCards≈∆ ˝æ›£¨iNum≈∆ ˝¡ø£¨bFirstBig = trueŒ™¥”¥ÛµΩ–°≈≈¡–
	//@return
	void GameLogic::sort(BYTE byCards[], int iNums,bool bFirstBig)
	{
		if(byCards == NULL)
			return ;

		for (int i =0; i < iNums - 1; i++)
		{
			for(int j = 0; j < iNums - i -1; j++)
			{
				if(bFirstBig)
				{
					if(byCards[j] < byCards[j+1])
					{
						int iTempBig = byCards[j];
						byCards[j] = byCards[j+1];
						byCards[j+1] = iTempBig;		
					}
				}
				else
				{
					if(byCards[j] > byCards[j+1])
					{
						int iTempBig = byCards[j];
						byCards[j] = byCards[j+1];
						byCards[j+1] = iTempBig;		
					}
				}
			}	
		}
	}

	bool GameLogic::WzFengZiLocked(int nDeskStation, BYTE byHandCard[MJ_MAX_HANDCARD],BYTE iCardCount,int iCaiShen)
	{
		int FengZiGrid[7];
		memset(FengZiGrid, 0, sizeof(FengZiGrid));
		int iCardNumber = iCardCount;
		for (int i = 0; i < iCardNumber; i ++)
		{
			if (byHandCard[i] == 31 && iCaiShen != 31)
			{
				FengZiGrid[0] ++;
			}
			else if (byHandCard[i] == 32 && iCaiShen != 32)
			{
				FengZiGrid[1] ++;
			}
			else if (byHandCard[i] == 33 && iCaiShen != 33)
			{
				FengZiGrid[2] ++;
			}
			else if (byHandCard[i] == 34 && iCaiShen != 34)
			{
				FengZiGrid[3] ++;
			}
			else if (byHandCard[i] == 35 && iCaiShen != 35)
			{
				FengZiGrid[4] ++;
			}
			else if (byHandCard[i] == 36 && iCaiShen != 36)
			{
				FengZiGrid[5] ++;
			}
			else if (byHandCard[i] == 37)
			{
				if (iCaiShen >= 31 && iCaiShen < 37)
					FengZiGrid[6] ++;
			}
		}

		if (FengZiGrid[0] == 1 || FengZiGrid[1] == 1 || FengZiGrid[2] == 1 ||
			FengZiGrid[3] == 1 || FengZiGrid[4] == 1 || FengZiGrid[5] == 1 || FengZiGrid[6] == 1)
		{
			return true;
		}

		return false;
	}

}
