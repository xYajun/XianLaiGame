#include "YZGameCreator.h"
#include <assert.h>

static YZGameCreator* sGameCreate = nullptr;

YZGameCreator* YZGameCreator::getInstance()
{
    if (nullptr == sGameCreate)
    {
        sGameCreate = new YZGameCreator();
    }
    return sGameCreate;
}

void YZGameCreator::addGame(UINT uNameId, UINT uKindId, GAMETYPE type,
                            GAME_CREATE_SELECTOR createGameSelector, ROOM_CREATE_SELECTOR createRoomSelector)
{
    ItemCreator creator;
    creator.type = type;
    creator.uNameId = uNameId;
    creator.uKindId = uKindId;
    creator.gameSelector = createGameSelector;
    creator.roomSelector = createRoomSelector;
    creator.priority = ++_basePriority;
    
    assert(creator.valid());
    auto value = std::make_pair(uNameId, creator);
    _creators.insert(value);
    setCurrentGame(creator.uNameId);
}

bool YZGameCreator::startGameClient(UINT uNameId, BYTE bDeskIndex, bool bAutoCreate)
{
    auto iter = _creators.find(uNameId);
    if (iter != _creators.end())
    {
        ItemCreator* creator = &(*iter).second;
        if (creator->valid())
        {
            YZGameUIBase* game = creator->gameSelector(bDeskIndex, bAutoCreate);
            
            if (nullptr != game)
            {
                auto scene = Scene::create();
                scene->addChild(game);
                Director::getInstance()->replaceScene(scene);
                return true;
            }
        }
    }
    return false;
}

bool YZGameCreator::startGameRoom(UINT uNameId, Node* parent)
{
    auto iter = _creators.find(uNameId);
    if (iter != _creators.end())
    {
        YZLayer* room = (*iter).second.roomSelector(parent);
        
        if (nullptr != room)
        {
            return true;
        }
    }
    return false;
}

void YZGameCreator::setCurrentGame(UINT uNameId)
{
    auto iter = _creators.find(uNameId);
    if (iter != _creators.end())
    {
        _currentCreator = &(*iter).second;
    }
    else
    {
        _currentCreator = nullptr;
    }
}

UINT YZGameCreator::getCurrentGameNameID() const
{
    assert(nullptr != _currentCreator);
    
    return (nullptr != _currentCreator) ? _currentCreator->uNameId : 0;
}

UINT YZGameCreator::getCurrentGameKindID() const
{
    assert(nullptr != _currentCreator);
    return (nullptr != _currentCreator) ? _currentCreator->uKindId : -1;
}

BYTE YZGameCreator::validGame(UINT uNameId)
{
    auto iter = _creators.find(uNameId);
    if (iter != _creators.end())
    {
        return iter->second.priority;
    }
    return INVALID_PRIORITY;
}

// 获取有效的游戏
bool YZGameCreator::getValidGames(std::vector<ComNameInfo*>* validGames)
{
    {
        std::vector<ComNameInfo*>().swap(*validGames);
    }
    
    validGames->resize(255);
    INT validCount = 0;
    GamesInfoModule()->transform([&](ComNameInfo* nameInfo, INT index)
                                 {
                                     BYTE level = validGame(nameInfo->uNameID);
                                     if (INVALID_PRIORITY != level)
                                     {
                                         (*validGames)[level] = nameInfo;
                                         ++validCount;
                                     }
                                 });
    
    validGames->resize(validCount);
    
    return !validGames->empty();
}

BYTE YZGameCreator::getGamePriority(UINT uNameId)
{
    auto iter = _creators.find(uNameId);
    if (iter != _creators.end())
    {
        return iter->second.priority;
    }
    return INVALID_PRIORITY;
}

YZGameCreator::GAMETYPE YZGameCreator::getCurrentGameType() const
{
    assert(nullptr != _currentCreator);
    return (nullptr != _currentCreator) ? _currentCreator->type : UNKNOWN;
}

YZGameCreator::YZGameCreator() : _currentCreator(nullptr), _basePriority(INVALID_PRIORITY)
{
    
}

YZGameCreator::~YZGameCreator()
{
}