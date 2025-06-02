#include "GameEntity.h"

namespace Leviathan
{

GameEntity* EntityManager::GetEntity(GameEntityID ID)
{
    GameEntity* Result = nullptr;
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

GameEntityID EntityManager::AddEntity(GameEntity NewEntity)
{
    NewEntity.ID = ++Counter;
    Entities.Add(NewEntity);
    NewEntity.bActive = true;
    if (NewEntity.Init) { NewEntity.Init(NewEntity.pData); }
    return Counter;
}

bool EntityManager::RemoveEntity(GameEntityID ID)
{
    bool bFound = false;
    for (int EntityIdx = 0; EntityIdx < Entities.Num; EntityIdx++)
    {
        if (Entities[EntityIdx].ID == ID)
        {
            GameEntity& EntityToRemove = Entities[EntityIdx];
            if (EntityToRemove.Term) { EntityToRemove.Term(EntityToRemove.pData); }

            GameEntity& LastToClear = Entities.Last();
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
            GameEntity& CurrEntity = Entities[EntityIdx];
            if (CurrEntity.Tick) { CurrEntity.Tick(CurrEntity.pData); }
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
        GameEntity& CurrEntity = Entities[EntityIdx];
        if (CurrEntity.Term) { CurrEntity.Term(CurrEntity.pData); }
        CurrEntity = {};
    }
    Entities.Empty();
}

} // namespace Leviathan

