//
//  FTPFileManager.hpp
//  PDKGame
//
//  Created by Dada on 16/7/22.
//
//

#ifndef FTPFileManager_hpp
#define FTPFileManager_hpp

#include <stdio.h>
#include <sys/stat.h>
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "../cocos2d/external/curl/include/ios/curl/curl.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../cocos2d/external/curl/include/android/curl/curl.h"
#else
#endif


class FTPFileManager : public Ref
{
public:
    static FTPFileManager *m_inst;
    static FTPFileManager *GetInst();
    
    int uploadFile2ServerByFtp(std::string fileName);
    
    void downloadFile2LocalByFtp(std::string fileName);
    
private:
    
    static size_t uploadCallBack(uint8_t *dataBack, size_t size, size_t nmemb, void *userp);
    
    static size_t downloadCallBack(uint8_t *dataBack, size_t size, size_t nmemb, void *userp);
};

#endif /* FTPFileManager_hpp */
