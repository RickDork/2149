#include "OrbAI.h"

COrbAI::COrbAI() : CTemplateAIController() {

    m_TrajAng = Util::RandomNumber( 0, 360 );
    
    do {
        m_DestAng = Util::RandomNumber( 0, 360 );
    } while( fabs( m_DestAng - m_TrajAng ) < 150.0f );
    
    m_StartVel = 300.0f;
    m_Alpha = 255.0f;

}

void COrbAI::Think() {

    CLuaContext * p = m_pEntityContext;
    COrbEntity * o = ( COrbEntity * )m_pTargetEntity;
    CShipEntity * ply = ( CShipEntity  * )m_pEntityContext->GetPlayerEntity();
    
    float speed = 100.0f + m_StartVel;
    
    o->Displace2( std::cos( m_TrajAng * DEG2RAD ) * speed, std::sin( m_TrajAng * DEG2RAD ) * speed );
    
    if( m_StartVel > 0.0f )
        m_StartVel -= 100.0f * p->GetFrameDelta();
    
    if( m_StartVel < 0.0f )
        m_StartVel = 0.0f;

    
    if( fabs( m_TrajAng - m_DestAng ) > 25.0f ) {
        
        float pdist = ( ( m_TrajAng > m_DestAng )? ( 360.0f - m_TrajAng ) + m_DestAng : fabs( m_TrajAng - m_DestAng ) );
        float ndist = ( ( m_TrajAng < m_DestAng )? ( 360.0f - m_DestAng ) + m_TrajAng : fabs( m_TrajAng - m_DestAng ) );
        
        float mul = ( pdist > ndist )? -1.0f : 1.0f;
        
        m_TrajAng += mul * 150.0f * p->GetFrameDelta();
        
        if( m_TrajAng > 360.0f )
            m_TrajAng -= 360.0f;
        
        if( m_TrajAng < 0.0f )
            m_TrajAng += 360.0f;
        
    }
    
    if( ply != NULL && ply->IsActive() ) {
        
        if( Util::Dist( ply->GetPos(), o->GetPos() ) < 300.0f ) {
    
            m_DestAng = Util::AngleBetweenPoints( ply->GetCenterX(), ply->GetCenterY(), o->GetX(), o->GetY() ) + 180.0f;
            m_StartVel += 135.0f * p->GetFrameDelta();
            
            if( m_StartVel > 350.0f )
                m_StartVel = 350.0f;
            
        }
        
    }
    
    m_Alpha -= 75.0f * p->GetFrameDelta();
    
    if( m_Alpha <= 0.0f ) {
     
        m_Alpha = 0.0f;
        
        if( o->IsActive() )
            o->KillMe();
        
    }
    
    o->SetColor( 1.0f, 1.0f, 1.0f, m_Alpha / 255.0f );

}