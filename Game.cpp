#include "Game.h"

CGame::CGame()
{

    m_GraphicsContext.SetDrawContext( &m_DrawContext );

    m_GraphicsContext.CreateHandle( "2149" );
    m_GraphicsContext.SetDrawMode( DrawMode::DRAW_MODE_2D );

    int shaderID = m_GraphicsContext.LoadShaderProgram( "vertex.v", "fragment.f" );
    int width, height;

    m_GraphicsContext.GetWindowSize( &width, &height );

    m_DrawContext.UseShaderProgram( shaderID );
    m_DrawContext.Calculate2DProjectionMatrix( width, height );
    m_DrawContext.Bind2DVertexBuffer();

    m_GameContext.SetGraphicsContext( &m_GraphicsContext );
    m_GameContext.SetDrawContext( &m_DrawContext );
    m_GameContext.SetEntityManager( &m_EntityManager );
    m_GameContext.SetTextureFactory( &m_TextureFactory );

    CGameState pGameState = new CGameState;
    pGameState->SetGameContext( &m_GameContext );

    m_StateMachine.AddState( "GAME", pGameState, true );

    while( true )
    {

        m_GameContext.BeginFrame();

            if( !m_StateMachine.RunState() )
                break;

        m_GameContext.EndFrame();

    }



}

CGame::~CGame()
{

    delete m_pStateAccessor;

}
