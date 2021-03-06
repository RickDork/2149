#include "GameState.h"
#include <sstream>

CGameState::CGameState() : CState()
{



}

void CGameState::Init()
{

    Log::Debug( "Doing game state init" );

    //m_fboCurBullets.Init( SCREEN_WIDTH, SCREEN_HEIGHT );
    //m_fboTrailBullets.Init( SCREEN_WIDTH, SCREEN_HEIGHT );
    //m_fboTrailBulletsTemp.Init( SCREEN_WIDTH, SCREEN_HEIGHT );
    
    m_BulletTrails.Init( m_pGameContext->DrawContext(), SCREEN_WIDTH, SCREEN_HEIGHT, .95f );
    m_fboNoiseImage.Init( SCREEN_WIDTH, SCREEN_HEIGHT, .05f, .05f );
    m_fboNoiseBG.Init( SCREEN_WIDTH, SCREEN_HEIGHT, .05f, .05f );
    m_fbo3DSpace.Init( SCREEN_WIDTH, SCREEN_HEIGHT );
    m_fboGameBuffer.Init( SCREEN_WIDTH, SCREEN_HEIGHT );
    m_fboShips.Init( SCREEN_WIDTH, SCREEN_HEIGHT );
    m_fboShipsHorizPassThru.Init( SCREEN_WIDTH, SCREEN_HEIGHT );
    m_fboExplosions.Init( SCREEN_WIDTH, SCREEN_HEIGHT );
    m_fboSpaceFog.Init( SCREEN_WIDTH, SCREEN_HEIGHT );
    
    for( int j = 0; j < 5; j++ ) {
     
        m_TrippyTrails[j].Init();
        
    }
  
    m_CurTrippyTrailFrame = 0;
    m_NextTrippyTrailFrameTime = 0;
    m_EndCutSceneTriggerTime = 0;
    m_GameTimer = 0;
    m_BLTCount = 0;

    m_bInit = true;

}

void CGameState::PostInit()
{

    Log::Debug( "Doing game state post-init" );

    m_pHUDFont = m_pGameContext->FontFactory()->GetFont( DEFAULT_FONT, 32 );
    
    m_PixelMat = GetTexture( "pixel.png" );
	m_LongKeyMat = GetTexture( "longkey_bright.png" );
    
    m_NextSecondsFlash = SDL_GetTicks() + SECONDS_FLASH_TIME;
    m_DrawSeconds = true;
    
    m_bPostInit = true;

    Log::Debug( "Ready to do game loop" );

}

void CGameState::OnStateSwitch() {

    m_pGameContext->SetStarBackgroundSpeedMul( 1.0f );
    m_pGameContext->ToggleStarFieldSlowFill( false );
    m_pGameContext->SetUpgradeSelectMenuDelay( 0 );
    m_pGameContext->CreateStarBackground();
    
    m_pGameContext->GameplayStart();
    
    m_pPlayerEntity = m_pGameContext->GetPlayerEntity();
    
    m_BulletTrails.Clear();

	m_bGamePaused = false;

    
    
}

void CGameState::Input()
{

    m_GameInput.Poll();

    static const float plyMoveSpeedX = 500.0f;
    static const float plyMoveSpeedY = 500.0f;
    /*
    if( m_pGameContext->IsCutsceneOn() ) {
    
        if( m_GameInput.KeyDown( SDL_SCANCODE_RETURN ) ) {
         
            if( m_EndCutSceneTriggerTime < 1 ) {
             
                m_EndCutSceneTriggerTime = SDL_GetTicks() + 2000;
                
            } else if( SDL_GetTicks() > m_EndCutSceneTriggerTime ) {
             
                m_pGameContext->Lua().CallEngineFunction( "SkipCutscene" );
                
            }
            
        } else if( m_EndCutSceneTriggerTime > 0 )
            m_EndCutSceneTriggerTime = 0;

    } else if( m_EndCutSceneTriggerTime > 0 )
        m_EndCutSceneTriggerTime = 0;
    */

    if( !m_pGameContext->IsGameFrozen() ) {
    
    if( m_pPlayerEntity && m_pGameContext->PlayerInputEnabled() ) {
        
            if( m_GameInput.KeyDown( SDL_SCANCODE_A ) )
                m_pPlayerEntity->Displace( -1.0f * plyMoveSpeedX * m_pGameContext->GetFrameDelta(), 0.0f );

            if( m_GameInput.KeyDown( SDL_SCANCODE_D ) )
                m_pPlayerEntity->Displace( plyMoveSpeedX * m_pGameContext->GetFrameDelta(), 0.0f );

            if( m_pGameContext->GetCurrentMission() != 4 ) {
        
                if( m_GameInput.KeyDown( SDL_SCANCODE_W ) )
                    m_pPlayerEntity->Displace( 0.0f, -1.0f * plyMoveSpeedY * m_pGameContext->GetFrameDelta() );

                if( m_GameInput.KeyDown( SDL_SCANCODE_S ) )
                    m_pPlayerEntity->Displace( 0.0f, plyMoveSpeedY * m_pGameContext->GetFrameDelta() );
                
            }
                
        if( m_pPlayerEntity->GetWrapEdges() ) {
            
            m_pPlayerEntity->FitIn( -500.0f, 0.0f, SCREEN_WIDTH + 500.0f, SCREEN_HEIGHT - 100.0f );
            
            float xpos = m_pPlayerEntity->GetX();
            float w = m_pPlayerEntity->GetSprite().GetRealSize( m_pGameContext->DrawContext() ).GetX();
            
            if( xpos + w <= 0.0f ) {
             
                xpos += SCREEN_WIDTH;
                m_pPlayerEntity->SetPos( xpos, m_pPlayerEntity->GetY() );
                
            } else if( xpos > SCREEN_WIDTH ) {
                
                xpos -= SCREEN_WIDTH;
                m_pPlayerEntity->SetPos( xpos, m_pPlayerEntity->GetY() );
                
            }
            
        } else
            m_pPlayerEntity->FitIn( 0.0f, 0.0f, SCREEN_WIDTH - 50.0f, SCREEN_HEIGHT - 100.0f );
        
        } else if( !m_pPlayerEntity ) {
         
            if( m_GameInput.KeyDown( SDL_SCANCODE_RETURN ) ) {
                
                m_pGameContext->IncrementRetryCount();
                m_pGameContext->GameplayStart();
                
            }
            
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

	if( m_GameInput.EventType() == SDL_KEYDOWN ) {

		if( m_GameInput.KeyDownOnce( SDLK_ESCAPE ) )
		{

			//If cutscene on, always allow player to press escape to skip, no exceptions.	
			if( m_pGameContext->IsCutsceneOn() ) {

				m_pGameContext->Lua().CallEngineFunction( "SkipCutscene" );

			//If game is already paused, always allow unpause.
			} else if( m_bGamePaused ) {
			
				
				m_bGamePaused = false;
				m_pGameContext->ToggleGameFrozen( false );
				m_pGameContext->PauseMusic( false );

			//If game is unpaused and there is no cutscene, we have to do further validation..
			} else if( !m_pGameContext->IsCutsceneOn() ) {

				//Only allow pause if player can enter input
				if( m_pGameContext->PlayerInputEnabled() ) {

					//Only allow pause if game is not already frozen (if it were, it would be for lua-induced reasons)
					if( !m_pGameContext->IsGameFrozen() ) {
				
						m_bGamePaused = true;
						m_pGameContext->ToggleGameFrozen( true );
						m_pGameContext->PauseMusic( true );

					}

				}

			}

		}

		if( m_bGamePaused ) {
		
			if( m_GameInput.KeyDownOnce( SDLK_f ) ) {

				m_pGameContext->GraphicsContext()->SetFullScreen( !m_pGameContext->GraphicsContext()->IsFullScreen() );

			}

			if( m_GameInput.KeyDownOnce( SDLK_RETURN ) ) {
		
				m_bContinue = false;

			}

		}

	}

	if( m_GameInput.EventType() == SDL_QUIT ) {
	
		m_bContinue = false;

	}


}

void CGameState::Think()
{
    
    if( !m_pGameContext->IsMissionOver() ) {

        m_pGameContext->GameLogic();

        if( !m_pGameContext->IsGameFrozen() ) {
            
            m_pPlayerEntity = m_pGameContext->GetPlayerEntity();
            
            if( m_pPlayerEntity ) {
                
                if( m_pGameContext->PlayerInputEnabled() && m_pPlayerEntity->CanShoot() && m_pGameContext->GetCurrentEnemyCount() > 0 )
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
                    
                    m_pGameContext->FireBulletFrom( ENTTYPE_PLYBULLET, m_pPlayerEntity, 300.0f );
                    
                    float speedmul = 1.0f;
                    
                    if( m_pGameContext->HasUpgrade( 9 ) )
                        speedmul = .5f;
                    
                    m_pPlayerEntity->SetNextShotTime( SDL_GetTicks() + 200.0f * speedmul );
                    
                }

            }
            
        }
        
    } else {
        
        if( m_pGameContext->GetCurrentMission() == 4 ) {
            
            OnStateSwitch();
            
        } else
            SwitchToAnotherState( "UPGRADESELECT" );
        
    }
    
    if( m_pGameContext->IsGameEnded() )
        m_bContinue = false;
    
}

void CGameState::PreDrawBullets() {
    
    if( m_pGameContext->IsGameFrozen() )
        return;

    int b1 = 0, b2 = 0;
    
    m_BulletTrails.BeginDrawToCurrentBuffer();
    m_pGameContext->DrawContext()->SetDrawColor( 1.0f, 1.0f, 1.0f, 1.0f );
    b1 = m_pGameContext->EntityManager()->DrawAllEntitiesAtDepth( 0 );
    m_pGameContext->GraphicsContext()->UseShader( 3 );
    b2 = m_pGameContext->EntityManager()->DrawAllEntitiesAtDepth( 3 );
    m_pGameContext->GraphicsContext()->UseShader( 0 );
    m_BulletTrails.EndDrawToCurrentBuffer();

    
    m_BLTCount = b1 + b2;
    
}

void CGameState::PostDrawBullets() {
    
    if( m_pGameContext->IsGameFrozen() )
        return;
    
    m_BulletTrails.BeginDrawToAccumBuffer();
    if( m_BLTCount > 0 ) {
        
        m_pGameContext->DrawContext()->SetDrawColor( 1.0f, 1.0f, 1.0f, 1.0f );
        m_BulletTrails.DrawCurrentBuffer();
        
    } else {
        
        glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        
    }
    m_BulletTrails.EndDrawToAccumBuffer();
    
}

void CGameState::DrawBullets() {
    

    m_pGameContext->DrawContext()->SetDrawColor( 1.0f, 1.0f, 1.0f, 1.0f );
   
    m_pGameContext->GraphicsContext()->UseShader( 2 );
    m_BulletTrails.DrawAccumBuffer();
    
    m_pGameContext->GraphicsContext()->UseShader( 0 );
    m_BulletTrails.DrawCurrentBuffer();

    
    
}

void CGameState::GenerateBackgroundNoiseImage() {
    
    m_pGameContext->DrawContext()->SetDrawColor( 1.0f, 1.0f, 1.0f, 1.0f );
    
    m_fboNoiseBG.BeginDrawingToFBO();
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );
    m_pGameContext->DrawContext()->UseShaderProgram( m_pGameContext->GraphicsContext()->GetShaderIDFromIndex( 8 ) );
    m_pGameContext->DrawContext()->Bind2DVertexBuffer();
    m_pGameContext->DrawContext()->DrawMaterial( *m_PixelMat, m_fboNoiseBG.GetOffsetX(), m_fboNoiseBG.GetOffsetY(), m_fboNoiseBG.GetWidth(), m_fboNoiseBG.GetHeight(), 1.0f, 0.0f, 0.0f, 1.0f );
    m_pGameContext->DrawContext()->UseShaderProgram( m_pGameContext->GraphicsContext()->GetShaderIDFromIndex( 0 ) );
    m_fboNoiseBG.EndDrawingToFBO();
    
    m_pGameContext->SetSpaceFogFBO( &m_fboNoiseBG );
    
}


void CGameState::GenerateNoiseImage() {
    
    m_pGameContext->DrawContext()->SetDrawColor( 1.0f, 1.0f, 1.0f, 1.0f );
 
    m_fboNoiseImage.BeginDrawingToFBO();
        glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        m_pGameContext->DrawContext()->UseShaderProgram( m_pGameContext->GraphicsContext()->GetShaderIDFromIndex( 5 ) );
        m_pGameContext->DrawContext()->Bind2DVertexBuffer();
            m_pGameContext->DrawContext()->DrawMaterial( *m_PixelMat, m_fboNoiseImage.GetOffsetX(), m_fboNoiseImage.GetOffsetY(), m_fboNoiseImage.GetWidth(), m_fboNoiseImage.GetHeight(), 1.0f, 0.0f, 0.0f, 1.0f );
        m_pGameContext->DrawContext()->UseShaderProgram( m_pGameContext->GraphicsContext()->GetShaderIDFromIndex( 0 ) );
    m_fboNoiseImage.EndDrawingToFBO();
    
    m_pGameContext->SetFunkyBackgroundFBO( &m_fboNoiseImage );
    m_pGameContext->SetPixelMat( m_PixelMat );

}



void CGameState::Draw()
{

        static bool generatedNoiseImage = false;
        
        if( !generatedNoiseImage ) {
            
            GenerateNoiseImage();
            GenerateBackgroundNoiseImage();
            generatedNoiseImage = true;
            
        }
    
        PreDrawBullets();
    
        m_fboExplosions.BeginDrawingToFBO();
            glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
            glClear( GL_COLOR_BUFFER_BIT );     
            m_pGameContext->DrawExplosions();
        m_fboExplosions.EndDrawingToFBO();
    
        //glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
        m_fboShips.BeginDrawingToFBO();
            glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
            glClear( GL_COLOR_BUFFER_BIT ); 
            m_pGameContext->EntityManager()->DrawAllEntitiesAtDepth( 1 );
        m_fboShips.EndDrawingToFBO();
        //glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
        m_fboSpaceFog.BeginDrawingToFBO();
            glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
            glClear( GL_COLOR_BUFFER_BIT ); 
            m_pGameContext->DrawSpaceFog( m_fboExplosions.GetTexture() );
        m_fboSpaceFog.EndDrawingToFBO();

        m_pGameContext->DrawContext()->SetDrawColor( 1.0f, 1.0f, 1.0f, 1.0f );
    
        m_pGameContext->GraphicsContext()->UseShader( 11 );
        glActiveTexture( GL_TEXTURE1 );
        m_fboExplosions.BindTexture();
        int l = glGetUniformLocation( m_pGameContext->GraphicsContext()->GetShaderIDFromIndex( 11 ), "texUnit2" );
        glUniform1i( l, 1 );
        glActiveTexture( GL_TEXTURE0 );
        m_fboShipsHorizPassThru.BeginDrawingToFBO();
            glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
            glClear( GL_COLOR_BUFFER_BIT ); 
            m_fboShips.DrawTexture( m_pGameContext->DrawContext() );
        m_fboShipsHorizPassThru.EndDrawingToFBO();
    
        m_pGameContext->GraphicsContext()->UseShader( 12 );
        l = glGetUniformLocation( m_pGameContext->GraphicsContext()->GetShaderIDFromIndex( 12 ), "texUnit2" );
        glUniform1i( l, 1 );
        
        m_fboShips.BeginDrawingToFBO();
            glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
            glClear( GL_COLOR_BUFFER_BIT ); 
            m_fboShipsHorizPassThru.DrawTexture( m_pGameContext->DrawContext() );
        m_fboShips.EndDrawingToFBO();
    
        if( m_pGameContext->GetCurrentMission() == 4 ) {
        
            if( SDL_GetTicks() > m_NextTrippyTrailFrameTime ) {
                
                m_pGameContext->GraphicsContext()->UseShader( 13 );
                m_TrippyTrails[m_CurTrippyTrailFrame].GetFBO().BeginDrawingToFBO();
                glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
                glClear( GL_COLOR_BUFFER_BIT ); 
                m_pGameContext->DrawStarBackground();
                m_fboShips.DrawTexture( m_pGameContext->DrawContext() );
                m_TrippyTrails[m_CurTrippyTrailFrame].GetFBO().EndDrawingToFBO();
        
                m_TrippyTrails[m_CurTrippyTrailFrame].GenerateRandomColor();
                m_TrippyTrails[m_CurTrippyTrailFrame].GenerateRandomPos();
                
                m_NextTrippyTrailFrameTime = SDL_GetTicks() + Util::RandomNumber( 10, 100 );
                
                if( ++m_CurTrippyTrailFrame >= 5 )
                    m_CurTrippyTrailFrame = 0;
                
            }
            
        }
        
        m_fboGameBuffer.BeginDrawingToFBO();
        
        glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
        glClear( GL_COLOR_BUFFER_BIT );
    
        m_pGameContext->DrawContext()->SetDrawColor( 1.0f, 1.0f, 1.0f, 1.0f );
    
        if( m_pGameContext->GetCurrentMission() != 4 || m_pGameContext->EndingTriggered() )
            m_pGameContext->DrawStarBackground();
    
        m_pGameContext->DrawContext()->SetDrawColor( 1.0f, 1.0f, 1.0f, 1.0f );
    
        m_fboSpaceFog.DrawTexture( m_pGameContext->DrawContext() );
        m_pGameContext->DrawSmoke();
    
        if( m_pGameContext->GetBossMode() )
            DrawBullets();
    
        m_pGameContext->DrawContext()->SetDrawColor( 1.0f, 1.0f, 1.0f, 1.0f );
        
    
        if( m_pGameContext->GetCurrentMission() == 4 && !m_pGameContext->EndingTriggered() ) { 
            
            static float blackHoleHeight = 0.0f;
            float speed = 3.5f;
            
            
            if( blackHoleHeight > 340.0f )
                speed = 6.0f;
            
            blackHoleHeight += speed * m_pGameContext->GetFrameDelta();
            
            m_pGameContext->DrawContext()->DrawMaterial( *m_PixelMat, 0.0f, 0.0f, SCREEN_WIDTH, blackHoleHeight, 0.0f, 0.0f, 0.0f, 1.0f );
         
            m_pGameContext->DrawContext()->SetDrawColor( 1.0f, 1.0f, 1.0f, 1.0f );
            
        }
    
        if( m_pGameContext->GetCurrentMission() == 4 && !m_pGameContext->EndingTriggered() ) {
        
            for( int j = 0; j < 5; j++ ) {
             
                m_TrippyTrails[j].Draw( m_pGameContext->DrawContext() );
                
            }
            
        }
    
        m_pGameContext->DrawContext()->SetDrawColor( 1.0f, 1.0f, 1.0f, 1.0f );
        
        m_fboShips.DrawTexture( m_pGameContext->DrawContext() );
        m_pGameContext->EntityManager()->DrawAllEntitiesAtDepth( 2 );
   
    
        if( !m_pGameContext->GetBossMode() )
            DrawBullets();
        else {
         
            float p = m_pGameContext->GetBossHealthPercent();
            
            if( ( int )m_pGameContext->GetBossHealth() > 0 ) {
            
                int nbars = 20 * p;
                
                if( nbars < 1 && p > 0.0f )
                    nbars = 1;
                
                for( int j = 0; j < 20; j++ ) {
                 
                    float r = 1.0f, g = 0.0f, b = 0.0f;
                    
                    if( j >= nbars )
                        r = .5f;
                        
                   m_pGameContext->DrawContext()->DrawMaterial( *m_PixelMat, 120.0f + j * 40.0f, 10.0f, 20.0f, 50.0f, r, g, b, 1.0f );
                    
                }
                
            } else if( m_pPlayerEntity && m_pPlayerEntity->IsActive() ) {
             
                m_pGameContext->Lua().CallLuaFunction( "BossDeath", "LEVEL" );
                
            }

            
        }
    

  //  m_pGameContext->FontFactory()->GetFont( "font.ttf", 32 )->DrawString( m_pGameContext->DrawContext(), "Test\nTest", 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f );
    
   // m_pGameContext->DrawContext()->DrawGLTexture( texture, 0.0f, 10.0f, 20000.0f, 128.0f, 1.0f, 1.0f, 1.0f, 1.0f );
    
    
        m_pGameContext->DrawContext()->SetDrawColor( 1.0f, 1.0f, 1.0f, 1.0f );
        m_fboExplosions.DrawTexture( m_pGameContext->DrawContext() );    
        
        m_fboGameBuffer.EndDrawingToFBO();
        
        m_pGameContext->GraphicsContext()->ClearBuffer();
    
        static int renderDelay = SDL_GetTicks() + 100;
        
        glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
    
        if( SDL_GetTicks() > renderDelay ) { 
            
            int renderShader = 0;
            
            if( m_pGameContext->GetCurrentMission() == 4 && !m_pGameContext->EndingTriggered() )
                renderShader = 10;
            
            m_pGameContext->GraphicsContext()->UseShader( renderShader );
            m_pGameContext->DrawContext()->Bind2DVertexArray();
        
            
            m_pGameContext->DrawContext()->SetDrawColor( 1.0f, 1.0f, 1.0f, 1.0f );
            m_fboGameBuffer.DrawTexture( m_pGameContext->DrawContext() );
            
            m_pGameContext->GraphicsContext()->UseShader( 0 );
            m_pGameContext->DrawContext()->Bind2DVertexArray();
            
        }  
    
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
     
        PostDrawBullets();
    
        if( m_pGameContext->ShouldDrawHUD() ) {
        
            float heightmul = .5f;
        
            Vector2< float > hudSize;
            hudSize.Set( SCREEN_WIDTH, 55.0f );
        
            Vector2< float > hudStart;
            hudStart.Set( 0.0f, SCREEN_HEIGHT - hudSize.GetY() );
        
            //m_pGameContext->DrawContext()->DrawMaterial( *m_PixelMat, hudStart.GetX(), hudStart.GetY(), hudSize.GetX(), hudSize.GetY(), 0.0f, 0.0f, 1.0f, .2f );
            
          //  m_pGameContext->DebugDrawQuadTree();
        
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
                 
                    float r = 0.1f, g = .8f, b = 0.1f, a = 1.0f;
                    
                    if( i >= nhealthbars ) {
                     
                        r = .2f;
                        g = .2f;
                        b = .2f;
                        
                    }
                    
                    m_pGameContext->DrawContext()->DrawMaterial( *m_PixelMat, hudStart.GetX() + 10 + i * 20, hudStart.GetY() + ( float )hudSize.GetY() * ( 1.0f - heightmul ) * .5f, 10.0f, hudSize.GetY() * heightmul, r, g, b, a );

                    if( i < nhealthbars ) {
                     
                        m_pGameContext->DrawContext()->DrawMaterial( *m_PixelMat, hudStart.GetX() + 12 + i * 20, hudStart.GetY() + ( float )hudSize.GetY() * ( 1.0f - heightmul ) * .5f + 2.0f, 6.0f, hudSize.GetY() * heightmul - 4.0f,  0.0f, .5f, 0.0f, 1.0f );

                    }
                    
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
            
            m_pGameContext->DrawTextPopups();
            
        }
    
        m_pGameContext->Lua().CallEngineFunction( "Draw" );

    
/*
        m_pGameContext->DrawContext()->DrawMaterial( *m_PixelMat, 10.0f, 545.0f, 150.0f, 10.0f, 1.0f, 1.0f, 1.0f, 0.5f );
        m_pGameContext->DrawContext()->DrawMaterial( *m_PixelMat, 11.0f, 546.0f, 148.0f * armor_mul, 8.0f, 0.3f, 0.3f, 1.0f, 1.0f );
        
        m_pGameContext->DrawContext()->DrawMaterial( *m_PixelMat, 10.0f, 560.0f, 150.0f, 10.0f, 1.0f, 1.0f, 1.0f, 0.5f );
        m_pGameContext->DrawContext()->DrawMaterial( *m_PixelMat, 11.0f, 561.0f, 148.0f * health_mul, 8.0f, 0.2f, 0.8f, 0.2f, 0.5f );

*/

	if( m_bGamePaused ) {
	
		m_pGameContext->DrawContext()->DrawMaterial( *m_PixelMat, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0, .5f );

        m_pGameContext->DrawContext()->DrawMaterial( *m_LongKeyMat, SCREEN_WIDTH * .5 - 130.0f, SCREEN_HEIGHT * .5f + 20.0f, 175.0f, 65.0f, 1.0f, 1.0f, 1.0f, 1.0f );
        m_pHUDFont->DrawString( m_pGameContext->DrawContext(), "ESCAPE      Unpause", SCREEN_WIDTH * .5f - 114.0f, SCREEN_HEIGHT * .5f + 29.0f, 1.0f, 1.0f, 1.0f, 1.0f );
 
		        
        m_pGameContext->DrawContext()->DrawMaterial( *m_LongKeyMat, SCREEN_WIDTH * .5 - 130.0f, SCREEN_HEIGHT * .5f + 100.0f, 50.0f, 65.0f, 1.0f, 1.0f, 1.0f, 1.0f );
        m_pHUDFont->DrawString( m_pGameContext->DrawContext(), "F       Toggle fullscreen", SCREEN_WIDTH * .5f - 114.0f, SCREEN_HEIGHT * .5f + 109.0f, 1.0f, 1.0f, 1.0f, 1.0f );
 
         m_pGameContext->DrawContext()->DrawMaterial( *m_LongKeyMat, SCREEN_WIDTH * .5 - 130.0f, SCREEN_HEIGHT * .5f + 180.0f, 150.0f, 65.0f, 1.0f, 1.0f, 1.0f, 1.0f );
        m_pHUDFont->DrawString( m_pGameContext->DrawContext(), "ENTER       Exit game", SCREEN_WIDTH * .5f - 114.0f, SCREEN_HEIGHT * .5f + 189.0f, 1.0f, 1.0f, 1.0f, 1.0f );


	}

   m_pGameContext->GraphicsContext()->SwapBuffers();


}
