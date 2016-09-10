//
//  UploadFile.cpp
//  PDKGame
//
//  Created by Dada on 16/7/18.
//
//

#include "UploadFile.h"



uploadFile* uploadFile::m_inst = NULL;

uploadFile* uploadFile::GetInst()
{
    if(!m_inst)
    {
        m_inst = new uploadFile();
        return m_inst;
    }
    return NULL;
}

void uploadFile::UpLoadFile(string photoPath)
{
    CCLOG("IN uploadFile::UpLoadFile(");
    CURLcode cURLcode;
    CURL *cURL;
    
    //Init
    cURLcode = curl_global_init(CURL_GLOBAL_SSL);
    if(CURLE_OK != cURLcode)
    {
        curl_global_cleanup();
        return ;
    }
    
    cURL = curl_easy_init();
    if(!cURL)
    {
        curl_easy_cleanup(cURL);
        curl_global_cleanup();
        return ;
    }
    curl_httppost *post = NULL;
    curl_httppost *last = NULL;
    
    CCLOG("photoPath : %s " , photoPath.c_str());
    
    //上传的文件的全路径并指定类型
    curl_formadd(&post, &last,CURLFORM_COPYNAME,"Photo",
                 CURLFORM_FILE,photoPath.c_str(),
                 CURLFORM_CONTENTTYPE,"Image/jpg",CURLFORM_END);

    ////设置多个参数
    //curl_formadd(&post, &last,CURLFORM_COPYNAME,"username",
    //            CURLFORM_COPYCONTENTS,"你好",CURLFORM_END);
    
    curl_easy_setopt(cURL, CURLOPT_URL,"http://www.dadagame.cn/upload/Photo/");
    curl_easy_setopt(cURL, CURLOPT_TIMEOUT,50);//超时时间10秒
    curl_easy_setopt(cURL, CURLOPT_HTTPPOST,post);//请求方式POST
    curl_easy_setopt(cURL, CURLOPT_WRITEFUNCTION,write_data); //设置回调
    curl_easy_setopt(cURL, CURLOPT_VERBOSE,1); //非零值表示你想CURL报告每一件意外的事情
    
    curl_easy_perform(cURL);
    
    curl_easy_cleanup(cURL);
    
    curl_global_cleanup();
    
}

size_t getcontentlengthfunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
    int r;
    long len = 0;
    /* _snscanf() is Win32 specific */
    //r = _snscanf(ptr, size * nmemb, "Content-Length: %ld\n", &len);
    r = sscanf((const char*)ptr, "Content-Length: %ld\n", &len);
    if (r) /* Microsoft: we don't read the specs */
        *((long *) stream) = len;
    return size * nmemb;
}

/* discard downloaded data */
size_t discardfunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
    return size * nmemb;
}
//write data to upload
size_t writefunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
    return fwrite(ptr, size, nmemb, (FILE*)stream);
}
/* read data to upload */
size_t readfunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
    FILE *f = (FILE*)stream;
    size_t n;
    if (ferror(f))
        return CURL_READFUNC_ABORT;
    n = fread(ptr, size, nmemb, f) * size;
    return n;
}



int uploadFile::upload(CURL *curlhandle, const char * remotepath, const char * localpath, long timeout, long tries)
{
    FILE *f;
    long uploaded_len = 0;
    CURLcode r = CURLE_GOT_NOTHING;
    int c;
    f = fopen(localpath, "rb");
    if (f == NULL) {
        perror(NULL);
        return 0;
    }
    
    curl_easy_setopt(curlhandle, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curlhandle, CURLOPT_URL, remotepath);
    curl_easy_setopt(curlhandle, CURLOPT_USERPWD, "wy:123456");
    if (timeout)
        curl_easy_setopt(curlhandle, CURLOPT_FTP_RESPONSE_TIMEOUT, timeout);
    curl_easy_setopt(curlhandle, CURLOPT_HEADERFUNCTION,getcontentlengthfunc);
    curl_easy_setopt(curlhandle, CURLOPT_HEADERDATA, &uploaded_len);
    curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, discardfunc);
    curl_easy_setopt(curlhandle, CURLOPT_READFUNCTION, readfunc);
    curl_easy_setopt(curlhandle, CURLOPT_READDATA, f);
    curl_easy_setopt(curlhandle, CURLOPT_FTPPORT, "-"); /* disable passive mode */
    curl_easy_setopt(curlhandle, CURLOPT_FTP_CREATE_MISSING_DIRS, 1L);
    curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION,write_data); //设置回调

    for (c = 0; (r != CURLE_OK) && (c < tries); c++) {
        if (c) { /* yes */
            curl_easy_setopt(curlhandle, CURLOPT_NOBODY, 1L);
            curl_easy_setopt(curlhandle, CURLOPT_HEADER, 1L);
            r = curl_easy_perform(curlhandle);
            if (r != CURLE_OK)
                continue;
            curl_easy_setopt(curlhandle, CURLOPT_NOBODY, 0L);
            curl_easy_setopt(curlhandle, CURLOPT_HEADER, 0L);
            fseek(f, uploaded_len, SEEK_SET);
            curl_easy_setopt(curlhandle, CURLOPT_APPEND, 1L);
        }
        else { /* no */
            curl_easy_setopt(curlhandle, CURLOPT_APPEND, 0L);
        }
        r = curl_easy_perform(curlhandle);
    }
    fclose(f);
    if (r == CURLE_OK)
        return 1;
    else {
        fprintf(stderr, "%s\n", curl_easy_strerror(r));
        return 0;
    }
}
// 下载
int uploadFile::download(CURL *curlhandle, const char * remotepath, const char * localpath, long timeout, long tries)
{
    FILE *f;
    curl_off_t local_file_len = -1 ;
    long filesize =0 ;
    CURLcode r = CURLE_GOT_NOTHING;
    struct stat file_info;
    int use_resume = 0;
    
    //获取本地文件大小信息
    if(stat(localpath, &file_info) == 0)
    {
        local_file_len = file_info.st_size;
        use_resume = 1;
    }
    //追加方式打开文件，实现断点续传
    f = fopen(localpath, "ab+");
    if (f == NULL) {
        perror(NULL);
        return 0;
    }
    curl_easy_setopt(curlhandle, CURLOPT_URL, remotepath);
    curl_easy_setopt(curlhandle, CURLOPT_USERPWD, "wy:123456");
    //连接超时设置
    curl_easy_setopt(curlhandle, CURLOPT_CONNECTTIMEOUT, timeout);
    //设置头处理函数
    curl_easy_setopt(curlhandle, CURLOPT_HEADERFUNCTION, getcontentlengthfunc);
    curl_easy_setopt(curlhandle, CURLOPT_HEADERDATA, &filesize);
    // 设置断点续传
    curl_easy_setopt(curlhandle, CURLOPT_RESUME_FROM_LARGE, use_resume?local_file_len:0);
    curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, f);
    curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION,write_data); //设置回调
    r = curl_easy_perform(curlhandle);
    fclose(f);
    if (r == CURLE_OK)
        return 1;
    else {
        fprintf(stderr, "%s\n", curl_easy_strerror(r));
        return 0;
    }
}

void uploadFile::upLoadFileByFTP(string photoPath)
{
    CURL *curlhandle = NULL;
    CURL *curldwn = NULL;
    curl_global_init(CURL_GLOBAL_ALL);
    curlhandle = curl_easy_init();
    curldwn = curl_easy_init();
    upload(curlhandle, "ftp://211.149.160.248/download",photoPath.c_str(), 1, 3);
    //download(curldwn, "ftp://211.149.160.248/download",(FileUtils::getInstance()->getWritablePath() + "test.txt").c_str(), 1, 3);
    curl_easy_cleanup(curlhandle);
    curl_easy_cleanup(curldwn);
    curl_global_cleanup();
}



size_t uploadFile::write_data(uint8_t *dataBack, size_t size, size_t nmemb, void *user_p)
{
    string szData = string((char*)dataBack); //返回的结果
    
    CCLOG("write_data : %s " , szData.c_str());
    
    return 0;
}