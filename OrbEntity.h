#ifndef ORBENTITY_H_
#define ORBENTITY_H_

#include "SoraIncludes.h"

class COrbEntity : public CAIEntity {
  
private:
    
    int m_Type;
    
public:
    
    COrbEntity();
    
    void SetType( int t ) {
     
        m_Type = t;
        
    }
    
    int GetType() {
     
        return m_Type;
        
    }
    
    virtual ~COrbEntity() { }
    
};

#endif

