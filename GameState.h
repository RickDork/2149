#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "SoraIncludes.h"

#include "GameContext.h"

class CGameState : public CState
{

private:

    CGameInput m_GameInput;
    CPhysicsWorld m_PhysicsWorld;

    CWorldEntity * m_pPlayerEntity;

	CTOFNContext * m_pGameContext;

public:

    CGameState();

	void SetGameContext( CTOFNContext * pGameContext )
	{

		m_pGameContext = pGameContext;

	}

    void Init();
    void PostInit();

    void Input();
    void Think();
    void Draw();

};

#endif
