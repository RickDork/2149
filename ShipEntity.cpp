#include "ShipEntity.h"

void CShipEntity::Draw() {

    if( m_pTrailsImage ) {
     
        if( m_TrailPositions.size() > 0 ) {
         
            float x = GetX();
            float y = GetY();
            
            for( int j = 0; j < m_TrailPositions.size(); j++ ) {
             
                float ox = m_TrailPositions[j].GetX();
                float oy = m_TrailPositions[j].GetY();
                
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
    
    BaseDraw();
    
}