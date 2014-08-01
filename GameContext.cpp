#include "GameContext.h"

CTOFNContext::CTOFNContext() : CGameContext(), m_pPlayerEntity( NULL ), m_MaxEnemyCount( 10 ), m_CurEnemyCount( 0 )
{

}

void CTOFNContext::InitializePhysicsWorld()
{

    m_PhysicsWorld.Initialize( 0.0f, 0.0f );
    m_CollisionCallback.SetContext( this );
    m_PhysicsWorld.SetContactListener( &m_CollisionCallback );

}

void CTOFNContext::InitializeLua()
{

    m_Lua.Initialize();
    SetLuaContext( this );

}

CShipEntity * CTOFNContext::CreatePlayerEntity()
{

    static const float defaultPlayerPosX = 350.0f;
    static const float defaultPlayerPosY = 500.0f;

    CShipEntity * ent = new CShipEntity;
    ent->SetClassTypeID( ENTTYPE_PLAYER );
    ent->CreatePhysicsBody( &m_PhysicsWorld, 64, 64 );
    ent->SetMaterial( m_pTextureFactory->GetObjectContent( "PLAYERSPRITE.png" ) );
    ent->DisablePhysicsMovement();
    ent->SetGravity( 0 );
    ent->SetPos( defaultPlayerPosX, defaultPlayerPosY );

    ent->AddGun( 32.0f, 12.0f );

    if( m_pPlayerEntity )
        m_pEntityManager->RemoveEntity( m_pPlayerEntity );

    m_pPlayerEntity = ent;

    m_pEntityManager->AddEntity( m_pPlayerEntity );

    return m_pPlayerEntity;

}

void CTOFNContext::FireBulletFrom( int type, CShipEntity * pShip, int dmg, float speed )
{

    const std::vector< Vector3 > & GunPos = pShip->GetGunPositions();
    float pX, pY;

    pShip.GetPos().Get( &pX, &pY );

    for( int j = 0; j < GunPos.size(); j++ )
    {

        float x, y;
        GunPos[j].Get( &x, &y );

        FireBulletFrom( type, pX + x, pY + y, dmg, speed );

    }

}

CAIEntity * CTOFNContext::FireBulletFrom( int type, float x, float y, int dmg, float speed )
{

    CAIEntity * ent = new CAIEntity;
    CBulletAI * aic = new CBulletAI;

    aic->SetSpeed( speed );
    aic->SetDamage( dmg );
    aic->SetDirection( ( type & ENTTYPE_ENBULLET )? DIR_DOWN : DIR_UP );

    ent->SetClassTypeID( type );
    ent->SetClassType( "BLT" );
    ent->CreatePhysicsBody( &m_PhysicsWorld, 5, 5 );
    ent->SetMaterial( m_pTextureFactory->GetObjectContent( "BULLET.png" ) );
    ent->DisablePhysicsMovement();
    ent->SetGravity( 0 );
    ent->SetPos( x, y );

    aic->SetTargetEntity( ent );
    ent->SetAIController( aic );

    m_pEntityManager->AddEntity( ent );
    m_pEntityManager->TrackEntity( "BLT", ent );

    return ent;

}

CShipEntity * CTOFNContext::CreateRandomEnemyEntity()
{

    CShipEntity * ent = new CShipEntity;
    CAIController * aic = new CEnemyAI;

    ent->SetClassTypeID( ENTTYPE_ENEMY );
    ent->SetClassType( "EN" );
    ent->CreatePhysicsBody( &m_PhysicsWorld, 64, 64 );
    ent->SetMaterial( m_pTextureFactory->GetObjectContent( "ENEMYSPRITE.png" ) );
    ent->DisablePhysicsMovement();
    ent->SetGravity( 0 );
    ent->SetPos( Util::RandomNumber( 100, 810 ), Util::RandomNumber( -500, -200 ) );

    aic->SetTargetEntity( ent );
    ent->SetAIController( aic );

    m_pEntityManager->AddEntity( ent );
    m_pEntityManager->TrackEntity( "EN", ent );

    m_CurEnemyCount++;

    return ent;

}

void CTOFNContext::DoEnemyGenerator()
{

    int n = ( m_MaxEnemyCount - m_CurEnemyCount );
    for( int j = 0; j < n; j++ )
    {

        CreateRandomEnemyEntity();

    }

}

void CTOFNContext::HandleEntityContact( void * pEntityA, int entTypeA, void * pEntityB, int entTypeB )
{

    //Due to the way our collision callback functions, entA is always a player ship or enemy ship
    CWorldEntity * entA = static_cast< CShipEntity * >( pEntityA );
    CWorldEntity * entB = NULL;

    if( entTypeB & ( ENTTYPE_ENBULLET | ENTTYPE_PLYBULLET ) )
        entB = static_cast< CAIEntity * >( pEntityB );
    else if( entTypeB & ENTTYPE_ENEMY )
        entB = static_cast< CShipEntity * >( pEntityB );
    else
        return; //Should never happen

    if( entTypeA & ENTTYPE_PLAYER )
    {

        if( entTypeB & ENTTYPE_ENEMY )
        {

            m_pEntityManager->DeleteEntity( entB );

        } else if( entTypeB & ENTTYPE_ENBULLET )
        {

            entA->Damage( static_cast< CBulletAI * >( entB->GetAIController() )->GetDamage() );
            m_pEntityManager->DeleteEntity( entB );

        }

    } else if( entTypeA & ENTTYPE_ENEMY )
    {

        if( entTypeB & ENTTYPE_PLYBBULLET )
        {

            entA->Damage( static_cast< CBulletAI * >( entB->GetAIController() )->GetDamage() );
            m_pEntityManager->DeleteEntity( entB );

        }

    }

}

void CTOFNContext::UpdateAllEntities()
{

    boost::ptr_vector< CEntityObject > * entityObjs = &m_pEntityManager->GetEntityObjects();

	for( boost::ptr_vector< CEntityObject >::iterator i = entityObjs->begin();
		 i != entityObjs->end(); i++ )
    {

        if( ( *i ).GetContent()->IsActive() )
        {

            ( *i ).GetContent()->Update();

            int type = ( *i ).GetContent()->GetClassTypeID();

            if( type & ( ENTTYPE_PLAYER | ENTTYPE_ENEMY ) )
            {

                CShipEntity * pShip = static_cast< CShipEntity * >( ( *i ).GetContent() );

                if( pShip->GetHealth() <= 0 )
                {

                    m_pEntityManager->DeleteEntity( pShip );

                }

            }

        }

    }

}

void CTOFNContext::GameLogic()
{

    m_PhysicsWorld.Update();

    UpdateAllEntities();

    //DoEnemyGenerator();

    m_Lua.CallEngineFunction( "GameLogic" );

    //In normal operations, entities should be "deleted", when this happens they aren't actually removed from the entity manager until after all update logic is finished.
    m_pEntityManager->RemoveAllDeletedEntities();

}
