//
//  DateUtils.cpp
//  zhmg3
//
//  Created by Dada on 16/6/27.
//
//

#include "DateUtils.h"

tm* DateUtils::getTime(){
    time_t time1;
    time(&time1);
    return localtime(&time1);
}

std::string DateUtils::getCurrentTime(bool format)
{
    if (format) {
        return __String::createWithFormat("%02d-%02d-%02d %02d:%02d:%02d" , getYear() , getMonth() , getDay() , getHour() , getMin() , getSec())->getCString();
    }else{
        return __String::createWithFormat("%d%02d%02d%02d%02d%02d" , getYear() , getMonth() , getDay() , getHour() , getMin() , getSec())->getCString();
    }
}


int DateUtils::getYear(){
    return DateUtils::getTime()->tm_year+1900;
}
int DateUtils::getMonth(){
    return DateUtils::getTime()->tm_mon+1;
}
int DateUtils::getDay(){
    return DateUtils::getTime()->tm_mday;
}
int DateUtils::getHour(){
    return DateUtils::getTime()->tm_hour;
}
int DateUtils::getMin(){
    return DateUtils::getTime()->tm_min;
}
int DateUtils::getSec(){
    return DateUtils::getTime()->tm_sec;
}