#ifndef LUACONTEXT_H_
#define LUACONTEXT_H_

#include "SoraIncludes.h"
#include "EntityManager.h"
#include "ShipEntity.h"

class CLuaContext : public CGameContext< CTOFNEntityManager >
{

public:

    virtual CShipEntity * GetPlayerEntity() = 0;
    virtual CShipEntity * CreatePlayerEntity() = 0;
    virtual CShipEntity * CreateRandomEnemyEntity() = 0;
    virtual  CAIEntity * FireBulletFrom( int type, float x, float y, int dmg, float speed ) = 0;
    virtual void FireBulletFrom( int type, CShipEntity * pShip, int dmg, float speed ) = 0;

	CLuaContext() : CGameContext() { }
	virtual ~CLuaContext() { }

};

#endif
