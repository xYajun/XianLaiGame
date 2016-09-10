//
//  UploadFile.hpp
//  PDKGame
//
//  Created by Dada on 16/7/18.
//
//

#ifndef UploadFile_h
#define UploadFile_h


#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

#include <sys/stat.h>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "../cocos2d/external/curl/include/ios/curl/curl.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../cocos2d/external/curl/include/android/curl/curl.h"
#else
#endif

class uploadFile : public Ref
{
public:
    static uploadFile *m_inst;
    static uploadFile *GetInst();
    
    void UpLoadFile(string photoPath);
    
    static size_t write_data(uint8_t *dataBack, size_t size, size_t nmemb, void *userp);
    
    void upLoadFileByFTP(string photoPath);
private:
    //FTP
    int upload(CURL *curlhandle, const char * remotepath, const char * localpath, long timeout, long tries);
    
    int download(CURL *curlhandle, const char * remotepath, const char * localpath, long timeout, long tries);
};

#endif /* UploadFile_hpp */
