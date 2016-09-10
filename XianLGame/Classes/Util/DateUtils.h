//
//  DateUtils.hpp
//  zhmg3
//
//  Created by Dada on 16/6/27.
//
//

#ifndef DateUtils_hpp
#define DateUtils_hpp

#include "cocos2d.h"

USING_NS_CC;

class DateUtils
{
public:    
    // 格式化时间返回
    static std::string getCurrentTime(bool format = true);
    // 获取系统毫秒时间
    static  tm* getTime();
    
    static double getMillSecond();
  
private:
    
    static  int getYear();
    
    static  int getMonth();
    
    static  int getDay();
    
    static  int getHour();
    
    static  int getMin();
    
    static  int getSec();
};

#endif /* DateUtils_hpp */
