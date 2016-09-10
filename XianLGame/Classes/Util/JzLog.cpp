//
//  JzLog.cpp
//  test
//
//  Created by pabble on 14-8-1.
//
//

#include "JzLog.h"
#include "cocos2d.h"
#include "DateUtils.h"

#define DEBUG false

static const int kMaxTextInView = 50;   //列表最多显示数目
static const int kMaxLogLen = 16*1024;
static const int kLogViewTag = 123456;  //列表tag

JzLog* JzLog::m_pSingle = NULL;

JzLog* JzLog::shareLog(){
    if (m_pSingle == NULL ){
        m_pSingle = new JzLog();
    }
    return m_pSingle;
}

JzLog::JzLog():
m_iLogCount(0)
,m_iWriteCache(5)
,m_bWriteToFile(false)
,m_iLogLevel(kLogLevelError)
,m_oStrWritePath("")
{
}

string  JzLog::subFile(string file){
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    return file.substr(file.rfind("\\")+1,file.length());
#endif
    return file.substr(file.rfind("/")+1,file.length());
}

void JzLog::setLogLevel(logLevel level){
    m_iLogLevel = level;
}

void JzLog::setWritePath(const char *pPath){
    m_oStrWritePath = pPath;
}

void JzLog::setWriteCache(int cache){
    m_iWriteCache  = cache;
}

void JzLog::setWriteToFile(bool bFlag){
    m_bWriteToFile = bFlag;
    if (bFlag) {
        CCLOG("logo file = %s",m_oStrWritePath.c_str());
    }
}

void JzLog::log(logLevel level, const char* pszFormat, ...){
    if(level <= m_iLogLevel){
        char szBuf[kMaxLogLen + 1] = {0};
        va_list ap;
        va_start(ap, pszFormat);
        vsnprintf(szBuf, kMaxLogLen, pszFormat, ap);
        va_end(ap);
        
        string temp = "";
        
        switch (level) {
            case kLogLevelError:
                temp="[error]";
                break;
            case kLogLevelWarning:
                temp="[warn]";
                break;
            case kLogLevelDebug:
                temp="[debug]";
                break;
            case kLoglevelInfo:
                temp="[info]";
                break;
            default:
                temp="[info]";
                break;
        }
        temp += "  ";
        temp += DateUtils::getCurrentTime() +  "  ";
        temp += szBuf;
        
        temp+="\n";
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        CCLOG("%s",temp.c_str());
#else
        printf("%s",temp.c_str());
#endif
        this->writeToFile(temp.c_str());
        
        if (DEBUG) {
            //刷新
            auto runningScene = Director::getInstance()->getRunningScene();
            if(runningScene != NULL){
                
                auto listView = (JzLogView*)runningScene->getChildByTag(kLogViewTag);
                
                if(listView != NULL){
                    
                    listView->insertLog(temp,level);
                }else{
                    
                    bindViewToScene(runningScene);
                }
            }
            
            //保持数据列表数目低于预设值
            if(m_logList.size() > kMaxTextInView)
                m_logList.erase(m_logList.begin());
            
            m_logList.push_back(JzLogStruct(level,temp));
        }
    }
}

void JzLog::writeToFile(const char* pCotent){
    if (!m_bWriteToFile) {
        return;
    }
    m_oStrLogCache += pCotent;
    m_iLogCount ++;
    if (m_iLogCount >= m_iWriteCache){
        ofstream stream;
        stream.open(m_oStrWritePath.c_str(),ios::app);
        stream.write(m_oStrLogCache.c_str(), m_oStrLogCache.size());
        stream.close();
        m_oStrLogCache = "";
        m_iLogCount = 0;
    }
}

void JzLog::flushFile(){
    if (!m_bWriteToFile) {
        return;
    }
    if(m_oStrLogCache.length() > 0){
        if (m_iLogCount >= m_iWriteCache){
            ofstream stream;
            stream.open(m_oStrWritePath.c_str(),ios::app);
            stream.write(m_oStrLogCache.c_str(), m_oStrLogCache.size());
            stream.close();
            m_oStrLogCache = "";
            m_iLogCount = 0;
        }
    }
}

void JzLog::deleteFile(){
    
}


void JzLog::bindViewToScene(Scene* parent){
    
    if (DEBUG) {
        if(parent->getChildByTag(kLogViewTag) == NULL){
            
            auto m_logView = JzLogView::create();
            m_logView->setTag(kLogViewTag);
            parent->addChild(m_logView,999);
            
            for(auto temp : m_logList){
                
                m_logView->insertLog(temp._logStr, temp._logLevel);
            }
        }
        
    }
};

bool JzLogView::init(){
    
    if(Layer::init() == false)  return false;
    
    m_listView = ListView::create();
    m_listView->setSwallowTouches(true);
    m_listView->setContentSize(Director::getInstance()->getWinSize());
    m_listView->setClippingEnabled(true);
    m_listView->setBackGroundColor(ccc3(50,50,50));
    m_listView->setBackGroundColorOpacity(150);
    m_listView->setBackGroundColorType(ListView::BackGroundColorType::SOLID);
    m_listView->setVisible(false);
    this->addChild(m_listView);
    
    m_button = Button::create();
    m_button->loadTextureNormal("open_log.png");
    m_button->setAnchorPoint(Point(1,1));
    m_button->setPosition(Point(Director::getInstance()->getWinSize().width,Director::getInstance()->getWinSize().height));
    this->addChild(m_button);
    
    m_button->addTouchEventListener([this](Ref* pSender,Widget::TouchEventType type){
        
        if(type != Widget::TouchEventType::ENDED)   return;
        
        m_listView->setVisible(!this->m_listView->isVisible());
    });
    
    return true;
}

void JzLogView::insertLog(string logStr,logLevel level){
    
    auto children = m_listView->getChildren();
    if(children.size() > kMaxTextInView){
        
        m_listView->removeItem(0);
    }
    
    float listWidth = Director::getInstance()->getWinSize().width;
    
    Text* logText = Text::create();
    logText->setAnchorPoint(ccp(0,0));
    logText->ignoreContentAdaptWithSize(false);
    logText->setContentSize(Size(listWidth,  60));
    logText->setString(logStr.c_str());
    logText->setFontSize(22);
    m_listView->addChild(logText);
    
    m_listView->refreshView();
    
    m_listView->scrollToBottom(0.1, true);
    
    switch (level) {
        case kLogLevelError:
            logText->setColor(ccc3(255,100,100));
            break;
        case kLogLevelWarning:
            logText->setColor(ccc3(255,247,123));
            break;
        case kLogLevelDebug:
            logText->setColor(ccc3(255,255,255));
            break;
        case kLoglevelInfo:
            logText->setColor(ccc3(255,123,123));
            break;
        default:
            logText->setColor(ccc3(0,0,0));
            break;
    }
}
