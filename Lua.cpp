#include "Lua.h"

CLuaContext * gLuaContext = NULL;
CTextureImage * gPixelImage = NULL;
long int gTicksPad = 0;

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

LuaCallBackFunction( AddToGenQueue )
{
    
    int i = 0;
    float p = 0.0f;
    
    if( LArgCount() >= 2 ) {
     
        i = LNumber( 1 );
        p = LNumber( 2 );
        
    }
    
    gLuaContext->AddEnemyToGenQueue( i, p );
    return 0;
    
}

LuaCallBackFunction( ClearGenQueue )
{
    
    gLuaContext->ClearGenQueue();
    return 0;
    
}

LuaCallBackFunction( GenerateEnemy )
{
    
    CShipEntity * e = NULL;

    if( LArgCount() >= 2 ) {
        e = gLuaContext->CreateRandomEnemyEntity( LNumber( 2 ) );
    } else
        e = gLuaContext->CreateRandomEnemyEntity();
    
    if( LArgCount() >= 1 ) {
        
        if( LBoolean( 1 ) ) {
            
            e->SetCountAsEnemy( true );
            gLuaContext->IncrementEnemyCount();
    
        }
        
    }
    
    return 0;

}

LuaCallBackFunction( CreateEnemyFormation )
{
    
    bool countasenemy = false;
    int etype = -1;
    int ftype = -1;
    
    if( LArgCount() >= 1 )
        countasenemy = lua_toboolean( pLuaState, 1 );

    if( LArgCount() >= 2 )
        etype = lua_tonumber( pLuaState, 2 );

    if( LArgCount() >= 3 )
        ftype = lua_tonumber( pLuaState, 3 );
    
    if( etype > -1 && ftype > -1 ) {
     
        gLuaContext->CreateEnemyFormation( countasenemy, etype, ftype );
        
    } else
        gLuaContext->CreateRandomEnemyFormation( countasenemy );
    
    return 0;
    
}

LuaCallBackFunction( DrawString ) {
 
    std::string font = LString( 1 );
    int size = LNumber( 2 );
    std::string text = LString( 3 );
    float x = LNumber( 4 );
    float y = LNumber( 5 );
    float r = LNumber( 6 );
    float g = LNumber( 7 );
    float b = LNumber( 8 );
    float a = LNumber( 9 );
    bool horizcenter = false, vertcenter = false;
    
    if( LArgCount() >= 10 )
        horizcenter = LBoolean( 10 );
    
    if( LArgCount() >= 11 )
        vertcenter = LBoolean( 11 );
    
    int type = 0;
    
    if( horizcenter )
        type = DRAW_TEXT_HORIZ_CENTER;
    else
        type = DRAW_TEXT_LEFT;
    
    if( vertcenter )
        type |= DRAW_TEXT_VERT_CENTER;
    
    CFontMaterial * f = gLuaContext->FontFactory()->GetFont( font, size );
    
    if( f ) {
     
        f->DrawString( gLuaContext->DrawContext(), text, x, y, r, g, b, a, type );
        
    }
    
    return 0;
    
}

LuaCallBackFunction( TicksElapsed ) {
 
    lua_pushnumber( pLuaState, gLuaContext->TicksElapsed() + gTicksPad );
    
    return 1;
    
}

LuaCallBackFunction( FreezeGameTicks ) {
    
    gLuaContext->FreezeTicksElapsed();
    
    return 0;
    
}

LuaCallBackFunction( UnfreezeGameTicks ) {
    
    gLuaContext->UnfreezeTicksElapsed();
    
    return 0;
    
}

LuaCallBackFunction( ScreenWidth ) {
 
    lua_pushnumber( pLuaState, SCREEN_WIDTH );
    
    return 1;
    
}

LuaCallBackFunction( ScreenHeight ) {
    
    lua_pushnumber( pLuaState, SCREEN_HEIGHT );
    
    return 1;
    
}

LuaCallBackFunction( GetDelta ) {
    
    lua_pushnumber( pLuaState, gLuaContext->GetFrameDelta() );
    
    return 1;
    
}

LuaCallBackFunction( GetRetryCount ) {
 
    lua_pushnumber( pLuaState, gLuaContext->GetRetryCount() );
    
    return 1;
    
}

LuaCallBackFunction( GetEnemyCount ) {
    
    lua_pushnumber( pLuaState, gLuaContext->GetEnemyCount() );
    
    return 1;
    
}

LuaCallBackFunction( GetCurrentMission ) {
 
    lua_pushnumber( pLuaState, gLuaContext->GetCurrentMission() );
    
    return 1;
    
}

LuaCallBackFunction( GotoNextMission ) {
    
    gLuaContext->NextMission();
    
    return 0;
    
}

LuaCallBackFunction( GetStringWidth ) {
 
    std::string font = LString( 1 );
    int size = LNumber( 2 );
    std::string text = LString( 3 );
    
    lua_pushnumber( pLuaState, gLuaContext->FontFactory()->GetFont( font, size )->GetStringWidth( text ) );
    
    return 1;
    
}

LuaCallBackFunction( DrawTexture ) {
 
    std::string texture = LString( 1 );
    float x = LNumber( 2 );
    float y = LNumber( 3 );
    int width = LNumber( 4 );
    int height = LNumber( 5 );
    
    float r = 1.0f, g = 1.0f, b = 1.0f, a = 1.0f;
    
    if( LArgCount() > 6 ) {
        
        r = LNumber( 6 ) / 255.0f;
        g = LNumber( 7 ) / 255.0f;
        b = LNumber( 8 ) / 255.0f;
        a = LNumber( 9 ) / 255.0f;

    }
    
    CTextureImage * i = NULL;
    
    if( texture == "pixel.png" ) {
      
        if( !gPixelImage )
            gPixelImage = gLuaContext->TextureFactory()->GetObjectContent( "pixel.png" );
        
        i = gPixelImage;
            
    } else
        i = gLuaContext->TextureFactory()->GetObjectContent( texture );
    
    if( i )
        gLuaContext->DrawContext()->DrawMaterial( *i, x, y, width, height, r, g, b, a );

    
    return 0;
    
}

LuaCallBackFunction( MakeStringFit ) {
 
    std::string text = LString( 1 );
    int width = LNumber( 2 );
    std::string font = LString( 3 );
    int fontsize = LNumber( 4 );
    
    lua_pushstring( pLuaState, gLuaContext->FontFactory()->GetFont( font, fontsize )->MakeFit( text, width ).c_str() );
    
    return 1;
    
}


LuaCallBackFunction( TogglePlayerInput ) {
    
    bool b = LBoolean( 1 );
    
    gLuaContext->TogglePlayerInput( b );
    
    return 0;
    
}



LuaCallBackFunction( SetDrawHUD ) {
    
    bool b = LBoolean( 1 );
    
    gLuaContext->SetShouldDrawHUD( b );
    
    return 0;
    
}

LuaCallBackFunction( PadTicks ) {
 
    int i = LNumber( 1 );
    
    gTicksPad = i;
    
    return 0;
    
}

LuaCallBackFunction( GetTicks2 ) {
 
    lua_pushnumber( pLuaState, SDL_GetTicks() + gTicksPad );
    
    return 1;
    
}

LuaCallBackFunction( GetPlayerKillCount ) {
 
    lua_pushnumber( pLuaState, gLuaContext->GetPlayerKillCount() );
    
    return 1;
    
}

LuaCallBackFunction( ToggleGameFrozen ) {
 
    bool b = LBoolean( 1 );
    
    gLuaContext->ToggleGameFrozen( b );
    
    return 0;
    
}

void CTOFNLua::CreateLuaHooks()
{
    
    LuaNameSpace( m_pLuaState, "ENGINE" );
    
    LuaFunctionRemove( m_pLuaState, GetTicks );
    LuaFunctionWithName( m_pLuaState, GetTicks, GetTicks2 );

    LuaNameSpace( m_pLuaState, "Game" );
        LuaNameSpaceFunction( m_pLuaState, "Game", GenerateEnemy );
        LuaNameSpaceFunction( m_pLuaState, "Game", SetMaxEnemyCount );
        LuaNameSpaceFunction( m_pLuaState, "Game", AddToGenQueue );
        LuaNameSpaceFunction( m_pLuaState, "Game", ClearGenQueue );
        LuaNameSpaceFunction( m_pLuaState, "Game", CreateEnemyFormation );
        LuaNameSpaceFunction( m_pLuaState, "Game", ScreenWidth );
        LuaNameSpaceFunction( m_pLuaState, "Game", ScreenHeight );
        LuaNameSpaceFunction( m_pLuaState, "Game", GetRetryCount );
        LuaNameSpaceFunction( m_pLuaState, "Game", GetEnemyCount );
        LuaNameSpaceFunction( m_pLuaState, "Game", GetCurrentMission );
        LuaNameSpaceFunction( m_pLuaState, "Game", GotoNextMission );
        LuaNameSpaceFunction( m_pLuaState, "Game", GetStringWidth );
        LuaNameSpaceFunction( m_pLuaState, "Game", MakeStringFit );
        LuaNameSpaceFunction( m_pLuaState, "Game", TogglePlayerInput );
        LuaNameSpaceFunction( m_pLuaState, "Game", PadTicks );
        LuaNameSpaceFunction( m_pLuaState, "Game", ToggleGameFrozen );
        LuaNameSpaceFunction( m_pLuaState, "Game", GetPlayerKillCount );
    
    LuaNameSpace( m_pLuaState, "Draw" );
        LuaNameSpaceFunction( m_pLuaState, "Draw", DrawString );
        LuaNameSpaceFunction( m_pLuaState, "Draw", DrawTexture );
        LuaNameSpaceFunction( m_pLuaState, "Draw", SetDrawHUD );
    
    LuaFunction( m_pLuaState, TicksElapsed );
    LuaFunction( m_pLuaState, FreezeGameTicks );
    LuaFunction( m_pLuaState, UnfreezeGameTicks );
    LuaFunction( m_pLuaState, GetDelta );
    
    
}
