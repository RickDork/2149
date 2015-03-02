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
    CFrameBufferObject m_fboBullets, m_fboBullets2;

    bool m_DrawSeconds;
    long int m_NextSecondsFlash;
    
    long int m_GameTimer;
    
public:

    CGameState();

    void Init();
    void PostInit();
    void OnStateSwitch();

    void Input();
    void Think();
    void Draw();

};

#endif
