#include "ShipEntity.h"

void CShipEntity::Draw() {

    
    CMatrix< float > mat;
    mat.Identity();
    float yoffset = 0.0f;
    
    if( m_bHoverEffect ) {
        
        yoffset = cos( m_HoverTheta * DEG2RAD ) * 5.0f;
        m_HoverTheta += m_pContext->GetFrameDelta() * 100.0f;
        
        if( m_HoverTheta > 360.0f )
            m_HoverTheta -= 360.0f;
        
        mat.Translate( 0.0f, yoffset, 0.0f );
        
    }
    
    if( m_pTrailsImage ) {
     
        if( m_TrailPositions.size() > 0 ) {
         
            float x = GetX();
            float y = GetY();
            
            for( int j = 0; j < m_TrailPositions.size(); j++ ) {
             
                float ox = m_TrailPositions[j].GetX();
                float oy = m_TrailPositions[j].GetY() + yoffset;
                
                m_pContext->DrawContext()->DrawMaterial( *m_pTrailsImage, x + ox, y + oy, 30.0f, 30.0f, m_TrailColor.GetX(), m_TrailColor.GetY(), m_TrailColor.GetZ(), m_TrailColor.GetW() );
                
                if( !m_pContext->GetBoolean( "GameFrozen" ) && SDL_GetTicks() > m_TrailNextColorChange ) {
                 
                    if( m_TrailColor.GetW() > 0.0f ) {
                     
                        m_TrailNextColorChange = SDL_GetTicks() + Util::RandomNumber( 60, 90 );
                        m_TrailColor.SetW( 0.0f );
                    
                    } else {
                        
                        m_TrailNextColorChange = SDL_GetTicks() + Util::RandomNumber( 20, 50 );
                        m_TrailColor.SetW( ( float )Util::RandomNumber( 80, 120 ) / 255.0f );
                    
                    }
                    
                }
                
            }
            
        }
        
    }


    
    if( m_bWrapEdges ) {
        
        float xpos = m_Position.GetX();
        float realw = m_Sprite.GetRealSize( m_pContext->DrawContext() ).GetX();
        CMatrix< float > copyMat = mat;
        
        if( xpos < 0.0f ) {
            
            copyMat.Translate( SCREEN_WIDTH + xpos, m_Position.GetY(), 0.0f );
            BaseDraw( &copyMat, true );
            
        } else if( xpos + realw > SCREEN_WIDTH ) {
            
            copyMat.Translate( xpos - SCREEN_WIDTH, m_Position.GetY(), 0.0f );
            BaseDraw( &copyMat, true );            
            
        }
        
    }
    
    BaseDraw( &mat );

    
}