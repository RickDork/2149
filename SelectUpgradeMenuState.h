#ifndef SELECTUPGRADEMENUSTATE_H_
#define SELECTUPGRADEMENUSTATE_H_

#include "SoraIncludes.h"

#include "GameContext.h"
#include "ShipEntity.h"
#include "Entity.h"
#include "Define.h"

class CPlayerUpgrade {
  
public:
    
    int m_Type;
    int m_EXPReq;
    std::string m_Desc;
    
    CPlayerUpgrade() : m_Type( -1 ), m_Desc( "" ), m_EXPReq( 0 ) {
        
    }
    
    CPlayerUpgrade( int type, int exp, std::string desc ) : m_Type( type ), m_Desc( desc ), m_EXPReq( exp ) {
        
    }
    
};

class CSelectUpgradeMenuState : public CState< CTOFNContext >
{
    
private:
    
 
    CGameInput m_GameInput;
    
    std::vector< int > m_UpgradesSelected;
    
    int m_UpgradeSelect;
    bool m_bSelectOne;
    bool m_bShowPlayerEXP;
    
    CTextureImage * m_PixelMat, * m_KeyMat, * m_LongKeyMat;
    CFontMaterial * m_pFont, * m_pTitleFont;
    
    std::vector< CPlayerUpgrade > m_PlayerUpgrades;
    std::vector< int > m_SelectableUpgrades;
    
public:
    
    CSelectUpgradeMenuState();
    
    void Init();
    void PostInit();
    void OnStateSwitch();
    
    void ResetSelectableUpgrades() {
     
        m_SelectableUpgrades.clear();
        
    }
    
    void AddSelectableUpgrade( int id ) {
     
        m_SelectableUpgrades.push_back( id );
        
    }
    
    void Input();
    void Think();
    void Draw();
    
};




#endif
