#ifndef GAME_H_
#define GAME_H_

#include "SoraIncludes.h"
#include "GameState.h"

class CGame
{

private:

    CGraphicsContext m_GraphicsContext;
    CDrawContext m_DrawContext;
    CGameContext m_GameContext;
    CStateMachine m_StateMachine;
    CTextureFactory m_TextureFactory;
	CEntityManager m_EntityManager;

public:

    CGame();
    ~CGame();

};

#endif
