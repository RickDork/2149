#include "Game.h"
#include <GLUT/GLUT.h>

CGame::CGame()
{
    
    Util::FixAppleRelativeDirectory();

    Log::ClearLog();
    Log::SetDebugLoggingEnabled( true );

	Log::Debug( "=======================APPLICATION INITIALIZING=======================" );

    m_GraphicsContext.SetDrawContext( &m_DrawContext );
    
    m_GraphicsContext.CreateHandle( "2149", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, false );
    m_GraphicsContext.SetDrawMode( DrawMode::DRAW_MODE_2D );
    
    int shaderID = m_GraphicsContext.LoadShaderProgram( "vertex.v", "fragment.f" );
    int width, height;

    m_GraphicsContext.GetWindowSize( &width, &height );
    
    m_GraphicsContext.SetClearColor( .1f, .1f, .1f, 1.0f );
    
    SDL_ShowCursor( 0 );

    m_DrawContext.UseShaderProgram( shaderID );
    m_DrawContext.Calculate2DProjectionMatrix( width, height );
    m_DrawContext.Bind2DVertexBuffer();

    Log::Debug( "Draw Context initialized" );
    
    m_FTContext.Init();
    m_FontFactory.SetFontLibrary( m_FTContext.GetLibrary() );
    
    Log::Debug( "Font Context initialized" );
    
    m_GameContext.SetGraphicsContext( &m_GraphicsContext );
    m_GameContext.SetDrawContext( &m_DrawContext );
    m_GameContext.SetEntityManager( &m_EntityManager );
    m_GameContext.SetTextureFactory( &m_TextureFactory );
    m_GameContext.SetFTContext( &m_FTContext );
    m_GameContext.SetFontFactory( &m_FontFactory );
    m_GameContext.Initialize();
    
    Log::Debug( "Game Context initialized" );

    CGameState * pGameState = new CGameState;
    pGameState->SetGameContext( &m_GameContext );
    
    CSelectUpgradeMenuState * pSelectUpgradeMenuState = new CSelectUpgradeMenuState;
    pSelectUpgradeMenuState->SetGameContext( &m_GameContext );
    
    CMainMenuState * pMainMenuState = new CMainMenuState;
    pMainMenuState->SetGameContext( &m_GameContext );

    m_StateMachine.AddState( "GAME", ( CStateBase * )pGameState );
    m_StateMachine.AddState( "UPGRADESELECT", ( CSelectUpgradeMenuState * )pSelectUpgradeMenuState );
    m_StateMachine.AddState( "MAINMENU", ( CMainMenuState * )pMainMenuState);
    
    m_StateMachine.SetActiveState( "GAME" );
    
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
