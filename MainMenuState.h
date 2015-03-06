#ifndef MAINMENUSTATE_H_
#define MAINMENUSTATE_H_

#include "SoraIncludes.h"
#include "GameContext.h"
#include <boost/filesystem.hpp>

class CMainMenuState : public CState< CTOFNContext > {

private:
    
    
    CGameInput m_GameInput;

    
    CTextureImage * m_PixelMat, * m_KeyMat, * m_LongKeyMat;
    CTextureImage * m_Title1, * m_Title2, * m_MenuBack, * m_MenuBack2;
    CFontMaterial * m_pFont;
    
    bool m_bIntroOn;
    
public:
    
    CMainMenuState();
    
    void Init();
    void PostInit();
    void OnStateSwitch();
    
    void Input();
    void Think();
    void Draw();
    
};



#endif
