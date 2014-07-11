#include "GameState.h"

CGameState::CGameState() : CState()
{



}

void CGame::Init()
{

    Log::Debug( "Doing game state init" );

    m_PhysicsWorld.Initialize( 0.0f, 0.0f );

    m_bInit = true;

}

void CGame::PostInit()
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

    if( m_GameInput.KeyDown( SDL_SCANCODE_ESCAPE ) )
    {

		   m_bContinue = false;
	}

}

void CGameState::Think()
{

    //No physics needed
    //m_PhysicsWorld.Update();

    m_pGameContext->EntityManager()->UpdateAllEntities();

    m_pGameContext->GameLogic();

    //In normal operations, entities should be "deleted", when this happens they aren't actually removed from the entity manager until after all update logic is finished.
    m_pGameContext->EntityManager()->RemoveAllDeletedEntities();

}

void CGameState::Draw()
{

    m_pGameContext->GraphicsContext()->ClearBuffer();

        m_pGameContext->EntityManager()->DrawAllEntities();

    m_pGameContext->GraphicsContext()->SwapBuffers();

}
