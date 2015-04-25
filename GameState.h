#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "SoraIncludes.h"

#include "GameContext.h"
#include "ShipEntity.h"
#include "Entity.h"
#include "Define.h"

#define SECONDS_FLASH_TIME 300
#define GAMEOVER_TEXT "YOU ARE SPACE DUST!"

class CGameState : public CState< CTOFNContext >
{

private:
    
    CTextureImage * m_PixelMat;

    CGameInput m_GameInput;
    CShipEntity * m_pPlayerEntity;
    CFontMaterial * m_pHUDFont;
    CTrailsEffect m_BulletTrails;
    //CFrameBufferObject m_fboCurBullets, m_fboTrailBullets, m_fboTrailBulletsTemp;
    
    CFrameBufferObject m_fboNoiseImage;
    CFrameBufferObject m_fboNoiseBG;
    CFrameBufferObject m_fbo3DSpace;
    CFrameBufferObject m_fboGameBuffer;
    GLuint t;
    bool m_DrawSeconds;
    long int m_NextSecondsFlash;
    int m_BLTCount;
    
    long int m_GameTimer;
    
    void PreDrawBullets();
    void DrawBullets();
    void PostDrawBullets();
    
    void GenerateNoiseImage();
    void GenerateBackgroundNoiseImage();
    
    unsigned long int m_EndCutSceneTriggerTime;
    
public:

    CGameState();
    virtual ~CGameState() {
        
        glDeleteTextures( 1, &t );
        
    }

    void Init();
    void PostInit();
    void OnStateSwitch();

    void Input();
    void Think();
    void Draw();

};

#endif
