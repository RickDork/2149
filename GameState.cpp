#include "GameState.h"

CGameState::CGameState() : CState()
{

    m_PhysicsWorld.Initialize( 0.0f, 0.0f );

}

void CGameState::Input()
{

    m_GameInput.Poll();

    if( m_GameInput.KeyDown( SDLK_ESCAPE ) )
        m_bContinue = false;

}

void CGameState::Think()
{

    m_PhysicsWorld.Update();

}

void CGameState::Draw()
{

    m_pGameContext->GraphicsContext()->ClearBuffer();
    m_pGameContext->GraphicsContext()->SwapBuffers();

}
