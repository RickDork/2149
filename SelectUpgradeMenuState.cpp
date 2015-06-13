#include "SelectUpgradeMenuState.h"


CSelectUpgradeMenuState::CSelectUpgradeMenuState() : CState()
{
    
    
    
}

void CSelectUpgradeMenuState::Init()
{
    
    m_PlayerUpgrades.push_back( CPlayerUpgrade( 0, 0, "Fire from 3 guns instead of the regular 2, however your guns are weaker." ) );
    m_PlayerUpgrades.push_back( CPlayerUpgrade( 1, 0, "1.5x more health." ) );
    m_PlayerUpgrades.push_back( CPlayerUpgrade( 2, 0, "Ship collisions do half the damage." ) );
    m_PlayerUpgrades.push_back( CPlayerUpgrade( 3, 2500, "2 more guns on your ship." ) );
    m_PlayerUpgrades.push_back( CPlayerUpgrade( 4, 2100, "2x more health." ) );
    m_PlayerUpgrades.push_back( CPlayerUpgrade( 5, 2000, "Health/EXP orbs are worth 1.5x more." ) );
    m_PlayerUpgrades.push_back( CPlayerUpgrade( 6, 2000, "Ship collisions only do a quarter the damage." ) );
    m_PlayerUpgrades.push_back( CPlayerUpgrade( 7, 1900, "Health/EXP orbs are worth 2.5x more." ) );
    m_PlayerUpgrades.push_back( CPlayerUpgrade( 8, 2600, "Your ship shoots in 8 directions." ) );
    m_PlayerUpgrades.push_back( CPlayerUpgrade( 9, 500, "Shoot twice as fast." ) );
    m_PlayerUpgrades.push_back( CPlayerUpgrade( 10, 0, "Bullets do a quarter less damage" ) );
    
    m_pFont = GetFont( DEFAULT_FONT, 32 );
    m_pTitleFont = GetFont( DEFAULT_FONT, 72 );
    
    m_PixelMat = GetTexture( "pixel.png" );
    m_KeyMat = GetTexture( "key.png" );
    m_LongKeyMat = GetTexture( "longkey.png" );

    m_bInit = true;
    
}

void CSelectUpgradeMenuState::PostInit()
{
    
    m_bPostInit = true;
    
}

void CSelectUpgradeMenuState::OnStateSwitch() {
 
    m_pGameContext->SetStarFieldComingAtYou( true );
    m_pGameContext->CreateStarBackground();
    m_pGameContext->StopMusic();
    
    
    m_bShowPlayerEXP = false;
    m_bSelectOne = true;
    m_UpgradeSelect = -1;
    
    ResetSelectableUpgrades();
    m_pGameContext->GetCurrentSelectableUpgrades();
    
    for( int j = 0; j < m_pGameContext->NumberOfSelectableUpgrades(); j++ ) {
        
        int id = m_pGameContext->GetSelectableUpgrade( j );
        AddSelectableUpgrade( id );
        
        if( m_PlayerUpgrades[id].m_EXPReq > 0 ) {
            
            m_bShowPlayerEXP = true;
            m_bSelectOne = false;
            
        }
        
    }
    
    if( m_bSelectOne ) {
     
        m_pGameContext->PlayMusic( "win.mp3", .1f );
        m_pGameContext->FadeMusic( 3000, .1f, 1.0f );
        
    } else
        m_pGameContext->PlayMusic( "win.mp3", 1.0f );
    
}

void CSelectUpgradeMenuState::Input()
{
    
    m_GameInput.Poll();
    
    if( StateTicksElapsed() > m_pGameContext->GetUpgradeMenuSelectMenuDelay() ) {

        if( m_GameInput.EventType() == SDL_KEYDOWN ) {
        
            if( m_GameInput.KeyDownOnce( SDLK_1 ) ) {
                
                m_UpgradeSelect = 1;
                
            }

            if( m_GameInput.KeyDownOnce( SDLK_2 ) ) {
                
                m_UpgradeSelect = 2;
                
            }
            
            if( m_GameInput.KeyDownOnce( SDLK_3 ) ) {
                
                m_UpgradeSelect = 3;
                
            }
            
            if( m_GameInput.KeyDownOnce( SDLK_4 ) ) {
                
                m_UpgradeSelect = 4;
                
            }
            
            if( m_GameInput.KeyDownOnce( SDLK_RETURN ) ) {
               
                m_pGameContext->SetStarFieldComingAtYou( false );
                SwitchToAnotherState( "GAME" );
                
            }
            
        }
        
    }
    
      
    if( m_GameInput.KeyDown( SDL_SCANCODE_ESCAPE ) )
    {
        
        QuitState();
        
    }
    
}

void CSelectUpgradeMenuState::Think() {

    if( m_UpgradeSelect > -1 ) {
     
        if( m_UpgradeSelect - 1 < m_pGameContext->NumberOfSelectableUpgrades() ) {
         
            int id = m_SelectableUpgrades[m_UpgradeSelect - 1];
            bool add = true;
            
            for( int j = 0; j < m_UpgradesSelected.size(); j++ ) {
             
                if( m_UpgradesSelected[j] == id ) {
                 
                    m_pGameContext->RemoveUpgrade( id, m_PlayerUpgrades[id].m_EXPReq );
                    m_UpgradesSelected.erase( m_UpgradesSelected.begin() + j );
                    add = false;
                    break;
                    
                }
                
            }
            
            if( add ) {
            
                if( m_bSelectOne && m_UpgradesSelected.size() > 0 ) {
                    
                    m_pGameContext->RemoveUpgrade( m_UpgradesSelected[0], m_PlayerUpgrades[m_UpgradesSelected[0]].m_EXPReq );
                    m_UpgradesSelected.clear();
                
                }
                
                if( m_pGameContext->GetPlayerEXP() >= m_PlayerUpgrades[id].m_EXPReq  ) {
                    
                    m_pGameContext->GiveUpgrade( id, m_PlayerUpgrades[id].m_EXPReq );
                    m_UpgradesSelected.push_back( id );
                    
                }
                
            }
            
        }
        
        m_UpgradeSelect = -1;
        
    }
    
}

void CSelectUpgradeMenuState::Draw() {

    m_pGameContext->GraphicsContext()->ClearBuffer();

    m_pGameContext->DrawStarBackground();
    
    if( StateTicksElapsed() > m_pGameContext->GetUpgradeMenuSelectMenuDelay() ) {
        
        static float titleAng = 0.0f;
        
        titleAng += 128.0f * m_pGameContext->GetFrameDelta();
        
        if( titleAng > 360.0f )
            titleAng -= 360.0f;
        
        m_pTitleFont->DrawString( m_pGameContext->DrawContext(), "Upgrade Select", SCREEN_WIDTH * .5f, SCREEN_HEIGHT * .1f + 10.0f * std::sin( titleAng * DEG2RAD ), 1.0f, 1.0f, 1.0f, 1.0f, DRAW_TEXT_HORIZ_CENTER | DRAW_TEXT_VERT_CENTER );
        
        if( m_bShowPlayerEXP ) {
         
            char expstr[255];
            sprintf( expstr, "You have %d EXP", m_pGameContext->GetPlayerEXP() );
            
            m_pFont->DrawString( m_pGameContext->DrawContext(), std::string( expstr ), SCREEN_WIDTH * .5f, SCREEN_HEIGHT * .1f + 75.0f, 1.0f, 1.0f, 1.0f, 1.0f, DRAW_TEXT_HORIZ_CENTER );
            
        } else if( m_bSelectOne ){
        
            m_pFont->DrawString( m_pGameContext->DrawContext(), "SELECT ONE", SCREEN_WIDTH * .5f, SCREEN_HEIGHT * .1f + 75.0f, 1.0f, 1.0f, 1.0f, 1.0f, DRAW_TEXT_HORIZ_CENTER );
            
        }
        
        
        float y = SCREEN_HEIGHT * .25f + 30.0f;
        
        for( int j = 0; j < m_SelectableUpgrades.size(); j++ ) {
            
            int id = m_SelectableUpgrades[j];
            CPlayerUpgrade upgrade = m_PlayerUpgrades[id];
            
            float x = SCREEN_WIDTH * .2f;
            float width = SCREEN_WIDTH * .6f;
            std::string desc = m_pFont->MakeFit( upgrade.m_Desc, width - 20.0f );
            float height = m_pFont->GetStringHeight( desc ) + 20.0f;
            int exp = upgrade.m_EXPReq;
            
            char key[2];
            sprintf( key, "%d", j + 1 );
            
            float r = .2f, g = .2f, b = .2f;
            
            for( int i = 0; i < m_UpgradesSelected.size(); i++ ) {
             
                if( m_UpgradesSelected[i] == id ) {
                    
                    r = .14f;
                    g = .28f;
                    b = .5f;
                    break;
                    
                }
                
            }
            
            m_pGameContext->DrawContext()->DrawMaterial( *m_KeyMat, x - 70, y, 64, 64, 1.0f, 1.0f, 1.0f, 1.0f );
            m_pFont->DrawString( m_pGameContext->DrawContext(), std::string( key ), x - 47.0f, y + 10.0f, 1.0f, 1.0f, 1.0f, 1.0f );
            
            m_pGameContext->DrawContext()->DrawMaterial( *m_PixelMat, x, y, width, height, r, g, b, 1.0f );
            m_pFont->DrawString( m_pGameContext->DrawContext(), desc, x + 10.0f, y, 1.0f, 1.0f, 1.0f, 1.0f );
            
            if( exp > 0 ) {
            
                float r = 1.0f, g = 1.0f, b = 1.0f;
                
                if( exp > m_pGameContext->GetPlayerEXP() ) {
                 
                    g = 0.0f;
                    b = 0.0f;
                    
                } else {
                 
                    r = 0.0f;
                    b = 0.0f;
                    
                }
                
                char expstr[255];
                sprintf( expstr, "%d EXP", exp );
                m_pFont->DrawString( m_pGameContext->DrawContext(), std::string( expstr ), x + width + 10.0f, y + height * .5f, r, g, b, 1.0f, DRAW_TEXT_VERT_CENTER );
                
            }
            
            y += height + 20.0f;
            
        }
        
        
        m_pGameContext->DrawContext()->DrawMaterial( *m_LongKeyMat, SCREEN_WIDTH * .15f, y + 20.0f, 160, 64, 1.0f, 1.0f, 1.0f, 1.0f );
        m_pFont->DrawString( m_pGameContext->DrawContext(), "ENTER  TO BEGIN MISSION", SCREEN_WIDTH * .15f + 25.0f, y + 30.0f, 1.0f, 1.0f, 1.0f, 1.0f );
        
    }
    
    m_pGameContext->GraphicsContext()->SwapBuffers();

    
}