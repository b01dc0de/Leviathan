#include "Entity.h"

namespace Game
{
    Entity* EntityManager::GetEntity(EntityID ID)
    {
        Entity* Result = nullptr;
        for (int EntityIdx = 0; EntityIdx < Entities.Num; EntityIdx++)
        {
            if (Entities[EntityIdx].ID == ID)
            {
                Result = &Entities[EntityIdx];
                break;
            }
        }
        return Result;
    }

    EntityID EntityManager::AddEntity(Entity NewEntity)
    {
        NewEntity.ID = ++Counter;
        Entities.Add(NewEntity);
        NewEntity.bActive = true;
        if (NewEntity.Init) { NewEntity.Init(); }
        return Counter;
    }

    bool EntityManager::RemoveEntity(EntityID ID)
    {
        bool bFound = false;
        for (int EntityIdx = 0; EntityIdx < Entities.Num; EntityIdx++)
        {
            if (Entities[EntityIdx].ID == ID)
            {
                Entity& EntityToRemove = Entities[EntityIdx];
                if (EntityToRemove.Term) { EntityToRemove.Term(); }

                Entity& LastToClear = Entities.Last();
                Entities.Remove(EntityIdx);
                LastToClear = {};

                bFound = true;
                break;
            }
        }
        return bFound;
    }

    void EntityManager::Tick()
    {
        for (int EntityIdx = 0; EntityIdx < Entities.Num; EntityIdx++)
        {
            if (Entities[EntityIdx].bActive)
            {
                Entity& CurrEntity = Entities[EntityIdx];
                if (CurrEntity.Tick) { CurrEntity.Tick(); }
            }
        }
    }

    void EntityManager::Init()
    {
        constexpr int InitCapacity = 32;
        Entities.Reserve(InitCapacity);
    }

    void EntityManager::Term()
    {
        for (int EntityIdx = 0; EntityIdx < Entities.Num; EntityIdx++)
        {
            Entity& CurrEntity = Entities[EntityIdx];
            if (CurrEntity.Term) { CurrEntity.Term(); }
            CurrEntity = {};
        }
        Entities.Empty();
    }
}

