#ifndef LUACONTEXT_H_
#define LUACONTEXT_H_

#include "SoraIncludes.h"
#include "EntityManager.h"
#include "ShipEntity.h"

class CLuaContext : public CGameContext< CTOFNEntityManager >
{

public:

    virtual void SetMaxEnemyCount( int n ) = 0;
    virtual CShipEntity * GetPlayerEntity() = 0;
    virtual CShipEntity * CreatePlayerEntity() = 0;
    virtual CShipEntity * CreateRandomEnemyEntity() = 0;
    virtual CShipEntity * CreateRandomEnemyEntity( int ) = 0;
    virtual int CreateEnemyFormation( bool, int, int ) = 0;
    virtual int CreateRandomEnemyFormation( bool ) = 0;
    virtual CAIEntity * FireBulletFrom( int, float, float, float, float, float ) = 0;
    virtual CAIEntity * FireBulletFrom( int type, float x, float y, float dmg, float speed ) = 0;
    virtual void FireBulletFrom( int type, CShipEntity * pShip, float speed ) = 0;
    virtual void AddEnemyToGenQueue( int, float ) = 0;
    virtual void ClearGenQueue() = 0;
    virtual long int TicksElapsed() = 0;
    virtual void FreezeTicksElapsed() = 0;
    virtual void UnfreezeTicksElapsed() = 0;
    virtual int GetRetryCount() = 0;
    virtual int GetEnemyCount() = 0;
    virtual int GetCurrentMission() = 0;
    virtual void SetRetryCount( int ) = 0;
    virtual void IncrementEnemyCount() = 0;
    virtual void NextMission() = 0;
    virtual void SetShouldDrawHUD( bool ) = 0;
    virtual void TogglePlayerInput( bool ) = 0;
    virtual int GetPlayerKillCount() = 0;
    virtual void ToggleGameFrozen( bool ) = 0;
    virtual void ToggleBossMode( bool ) = 0;
    virtual void SetBossHealthPercent( float ) = 0;
    virtual void SetBossHealth( float ) = 0;
    virtual void ToggleScene( bool ) = 0;
    virtual void CreateExplosionsAndSmoke( int, int, float, float ) = 0;
    virtual void DrawFunkyBackground1() = 0;
    virtual void DrawFunkyBackground2() = 0;
    virtual void TriggerEnding() = 0;
    virtual void ExplodePlayer() = 0;
    virtual void PlayMusic( std::string ) = 0;
    virtual void PlayMusic( std::string, float ) = 0;
    virtual void PlaySound( std::string, float ) = 0;
    virtual void ClearAllEnemies() = 0;
    virtual void FadeMusic( int, float, float ) = 0;
    virtual void EndGame() = 0;
    
	CLuaContext() : CGameContext() { }
	virtual ~CLuaContext() { }

};

#endif
