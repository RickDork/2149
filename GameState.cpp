#include "GameState.h"

CGameState::CGameState() : CState()
{



}

void CGameState::Init()
{

    Log::Debug( "Doing game state init" );

    m_pGameContext->InitializePhysicsWorld();
    m_pGameContext->InitializeLua();
    m_pGameContext->InitializeGraphics();
    m_pGameContext->InitializeData();
    
    m_fboBullets.Init( SCREEN_WIDTH, SCREEN_HEIGHT );
    m_fboBullets2.Init( SCREEN_WIDTH, SCREEN_HEIGHT );

    m_bInit = true;

}

void CGameState::PostInit()
{

    Log::Debug( "Doing game state post-init" );

	m_pGameContext->TextureFactory()->NewTexture( "player2.png" );
	m_pGameContext->TextureFactory()->NewTexture("star.png");
    m_pGameContext->TextureFactory()->NewTexture("ball.png");
    m_pGameContext->TextureFactory()->NewTexture("pixel.png");
	m_pGameContext->TextureFactory()->NewTexture("bullet.png");
	m_pGameContext->TextureFactory()->NewTexture("Enemy.png");
	m_pGameContext->TextureFactory()->NewTexture("Enemy2.png");
	m_pGameContext->TextureFactory()->NewTexture("Enemy3.png");
	m_pGameContext->TextureFactory()->NewTexture("Enemy4.png");
    
    m_pGameContext->FontFactory()->NewFont( "font.ttf", 32 );
    
    texture = m_pGameContext->FontFactory()->GetFont( "font.ttf", 32 )->GetFontSheet().GetTexture();

    m_PixelMat = m_pGameContext->TextureFactory()->GetObjectContent( "pixel.png" );
    
    m_pPlayerEntity = m_pGameContext->CreatePlayerEntity();

	m_pGameContext->CreateStarBackground();

    m_bPostInit = true;

    Log::Debug( "Ready to do game loop" );

}

void CGameState::Input()
{

    m_GameInput.Poll();

    static const float plyMoveSpeedX = 500.0f;
    static const float plyMoveSpeedY = 500.0f;

    if( m_GameInput.KeyDown( SDL_SCANCODE_A ) )
        m_pPlayerEntity->Displace( -1.0f * plyMoveSpeedX * m_pGameContext->GetFrameDelta(), 0.0f );

    if( m_GameInput.KeyDown( SDL_SCANCODE_D ) )
        m_pPlayerEntity->Displace( plyMoveSpeedX * m_pGameContext->GetFrameDelta(), 0.0f );

    if( m_GameInput.KeyDown( SDL_SCANCODE_W ) )
        m_pPlayerEntity->Displace( 0.0f, -1.0f * plyMoveSpeedY * m_pGameContext->GetFrameDelta() );

    if( m_GameInput.KeyDown( SDL_SCANCODE_S ) )
        m_pPlayerEntity->Displace( 0.0f, plyMoveSpeedY * m_pGameContext->GetFrameDelta() );
    
    m_pPlayerEntity->FitIn( 0.0f, 0.0f, SCREEN_WIDTH - 50.0f, SCREEN_HEIGHT - 50.0f );
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

    m_pGameContext->GameLogic();

    if( m_pPlayerEntity->CanShoot() )
    {
        
        m_pGameContext->FireBulletFrom( ENTTYPE_PLYBULLET, m_pPlayerEntity, 500.0f );
        m_pPlayerEntity->SetNextShotTime( SDL_GetTicks() + 200 );
        
    }
    
}

void CGameState::Draw()
{

    m_pGameContext->GraphicsContext()->ClearBuffer();

        m_pGameContext->DrawStarBackground();

        m_pGameContext->EntityManager()->DrawAllEntitiesAtDepth( 1 );
        
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
    
        float armor_mul = ( m_pPlayerEntity->GetArmor() > 0.0f )? m_pPlayerEntity->GetArmor() / 100.0f : 0.0f;
        float health_mul = ( m_pPlayerEntity->GetHealth() > 0.0f )? m_pPlayerEntity->GetHealth() / 100.0f : 0.0f;
/*
        m_pGameContext->DrawContext()->DrawMaterial( *m_PixelMat, 10.0f, 545.0f, 150.0f, 10.0f, 1.0f, 1.0f, 1.0f, 0.5f );
        m_pGameContext->DrawContext()->DrawMaterial( *m_PixelMat, 11.0f, 546.0f, 148.0f * armor_mul, 8.0f, 0.3f, 0.3f, 1.0f, 1.0f );
        
        m_pGameContext->DrawContext()->DrawMaterial( *m_PixelMat, 10.0f, 560.0f, 150.0f, 10.0f, 1.0f, 1.0f, 1.0f, 0.5f );
        m_pGameContext->DrawContext()->DrawMaterial( *m_PixelMat, 11.0f, 561.0f, 148.0f * health_mul, 8.0f, 0.2f, 0.8f, 0.2f, 0.5f );

*/
   m_pGameContext->GraphicsContext()->SwapBuffers();


}
