#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "SoraIncludes.h"

class CGameState : public CState
{

private:

    CGameInput m_GameInput;
    CPhysicsWorld m_PhysicsWorld;

    CWorldEntity * m_pPlayerEntity;

public:

    CGameState();

    void Init();
    void PostInit();

    void Input();
    void Think();
    void Draw();

};

#endif
