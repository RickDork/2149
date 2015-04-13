#include "SmokePlume.h"

void CSmokePlume::Init( float x, float y ) {

    m_ParticleEngine.Init( MAX_SMOKE_PLUMES, 1 );
    
    m_bKillMe = false;
    
    for( int j = 0; j < MAX_SMOKE_PLUMES; j++ ) {
     
        m_Plumes[j].m_Vel.Set( Util::RandomNumber( -20, 20 ), Util::RandomNumber( -20, 20 ) );
        m_Plumes[j].m_Alpha = 70.0f;
        
        float rgb = Util::RandomNumber( 80, 200 ) / 255.0f;
        float size = Util::RandomNumber( 70, 110 );
        
        m_Plumes[j].m_Color = rgb;
        
        m_ParticleEngine.SetParticleSize( j, size, size );
        m_Plumes[j].m_Pos.Set( x + Util::RandomNumber( -30, 30 ) - size * .5f, y + Util::RandomNumber( -30, 30 ) - size * .5f );

        
    }
    
}

void CSmokePlume::Think( float delta ) {

    if( !m_bKillMe ) {
    
        bool killme = true;
        
        for( int j = 0; j < MAX_SMOKE_PLUMES; j++ ) {
         
            m_Plumes[j].m_Alpha = Util::MinF( m_Plumes[j].m_Alpha, -delta * 20.0f, 0.0f );
            
            if( ( int )m_Plumes[j].m_Alpha > 0 )
                killme = false;
            
            float x = m_Plumes[j].m_Pos.GetX();
            float y = m_Plumes[j].m_Pos.GetY();
            float color = m_Plumes[j].m_Color;
            
            x += m_Plumes[j].m_Vel.GetX() * delta;
            y += ( m_Plumes[j].m_Vel.GetY() + 150.0f ) * delta;
            
            m_Plumes[j].m_Pos.Set( x, y );
            
            m_ParticleEngine.SetParticleColor( j, color, color, color, m_Plumes[j].m_Alpha / 255.0f );
            m_ParticleEngine.SetParticlePos( j, x, y );
            
        }
        
        m_bKillMe = killme;
        
    }
    
}

void CSmokePlume::Draw() {

    m_ParticleEngine.BindVertexBuffers();
    m_ParticleEngine.Draw();
    
}