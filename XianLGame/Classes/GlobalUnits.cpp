
#include <iostream>
#include <cstring>
#include "GlobalUnits.h"
#include <stdlib.h>
#include <stdio.h>
#include <regex>

#include "YZExternal/YZJSON/JsonReader.h"


CGlobalUnits					g_GlobalUnits;	

//////////////////////////////////////////////////////////////////////////


CGlobalUnits::CGlobalUnits(void)
{
	m_bFirstEnterGame = true;
	return;
}


CGlobalUnits::~CGlobalUnits(void)
{

}


bool CGlobalUnits::InitGlobalUnits()
{
	return true;
}


std::string CGlobalUnits::getRoleTitleStr(long long _i64Diamond)
{
	const char* fname = "rank_title.json";
	JSONREADER->readJsonFromFile(fname);

	int tnum = JSONREADER->getJsonNum(fname);
	log("the json data  numbers is :%d",tnum);

	for(int i=1; i<tnum; i++)
	{
		long long diamond_standard = JSONREADER->getInt(fname,i,"DiamondCount");

		int j = i+1;
		if (j >= tnum){j = i;}
		long long diamond_standard_next = JSONREADER->getInt(fname,j,"DiamondCount");
		
		if (_i64Diamond >= diamond_standard)
		{
			// 如果玩家金币大于最大标准 则无需判定区间
			if (_i64Diamond >= 100000000)
			{
				auto tname = JSONREADER->getString(fname,tnum-1,"Titlename");
				return tname;
			}
			else if (_i64Diamond < diamond_standard_next)
			{
				auto tname = JSONREADER->getString(fname,i,"Titlename");
				return tname;
			}
		}
		
	}


}