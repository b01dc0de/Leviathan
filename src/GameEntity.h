#ifndef LEVIATHAN_GAMEENTITY_H
#define LEVIATHAN_GAMEENTITY_H

#include "Leviathan.h"

namespace Leviathan
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

#endif // LEVIATHAN_GAMEENTITY_H

