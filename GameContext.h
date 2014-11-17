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
#include "EnemyData.h"

#define MAX_STARS 1500

class CTOFNContext : public CLuaContext
{

private:

    CShipEntity * m_pPlayerEntity;
    int m_MaxEnemyCount;
    int m_CurEnemyCount;

    boost::ptr_vector< CStar > m_pStars;
    GLuint m_InstancedBuffer, m_InstancedRGBABuffer;

    CTOFNLua m_Lua;
    CPhysicsWorld m_PhysicsWorld;
    CTextureImage img;
    CCollisionCallback m_CollisionCallback;

	std::vector< CEnemyData > m_EnemyData;
    
    bool bIter;
    std::vector< CEntity * > bulletvec;

    void DoEnemyGenerator();

	void RandomizeStar( CStar * );

	void LoadEnemyData();
 
public:

    void InitializePhysicsWorld();
    void InitializeLua();
    void InitializeGraphics();
    void InitializeData();

    CShipEntity * GetPlayerEntity()
    {

        return m_pPlayerEntity;

    }

    void UpdateAllEntities();

    void CreateStarBackground();
    void DrawStarBackground();

    void DestroyShip( CShipEntity *, bool );

    CAnimEntity * CreateExplosion( int, float, float );
    CShipEntity * CreatePlayerEntity();
	CShipEntity * CreateEnemyEntity( int, float, float );
    CShipEntity * CreateRandomEnemyEntity();
    CAIEntity * FireBulletFrom( int, float, float, int, float );
    void FireBulletFrom( int, CShipEntity * , int, float );

    void HandleEntityContact( void *, int, void *, int );

    void GameLogic();

    CTOFNContext();

};

#endif

