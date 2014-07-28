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

CWorldEntity * CTOFNContext::CreatePlayerEntity()
{

    static const float defaultPlayerPosX = 350.0f;
    static const float defaultPlayerPosY = 500.0f;

    CWorldEntity * ent = new CWorldEntity;
    ent->SetClassTypeID( ENTTYPE_PLAYER );
    ent->CreatePhysicsBody( &m_PhysicsWorld, 64, 64 );
    ent->SetMaterial( m_pTextureFactory->GetObjectContent( "PLAYERSPRITE.png" ) );
    ent->DisablePhysicsMovement();
    ent->SetGravity( 0 );
    ent->SetPos( defaultPlayerPosX, defaultPlayerPosY );

    if( m_pPlayerEntity )
        m_pEntityManager->RemoveEntity( m_pPlayerEntity );

    m_pPlayerEntity = ent;

    m_pEntityManager->AddEntity( m_pPlayerEntity );

    return m_pPlayerEntity;

}

CWorldEntity * CTOFNContext::FireBulletFrom( int type, float x, float y, float speed )
{

    CAIEntity * ent = new CAIEntity;
    CBulletAI * aic = new CBulletAI;

    aic->SetSpeed( speed );
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

CWorldEntity * CTOFNContext::CreateRandomEnemyEntity()
{

    CAIEntity * ent = new CAIEntity;
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

void CTOFNContext::HandlePlayerContact( void * pPlayer, void * pEntity )
{

    CWorldEntity * ply = static_cast< CWorldEntity * >( pPlayer );
    CWorldEntity * ent = static_cast< CWorldEntity * >( pEntity );

    if( ent->GetClassTypeID() & ENTTYPE_ENEMY )
    {

        m_pEntityManager->DeleteEntity( ent );

    } else if( ent->GetClassTypeID() & ENTTYPE_ENBBULLET )
    {



    }

}

void CTOFNContext::HandleEntityContact( void * pEntityA, void * pEntityB )
{

    CWorldEntity * entA = static_cast< CWorldEntity * >( pEntityA );
    CWorldEntity * entB = static_cast< CWorldEntity * >( pEntityB );

    if( ( entA->GetClassTypeID() | entB->GetClassTypeID() ) & ENTTYPE_ENEMY )
    {

        CWorldEntity * targetEnt;
        CWorldEntity * bulletEnt;
        bool bulletContact = false;

        if( entA->GetClassTypeID() & ENTTYPE_ENBBULLET )
        {

            bulletEnt = entA;
            targetEnt = entB;
            bulletContact = true;

        } else
        {

            bulletEnt = entB;
            targetEnt = entA;
            bulletContact = true;

        }

    }

}

void CTOFNContext::GameLogic()
{

    m_PhysicsWorld.Update();

    m_pEntityManager->UpdateAllEntities();

    //DoEnemyGenerator();

    //In normal operations, entities should be "deleted", when this happens they aren't actually removed from the entity manager until after all update logic is finished.
    m_pEntityManager->RemoveAllDeletedEntities();

}
