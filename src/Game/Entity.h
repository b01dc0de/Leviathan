#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include "../Leviathan.h"

namespace Game
{
    using EntityID = int;
    using EntityCallback = void (*)(void);

    constexpr EntityID InvalidID = 0;

    struct Entity
    {
        EntityID ID = InvalidID;
        bool bActive = false;
        EntityCallback Tick = nullptr;
        EntityCallback Init = nullptr;
        EntityCallback Term = nullptr;
    };

    struct EntityManager
    {
        Array<Entity> Entities;
        EntityID Counter = InvalidID;

        Entity* GetEntity(EntityID ID);
        EntityID AddEntity(Entity NewEntity);
        bool RemoveEntity(EntityID ID);
        void Tick();
        void Init();
        void Term();
    };
}

using GameEntity = Game::Entity;
using GameEntityManager = Game::EntityManager;

#endif // GAME_ENTITY