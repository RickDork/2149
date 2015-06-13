#include "TrippyTrails.h"

void CTrippyTrails::Init() {
    
    m_FBO.Init( SCREEN_WIDTH, SCREEN_HEIGHT );
    
}

void CTrippyTrails::GenerateRandomColor() {
    
    float r = 0.0f, g = 0.0f, b = 0.0f;
    
    while( r < 100.0f && g < 100.0f && b < 100.0f ) {
        
        r = ( float )Util::RandomNumber( 0, 255 );
        g = ( float )Util::RandomNumber( 0, 255 );
        b = ( float )Util::RandomNumber( 0, 255 );
        
    }
    
    m_Color.Set( r / 255.0f, g / 255.0f, b / 255.0f, .2f );
    
    
}

void CTrippyTrails::GenerateRandomPos() { 

    float x = ( float )Util::RandomNumber( -5, 5 );
    float y = ( float )Util::RandomNumber( -5, 5 );
    
    m_Pos.Set( x, y );
    
}

void CTrippyTrails::Draw( CDrawContext * pDrawContext ) {

    pDrawContext->SetDrawColor( m_Color.GetX(), m_Color.GetY(), m_Color.GetZ(), m_Color.GetW() );
    
    CMatrix< float > mat;
    mat.Identity();
    mat.Translate( m_Pos.GetX(), SCREEN_HEIGHT - m_Pos.GetY(), 0.0f );
    
    m_FBO.DrawTexture( pDrawContext, &mat );
    
}