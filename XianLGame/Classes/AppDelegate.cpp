#include "AppDelegate.h"
#include "LoginLayer.h"
#include "HallLayer.h"
#include "YZExternal/MD5/Encryption.h"
#include <SimpleAudioEngine.h>
#include "Util/JzLog.h"
#include "LoginLayer.h"

USING_NS_CC;
using namespace CocosDenshion;

//只能处理目录:lpPath只能是路径
//void LFind(char *lpPath,std::vector<std::string>& vecImage,std::vector<std::string>& vecName)
//{
//   char szFind[MAX_PATH];
//   char szFile[MAX_PATH];

//    WIN32_FIND_DATAA FindFileData;

//    strcpy(szFind,lpPath);
//    strcat(szFind,"//*.*");

//    HANDLE hFind=::FindFirstFileA(szFind,&FindFileData);
 //   if(INVALID_HANDLE_VALUE == hFind)
//		return;
//    while(TRUE)
//    {
//        if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
//        {
//            if(FindFileData.cFileName[0]!='.')
//            {
//                strcpy(szFile,lpPath);
 //               strcat(szFile,"//");
//                strcat(szFile,FindFileData.cFileName);
//                LFind(szFile,vecImage,vecName);
 //           }
 //       }
//        else
//        {
//			//std::string strTemp = FindFileData.cFileName;
//			//if(strTemp.)
//			char* pTemp = FindFileData.cFileName+strlen(FindFileData.cFileName)-3;
//			if( strcmp(pTemp,"png")==0 || strcmp(pTemp,"jpg")==0 )
//			{
//				std::string strFileName = lpPath;
//				strFileName += "/";
//				strFileName += FindFileData.cFileName;
//				vecImage.push_back(strFileName);
//				vecName.push_back(FindFileData.cFileName);
//			}
//
//        }
//        if(!FindNextFileA(hFind,&FindFileData))
//            break;
//    }
//    FindClose(hFind);
//}


AppDelegate::AppDelegate()
{
//	std::vector<std::string> vecFile;
//	std::vector<std::string> vecName;
//	LFind("fishKing/Input",vecFile,vecName);
//	for(int i=0;i<vecFile.size();i++)
//	{
//		Image ima;
//		std::string strFilePath;
//		strFilePath = "fishKing/Output/";
//		strFilePath += vecName[i];
//
//		Data data = FileUtils::getInstance()->getDataFromFile(vecFile[i]);
//		data=decryptData(data);
//		if (ima.initWithImageData(data.getBytes(),data.getSize()))
//		{
//			ima.saveToFile(strFilePath);
//		}
//	}

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
		glview = GLViewImpl::createWithRect("ZHMG", Rect(0, 0, 1136, 640));
       // glview = GLViewImpl::createWithRect("ZHMG", Rect(0, 0, 1136*0.8f, 640*0.8f));
        director->setOpenGLView(glview);
    }

    director->getOpenGLView()->setDesignResolutionSize(1136, 640, ResolutionPolicy::FIXED_WIDTH);

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    JzLog::shareLog()->setWritePath((FileUtils::getInstance()->getWritablePath()+"/log.txt").c_str());
    JzLog::shareLog()->setWriteToFile(true);
    JzLog::shareLog()->setLogLevel(kLogLevelDebug);
    JzLog::shareLog()->setWriteCache(1);
    
    FileUtils::getInstance()->addSearchPath("res");

    // create a scene. it's an autorelease object
	//auto scene = HallLayer::createScene();
	auto scene = LoginLayer::createScene();
    
    
	//auto scene = Welcome::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
