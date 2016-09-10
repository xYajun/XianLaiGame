#ifndef __NH_YZLOG_H__
#define __NH_YZLOG_H__

#include "cocos2d.h"
USING_NS_CC;

// cocos2d debug
#if !defined(COCOS2D_DEBUG) || COCOS2D_DEBUG == 0
#define YZLOG(...) do {} while (0)
#define YZLOG_DEBUG(...)       do {} while (0)
#define YZLOG_ERROR(...)   do {} while (0)
#define YZLOG_INFO(...)  do {} while (0)
#define YZLOG_WARNING(...)   do {} while (0)

#elif (COCOS2D_DEBUG == 1)
#define YZLOG(format,...)			YZLog::logDebug("%s (%s:%d) " format, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define YZLOG_DEBUG(format,...)		YZLog::logDebug("Text >> (%s:%d) " format, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define YZLOG_ERROR(format,...)		YZLog::logError("Error >> (%s:%d) " format, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define YZLOG_INFO(format,...)		YZLog::logInfo("Information >> (%s:%d) " format, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define YZLOG_WARNING(format,...)	YZLog::logWarning("Warning >> (%s:%d) " format, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#elif COCOS2D_DEBUG > 1
#define YZLOG(format,...)			YZLog::logDebug("%s (%s:%d) " format, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define YZLOG_DEBUG(format,...)		YZLog::logDebug("Text >> (%s:%d) " format, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define YZLOG_ERROR(format,...)		YZLog::logError("Error >> (%s:%d) " format, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define YZLOG_INFO(format,...)		YZLog::logInfo("Information >> (%s:%d) " format, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define YZLOG_WARNING(format,...)	YZLog::logWarning("Warning >> (%s:%d) " format, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define YZLOG(...) 
#define YZLOG_DEBUG(...)	
#define YZLOG_ERROR(...)		
#define YZLOG_INFO(...)		
#define YZLOG_WARNING(...)	
#endif // COCOS2D_DEBUG

namespace YZ {

enum YZLOG_LEVEL
{
	LOG_DEBUG = 0,
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR
};

class YZLog
{
public:
	static void logDebug(const char* format, ...);
	static void logError(const char* format, ...);
	static void logInfo(const char* format, ...);
	static void logWarning(const char* format, ...);
	static void logFile(const char* format, ...);
};

};

#endif		//__NH_YZLOG_H__
