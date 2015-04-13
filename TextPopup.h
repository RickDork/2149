#ifndef TEXTPOPUP_H_
#define TEXTPOPUP_H_

#include "SoraIncludes.h"

class CTextPopup {
    
private:
    
    Vector2< float > m_Pos;
    Vector4< float > m_Color;
    
    float m_FadeSpeed;
    bool m_bKillMe;
    
    std::string m_Text;
    CFontMaterial * m_pFont;
    
public:
    
    CTextPopup() : m_bKillMe( false ) {
        
    }
    
    void SetFont( CFontMaterial * pFont ) {
     
        m_pFont = pFont;
        
    }
    
    void SetText( std::string s ) {
     
        m_Text = s;
        
    }
    
    void SetColor( float r, float g, float b, float a ) {
     
        m_Color.Set( r / 255.0f, g / 255.0f, b / 255.0f, a );
        
    }
    
    void SetFadeSpeed( float f ) {
     
        m_FadeSpeed = f;
        
    }
    
    void SetPos( float x, float y ) {
     
        m_Pos.Set( x, y );
        
    }
    
    bool KillMe() {
     
        return m_bKillMe;
        
    }
    
    void Think( float );
    void Draw( CDrawContext * );
    
    
};

#endif
