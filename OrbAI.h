#ifndef ORBAI_H_
#define ORBAI_H_

#include "SoraIncludes.h"
#include "LuaContext.h"
#include "OrbEntity.h"

#include <cmath>

class COrbAI : public CTemplateAIController< CLuaContext > {
  
private:
    
    float m_TrajAng;
    float m_DestAng;
    float m_StartVel;
    float m_Alpha;
    
public:
    
    COrbAI();
    virtual ~COrbAI() {
    
    }
    
    void Think();
    
};

#endif