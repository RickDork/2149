#ifndef SMOKEPLUME_H_
#define SMOKEPLUME_H_

#include "SoraIncludes.h"

#define MAX_SMOKE_PLUMES 12

class CPlumeInstance {
  
private:
    
public:
    
    Vector2< float > m_Pos, m_Vel;
    float m_Color;
    float m_Alpha;
    
    CPlumeInstance(): m_Alpha( 255.0f ) {
        
    }
    
};

class CSmokePlume {
    
private:
    
    CInstancedParticleEngine m_ParticleEngine;
    CPlumeInstance m_Plumes[MAX_SMOKE_PLUMES];
    bool m_bKillMe;
    
public:
    
    bool KillMe() {
     
        return m_bKillMe;
        
    }
    
    void Think( float );
    void Draw();
    
    void Init( float, float );
    
};

#endif
