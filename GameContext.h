#ifndef TOFNCONTEXT_H_
#define TOFNCONTEXT_H_

#include "EnemyAI.h"
#include "SoraIncludes.h"
#include "EntityManager.h"
#include "Entity.h"
#include "CollisionCallback.h"
#include "Bullet.h"

class CTOFNContext : public CGameContext< CTOFNEntityManager >
{

private:

    CWorldEntity * m_pPlayerEntity;
    int m_MaxEnemyCount;
    int m_CurEnemyCount;

    CPhysicsWorld m_PhysicsWorld;
    CCollisionCallback m_CollisionCallback;

    void DoEnemyGenerator();

public:

    void InitializePhysicsWorld();

    CWorldEntity * GetPlayerEntity()
    {

        return m_pPlayerEntity;

    }

    CWorldEntity * CreatePlayerEntity();
    CWorldEntity * CreateRandomEnemyEntity();

    void HandlePlayerContact( void *, void * );
    void HandleEntityContact( void *, void * );

    void GameLogic();

    CTOFNContext();

};

#endif

