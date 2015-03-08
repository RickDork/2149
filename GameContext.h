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

#define MAX_STARS 2000

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
    int m_RetryCount;
    int m_CurrentMission;
    int m_StartingEXP;
    int m_PlayerKillCount;
    bool m_bMissionOver;
    bool m_bGameFrozen;
    float m_StarFieldSpeedMul;
    bool m_bBossMode;
    float m_BossHealthPercent;

    boost::ptr_vector< CStar > m_pStars;
    boost::ptr_vector< CParticleExplosion > m_pExplosions;
    
    std::vector< int > m_Upgrades;
    
    CInstancedParticleEngine m_StarEngine;
    bool m_bCreatedStarField, m_bStarFieldUpgradeSelect;
    bool m_bStarFieldSlowFill;
    int m_StarFieldSlowFillIndex;
    long int m_StarFieldSlowFillNextTime;
    int m_UpgradeSelectMenuDelay;
    //GLuint m_InstancedBuffer, m_InstancedRGBABuffer;

    CTOFNLua m_Lua;
    CPhysicsWorld m_PhysicsWorld;
    CTextureImage img;
    CCollisionCallback m_CollisionCallback;

	std::vector< CEnemyData > m_EnemyData;
    std::vector< CEnemyGenQueue > m_EnemyGenQueue;
    
    int m_MaxGenQueueProbability;
    
    bool m_bGameTicksFrozen;
    long int m_GameTicksFreezeTime;
    
    bool m_bDrawHUD, m_bPlayerInput;
    bool bIter;
    std::vector< CEntity * > bulletvec;
    
    long int m_GameStartTime;

    void DoEnemyGenerator();

	void RandomizeStar( CStar * );

	void LoadEnemyData();

    std::vector< int > m_SelectableUpgrades;
    
public:
    

    void InitializePhysicsWorld();
    void InitializeLua();
    void InitializeGraphics();
    void InitializeData();
    void Initialize();
    
    void GameplayStart();
    
    void NextMission();
    
    void SetStarBackgroundSpeedMul( float m ) {
     
        m_StarFieldSpeedMul = m;
        
    }
    
    bool IsGameFrozen() {
     
        return m_bGameFrozen;
        
    }
    
    void ToggleGameFrozen( bool b ) {
     
        m_bGameFrozen = b;
        
        if( b )
            FreezeTicksElapsed();
        else
            UnfreezeTicksElapsed();
        
    }
    
    float GetBossHealthPercent() {
     
        return m_BossHealthPercent;
        
    }
    
    void SetBossHealthPercent( float f ) {
     
        m_BossHealthPercent = f;
        
    }
    
    bool ShouldDrawHUD() {
     
        return m_bDrawHUD;
        
    }
    
    bool PlayerInputEnabled() {
     
        return m_bPlayerInput;
        
    }
    
    int GetPlayerKillCount() {
     
        return m_PlayerKillCount;
        
    }
    
    bool GetBossMode() {
     
        return m_bBossMode;
        
    }
    
    void ToggleBossMode( bool b ) {
     
        m_bBossMode = b;
        
    }
    
    void GetCurrentSelectableUpgrades();
    
    int NumberOfSelectableUpgrades() {
     
        return m_SelectableUpgrades.size();
        
    }
    
    int GetSelectableUpgrade( int i ) {
     
        return m_SelectableUpgrades.at( i );
        
    }
    
    void ToggleStarFieldSlowFill( bool b ) {
     
        m_bStarFieldSlowFill = b;
        
    }
    
    void SetStarFieldComingAtYou( bool b ) {
     
        m_bStarFieldUpgradeSelect = b;
        
    }
    
    void SetUpgradeSelectMenuDelay( int i ) {
     
        m_UpgradeSelectMenuDelay = i;
        
    }
    
    int GetUpgradeMenuSelectMenuDelay() {
     
        return m_UpgradeSelectMenuDelay;
        
    }
    
    void TogglePlayerInput( bool b ) {
     
        m_bPlayerInput = b;
        
    }
    
    bool IsMissionOver() {
        
        return m_bMissionOver;
        
    }
    
    void SetShouldDrawHUD( bool b ) {
     
        m_bDrawHUD = b;
        
    }
    
    int GetCurrentMission() {
        
        return m_CurrentMission;
        
    }
    
    int GetEnemyCount() {
     
        return m_CurEnemyCount;
        
    }
    
    int GetRetryCount() {
     
        return m_RetryCount;
        
    }
    
    void SetRetryCount( int c ) {
     
        m_RetryCount = c;
        
    }
    
    void IncrementRetryCount() {
     
        m_RetryCount++;
        
    }

    void SetGameStartTime( int t ) {
     
        m_GameStartTime = t;
        
    }
    
    CTOFNLua & Lua() {
     
        return m_Lua;
        
    }
    
    void IncrementEnemyCount() {
     
        m_CurEnemyCount++;
        
    }
    
    void FreezeTicksElapsed() {
     
        m_GameTicksFreezeTime = TicksElapsed();
        m_bGameTicksFrozen = true;
        
    }
    
    void UnfreezeTicksElapsed() {
     
        m_GameStartTime = SDL_GetTicks() - m_GameTicksFreezeTime;
        m_bGameTicksFrozen = false;
        
    }
    
    long int TicksElapsed() {
        
        return ( m_bGameTicksFrozen )? m_GameTicksFreezeTime : SDL_GetTicks() - m_GameStartTime;
        
    }
    
    void SetMaxEnemyCount( int n ) {
     
        m_MaxEnemyCount = n;
        
    }
    
    int GetCurrentEnemyCount() {
     
        return m_CurEnemyCount;
        
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

    void GiveUpgrade( int, int );
    void RemoveUpgrade( int, int );
    bool HasUpgrade( int ); 
    
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
    int CreateEnemyFormation( bool, int, int );
    int CreateRandomEnemyFormation( bool );
    CShipEntity * CreateRandomEnemyEntity();
    CShipEntity * CreateRandomEnemyEntity( int );
    int GetRandomEnemyType();
    CAIEntity * FireBulletFrom( int, float, float, float, float, float );
    CAIEntity * FireBulletFrom( int, float, float, float, float );
    void FireBulletFromGunAtAngle( int, int, CShipEntity *, float, float );
    void FireBulletFrom( int, CShipEntity * , float );

    void HandleEntityContact( void *, int, void *, int );

    void GameLogic();

    CTOFNContext();
    virtual ~CTOFNContext() { }

};

#endif

