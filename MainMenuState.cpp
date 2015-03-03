#include "MainMenuState.h"


CMainMenuState::CMainMenuState() : CState()
{
    
    
    
}

void CMainMenuState::Init()
{

    m_pFont = GetFont( DEFAULT_FONT, 32 );
    
    m_PixelMat = GetTexture( "pixel.png" );
    m_KeyMat = GetTexture( "key.png" );
    m_LongKeyMat = GetTexture( "longkey.png" );
    m_Title1 = GetTexture( "2149_t1.png" );
    m_Title2 = GetTexture( "2149_t2.png" );
    m_MenuBack = GetTexture( "menuback.png" );
    
    m_bInit = true;
    
}

void CMainMenuState::PostInit()
{
    
    m_bPostInit = true;
    
}

void CMainMenuState::OnStateSwitch() {

    
}

void CMainMenuState::Input()
{
    
    m_GameInput.Poll();
    
    
    if( m_GameInput.EventType() == SDL_KEYDOWN ) {
        
        if( m_GameInput.KeyDownOnce( SDLK_1 ) ) {

            
        }
        
        if( m_GameInput.KeyDownOnce( SDLK_2 ) ) {
            

            
        }
        
    }
    
    
    if( m_GameInput.KeyDown( SDL_SCANCODE_ESCAPE ) )
    {
        
        QuitState();
        
    }
    
}

void CMainMenuState::Think() {
    
}

void CMainMenuState::Draw() {
    
    m_pGameContext->GraphicsContext()->ClearBuffer();
    
    
        static long int nextFlash = SDL_GetTicks() + 400;
        static bool drawT2 = true;
        
        if( SDL_GetTicks() > nextFlash ) {
        
            drawT2 = !drawT2;
            nextFlash = SDL_GetTicks() + 600;
            
        }
        
    m_pGameContext->DrawContext()->DrawMaterial( *m_MenuBack, 0.0f, -200.0f, SCREEN_WIDTH, 1000, 1.0f, 1.0f, 1.0f, 1.0f );
    
    
        m_pGameContext->DrawContext()->DrawMaterial( *m_Title1, SCREEN_WIDTH * .1f + 20.0f, 50.0f, 400, 256, 1.0f, 1.0f, 1.0f, 1.0f );
    
        if( drawT2)
            m_pGameContext->DrawContext()->DrawMaterial( *m_Title2, SCREEN_WIDTH * .1f + 430.0f, 50.0f, 400, 256, 1.0f, 1.0f, 1.0f, 1.0f );
    
    
    m_pGameContext->GraphicsContext()->SwapBuffers();
    
}
