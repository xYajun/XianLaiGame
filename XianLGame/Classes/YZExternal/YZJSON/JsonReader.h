
//by kevin 2013
#ifndef _JSONREADER_
#define  _JSONREADER_

#include "cocos2d.h"
#include  "json/stringbuffer.h"
#include <map>
#include <string>
using namespace std;

//#define ServerListName		"serverlist.json"

USING_NS_CC;
#define JSONREADER JsonReader::getInstance()
class JsonReader:public cocos2d::Ref
{
public:
	static JsonReader* getInstance();
	virtual void init();
	
	void readJsonFromFile(const char *fileName);
	void readJsonFromData(const char *fileName,const char *pszDescription,bool withClean=true);
	const char* getString(const char* fileName,int rowKey,const char* name);
	int getInt(const char* fileName,int rowKey,const char* name);

	int getJsonNum(const char* fileName);
protected:
	int getKeyIndex(const char *fileName,const char* keyStr);
	void* getJsonFromName(const char* fileName);
protected:
	static JsonReader* instance;

	map<string,void*> rootMap;
};
#endif //_TOOLS_H_
