//
//  ftp-manager.cpp
//  PDKGame
//
//  Created by Dada on 16/7/21.
//
//

#include "ftp-manager.h"




ftpmanager* ftpmanager::m_inst = NULL;

ftpmanager* ftpmanager::GetInst()
{
    if(!m_inst)
    {
        m_inst = new ftpmanager();
        return m_inst;
    }
    return NULL;
}

int get_file_size(FILE *file)
{
    int size = 0;
    fseek(file, 0L, SEEK_END);
    size = ftell(file);
    fseek(file, 0L, SEEK_SET);
    return size;
}

CURL *curl_init()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL *curl = curl_easy_init();
    if(NULL == curl)
    {
        fprintf(stderr, "Init curl failed.\n");
        exit(1);
    }
    return curl;
}

void curl_set_upload_opt(CURL *curl, const char *url, const char *user_key, FILE *file)
{
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_USERPWD, user_key);
    curl_easy_setopt(curl, CURLOPT_READDATA, file);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1);
    curl_easy_setopt(curl, CURLOPT_INFILESIZE, get_file_size(file));
    curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);
    //	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
}

void curl_set_download_opt(CURL *curl, const char *url, const char *user_key, FILE *file)
{
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_USERPWD, user_key);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
    //	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
}

void curl_exit(CURL *curl)
{
    curl_easy_cleanup(curl);
    curl_global_cleanup();
}

CURLcode curl_perform(CURL *curl)
{
    CURLcode ret = curl_easy_perform(curl);
    if(ret != 0)
    {
        fprintf(stderr, "Perform curl failed.\n");
        curl_exit(curl);
        exit(1);
    }
    return ret;
}


/****************ftp upload & download api******************/
FTP_STATE ftpmanager::ftp_upload(const FTP_OPT ftp_option)
{
    FTP_STATE state;
    CURL *curl;;
    FILE *fp = fopen(ftp_option.file, "r");
    if(NULL == fp)
    {
        fprintf(stderr, "Open file failed at %s:%d\n", __FILE__, __LINE__);
        return FTP_UPLOAD_FAILED;
    }
    
    curl = curl_init();
    curl_set_upload_opt(curl, ftp_option.url, ftp_option.user_key, fp);
    if(CURLE_OK == curl_perform(curl))
        state = FTP_UPLOAD_SUCCESS;
    else
        state = FTP_UPLOAD_FAILED;
    
    curl_exit(curl);
    fclose(fp);
    return state;
}

FTP_STATE ftpmanager::ftp_download(const FTP_OPT ftp_option)
{
    FTP_STATE state;
    CURL *curl;
    FILE *fp = fopen(ftp_option.file, "w");
    if(NULL == fp)
    {
        fprintf(stderr, "Open file failed at %s:%d\n", __FILE__, __LINE__);
        return FTP_UPLOAD_FAILED;
    }
    
    curl = curl_init();
    curl_set_download_opt(curl, ftp_option.url, ftp_option.user_key, fp);
    if(CURLE_OK == curl_perform(curl))
        state = FTP_DOWNLOAD_SUCCESS;
    else
        state = FTP_DOWNLOAD_FAILED;
    
    curl_exit(curl);
    fclose(fp);
    return state;
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
        if(!out->stream)
            return -1; /* failure, can't open file to write */
    }
    return fwrite(buffer, size, nmemb, out->stream);
}

void ftpmanager::ftp_down(){
    CURL *curl;
    CURLcode res;
    struct FtpFile ftpfile={
        "/Volumes/Work/trank.txt", /* name to store the file as if successful */
        NULL
    };
    
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    curl = curl_easy_init();
    if(curl) {
        /*
         * You better replace the URL with one that works!
         */
        curl_easy_setopt(curl, CURLOPT_URL,
                         "ftp://211.149.160.248/download/trank.txt");
        /* Define our callback to get called when there's data to be written */
        curl_easy_setopt(curl, CURLOPT_USERPWD, "wy:123456");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
        /* Set a pointer to our struct to pass to the callback */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);
        
        /* Switch on full protocol/debug output */
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        
        res = curl_easy_perform(curl);
        
        /* always cleanup */
        curl_easy_cleanup(curl);
        
        if(CURLE_OK != res) {
            /* we failed */
            fprintf(stderr, "curl told us %d\n", res);
        }
    }
    
    if(ftpfile.stream)
        fclose(ftpfile.stream); /* close the local file */
    
    curl_global_cleanup();

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

#define REMOTE_URL      "ftp://211.149.160.248/download/"
int ftpmanager::ftp_upload(std::string filePath , std::string fileName){
    CURL *curl;
    CURLcode res;
    FILE *hd_src;
    struct stat file_info;
    curl_off_t fsize;
    
    struct curl_slist *headerlist=NULL;
    //static const char buf_1 [] = "RNFR ";
   // static const char buf_2 [] = "RNTO " RENAME_FILE_TO;
    
    /* get the file size of the local file */
    if(stat(filePath.c_str(), &file_info)) {
        printf("Couldnt open '%s': %s\n", filePath.c_str(), strerror(errno));
        return 1;
    }
    fsize = (curl_off_t)file_info.st_size;
    
    printf("Local file size: %" CURL_FORMAT_CURL_OFF_T " bytes.\n", fsize);
    
    /* get a FILE * of the same file */
    hd_src = fopen(filePath.c_str(), "rb");
    
    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);
   
    std::string urlPath = REMOTE_URL + fileName;
    
    /* get a curl handle */
    curl = curl_easy_init();
    if(curl) {
        /* build a list of commands to pass to libcurl */
        //headerlist = curl_slist_append(headerlist, buf_1);
        headerlist = curl_slist_append(headerlist, fileName.c_str());
        
        curl_easy_setopt(curl, CURLOPT_USERPWD, "wy:123456");

        /* we want to use our own read function */
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
        
        /* enable uploading */
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        
        /* specify target */
        curl_easy_setopt(curl, CURLOPT_URL, urlPath.c_str());
        
        /* pass in that last of FTP commands to run after the transfer */
        curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);
        
        /* now specify which file to upload */
        curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
        
        curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                         (curl_off_t)fsize);
        
        /* Now run off and do what you've been told! */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        
        /* clean up the FTP commands list */ 
        curl_slist_free_all (headerlist);
        
        /* always cleanup */ 
        curl_easy_cleanup(curl);
    }
    fclose(hd_src); /* close the local file */ 
    
    curl_global_cleanup();
    
    return 0;
}


void ftpmanager::upLoadFileByFTP(string photoPath)
{
    
    ftp_upload("/Volumes/Work/shengli.mp3" ,"shengli.mp3");

}