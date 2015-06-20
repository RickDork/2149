#include "MainMenuState.h"


CMainMenuState::CMainMenuState() : CState()
{
    
    
    
}

void CMainMenuState::Init()
{

    m_pFont = GetFont( DEFAULT_FONT, 24 );
    
    m_PixelMat = GetTexture( "pixel.png" );
    m_KeyMat = GetTexture( "key.png" );
    m_LongKeyMat = GetTexture( "longkey_bright.png" );
    m_Title1 = GetTexture( "2149_t1.png" );
    m_Title2 = GetTexture( "2149_t2.png" );
    m_MenuBack = GetTexture( "2149menuback1.png" );
    m_MenuBack2 = GetTexture( "2149menuback2.png" );
    
    m_bIntroOn = false;
    m_bInit = true;
    
}

void CMainMenuState::PostInit()
{
    
    m_bPostInit = true;
    
}

void CMainMenuState::OnStateSwitch() {
    
    m_pGameContext->PlayMusic( "menu.mp3", .1f );
    m_pGameContext->FadeMusic( 2000, .1f, 1.0f );

    
}

void CMainMenuState::Input()
{
    
    m_GameInput.Poll();
    
    if( StateTicksElapsed() > 4000 && !m_bIntroOn )
    {
        
		if(  m_GameInput.KeyDown( SDL_SCANCODE_RETURN ) ) { 

			m_bIntroOn = true;
			m_pGameContext->Lua().CallEngineFunction( "StartIntro" );

		}

		if( m_GameInput.EventType() == SDL_KEYDOWN ) {

			if( m_GameInput.KeyDownOnce( SDLK_f ) ) {

				m_pGameContext->GraphicsContext()->SetFullScreen( !m_pGameContext->GraphicsContext()->IsFullScreen() );

			}

		}
        
    }

    if( m_GameInput.EventType() == SDL_QUIT || m_GameInput.KeyDown( SDL_SCANCODE_ESCAPE ) )
    {
        
        QuitState();
        
    }
    
}

void CMainMenuState::Think() {
    
    std::string stateChange( m_pGameContext->Lua().GetEngineString( "StateChange" ) );
    
    if( stateChange.length() > 0 ) {
     
        if( stateChange == "UPGRADESELECT" ) {
         
            m_pGameContext->SetUpgradeSelectMenuDelay( 7000 );
            m_pGameContext->ToggleStarFieldSlowFill( true );
            
        }
        
        SwitchToAnotherState( stateChange );
        
    }
    
}

void CMainMenuState::Draw() {
    
    m_pGameContext->GraphicsContext()->ClearBuffer();
    
    
        static long int fadeinTime = SDL_GetTicks() + 2000;
        static long int nextFlash = SDL_GetTicks() + 400;
        static bool drawT2 = true;
        static float alpha = 0.0f;
    
        if( SDL_GetTicks() > nextFlash ) {
        
            drawT2 = !drawT2;
            nextFlash = SDL_GetTicks() + 600;
            
        }
    
    if( SDL_GetTicks() > fadeinTime ) {
     
        alpha += .1 * m_pGameContext->GetFrameDelta();
        
        if( alpha > 1.0f )
            alpha = 1.0f;
        
    }
    
    
        if( drawT2 )
            m_pGameContext->DrawContext()->DrawMaterial( *m_MenuBack2, 0.0f, -300.0f, SCREEN_WIDTH, 1050, 1.0f, 1.0f, 1.0f, alpha );
        else
            m_pGameContext->DrawContext()->DrawMaterial( *m_MenuBack, 0.0f, -300.0f, SCREEN_WIDTH, 1050, 1.0f, 1.0f, 1.0f, alpha );
        
            m_pGameContext->DrawContext()->DrawMaterial( *m_Title1, SCREEN_WIDTH * .1f + 20.0f, 50.0f, 400, 256, 1.0f, 1.0f, 1.0f, 1.0f );
        
        m_pGameContext->DrawContext()->DrawMaterial( *m_LongKeyMat, SCREEN_WIDTH * .5 - 90.0f, SCREEN_HEIGHT * .5f + 30.0f, 120.0f, 50.0f, 1.0f, 1.0f, 1.0f, alpha );
    
        m_pFont->DrawString( m_pGameContext->DrawContext(), "ENTER       Start Game", SCREEN_WIDTH * .5f - 74.0f, SCREEN_HEIGHT * .5f + 39.0f, 1.0f, 1.0f, 1.0f, alpha );
 
         m_pGameContext->DrawContext()->DrawMaterial( *m_LongKeyMat, SCREEN_WIDTH * .5 - 95.0f, SCREEN_HEIGHT * .5f + 90.0f, 135.0f, 50.0f, 1.0f, 1.0f, 1.0f, alpha );
        
        m_pFont->DrawString( m_pGameContext->DrawContext(), "SPACE       Load Last Save", SCREEN_WIDTH * .5f - 74.0f, SCREEN_HEIGHT * .5f + 100.0f, 1.0f, 1.0f, 1.0f, alpha );
       
         m_pGameContext->DrawContext()->DrawMaterial( *m_LongKeyMat, SCREEN_WIDTH * .5 - 55.0f, SCREEN_HEIGHT * .5f + 150.0f, 40.0f, 50.0f, 1.0f, 1.0f, 1.0f, alpha );
        
        m_pFont->DrawString( m_pGameContext->DrawContext(), "F             Toggle fullscreen", SCREEN_WIDTH * .5f - 40.0f, SCREEN_HEIGHT * .5f + 160.0f, 1.0f, 1.0f, 1.0f, alpha );
       
        m_pGameContext->DrawContext()->DrawMaterial( *m_LongKeyMat, SCREEN_WIDTH * .5 - 100.0f, SCREEN_HEIGHT * .5f + 210.0f, 140.0f, 50.0f, 1.0f, 1.0f, 1.0f, alpha );
        
        m_pFont->DrawString( m_pGameContext->DrawContext(), "ESCAPE      Quit", SCREEN_WIDTH * .5f - 82.0f, SCREEN_HEIGHT * .5f + 220.0f, 1.0f, 1.0f, 1.0f, alpha );
        
    
        if( drawT2)
            m_pGameContext->DrawContext()->DrawMaterial( *m_Title2, SCREEN_WIDTH * .1f + 440.0f, 50.0f, 400, 256, 1.0f, 1.0f, 1.0f, 1.0f );
    
        m_pGameContext->Lua().CallEngineFunction( "MainMenuDraw" );
    
    
    m_pGameContext->GraphicsContext()->SwapBuffers();
    
}
