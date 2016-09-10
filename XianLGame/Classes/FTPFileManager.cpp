//
//  FTPFileManager.cpp
//  PDKGame
//
//  Created by Dada on 16/7/22.
//
//

#include "FTPFileManager.h"

#define SERVER_PATH "ftp://211.149.160.248/download/"
#define SERVER_USER_INFO "wy:123456"
#include "Sound/YZAudioEngine.h"
#include "GameTable/ErMahjong/ErMahjongGameTableUI.h"

FTPFileManager* FTPFileManager::m_inst = NULL;

FTPFileManager* FTPFileManager::GetInst()
{
    if(!m_inst)
    {
        m_inst = new FTPFileManager();
        return m_inst;
    }
    return NULL;
}



static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
    curl_off_t nread;
    FILE *f = (FILE*)stream;
    size_t retcode = fread(ptr, size, nmemb, f);
    nread = (curl_off_t)retcode;
    
    fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T
            " bytes from file\n", nread);
    return retcode;
}


int FTPFileManager::uploadFile2ServerByFtp(std::string fileName){
    CURL *curl;
    CURLcode res;
    FILE *hd_src;
    struct stat file_info;
    curl_off_t fsize;
    
    struct curl_slist *headerlist=NULL;
    
    std::string filePath = FileUtils::getInstance()->getWritablePath() + fileName;
    
    CCLOG("upload fileName : %s" , filePath.c_str());

    if(stat(filePath.c_str(), &file_info)) {
        log("Couldnt open '%s': %s\n", filePath.c_str(), strerror(errno));
        return 1;
    }
    fsize = (curl_off_t)file_info.st_size;
    
    log("Local file size: %" CURL_FORMAT_CURL_OFF_T " bytes.\n", fsize);
    
    hd_src = fopen(filePath.c_str(), "rb");
    
    curl_global_init(CURL_GLOBAL_ALL);
    
    std::string urlPath = SERVER_PATH + fileName;
    
    curl = curl_easy_init();
    if(curl) {
        headerlist = curl_slist_append(headerlist, fileName.c_str());
        curl_easy_setopt(curl, CURLOPT_USERPWD, SERVER_USER_INFO);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl, CURLOPT_URL, urlPath.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);
        curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
        curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,(curl_off_t)fsize);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,uploadCallBack); //设置回调
        res = curl_easy_perform(curl);
        
        if(res != CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
            ErMahjong::GameTableUI::getInstance()->uploadFileEnd(fileName);
        }else{
            ErMahjong::GameTableUI::getInstance()->uploadFileEnd(fileName);
        }
        
        curl_slist_free_all (headerlist);
        
        curl_easy_cleanup(curl);
    }
    fclose(hd_src); /* close the local file */
    
    curl_global_cleanup();
    
    return 0;
}


struct FtpFile {
    const char *filename;
    FILE *stream;
};

static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
    struct FtpFile *out=(struct FtpFile *)stream;
    if(out && !out->stream) {
        /* open file for writing */
        out->stream=fopen(out->filename, "wb");
        if(!out->stream){
            CCLOG("failure, can't open file to write...");
            return -1;
        }
             /* failure, can't open file to write */
    }
    return fwrite(buffer, size, nmemb, out->stream);
}

void FTPFileManager::downloadFile2LocalByFtp(std::string _fileName){
    
    CURL *curl;
    CURLcode res;
    
    FtpFile ftpfile;
    ftpfile.filename =  __String::createWithFormat("%s%s%s",FileUtils::getInstance()->getWritablePath().c_str(),_fileName.c_str(),".amr")->getCString();
    ftpfile.stream = NULL;
    
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        std::string serverPath = SERVER_PATH + _fileName + ".amr";
        
        CCLOG("serverPath : %s" , serverPath.c_str());
        
        curl_easy_setopt(curl, CURLOPT_URL,serverPath.c_str());
        curl_easy_setopt(curl, CURLOPT_USERPWD, SERVER_USER_INFO);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        CCLOG("downloadFile2LocalByFtp res : %s" , curl_easy_strerror(res));

        if(ftpfile.stream)
                fclose(ftpfile.stream); /* close the local file */

        if(CURLE_OK != res) {
            /* we failed */
            CCLOG(" : %s" , serverPath.c_str());
            fprintf(stderr, "curl told us %d\n", res);
        }else{
            ErMahjong::GameTableUI::getInstance()->downloadFileEnd(_fileName);
        }
    }

    curl_global_cleanup();
    
}


size_t FTPFileManager::uploadCallBack(uint8_t *dataBack, size_t size, size_t nmemb, void *userp)
{
    string szData = string((char*)dataBack); //返回的结果
    
    CCLOG(" uploadCallBack write_data : %s " , szData.c_str());
    
    return 0;
}

size_t FTPFileManager::downloadCallBack(uint8_t *dataBack, size_t size, size_t nmemb, void *userp){
    string szData = string((char*)dataBack); //返回的结果
    
    CCLOG(" downloadCallBack write_data : %s " , szData.c_str());
    
    
    return 0;
    
}
