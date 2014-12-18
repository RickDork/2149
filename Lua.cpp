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

LuaCallBackFunction( SetMaxEnemyCount )
{
    
    int n = 0;
    
    if( LArgCount() >= 1 )
        n = LNumber( 1 );
    
    gLuaContext->SetMaxEnemyCount( n );
    
    return 0;
    
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
    
    LuaNameSpace( m_pLuaState, "ENGINE" );

    LuaNameSpace( m_pLuaState, "Game" );
        LuaNameSpaceFunction( m_pLuaState, "Game", GenerateEnemy );
        LuaNameSpaceFunction( m_pLuaState, "Game", SetMaxEnemyCount );
    
}
