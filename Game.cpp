#include "Game.h"

CGame::CGame()
{

    Log::ClearLog();
    Log::SetDebugLoggingEnabled( true );

	Log::Debug( "=======================APPLICATION INITIALIZING=======================" );

    m_GraphicsContext.SetDrawContext( &m_DrawContext );

    m_GraphicsContext.CreateHandle( "2149", 0, 0, 800, 600, false );
    m_GraphicsContext.SetDrawMode( DrawMode::DRAW_MODE_2D );

    int shaderID = m_GraphicsContext.LoadShaderProgram( "vertex.v", "fragment.f" );
    int width, height;

    m_GraphicsContext.GetWindowSize( &width, &height );

    m_DrawContext.UseShaderProgram( shaderID );
    m_DrawContext.Calculate2DProjectionMatrix( width, height );
    m_DrawContext.Bind2DVertexBuffer();

    Log::Debug( "Draw Context initialized" );

    m_GameContext.SetGraphicsContext( &m_GraphicsContext );
    m_GameContext.SetDrawContext( &m_DrawContext );
    m_GameContext.SetEntityManager( &m_EntityManager );
    m_GameContext.SetTextureFactory( &m_TextureFactory );

    Log::Debug( "Game Context initialized" );

    CGameState * pGameState = new CGameState;
    pGameState->SetGameContext( &m_GameContext );

    m_StateMachine.AddState( "GAME", ( CStateBase * )pGameState, true );

    Log::Debug( "State Machine initialized\nStarting main loop" );

    while( true )
    {

        m_GameContext.BeginFrame();

            if( !m_StateMachine.RunState() )
                break;

        m_GameContext.EndFrame();

    }

     Log::Debug( "Exiting main loop" );

}

CGame::~CGame()
{

    //delete m_pStateAccessor;

}
