//
//  JzLog.h
//  test
//
//  Created by pabble on 14-8-1.
//
//

#ifndef __test__JzLog__
#define __test__JzLog__

#include <iostream>
#include <fstream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

#define FORMAT "[%s %s::%s::%d] %s"

#define JzDebug(fmt,...)    \
JzLog::shareLog()->log(kLogLevelDebug,FORMAT,__TIME__,JzLog::subFile(__FILE__).c_str(),__FUNCTION__,__LINE__, cocos2d:: __String::createWithFormat(fmt, ##__VA_ARGS__)->getCString())

#define jzInfo(fmt,...)    \
JzLog::shareLog()->log(kLoglevelInfo,FORMAT,__TIME__,JzLog::subFile(__FILE__).c_str(),__FUNCTION__,__LINE__, cocos2d:: __String::createWithFormat(fmt, ##__VA_ARGS__)->getCString())

#define jzWarn(fmt,...)    \
JzLog::shareLog()->log(kLogLevelWarning,FORMAT,__TIME__,JzLog::subFile(__FILE__).c_str(),__FUNCTION__,__LINE__, cocos2d:: __String::createWithFormat(fmt, ##__VA_ARGS__)->getCString())

#define jzError(fmt,...)    \
JzLog::shareLog()->log(kLogLevelError,FORMAT,__TIME__,JzLog::subFile(__FILE__).c_str(),__FUNCTION__,__LINE__, cocos2d:: __String::createWithFormat(fmt, ##__VA_ARGS__)->getCString())


using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;

typedef enum LogLevel{
    kLogLevelError = 1,
    kLogLevelWarning = 2,
    kLogLevelDebug = 3,
    kLoglevelInfo = 4,
}logLevel;

struct JzLogStruct{
    
    logLevel _logLevel;
    string _logStr;
    
    JzLogStruct(logLevel __logLevel,string __logStr){
        
        this->_logLevel = __logLevel;
        this->_logStr = __logStr;
    }
};

class JzLog;
class JzLogView;

class JzLog {
    
public:
    static JzLog* shareLog();
    
    static string subFile(string file);
    
    static void deleteFile();
    
    void setLogLevel(logLevel level);
    
    void setWritePath(const char* pPath);
    
    void setWriteCache(int cache);
    
    void setWriteToFile(bool bFlag);
    
    void flushFile();
    
    void log(logLevel level, const char* pszFormat, ...);
        
    void bindViewToScene(Scene* parent);
private:
    JzLog();
    
    void writeToFile(const char* pCotent);
    
    static JzLog* m_pSingle;
    
    logLevel m_iLogLevel;
    
    string m_oStrWritePath;
    
    string m_oStrLogCache;
    
    vector<JzLogStruct> m_logList;
    
    //    JzLogView* m_logView;
    
    int m_iWriteCache;
    
    bool m_bWriteToFile;
    
    int m_iLogCount;
};

class JzLogView : public Layer{
    
private:
    
    ListView* m_listView;
    
    Button* m_button;
private:
    
    bool init();
    
    void insertLog(string logStr,logLevel level);
    
    CREATE_FUNC(JzLogView);
    
    friend class JzLog;
};

#endif /* defined(__test__JzLog__) */
