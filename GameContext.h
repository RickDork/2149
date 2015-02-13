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
#include "ParticleExplosion.h"
#include "Define.h"
#include "OrbEntity.h"
#include "OrbAI.h"

#define MAX_STARS 1500

class CEnemyGenQueue {
  
public:
    
    int m_Type;
    float m_Probability;
    
    CEnemyGenQueue() : m_Type( 0 ), m_Probability( 1.0f ) {
        
    }
    
    CEnemyGenQueue( int i, float p ) : m_Type( i ), m_Probability( p ) {
        
    }
    
};

class CTOFNContext : public CLuaContext
{

private:

    CShipEntity * m_pPlayerEntity;
    int m_MaxEnemyCount;
    int m_CurEnemyCount;
    int m_NextEnemySpawn;
    int m_PlayerEXP;

    boost::ptr_vector< CStar > m_pStars;
    boost::ptr_vector< CParticleExplosion > m_pExplosions;
    
    CInstancedParticleEngine m_StarEngine;
    //GLuint m_InstancedBuffer, m_InstancedRGBABuffer;

    CTOFNLua m_Lua;
    CPhysicsWorld m_PhysicsWorld;
    CTextureImage img;
    CCollisionCallback m_CollisionCallback;

	std::vector< CEnemyData > m_EnemyData;
    std::vector< CEnemyGenQueue > m_EnemyGenQueue;
    
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

    void SetMaxEnemyCount( int n ) {
     
        m_MaxEnemyCount = n;
        
    }
    
    int GetPlayerEXP() {
     
        return m_PlayerEXP;
        
    }
    
    void SetPlayerEXP( int exp ) {
     
        m_PlayerEXP = exp;
        
    }
    
    CShipEntity * GetPlayerEntity()
    {

        return m_pPlayerEntity;

    }

    void UpdateAllEntities();
    void UpdateExplosions();

    void CreateStarBackground();
    void DrawStarBackground();
    void DrawExplosions();

    void DestroyShip( CShipEntity *, bool );
    
    void AddEnemyToGenQueue( int, float );
    void ClearGenQueue();

    COrbEntity * CreateOrb( int, float, float );
    void CreateOrbs( int, int, float, float );
    CParticleExplosion * CreateExplosion( int, float, float );
    CShipEntity * CreatePlayerEntity();
	CShipEntity * CreateEnemyEntity( int, float, float, float );
    int CreateRandomEnemyFormation( bool );
    CShipEntity * CreateRandomEnemyEntity();
   
    CAIEntity * FireBulletFrom( int, float, float, float, float );
    void FireBulletFrom( int, CShipEntity * , float );

    void HandleEntityContact( void *, int, void *, int );

    void GameLogic();

    CTOFNContext();

};

#endif

