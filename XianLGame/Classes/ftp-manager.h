//
//  ftp-manager.hpp
//  PDKGame
//
//  Created by Dada on 16/7/21.
//
//

#ifndef ftp_manager_h
#define ftp_manager_h

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


/*FTP OPERATION CODE*/
typedef enum FTP_STATE
{
    FTP_UPLOAD_SUCCESS,
    FTP_UPLOAD_FAILED,
    FTP_DOWNLOAD_SUCCESS,
    FTP_DOWNLOAD_FAILED
}FTP_STATE;

/*FTP OPERATIONS OPTIONS*/
typedef struct FTP_OPT
{
    char *url;		/*url of ftp*/
    char *user_key;		/*username:password*/
    char *file;		/*filepath*/
}FTP_OPT;

class ftpmanager : public Ref
{
public:
    static ftpmanager *m_inst;
    static ftpmanager *GetInst();
    
    static size_t write_data(uint8_t *dataBack, size_t size, size_t nmemb, void *userp);
    
    void upLoadFileByFTP(string photoPath);
private:
    
    void ftp_down();
    
    int ftp_upload(std::string filePath , std::string fileName);
    
    /*upload file to ftp server*/
    FTP_STATE ftp_upload(const FTP_OPT ftp_option);
    
    /*download file from ftp server*/
    FTP_STATE ftp_download(const FTP_OPT ftp_option);
};
#endif /* ftp_manager_hpp */
