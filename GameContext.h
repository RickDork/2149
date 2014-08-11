#ifndef TOFNCONTEXT_H_
#define TOFNCONTEXT_H_

#include "EnemyAI.h"
#include "SoraIncludes.h"
#include "EntityManager.h"
#include "Entity.h"
#include "CollisionCallback.h"
#include "Bullet.h"
#include "ShipEntity.h"
#include "Lua.h"
#include "LuaContext.h"
#include "Star.h"

class CTOFNContext : public CLuaContext
{

private:

    CShipEntity * m_pPlayerEntity;
    int m_MaxEnemyCount;
    int m_CurEnemyCount;

    boost::ptr_vector< CStar > m_pStars;
    const int m_MaxStars;
    int m_InstancedBuffer, m_InstancedRGBABuffer;

    CTOFNLua m_Lua;
    CPhysicsWorld m_PhysicsWorld;
    CCollisionCallback m_CollisionCallback;

    void DoEnemyGenerator();

public:

    void InitializePhysicsWorld();
    void InitializeLua();
    void InitializeGraphics();

    CShipEntity * GetPlayerEntity()
    {

        return m_pPlayerEntity;

    }

    void UpdateAllEntities();

    void CreateStarBackground();
    void DrawStarBackground();

    CShipEntity * CreatePlayerEntity();
    CShipEntity * CreateRandomEnemyEntity();
    CAIEntity * FireBulletFrom( int, float, float, int, float );
    void FireBulletFrom( int, CShipEntity * , int, float );

    void HandleEntityContact( void *, int, void *, int );

    void GameLogic();

    CTOFNContext();

};

#endif

