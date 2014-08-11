#include "Lua.h"

CLuaContext * gLuaContext = NULL;

void SetLuaContext( CLuaContext * pLuaContext )
{

    gLuaContext = pLuaContext;

}


void CTOFNLua::CallEngineFunction( const char * f )
{

    CallLuaFunction( f, "ENGINE" );

}

LuaCallBackFunction( GenerateEnemy )
{

    int n = 1;

    if( LArgCount() >= 1 )
        n = LNumber( 1 );

    for( int j = 0; j < n; j++ )
        gLuaContext->CreateRandomEnemyEntity();

    return 0;

}

void CTOFNLua::CreateLuaHooks()
{

    LuaNameSpace( m_pLuaState, "Game" );
        LuaNameSpaceFunction( m_pLuaState, "Game", GenerateEnemy );

}
