#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "SoraIncludes.h"

#include "GameContext.h"
#include "ShipEntity.h"
#include "Entity.h"

class CGameState : public CState< CTOFNContext >
{

private:

    CGameInput m_GameInput;

    CShipEntity * m_pPlayerEntity;

public:

    CGameState();

    void Init();
    void PostInit();

    void Input();
    void Think();
    void Draw();

};

#endif
