#include "JsonReader.h"
#include "json/document.h"
using namespace rapidjson;

JsonReader* JsonReader::instance = NULL;
JsonReader* JsonReader::getInstance()

{
	if (instance == NULL)
	{
		instance = new JsonReader();
		instance->init();
	}
	return instance;
}

void JsonReader::init()
{

}

void JsonReader::readJsonFromFile(const char *fileName)
{
	ssize_t size = 0; 
	string pszDescription= cocos2d::FileUtils::getInstance()->getStringFromFile(fileName); 
	readJsonFromData(fileName,pszDescription.c_str());
	//free(pszDescription);
}

void JsonReader::readJsonFromData(const char *fileName,const char* pszDescription,bool withClean)
{
	do { 
		if (pszDescription == NULL) 
		{
			return;
		}
		if(rootMap.find(string(fileName))!=rootMap.end())
		{
			return;
		}
		Document* doc = new Document();
		doc->Parse<kParseDefaultFlags>(pszDescription);
		CC_BREAK_IF(doc->HasParseError());
		rootMap[string(fileName)] = doc;
		
	} while (0); 
	return;
}


int JsonReader::getKeyIndex(const char *fileName,const char* keyStr)
{
	Document* doc = (Document*)getJsonFromName(fileName);
	if(doc==NULL)
	{
		return -1;
	}
	rapidjson::Value &row = ((*doc)[SizeType(0)]);
	if(row.IsArray())
	{
		for (int i=0;i<(int)row.Capacity();i++)
		{
			if(string(row[SizeType(i)].GetString())==string(keyStr))
			{
				return i;
			}			
		}
	}
	return -1;
}

const char* JsonReader::getString(const char* fileName,int rowKey,const char* name)
{
	int col_Index = getKeyIndex(fileName,name);
	if(col_Index==-1)
	{
		return "";
	}
	Document* doc = (Document*)getJsonFromName(fileName);
	if(doc==NULL)
	{
		return "";
	}
	for (int i=1;i<(int)doc->Capacity();i++)
	{
		rapidjson::Value &row = (*doc)[SizeType(i)];
		if(row.IsArray())
		{
			if(row[SizeType(0)].IsInt())
			{
				if(row[SizeType(0)].GetInt()==rowKey)
				{
					if(row[SizeType(col_Index)].IsString())
					{
						return row[SizeType(col_Index)].GetString();
					}
					else if(row[SizeType(col_Index)].IsInt())
					{
						__String* ret = __String::createWithFormat("%d",row[SizeType(col_Index)].GetInt());						
						return ret->getCString();
					}
					
				}
			}			
		}
	}
	return "";
}

int JsonReader::getInt(const char* fileName,int rowKey,const char* name)
{
	int col_Index = getKeyIndex(fileName,name);
	if(col_Index==-1)
	{
		return INT_MIN;
	}
	Document* doc = (Document*)getJsonFromName(fileName);
	if(doc==NULL)
	{
		return INT_MIN;
	}
	for (int i=1;i<(int)doc->Capacity();i++)
	{
		rapidjson::Value &row = (*doc)[SizeType(i)];
		if(row.IsArray())
		{
			if(row[SizeType(0)].IsInt())
			{
				if(row[SizeType(0)].GetInt()==rowKey)
				{
					rapidjson::Value &v  =  row[SizeType(col_Index)];
					return v.GetInt();
				}
			}
		}
	}
	return INT_MIN;
}


//const char* JsonReader::getKeyStr(const char *fileName,int index)
//{
//	CCArray* ary = (CCArray*)keyDic.objectForKey(fileName);
//	CCAssert(ary != NULL, "JsonReader::getKeyStr");
//	CCAssert(index < ary->count(),"JsonReader::getKeyStr");
//	return ((CCString*)ary->objectAtIndex(index))->getCString();
//}



//CCString* JsonReader::getCCStringByInt(const char* fileName,int rowKey,const char* name)
//{
//	return getCCStringByString(fileName,CCString::createWithFormat("%d",rowKey)->getCString(),name);
//}
//
//CCString* JsonReader::getCCStringByString(const char* fileName,const char* rowKey,const char* name)
//{
//	CCDictionary* dic = (CCDictionary*)rootDic.objectForKey(fileName);
//	if(dic==NULL) return NULL;
//	CCDictionary* rowDic  = (CCDictionary*)dic->objectForKey(rowKey);
//	if(rowDic==NULL) return NULL;
//	CCString* cstr = (CCString*)rowDic->objectForKey(name);
//	return cstr;
//}


//const char* JsonReader::getStringByString(const char* fileName,const char* rowKey,const char* name)
//{
//	return getCCStringByString(fileName,rowKey,name)->getCString();
//}
//

//
//int JsonReader::getIntByString(const char* fileName,const char* rowKey,const char* name)
//{
//	return getCCStringByString(fileName,rowKey,name)->intValue();
//}

//CCDictionary* JsonReader::getDicFromOneRow(const char* fileName,int rowKey)
//{
	//CCDictionary* dic = (CCDictionary*)rootDic.objectForKey(fileName);
	//if(dic==NULL) return NULL;
	//CCDictionary* rowDic  = (CCDictionary*)dic->objectForKey(CCString::createWithFormat("%d",rowKey)->getCString());
	//return rowDic;
//}

//CCDictionary* JsonReader::getDicFromOneRow(const char* fileName,const char* rowKey)
//{
	//CCDictionary* dic = (CCDictionary*)rootDic.objectForKey(fileName);
	//if(dic==NULL) return NULL;
	//CCDictionary* rowDic  = (CCDictionary*)dic->objectForKey(rowKey);
	//return rowDic;
//}

int JsonReader::getJsonNum(const char* fileName)
{
	Document* doc = (Document*)getJsonFromName(fileName);
	if(doc==NULL)
	{
		return 0;
	}
	if(doc->IsArray())
	{
		return doc->Capacity();
	}
	return 0;
}

void* JsonReader::getJsonFromName(const char* fileName)
{
	string fn = string(fileName);
	if(rootMap.find(fn)==rootMap.end())
	{
		return NULL;
	}
	return rootMap[fn];
}

