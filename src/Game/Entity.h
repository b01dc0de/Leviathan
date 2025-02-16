#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include "../Leviathan.h"

namespace Game
{
    using GameEntityID = int;
    using GameEntityCallback = void (*)(void*);

    constexpr GameEntityID InvalidID = 0;

    struct GameEntity
    {
        GameEntityID ID = InvalidID;
        bool bActive = false;
        void* pData = nullptr;
        GameEntityCallback Tick = nullptr;
        GameEntityCallback Init = nullptr;
        GameEntityCallback Term = nullptr;
    };

    struct EntityManager
    {
        Array<GameEntity> Entities;
        GameEntityID Counter = InvalidID;

        GameEntity* GetEntity(GameEntityID ID);
        GameEntityID AddEntity(GameEntity NewEntity);
        bool RemoveEntity(GameEntityID ID);
        void Tick();
        void Init();
        void Term();
    };
}

using Game::GameEntity;
using Game::EntityManager;

#endif // GAME_ENTITY