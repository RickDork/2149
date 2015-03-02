#include "GameState.h"
#include <sstream>

CGameState::CGameState() : CState()
{



}

void CGameState::Init()
{

    Log::Debug( "Doing game state init" );

    m_fboBullets.Init( SCREEN_WIDTH, SCREEN_HEIGHT );
    m_fboBullets2.Init( SCREEN_WIDTH, SCREEN_HEIGHT );
    
    m_GameTimer = 0;

    m_bInit = true;

}

void CGameState::PostInit()
{

    Log::Debug( "Doing game state post-init" );

    m_pHUDFont = m_pGameContext->FontFactory()->GetFont( DEFAULT_FONT, 32 );
    
    m_PixelMat = m_pGameContext->TextureFactory()->GetObjectContent( "pixel.png" );
    
    m_NextSecondsFlash = SDL_GetTicks() + SECONDS_FLASH_TIME;
    m_DrawSeconds = true;
    
    m_bPostInit = true;
    
    OnStateSwitch();

    Log::Debug( "Ready to do game loop" );

}

void CGameState::OnStateSwitch() {
    
    m_pGameContext->CreateStarBackground();
    
    m_pGameContext->GameplayStart();
    
    m_pPlayerEntity = m_pGameContext->GetPlayerEntity();

    
}

void CGameState::Input()
{

    m_GameInput.Poll();

    static const float plyMoveSpeedX = 500.0f;
    static const float plyMoveSpeedY = 500.0f;

    if( m_pPlayerEntity && m_pGameContext->PlayerInputEnabled() ) {
    
        if( m_GameInput.KeyDown( SDL_SCANCODE_A ) )
            m_pPlayerEntity->Displace( -1.0f * plyMoveSpeedX * m_pGameContext->GetFrameDelta(), 0.0f );

        if( m_GameInput.KeyDown( SDL_SCANCODE_D ) )
            m_pPlayerEntity->Displace( plyMoveSpeedX * m_pGameContext->GetFrameDelta(), 0.0f );

        if( m_GameInput.KeyDown( SDL_SCANCODE_W ) )
            m_pPlayerEntity->Displace( 0.0f, -1.0f * plyMoveSpeedY * m_pGameContext->GetFrameDelta() );

        if( m_GameInput.KeyDown( SDL_SCANCODE_S ) )
            m_pPlayerEntity->Displace( 0.0f, plyMoveSpeedY * m_pGameContext->GetFrameDelta() );
        
        m_pPlayerEntity->FitIn( 0.0f, 0.0f, SCREEN_WIDTH - 50.0f, SCREEN_HEIGHT - 100.0f );
        
    } else {
     
        if( m_GameInput.KeyDown( SDL_SCANCODE_RETURN ) ) {
         
            m_pGameContext->IncrementRetryCount();
            m_pGameContext->GameplayStart();
            
        }
        
    }
    
/*
    if( m_GameInput.KeyDown( SDL_SCANCODE_SPACE ) )
    {

		if( m_pPlayerEntity->CanShoot() )
		{

			m_pGameContext->FireBulletFrom( ENTTYPE_PLYBULLET, m_pPlayerEntity, 500.0f );
			m_pPlayerEntity->SetNextShotTime( SDL_GetTicks() + 200 );

		}

    }
*/
    if( m_GameInput.KeyDown( SDL_SCANCODE_ESCAPE ) )
    {

		   m_bContinue = false;
	}

}

void CGameState::Think()
{
    
    if( !m_pGameContext->IsMissionOver() ) {

        m_pGameContext->GameLogic();

        m_pPlayerEntity = m_pGameContext->GetPlayerEntity();
        
        if( m_pPlayerEntity ) {
            
            if( m_pPlayerEntity->CanShoot() && m_pGameContext->GetCurrentEnemyCount() > 0 )
            {
                
                if( m_pGameContext->HasUpgrade( 8 ) ) {
                
                    m_pGameContext->FireBulletFromGunAtAngle( 0, ENTTYPE_PLYBULLET, m_pPlayerEntity, 500.0f,  135.0f );
                    m_pGameContext->FireBulletFromGunAtAngle( 1, ENTTYPE_PLYBULLET, m_pPlayerEntity, 500.0f,  45.0f );
                    
                    m_pGameContext->FireBulletFromGunAtAngle( 0, ENTTYPE_PLYBULLET, m_pPlayerEntity, 500.0f,  180.0f );
                    m_pGameContext->FireBulletFromGunAtAngle( 1, ENTTYPE_PLYBULLET, m_pPlayerEntity, 500.0f,  0.0f );
                    
                    m_pGameContext->FireBulletFromGunAtAngle( 0, ENTTYPE_PLYBULLET, m_pPlayerEntity, 500.0f,  -135.0f );
                    m_pGameContext->FireBulletFromGunAtAngle( 1, ENTTYPE_PLYBULLET, m_pPlayerEntity, 500.0f,  -45.0f );
                    
                    m_pGameContext->FireBulletFromGunAtAngle( 0, ENTTYPE_PLYBULLET, m_pPlayerEntity, 500.0f,  270.0f );
                    m_pGameContext->FireBulletFromGunAtAngle( 1, ENTTYPE_PLYBULLET, m_pPlayerEntity, 500.0f,  270.0f );
                    
                } else if( m_pGameContext->HasUpgrade( 3 ) ) {
                    
                    m_pGameContext->FireBulletFromGunAtAngle( 0, ENTTYPE_PLYBULLET, m_pPlayerEntity, 500.0f,  135.0f );
                    m_pGameContext->FireBulletFromGunAtAngle( 1, ENTTYPE_PLYBULLET, m_pPlayerEntity, 500.0f,  45.0f );
                    
                }
                
                m_pGameContext->FireBulletFrom( ENTTYPE_PLYBULLET, m_pPlayerEntity, 500.0f );
                
                float speedmul = 1.0f;
                
                if( m_pGameContext->HasUpgrade( 9 ) )
                    speedmul = .5f;
                
                m_pPlayerEntity->SetNextShotTime( SDL_GetTicks() + 200.0f * speedmul );
                
            }

        }
        
    } else {
     
        SwitchToAnotherState( "UPGRADESELECT" );
        
    }
    
}

void CGameState::Draw()
{

    m_pGameContext->GraphicsContext()->ClearBuffer();

        m_pGameContext->DrawStarBackground();

        m_pGameContext->EntityManager()->DrawAllEntitiesAtDepth( 1 );
        m_pGameContext->EntityManager()->DrawAllEntitiesAtDepth( 2 );
    
        m_fboBullets.BeginDrawingToFBO();
            m_pGameContext->DrawContext()->SetDrawColor( 1.0f, 1.0f, 1.0f, 1.0f );
            m_pGameContext->EntityManager()->DrawAllEntitiesAtDepth( 0 );
        m_fboBullets.EndDrawingToFBO();

        CMatrix< float > mat;
        mat.Identity();
        mat.Translate( 0.0f, SCREEN_HEIGHT, 0.0f);

        m_pGameContext->DrawContext()->SetDrawColor( 1.0f, 1.0f, 1.0f, 1.0f );
        m_fboBullets2.DrawTexture( m_pGameContext->DrawContext(), &mat );
        
        m_fboBullets2.BeginDrawingToFBO();
            glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
            glClear( GL_COLOR_BUFFER_BIT );
            m_pGameContext->DrawContext()->SetDrawColor( 1.0f, 1.0f, 1.0f, .95f );
            m_fboBullets.DrawTexture( m_pGameContext->DrawContext(), &mat );
        m_fboBullets2.EndDrawingToFBO();


        m_fboBullets.BeginDrawingToFBO();
            glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
            glClear( GL_COLOR_BUFFER_BIT );
            m_pGameContext->DrawContext()->SetDrawColor( 1.0f, 1.0f, 1.0f, 1.0f );
            m_fboBullets2.DrawTexture( m_pGameContext->DrawContext(), &mat );
        m_fboBullets.EndDrawingToFBO();

  //  m_pGameContext->FontFactory()->GetFont( "font.ttf", 32 )->DrawString( m_pGameContext->DrawContext(), "Test\nTest", 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f );
    
   // m_pGameContext->DrawContext()->DrawGLTexture( texture, 0.0f, 10.0f, 20000.0f, 128.0f, 1.0f, 1.0f, 1.0f, 1.0f );
    
        m_pGameContext->DrawExplosions();
    
        if( m_pGameContext->ShouldDrawHUD() ) {
        
            float heightmul = .5f;
        
            Vector2< float > hudSize;
            hudSize.Set( SCREEN_WIDTH, 55.0f );
        
            Vector2< float > hudStart;
            hudStart.Set( 0.0f, SCREEN_HEIGHT - hudSize.GetY() );
        
            m_pGameContext->DrawContext()->DrawMaterial( *m_PixelMat, hudStart.GetX(), hudStart.GetY(), hudSize.GetX(), hudSize.GetY(), 0.0f, 0.0f, 0.0f, 1.0f );
        
            if( m_pPlayerEntity ) {
            
                int nhealthbars = 0;
                int maxhealthbars = 0;
                
                if( m_pPlayerEntity->GetMaxHealth() > 150.0f ) {

                    maxhealthbars = m_pPlayerEntity->GetMaxHealth() / 20 + 1;
                    nhealthbars = m_pPlayerEntity->GetHealth() / 20 + 1;
                    
                } else {
                
                    maxhealthbars = m_pPlayerEntity->GetMaxHealth() / 15 + 1;
                    nhealthbars = m_pPlayerEntity->GetHealth() / 15 + 1;

                
                }
                    
                if( m_pPlayerEntity->GetHealth() <= 0 )
                    nhealthbars = 0;
            
                for( int i = 0; i < maxhealthbars; i++ ) {
                 
                    float r = 0.0f, g = .6f, b = 0.0f, a = 1.0f;
                    
                    if( i >= nhealthbars ) {
                     
                        r = .2f;
                        g = .2f;
                        b = .2f;
                        
                    }
                    
                    m_pGameContext->DrawContext()->DrawMaterial( *m_PixelMat, hudStart.GetX() + 10 + i * 20, hudStart.GetY() + ( float )hudSize.GetY() * ( 1.0f - heightmul ) * .5f, 10.0f, hudSize.GetY() * heightmul, r, g, b, a );
                    
                }
            }
        
            char expstr[255];
            sprintf( expstr, "EXP: %d", m_pGameContext->GetPlayerEXP() );
        
            int textwidth = m_pHUDFont->GetStringWidth( expstr );
            m_pHUDFont->DrawString( m_pGameContext->DrawContext(), expstr, SCREEN_WIDTH - 10 - textwidth, SCREEN_HEIGHT - 50.0f, 1.0f, 1.0f, 1.0f, 1.0f );
        
            if( m_pPlayerEntity )
                m_GameTimer = m_pGameContext->TicksElapsed();
        
            Vector2< int > time = Util::ConvertTicksIntoMS( m_GameTimer );
        
            char timestr[255];
        
            if( m_DrawSeconds )
                sprintf( timestr, "%02d:%02d", time.GetX(), time.GetY() );
            else
                sprintf( timestr, "%02d:", time.GetX() );
        
            if( !m_pPlayerEntity ) {

                m_pGameContext->FontFactory()->GetFont( DEFAULT_FONT, 64 )->DrawString( m_pGameContext->DrawContext(), GAMEOVER_TEXT, SCREEN_WIDTH * .5f, SCREEN_HEIGHT * .5f, 0.8f, 0.0f, 0.0f, 1.0f, DRAW_TEXT_VERT_CENTER | DRAW_TEXT_HORIZ_CENTER );
                
                m_pHUDFont->DrawString( m_pGameContext->DrawContext(), "Press [ENTER] to restart", SCREEN_WIDTH * .5f, SCREEN_HEIGHT * .5f + 50, 1.0f, 1.0f, 1.0f, 1.0f, DRAW_TEXT_HORIZ_CENTER );
                
            }
        
            /*
            if( SDL_GetTicks() > m_NextSecondsFlash ) {
                m_NextSecondsFlash = SECONDS_FLASH_TIME + SDL_GetTicks();
                m_DrawSeconds = !m_DrawSeconds;
            }*/
        
            m_pHUDFont->DrawString( m_pGameContext->DrawContext(), timestr, SCREEN_WIDTH * .5 - 50, SCREEN_HEIGHT - 50.0f, 1.0f, 1.0f, 1.0f, 1.0f );
            
        }
    
        m_pGameContext->Lua().CallEngineFunction( "Draw" );
    
    
/*
        m_pGameContext->DrawContext()->DrawMaterial( *m_PixelMat, 10.0f, 545.0f, 150.0f, 10.0f, 1.0f, 1.0f, 1.0f, 0.5f );
        m_pGameContext->DrawContext()->DrawMaterial( *m_PixelMat, 11.0f, 546.0f, 148.0f * armor_mul, 8.0f, 0.3f, 0.3f, 1.0f, 1.0f );
        
        m_pGameContext->DrawContext()->DrawMaterial( *m_PixelMat, 10.0f, 560.0f, 150.0f, 10.0f, 1.0f, 1.0f, 1.0f, 0.5f );
        m_pGameContext->DrawContext()->DrawMaterial( *m_PixelMat, 11.0f, 561.0f, 148.0f * health_mul, 8.0f, 0.2f, 0.8f, 0.2f, 0.5f );

*/
   m_pGameContext->GraphicsContext()->SwapBuffers();


}
