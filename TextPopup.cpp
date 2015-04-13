#include "TextPopup.h"

void CTextPopup::Think( float delta ) {

    m_Color.SetW( Util::MinF( m_Color.GetW(), delta * -m_FadeSpeed, 0.0f ) );
    
    if( ( int )m_Color.GetW() < 1 ) {
     
        m_bKillMe = true;
        
    }
    
    
}

void CTextPopup::Draw( CDrawContext * pDrawContext ) {

    m_pFont->DrawString( pDrawContext, m_Text, m_Pos.GetX(), m_Pos.GetY(), m_Color.GetX(), m_Color.GetY(), m_Color.GetZ(), m_Color.GetW() / 255.0f, DRAW_TEXT_VERT_CENTER | DRAW_TEXT_HORIZ_CENTER );
    
}