#ifndef TRIPPYTRAILS_H_
#define TRIPPYTRAILS_H_

#include "Define.h"
#include "SoraIncludes.h"

class CTrippyTrails {

private:
    
    Vector4< float > m_Color;
    Vector2< float > m_Pos;
    CFrameBufferObject m_FBO;
    
public:
    
    CFrameBufferObject & GetFBO() {
     
        return m_FBO;
        
    }
    
    void Init();
    void GenerateRandomColor();
    void GenerateRandomPos();
    void Draw( CDrawContext * );
    
};

#endif
