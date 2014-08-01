#include "GameState.h"

CGameState::CGameState() : CState()
{



}

void CGameState::Init()
{

    Log::Debug( "Doing game state init" );

    m_pGameContext->InitializePhysicsWorld();
    m_pGameContext->InitializeLua();

    m_bInit = true;

}

void CGameState::PostInit()
{

    Log::Debug( "Doing game state post-init" );

    m_pPlayerEntity = m_pGameContext->CreatePlayerEntity();

    m_bPostInit = true;

    Log::Debug( "Ready to do game loop" );

}

void CGameState::Input()
{

    m_GameInput.Poll();

    static const float plyMoveSpeedX = 100.0f;
    static const float plyMoveSpeedY = 100.0f;

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

        m_pGameContext->FireBulletFrom( ENTTYPE_PLYBULLET, m_pPlayerEntity, 5, 600.0f );

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

        m_pGameContext->EntityManager()->DrawAllEntities();

    m_pGameContext->GraphicsContext()->SwapBuffers();

}
