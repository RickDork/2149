#include "GameState.h"

CGameState::CGameState() : CState()
{



}

void CGameState::Init()
{

    Log::Debug( "Doing game state init" );

    m_pGameContext->InitializePhysicsWorld();
    m_pGameContext->InitializeLua();
    m_pGameContext->InitializeGraphics();
    m_pGameContext->InitializeData();

    m_bInit = true;

}

void CGameState::PostInit()
{

    Log::Debug( "Doing game state post-init" );

	m_pGameContext->TextureFactory()->NewTexture( "player2.png" );
	m_pGameContext->TextureFactory()->NewTexture("star.png");
	m_pGameContext->TextureFactory()->NewTexture("bullet.png");
	m_pGameContext->TextureFactory()->NewTexture("Enemy.png");
	m_pGameContext->TextureFactory()->NewTexture("Enemy2.png");
	m_pGameContext->TextureFactory()->NewTexture("Enemy3.png");
	m_pGameContext->TextureFactory()->NewTexture("Enemy4.png");

    m_pPlayerEntity = m_pGameContext->CreatePlayerEntity();

	m_pGameContext->CreateStarBackground();

    m_bPostInit = true;

    Log::Debug( "Ready to do game loop" );

}

void CGameState::Input()
{

    m_GameInput.Poll();

    static const float plyMoveSpeedX = 400.0f;
    static const float plyMoveSpeedY = 400.0f;

    if( m_GameInput.KeyDown( SDL_SCANCODE_A ) )
        m_pPlayerEntity->Displace( -1.0f * plyMoveSpeedX * m_pGameContext->GetFrameDelta(), 0.0f );

    if( m_GameInput.KeyDown( SDL_SCANCODE_D ) )
        m_pPlayerEntity->Displace( plyMoveSpeedX * m_pGameContext->GetFrameDelta(), 0.0f );

    if( m_GameInput.KeyDown( SDL_SCANCODE_W ) )
        m_pPlayerEntity->Displace( 0.0f, -1.0f * plyMoveSpeedY * m_pGameContext->GetFrameDelta() );

    if( m_GameInput.KeyDown( SDL_SCANCODE_S ) )
        m_pPlayerEntity->Displace( 0.0f, plyMoveSpeedY * m_pGameContext->GetFrameDelta() );

    if( m_GameInput.KeyDown( SDL_SCANCODE_SPACE ) )
    {

		if( m_pPlayerEntity->CanShoot() )
		{

			m_pGameContext->FireBulletFrom( ENTTYPE_PLYBULLET, m_pPlayerEntity, 5, 600.0f );
			m_pPlayerEntity->SetNextShotTime( SDL_GetTicks() + 300 );

		}

    }

    if( m_GameInput.KeyDown( SDL_SCANCODE_ESCAPE ) )
    {

		   m_bContinue = false;
	}

}

void CGameState::Think()
{

    m_pGameContext->GameLogic();

}

void CGameState::Draw()
{

    m_pGameContext->GraphicsContext()->ClearBuffer();

        m_pGameContext->DrawStarBackground();

        m_pGameContext->EntityManager()->DrawAllEntities();

    m_pGameContext->GraphicsContext()->SwapBuffers();

}
