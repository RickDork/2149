#include "GameContext.h"

CTOFNContext::CTOFNContext() : CGameContext(), m_pPlayerEntity( NULL ), m_MaxEnemyCount( 10 ), m_CurEnemyCount( 0 )
{

}

CWorldEntity * CTOFNContext::CreatePlayerEntity()
{

    static const float defaultPlayerPosX = 350.0f;
    static const float defaultPlayerPosY = 500.0f;

    CWorldEntity * ent = new CWorldEntity;
    ent->CreatePhysicsBody( 64, 64 );
    ent->SetMaterial( m_pTextureFactory->GetObjectContent( "PLAYERSPRITE.png" ) );
    ent->DisablePhysics();
    ent->SetPos( defaultPlayerPosX, defaultPlayerPosY );

    if( m_pPlayerEntity )
        m_pEntityManager->RemoveEntity( m_pPlayerEntity );

    m_pPlayerEntity = ent;

    m_pEntityManager->AddEntity( m_pPlayerEntity );

    return m_pPlayerEntity;

}

CWorldEntity * CTOFNContext::CreateRandomEnemyEntity()
{

    CAIEntity * ent = new CAIEntity;
    CAIController * aic = new CEnemyAI;
    ent->SetClassType( "EN" );
    ent->CreatePhysicsBody( 64, 64 );
    ent->SetMaterial( m_pTextureFactory->GetObjectContent( "ENEMYSPRITE.png" ) );
    ent->DisablePhysics();
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

void CTOFNContext::GameLogic()
{

    //DoEnemyGenerator();

}
