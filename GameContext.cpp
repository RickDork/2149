#include "GameContext.h"


#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

CTOFNContext::CTOFNContext() : CLuaContext(), m_pPlayerEntity( NULL ), m_MaxEnemyCount( 3 ), m_CurEnemyCount( 0 ), m_NextEnemySpawn( 0 ), m_PlayerEXP( 0 ), m_bGameTicksFrozen( false ), m_GameTicksFreezeTime( 0 ), m_RetryCount( 0 ), m_CurrentMission( 3 ), m_bDrawHUD( true ), m_bCreatedStarField( false ), m_bStarFieldUpgradeSelect( false ), m_StartingEXP( 0 ), m_bMissionOver( false ), m_PlayerKillCount( 0 ), m_bGameFrozen( false ), m_StarFieldSpeedMul( 1.0f ), m_bStarFieldSlowFill( false ), m_StarFieldSlowFillIndex( 0 ), m_StarFieldSlowFillNextTime( 0 ), m_bBossMode( false ), m_BossHealthPercent( 1.0f ),
    m_bCutScene( false ), m_BossHealth( 0.0f ), m_bPlayerInvincible( false ), m_pSpaceFogFBO( NULL ), m_bEnding( false ), m_NextBulletSound( 0 ), m_pCurMusChannel( NULL )
{
    
    m_QuadTree.Init( 0, -100, 1200 );

}

void CTOFNContext::InitializePhysicsWorld()
{

    //m_PhysicsWorld.Initialize( 0.0f, 0.0f );
    m_CollisionCallback.SetContext( this );
   // m_PhysicsWorld.SetContactListener( &m_CollisionCallback );

}

void CTOFNContext::InitializeLua()
{

    m_Lua.Initialize();
    SetLuaContext( this );
    
    m_Lua.ReadFile( "scripts/main.lua" );
    
}


void CTOFNContext::InitializeGraphics()
{

    m_pGraphicsContext->LoadShaderProgram( "star.v", "star.f" );
    m_pGraphicsContext->LoadShaderProgram( "bullet.v", "bullet.f" );
    m_pGraphicsContext->LoadShaderProgram( "funkybullet.v", "funkybullet.f" );
    m_pGraphicsContext->LoadShaderProgram( "explosion.v", "explosion.f" );
    m_pGraphicsContext->LoadShaderProgram( "vertex.v", "noiseimage.f" );
    m_pGraphicsContext->LoadShaderProgram( "funkybg.v", "funkybg.f" );
    m_pGraphicsContext->LoadShaderProgram( "funkybg.v", "funkybg2.f" );
    m_pGraphicsContext->LoadShaderProgram( "vertex.v", "noisebg.f" );
    m_pGraphicsContext->LoadShaderProgram( "vertex.v", "spacefog.f" );
    m_pGraphicsContext->LoadShaderProgram( "vertex.v", "space3d.f" );
    m_pGraphicsContext->LoadShaderProgram( "horizblur.v", "horizblur.f" );
    m_pGraphicsContext->LoadShaderProgram( "vertblur.v", "vertblur.f" );
    m_pGraphicsContext->LoadShaderProgram( "vertex.v", "solidwhite.f" );
    
    int width, height;

     m_pGraphicsContext->GetWindowSize( &width, &height );
    
    m_StarEngine.Init( MAX_STARS, 1 );

    for( int j = 13; j >= 0; j-- )
    {

        int id = m_pGraphicsContext->GetShaderIDFromIndex( j );

        m_pDrawContext->UseShaderProgram( id );

        m_pDrawContext->Calculate2DProjectionMatrix( width, height );

        CMatrix< float > viewMat;
        viewMat.Identity();
        m_pDrawContext->UpdateViewMatrix( &viewMat );

    }

}

void CTOFNContext::InitializeData()
{

    LoadEnemyData();

}

void CTOFNContext::Initialize() {
    
    InitializePhysicsWorld();
    InitializeLua();
    InitializeGraphics();
    InitializeData();
    
    LoadResources( "files.res" );
    
}

void CTOFNContext::GetCurrentSelectableUpgrades() {
    
    m_SelectableUpgrades.clear();
    
    switch( m_CurrentMission ) {
     
        case 1:
            for( int i = 0; i < 3; i++ )
                m_SelectableUpgrades.push_back( i );
            m_SelectableUpgrades.push_back( 10 );
            break;
        case 2:
            for( int i = 3; i < 6; i++ )
                m_SelectableUpgrades.push_back( i );
            break;
        case 3:
            for( int i = 6; i < 10; i++ )
                m_SelectableUpgrades.push_back( i );
            break;
            
    }
    
}

void CTOFNContext::TriggerEnding() {
    

    for( int j = 0; j < 64; j++ ){
    
        FMOD::Channel * pChannel;
        
        if( SoundContext()->GetSystem()->getChannel( j, &pChannel ) == FMOD_OK ) {
        
            pChannel->stop();
            
        }
        
    }
    
    int n = 0;
    for( ; n < MAX_STARS; n++ )
    {
        
        CStar * s = &m_pStars[n];
        s->m_Speed = Util::RandomNumber( 90, 135 );
        
    }
    
    m_bEnding = true;
    m_pEntityManager->RemoveAllQueuedEntities();
    m_pEntityManager->RemoveAllDeletedEntities();
    m_pEntityManager->RemoveAllEntities();
    
    m_pSmokePlumes.clear();
    m_pExplosions.clear();
    
    m_pPlayerEntity = NULL;
    m_CurEnemyCount = 0;
    m_MaxEnemyCount = 0;
    m_EnemyGenQueue.clear();
    m_MaxGenQueueProbability = 0;
    m_NextEnemySpawn = 0;
    CreatePlayerEntity();
    
}

void CTOFNContext::ClearAllEnemies() {
 
    
    m_pEntityManager->RemoveAllDeletedEntities();
    
    boost::ptr_vector< CEntityObject > & e = m_pEntityManager->GetEntityObjects();
    boost::ptr_vector< CEntityObject >::iterator iter = e.begin();
    boost::ptr_vector< CEntityObject >::iterator end = e.end();
    
    while( iter != end ) {
    
        CEntity * ent = ( *iter ).GetContent();
        
        if( ent->GetClassType() == "EN" ) {
         
            m_pEntityManager->DeleteEntity( ent );
            
        } 
        
        iter++;
        
    }
    
}

void CTOFNContext::GameplayStart() {
 
    m_pEntityManager->RemoveAllQueuedEntities();
    m_pEntityManager->RemoveAllDeletedEntities();
    m_pEntityManager->RemoveAllEntities();
    
    m_pSmokePlumes.clear();
    m_pExplosions.clear();
    
    m_pPlayerEntity = NULL;
    m_CurEnemyCount = 0;
    m_MaxEnemyCount = 0;
    m_EnemyGenQueue.clear();
    m_MaxGenQueueProbability = 0;
    m_NextEnemySpawn = 0;
    m_PlayerEXP = m_StartingEXP;
    m_bGameTicksFrozen = false;
    m_GameTicksFreezeTime = SDL_GetTicks();
    m_bMissionOver = false;
    m_bBossMode = false;
    m_bGameFrozen = false;
    
   // if( m_CurrentMission == 4 )
        m_bPlayerInvincible = true;
  
    this->GiveUpgrade( 7, 0 );
    this->GiveUpgrade( 8, 0 );
    
    m_PlayerKillCount = 0;
    
    CreatePlayerEntity();
    SetGameStartTime( SDL_GetTicks() );
   
    m_Lua.CallEngineFunction( "GameInit" );
    
}

bool CTOFNContext::GetBoolean( std::string s ) {
 
    if( s == "GameFrozen" )
        return m_bGameFrozen;
        
    return false;
    
}

void CTOFNContext::NextMission() {
    
    m_RetryCount = 0;
    m_CurrentMission++;
    
    m_bMissionOver = true;
    
    //GameplayStart();
    
}

void CTOFNContext::LoadEnemyData()
{
    
    Log::Debug( "Reading enemy data" );

    CJsonObject jsonData;
    jsonData.Open( "enemy.txt" );

    rapidjson::Document & doc = jsonData.GetDocument();
  
    int nEnemy = doc.Size();

    for( rapidjson::SizeType j = 0; j < nEnemy; j++ )
    {

		CEnemyData d;

        d.m_Health = doc[j]["health"].GetInt();
        d.m_SpeedMin = doc[j]["speedmin"].GetInt();
        d.m_SpeedMax = doc[j]["speedmax"].GetInt();
        d.m_Width = d.m_CollisionBoxWidth = doc[j]["width"].GetInt();
        d.m_Height = d.m_CollisionBoxHeight = doc[j]["height"].GetInt();

		if( doc[j].HasMember( "colwidth" ) )
			d.m_CollisionBoxWidth = doc[j]["colwidth"].GetInt();

		if( doc[j].HasMember( "colheight" ) )
			d.m_CollisionBoxHeight = doc[j]["colheight"].GetInt();

        d.m_Sprite = doc[j]["sprite"].GetString();
        d.m_bCanBeInFormation = true;
        
        if( doc[j].HasMember( "formation" ) ) {
         
            if( doc[j]["formation"].GetInt() == 0 )
                d.m_bCanBeInFormation = false;
            
            
        }

        if( doc[j].HasMember( "trail" ) )
        {
            
            rapidjson::Value & trailarray = doc[j]["trail"];
            
            if( trailarray.IsArray() )
            {
                
                rapidjson::SizeType nTrails = doc[j]["trail"].Size();
                for( rapidjson::SizeType i = 0; i < nTrails; i++ )
                {
                    
                    rapidjson::Value & v = doc[j]["trail"][i];
                    d.m_TrailPos.push_back( Vector2< float >( v["x"].GetDouble(), v["y"].GetDouble() ) );
                    d.m_TrailColor.push_back( Vector3< float >( v["r"].GetDouble(), v["g"].GetDouble(), v["b"].GetDouble() ) );

                    
                }
                
            }
            
        }


		if( doc[j].HasMember( "gun" ) )
		{

			rapidjson::Value & gunarray = doc[j]["gun"];

			if( gunarray.IsArray() )
			{

				rapidjson::SizeType nGun = doc[j]["gun"].Size();
				for( rapidjson::SizeType i = 0; i < nGun; i++ )
				{

					rapidjson::Value & v = doc[j]["gun"][i];
                    d.m_GunDmg.push_back( v["dmg"].GetDouble() );
					d.m_GunPos.push_back( Vector2< float >( v["x"].GetDouble(), v["y"].GetDouble() ) );

				}

			}

		}

		m_EnemyData.push_back( d );

    }

    Log::Debug( "Done reading enemy data" );

}

void CTOFNContext::GiveUpgrade( int type, int cost ) {

    m_PlayerEXP -= cost;
    m_StartingEXP = m_PlayerEXP;
    
    m_Upgrades.push_back( type );
    
}

void CTOFNContext::RemoveUpgrade( int type, int cost ) {
    
    m_PlayerEXP += cost;
    m_StartingEXP = m_PlayerEXP;
    
    for( int j = 0; j < m_Upgrades.size(); j++ )
    {
        
        if( m_Upgrades[j] == type ) {
        
            m_Upgrades.erase( m_Upgrades.begin() + j );
            return; 
            
        }
        
    }
    
}

void CTOFNContext::ExplodePlayer() {
    
    const Vector2< float > & pos = m_pPlayerEntity->GetPos();
    Vector2< int > size = m_pPlayerEntity->GetSize();
    
    CreateExplosionsAndSmoke( 0, pos.GetX() + ( float )size.GetX() * .5f, pos.GetY() + ( float )size.GetY() * .5f );
    m_pPlayerEntity->SetDraw( false );
    
}

CShipEntity * CTOFNContext::CreatePlayerEntity()
{

    static float defaultPlayerPosX = 350.0f;
    static float defaultPlayerPosY = 500.0f;

    if( m_CurrentMission == 4 ) {
     
        if( m_bEnding ) {
            
            
            defaultPlayerPosX = 230.0f;
            defaultPlayerPosY = 450.0f;            
            
        } else {
        
            defaultPlayerPosX = 730.0f;
            defaultPlayerPosY = 250.0f;
        
        }
        
    }
    
	Log::Debug( "Creating player entity" );
    
    float gunDmg = 5.0f;
    float health = 150.0f;
    
    if( HasUpgrade( 0 ) || HasUpgrade( 3 ) || HasUpgrade( 8 ) )
        gunDmg = 3.0f;

    if( HasUpgrade( 4 ) )
        health = 300.0f;
    else if( HasUpgrade( 1 ) )
        health = 225.0f;
    
    
    CShipEntity * ent = new CShipEntity;
	ent->SetContext( this );
    ent->SetClassTypeID( ENTTYPE_PLAYER );
	//ent->CreatePhysicsBody( m_PhysicsWorld.GetPhysicsWorld(), 64, 64 );
    
    if( m_CurrentMission == 4 && !m_bEnding ) {
        ent->SetMaterial( m_pTextureFactory->GetObjectContent( "player3.png" ) );
        ent->SetWrapEdges( true );
    } else {
        ent->SetMaterial( m_pTextureFactory->GetObjectContent( "player2.png" ) );
        ent->SetWrapEdges( false );
    }
    
    ent->SetSize( 80, 80 );
    //ent->DisablePhysicsMovement();
    //ent->SetGravity( 0 );
    ent->SetPos( defaultPlayerPosX, defaultPlayerPosY );
    ent->SetDrawDepth( 1 );
    ent->SetHoverEffect( true );
    ent->SetHealth( health );
    ent->SetMaxHealth( health );
    ent->SetArmor( 0.0f );
    
    ent->AddGun( .3f, .23f, gunDmg );
	ent->AddGun( .7f, .23f, gunDmg );
    
    ent->SetTrailsImage( TextureFactory()->GetObjectContent( "shiptrail.png" ) );
    ent->AddTrails( .33f, .8f );
    ent->SetTrailColor( .52, .8f, 0.9f, 1.0f );

    CBoxCollisionBody * colBody = new CBoxCollisionBody;
    colBody->FitFromCenter( 80.0f, 80.0f, 64.0f, 64.0f );
    ent->SetCollisionBody( colBody );
    
    if( HasUpgrade( 0 ) )
        ent->AddGun( .5f, .1f, gunDmg );
    
    if( m_pPlayerEntity )
        m_pEntityManager->RemoveEntity( m_pPlayerEntity );

    m_pPlayerEntity = ent;

    m_pEntityManager->AddEntity( m_pPlayerEntity );
    m_QuadTree.AddEntity( m_pPlayerEntity );

    
	Log::Debug( "Created player entity" );

    return m_pPlayerEntity;

}

void CTOFNContext::FireBulletFromGunAtAngle( int gun, int type, CShipEntity * pShip, float speed, float angle )
{
    
    const std::vector< Vector3< float > > & GunPos = pShip->GetGunPositions();
    float pX, pY;
    
    pShip->GetPos().Get( &pX, &pY );
        
    float x, y;
    float dmg = pShip->GetGunDamage( gun );
    
    GunPos[gun].Get( &x, &y );
    
    FireBulletFrom( type, pX + x, pY + y, dmg, speed, angle );
    
    
}


void CTOFNContext::FireBulletFrom( int type, CShipEntity * pShip, float speed )
{

    const std::vector< Vector3< float > > & GunPos = pShip->GetGunPositions();
    float pX, pY;

    pShip->GetPos().Get( &pX, &pY );

    for( int j = 0; j < GunPos.size(); j++ )
    {

        float x, y;
        float dmg = pShip->GetGunDamage( j );

        GunPos[j].Get( &x, &y );
        
        FireBulletFrom( type, pX + x, pY + y, dmg, speed );

    }

}



CSmokePlume * CTOFNContext::CreateSmokePlume( float x, float y )
{
    
    
    CSmokePlume * s = new CSmokePlume;
    //p->SetIsSmoke( true );
    s->Init( x, y );
    m_pSmokePlumes.push_back( s );

    return s;
    
}

void CTOFNContext::CreateExplosionsAndSmoke( int count, int type, float x, float y ) {
    
    for( int j = 0; j < count; j++ ) {
     
        float tx = 0.0f, ty = 0.0f;
        
        if( j > 0 ) {
            
            tx = Util::RandomNumber( -50, 50 );
            ty = Util::RandomNumber( -50, 50 );
            
        }
        
        CreateExplosion( type, x + tx, y + ty);
        CreateSmokePlume( x + tx * .8f, y + ty * .8f );
        
    }
    
    FMOD::Channel * exp = SoundContext()->PlaySound( SoundFactory()->GetSound( "explosion.wav" ));
    exp->setPitch( ( float )Util::RandomNumber( 75, 150 ) / 100.0f );
    exp->setVolume( ( float )Util::RandomNumber( 75, 150 ) / 100.0f );
    
}

void CTOFNContext::CreateExplosionsAndSmoke( int type, float x, float y ) {
    
    int count = Util::RandomNumber( 2, 4 );
    
    CreateExplosionsAndSmoke( count, type, x, y );
    
}

CParticleExplosion * CTOFNContext::CreateExplosion( int type, float x, float y )
{
    
    float range = -1.0f;
    float size = 60.0f;
    bool mega = false;
    
    if( type > -1 ) {
    
        if( Util::RandomNumber( 1, 10 ) == 2 ) {
         
            range = 4000.0f;
            size = 120.0f;
            mega = true;
        }
        
    } else {
    
        type = 0;
        
    }

        
        
    CParticleExplosion * p = new CParticleExplosion;
    p->Init( ( mega ) ? 2 : 1, range, x, y );
    m_pExplosions.push_back( p );
    
    CWorldEntity * ent = new CWorldEntity;

	ent->SetContext( this );
    ent->SetClassTypeID( ENTTYPE_EXPLOSION );
    
    if( Util::RandomNumber( 1, 10 ) < 2 )
        ent->SetClassType( "XPL-NODMG" );
    else
        ent->SetClassType( "XPL" );

    ent->SetPos( x - size * .5f, y - size * .5f );
    //ent->CreatePhysicsBody( m_PhysicsWorld.GetPhysicsWorld(), size, size );
    
    CBoxCollisionBody * colBody = new CBoxCollisionBody;
    colBody->SetBox( 0.0f, 0.0f, size, size );
    ent->SetCollisionBody( colBody );
    
    p->SetExplosionEntity( ent );
    m_pEntityManager->QueueEntity( ent );
    m_QuadTree.AddEntity( ent );
    
    return p;

}

CAIEntity * CTOFNContext::FireBulletFrom( int type, float x, float y, float dmg, float speed ) {
 
    float ang = 0.0f;
    
    if( type == ENTTYPE_ENBULLET && m_pPlayerEntity && m_pPlayerEntity->IsActive() ) {
  
        ang = 270.0f;
        
        Vector2< float > p = m_pPlayerEntity->GetPos();
        
        if( Util::RandomNumber( 1, 10 ) < 8 )
            ang = -Util::AngleBetweenPoints( x, y, p.GetX() + 45 + Util::RandomNumber( -50, 50 ), p.GetY() + 45 + Util::RandomNumber( -50, 50 ) );
        
        if( ang < 0.0f )
            ang += 360.0f;
        
        ang = Util::MinF( ang, 0.0f, 200.0f );
        ang = Util::MaxF( ang, 0.0f, 340.0f );
        
    } else {
        
        ang = 90.0f;
        
    }
    
    return FireBulletFrom( type, x, y, dmg, speed, ang );
    
}

bool CTOFNContext::HasUpgrade( int type ) {
 
    for( int j = 0; j < m_Upgrades.size(); j++ ) {
     
        if( m_Upgrades[j] == type )
            return true;
        
    }
    
    return false;
    
}


CAIEntity * CTOFNContext::FireBulletFrom( int type, float x, float y, float dmg, float speed, float angle )
{

    CAIEntity * ent = new CAIEntity;
    CBulletAI * aic = new CBulletAI;
    
    float r, g, b;
    float sx = 1.0f, sy = 1.0f;
    
    std::string sprite = "bullet.png";
    
    if( type == ENTTYPE_ENBULLET ) {
        
        r = 255.0f;
        g = Util::RandomNumber( 50, 150 );
        b = Util::RandomNumber( 50, 150 );
        
        //Vector2< float > p = m_pPlayerEntity->GetPos();
        
        aic->SetAngle( angle );
        
        sprite = "ball.png";
        sx = .5f;
        sy = .5f;
        
    } else {
     
        r = 0.0f;
        g = 255.0f;
        b = 0.0f;
        
        aic->SetAngle( angle );
        
    }

    aic->SetSpeed( speed );
    aic->SetDamage( dmg );
    aic->SetDirection( ( type & ENTTYPE_ENBULLET )? DIR_DOWN : DIR_UP );

	ent->SetContext( this );
    ent->SetClassTypeID( type );
    ent->SetClassType( "BLT" );
	//ent->CreatePhysicsBody( m_PhysicsWorld.GetPhysicsWorld(), 5, 5 );
    ent->SetMaterial( m_pTextureFactory->GetObjectContent( sprite ) );
    //ent->DisablePhysicsMovement();
    //ent->SetGravity( 0 );
    ent->SetPos( x, y );
    ent->SetScale( sx, sy );
    ent->SetColor( r / 255.0f, g / 255.0f, b / 255.0f, 1.0f );

    CBoxCollisionBody * colBody = new CBoxCollisionBody;
    colBody->SetBox( 0.0f, 0.0f, 5.0f, 5.0f );
    
    ent->SetCollisionBody( colBody );
    
    aic->SetTargetEntity( ent );
    ent->SetAIController( aic );
    
    if( bIter )
        bulletvec.push_back( ent );
    else {
        
        m_pEntityManager->AddEntity( ent );
        m_QuadTree.AddEntity( ent );
    }
    
    if( SDL_GetTicks() > m_NextBulletSound ) {
        
        FMOD::Channel * blt = SoundContext()->PlaySound( SoundFactory()->GetSound( "bullet.wav" ));
        //blt->setPitch( ( float )Util::RandomNumber( 75, 150 ) / 100.0f );
        blt->setVolume( .2f );
        
        m_NextBulletSound = SDL_GetTicks() + Util::RandomNumber( 50, 100 );
        
    }
        
    
    return ent;

}

int CTOFNContext::GetRandomEnemyType() {
 
    int genindex = 0;
    
    if( m_EnemyGenQueue.size() > 1 ) {
        
        int r = Util::RandomNumber( 0, m_MaxGenQueueProbability );
        int totalprob = 0;
        
        for( int j = 0; j < m_EnemyGenQueue.size(); j++ ) {
            
            totalprob += m_EnemyGenQueue[j].m_Probability * 10.0f;
            
            if( totalprob > r )
                break;
            else
                genindex++;
            
        }
        
    }
    
    if( genindex >= m_EnemyGenQueue.size() )
        genindex = m_EnemyGenQueue.size() - 1;
    
    return m_EnemyGenQueue[genindex].m_Type;
    
}

CShipEntity * CTOFNContext::CreateRandomEnemyEntity( int type )
{
    
    int width, height;
    
    m_pGraphicsContext->GetWindowSize( &width, &height );
    
    CEnemyData & d = m_EnemyData[type];
    
    float x = Util::RandomNumber( 10, width - 100 );
    float y = Util::RandomNumber( -500, -100 );
    float speed =  Util::RandomNumber( d.m_SpeedMin, d.m_SpeedMax );
    
    return CreateEnemyEntity( type, x, y, speed );
    
}


CShipEntity * CTOFNContext::CreateRandomEnemyEntity()
{

    return CreateRandomEnemyEntity( GetRandomEnemyType() );

}

int CTOFNContext::CreateEnemyFormation( bool countasenemy, int enemytype, int formationtype ) {
 
    Vector2< float > offset;
    int spritewidth = 0, spriteheight = 0;
    int formationwidth = 0, formationheight = 0;
    
    std::vector< Vector2< float > > formationpos;
    std::vector< Vector2< float > > enemypos;
    
    CEnemyData & d = m_EnemyData[enemytype];
    
    if( !d.m_bCanBeInFormation ) {
     
        int num = 5;
        for( int i = 0; i < num; i++ ) {
            
            CShipEntity * e = CreateRandomEnemyEntity( enemytype );
            e->SetCountAsEnemy( countasenemy );
            
        }
        
        if( countasenemy )
            m_CurEnemyCount += num;
        
        return num;
        
    }
    
    float speed =  Util::RandomNumber( d.m_SpeedMin, d.m_SpeedMax );
    
    spritewidth = d.m_Width;
    spriteheight = d.m_Height;
    
    switch( formationtype ) {
            
        case 0:
            formationpos.push_back( Vector2< float >( 0.0f, 0.0f ) );
            formationpos.push_back( Vector2< float >( 1.0f, 1.0f ) );
            formationpos.push_back( Vector2< float >( 2.0f, 2.0f ) );
            formationpos.push_back( Vector2< float >( 3.0f, 1.0f ) );
            formationpos.push_back( Vector2< float >( 4.0f, 0.0f ) );
            break;
            
        case 1:
            formationpos.push_back( Vector2< float >( 0.0f, 1.5f ) );
            formationpos.push_back( Vector2< float >( 1.0f, 0.0f ) );
            formationpos.push_back( Vector2< float >( 2.0f, 1.5f ) );
            formationpos.push_back( Vector2< float >( 1.0f, 1.0f ) );
            formationpos.push_back( Vector2< float >( 1.0f, 2.0f ) );
            break;
            
        case 2:
            formationpos.push_back( Vector2< float >( 0.0f, 0.0f ) );
            formationpos.push_back( Vector2< float >( 1.5f, 0.0f ) );
            formationpos.push_back( Vector2< float >( 0.0f, 1.5f ) );
            formationpos.push_back( Vector2< float >( 1.5f, 1.5f ) );
            formationpos.push_back( Vector2< float >( .75f, 3.0f ) );
            break;
            
        case 3:
            formationpos.push_back( Vector2< float >( 0.0f, 0.0f ) );
            formationpos.push_back( Vector2< float >( 0.0f, 1.5f ) );
            formationpos.push_back( Vector2< float >( 0.0f, 3.0f ) );
            formationpos.push_back( Vector2< float >( 0.0f, 4.5f ) );
            formationpos.push_back( Vector2< float >( 0.0f, 6.0f ) );
            break;
            
    }
    
    for( int i = 0; i < formationpos.size(); i++ ) {
        
        float x = formationpos[i].GetX() * ( float )spritewidth;
        float y = formationpos[i].GetY() * ( float )spriteheight;
        
        enemypos.push_back( Vector2< float >( x, y ) );
        
        float x1 = x + spritewidth;
        float y1 = y + spriteheight;
        
        if( x1 > formationwidth )
            formationwidth = x1;
        
        if( y1 > formationheight )
            formationheight = y1;
        
    }
    
    float halfwidth = formationwidth * .5f;
    
    offset.Set( Util::RandomNumber( -halfwidth, SCREEN_WIDTH - halfwidth ),
               Util::RandomNumber( -formationheight * 2.5f, -formationheight * 3.0f ) );
    
    float flyingangle = 270.0f;
    float anglespeed = Util::RandomNumber( 175, 275 );
    
    for( int i = 0; i < enemypos.size(); i++ ) {
        
        CShipEntity * e = CreateEnemyEntity( enemytype, enemypos[i].GetX() + offset.GetX(), enemypos[i].GetY() + offset.GetY(), speed );
        e->SetCountAsEnemy( countasenemy );
        
        if( formationtype == 3 ) {
            
            CEnemyAI * ai = static_cast< CEnemyAI* >( e->GetAIController() );
            ai->SetEnemyType( enemytype );
            ai->SetFlyingAngle( flyingangle );
            ai->SetFlyingAngleSpeed( anglespeed );
            ai->SetCrazyFlying( true );
            
            float radius = ( 90.0f / anglespeed ) * Util::MinMaxF( speed, -300.0f, 300.0f );
            float flyoffset = cos( flyingangle * DEG2RAD ) * -radius;
            
            e->SetPos( e->GetX() + flyoffset, e->GetY() );
            
            flyingangle += 60.0f;
            
            
        }
        
    }
    
    int c = enemypos.size();
    
    if( countasenemy )
        m_CurEnemyCount += c;
    
    return c;

    
}

int CTOFNContext::CreateRandomEnemyFormation( bool countasenemy ) {
    
    int formationtype = Util::RandomNumber( 0, 3 );
    int enemytype = GetRandomEnemyType();
    
    return CreateEnemyFormation( countasenemy, enemytype, formationtype );
    
}

CShipEntity * CTOFNContext::CreateEnemyEntity( int type, float x, float y, float speed )
{

    CShipEntity * ent = new CShipEntity;
    CEnemyAI * aic = new CEnemyAI;

	CEnemyData & d = m_EnemyData[type];

    //What a hack
    if( ( int )speed == 0 ) {
     
        speed =  Util::RandomNumber( d.m_SpeedMin, d.m_SpeedMax );
        
    }
    
    if( type == 9 ) {
     
        x = 0.0f;
        y = -500.0f;
        
    }
    
    float wscale = 1.15f;
    float hscale = 1.15f;
    
    if( type == 9 ) {
     
        wscale = hscale = 1.0f;
        
    }
    
    float width = d.m_Width * wscale;
    float height = d.m_Height * hscale;
    
	ent->SetContext( this );
    ent->SetClassTypeID( ENTTYPE_ENEMY );
    ent->SetClassType( "EN" );
  //  ent->CreatePhysicsBody( m_PhysicsWorld.GetPhysicsWorld(), d.m_CollisionBoxWidth, d.m_CollisionBoxHeight );
    ent->SetMaterial( m_pTextureFactory->GetObjectContent( d.m_Sprite ) );
	ent->SetSize( width, height );
    //ent->DisablePhysicsMovement();
    //ent->SetGravity( 0 );
	ent->SetHealth( d.m_Health );
    ent->SetMaxHealth( d.m_Health );
    ent->SetArmor( 0.0f );
    ent->SetPos( x, y );
    ent->SetDrawDepth( 1 );
    ent->SetShipType( type );

    aic->SetEntityContext( this );
    
    CBoxCollisionBody * colBody = new CBoxCollisionBody;
    colBody->FitFromCenter( width, height, d.m_CollisionBoxWidth, d.m_CollisionBoxHeight );
    ent->SetCollisionBody( colBody );
    
    if( type != 5 && type != 7 )
        aic->SetSpeed( speed );
    else {
     
        float xvel = ( Util::RandomNumber( 0, 1 ) == 0 )? Util::RandomNumber( -speed, -speed * .3f ) : Util::RandomNumber( speed *.3f,  speed );
        
        aic->SetSpeed( xvel, speed );
    
    }
    
    if( type == 9 ) {
     
        float mul = 1.0f;
        
        if( HasUpgrade( 9 ) )
            mul += 1.0f;
        
        if( HasUpgrade( 8 ) )
            mul += 1.0f;
        else if( HasUpgrade( 3 ) )
            mul += .5f;
        
        ent->SetHealth( mul * d.m_Health );
        ent->SetMaxHealth( mul * d.m_Health );
        
    }
    
    aic->SetEnemyType( type );
    
    std::vector< Vector2< float > > & trails = d.m_TrailPos;
    
    if( trails.size() > 0 ) {
    
        ent->SetTrailsImage( TextureFactory()->GetObjectContent( "shiptrail.png" ) );
        
        for( int j = 0; j < trails.size(); j++ ) {
         
            ent->AddTrails( trails[j].GetX(),  trails[j].GetY() );
            ent->SetTrailColor( d.m_TrailColor[j].GetX(), d.m_TrailColor[j].GetY(), d.m_TrailColor[j].GetZ(), 1.0f );
            
        }
        
    }

	std::vector< Vector2< float > > & v = d.m_GunPos;

	for( int j = 0; j < v.size(); j++ )
	{

		ent->AddGun( v[j].GetX(), v[j].GetY(), d.m_GunDmg[j] );

	}

    aic->SetTargetEntity( ent );
    ent->SetAIController( aic );
    
    m_QuadTree.AddEntity( ent );
    
    m_pEntityManager->AddEntity( ent );
    m_pEntityManager->TrackEntity( "EN", ent );

    return ent;

}

void CTOFNContext::DoEnemyGenerator()
{
    
    
    if( m_EnemyGenQueue.size() < 1 )
        return;

    int n = ( m_MaxEnemyCount - m_CurEnemyCount );
    for( int j = 0; j < n; j++ )
    {

        bool createformation = false;

        
        if( m_CurEnemyCount < m_MaxEnemyCount - 3 )
            if( Util::RandomNumber( 1, 10 ) <= 3 )
                createformation = true;
        
        
        if( !createformation ) {
            
            CShipEntity * e = CreateRandomEnemyEntity();
            e->SetCountAsEnemy( true );
            m_CurEnemyCount++;
            
        } else
            j += CreateRandomEnemyFormation( true ) - 1;

    }
    

}

void CTOFNContext::AddPlayerPopupText( std::string text, float x, float y, float r, float g, float b ) {
 
    CTextPopup * t = new CTextPopup;
    t->SetText( text );
    t->SetPos( x, y );
    t->SetColor( r, g, b, 255.0f );
    t->SetFadeSpeed( 255.0f );
    t->SetFont( m_pFontFactory->GetFont( DEFAULT_FONT, 24 ) );
    m_pTextPopups.push_back( t );
    
}

void CTOFNContext::UpdateTextPopups() {
 
    boost::ptr_vector< CTextPopup >::iterator iter = m_pTextPopups.begin();
    
    for( ; iter != m_pTextPopups.end(); ) {
     
        if( ( *iter ).KillMe() )
            iter = m_pTextPopups.erase( iter );
        else {
            
            ( *iter ).Think( GetFrameDelta() );
            iter++;
            
        }
        
    }
    
}

void CTOFNContext::DrawTextPopups() {

    boost::ptr_vector< CTextPopup >::iterator iter = m_pTextPopups.begin();
    
    for( ; iter != m_pTextPopups.end(); iter++ ) {
        
        ( *iter ).Draw( m_pDrawContext );
        
    }
    
}

COrbEntity * CTOFNContext::CreateOrb( int type, float x, float y ) {
 
    COrbEntity * ent = new COrbEntity;
    COrbAI * aic = new COrbAI;
    
    std::string orbspr = "orb.png";
    
    if( type == 1 )
        orbspr = "orb2.png";
    
    ent->SetContext( this );
    aic->SetEntityContext( this );
    CTextureImage * m = m_pTextureFactory->GetObjectContent( orbspr );
    ent->SetClassTypeID( ENTTYPE_ORB );
    ent->SetDrawDepth( 2 );
    //ent->CreatePhysicsBody( m_PhysicsWorld.GetPhysicsWorld(), 20.0f, 20.0f );
    ent->SetMaterial( m);
    ent->SetSize( 15, 15 );
    //ent->DisablePhysicsMovement();
    //ent->SetGravity( 0 );
    ent->SetPos( x, y );
    ent->SetType( type );
    
    CBoxCollisionBody * colBody = new CBoxCollisionBody;
    colBody->SetBox( 0.0f, 0.0f, 20.0f, 20.0f );
    
    ent->SetCollisionBody( colBody );
    
    aic->SetTargetEntity( ent );
    ent->SetAIController( aic );
    
    m_pEntityManager->QueueEntity( ent );
    m_QuadTree.AddEntity( ent );
    
    return ent;
    
}

void CTOFNContext::CreateOrbs( int type, int count, float x, float y ) {
 
    for( int i = 0; i < count; i++ ) {
        
        COrbEntity * ent = CreateOrb( type, x, y );
        
    }
    
}

void CTOFNContext::DestroyShip( CShipEntity * e, bool quiet )
{

    if( !quiet )
    {

        const Vector2< float > & pos = e->GetPos();
        Vector2< int > size = e->GetSize();

        CreateExplosionsAndSmoke( 0, pos.GetX() + ( float )size.GetX() * .5f, pos.GetY() + ( float )size.GetY() * .5f );

    }

    m_pEntityManager->DeleteEntity( e );

    if( SDL_GetTicks() > m_NextEnemySpawn && Util::RandomNumber( 1, 5 ) < 3 )
        m_NextEnemySpawn = SDL_GetTicks() + Util::RandomNumber( 500, 1000 );
    
    if( e->CountAsEnemy() )
        m_CurEnemyCount--;

}

void CTOFNContext::FadeMusic( int fade, float volume, float volume2 ) {
 
    if( m_pCurMusChannel ) {
     
        unsigned long long clock;
        m_pCurMusChannel->getDSPClock( NULL, &clock );
        
        m_pCurMusChannel->addFadePoint( clock, volume );
        m_pCurMusChannel->addFadePoint( clock + fade, volume2 );
        
    }
    
}

void CTOFNContext::PauseMusic( bool b ) {

	if( m_pCurMusChannel ) 
		m_pCurMusChannel->setPaused( b );

}

void CTOFNContext::StopMusic() { 
 
    if( m_pCurMusChannel )
        m_pCurMusChannel->stop();
    
}

void CTOFNContext::PlayMusic( std::string dir, float volume ) {
    
    StopMusic();
    
    m_pCurMusChannel = SoundContext()->PlaySound( SoundFactory()->GetSound( dir ) );
    m_pCurMusChannel->setVolume( volume );
    
}


void CTOFNContext::PlayMusic( std::string dir ) {
 
    StopMusic();
    
    m_pCurMusChannel = SoundContext()->PlaySound( SoundFactory()->GetSound( dir ) );
    
}

void CTOFNContext::PlaySound( std::string dir, float vol ) {
    
    FMOD::Channel * c = SoundContext()->PlaySound( SoundFactory()->GetSound( dir ) );
    c->setVolume( vol );
    
}

void CTOFNContext::HandleEntityContact( void * pEntityA, int entTypeA, void * pEntityB, int entTypeB )
{

    //Due to the way our collision callback functions, entA is always a player ship or enemy ship
    CShipEntity * entA = static_cast< CShipEntity * >( pEntityA );

	CAIEntity * bulletEnt = NULL;
	CBulletAI * bulletAI = NULL;
	CShipEntity * shipEnt = NULL;
    CWorldEntity * explosionEnt = NULL;
    COrbEntity * orbEnt = NULL;

	bool playerInvolved = ( entTypeA & ( ENTTYPE_PLAYER ) );
	bool bulletCollision = ( entTypeB & ( ENTTYPE_PLYBULLET | ENTTYPE_ENBULLET ) );
    bool explosionCollision = ( entTypeB & ( ENTTYPE_EXPLOSION ) );
	bool playerBullet = ( entTypeB & ( ENTTYPE_PLYBULLET ) );
    bool orbCollision = ( entTypeB & ( ENTTYPE_ORB ) );
    
	if( bulletCollision )
	{

		bulletEnt = static_cast< CAIEntity * >( pEntityB );
		bulletAI = static_cast< CBulletAI * >( bulletEnt->GetAIController() );

    } else if( explosionCollision ) {
      
        explosionEnt = static_cast< CWorldEntity * >( pEntityB );
        
    } else if( orbCollision ) {
    
        orbEnt = static_cast< COrbEntity * >( pEntityB );
        
    } else
	{

		shipEnt = static_cast< CShipEntity * >( pEntityB );

	}
    
    if( !entA->IsActive() || ( bulletCollision && !bulletEnt->IsActive() ) || ( !orbCollision && !explosionCollision && !bulletCollision && !shipEnt->IsActive() ) || ( explosionCollision && !explosionEnt->IsActive() ) || ( orbCollision && !orbEnt->IsActive() ) )
        return;

	if( playerInvolved )
	{

		if( bulletCollision )
		{

			if( !playerBullet )
			{
                
                float mul = 1.0f;
                
                if( HasUpgrade( 10 ) )
                    mul = .75f;

                entA->SetLastHurtTime( SDL_GetTicks() );
                entA->Damage( bulletAI->GetDamage() * mul );
                m_pEntityManager->DeleteEntity( bulletEnt );


			}

        } else if( explosionCollision ) {
            
            entA->SetLastHurtTime( SDL_GetTicks() );
            entA->Damage( 1 );
            
        } else if( entTypeB & ENTTYPE_POWERUP )
		{




        } else if( orbCollision ) {
            
            int t = orbEnt->GetType();
            
            float mul = 1.0f;
            
            if( HasUpgrade( 7 ) )
                mul = 2.5f;
            else if( HasUpgrade( 5 ) )
                mul = 1.5f;
            
            
            FMOD::Channel * blt = SoundContext()->PlaySound( SoundFactory()->GetSound( "bullet.wav" ));
            blt->setPitch( 1.5f );
            blt->setVolume( .5f );
            
        
            if( t == 0 ) {
                
                float hp  = 7.0f * mul;
                entA->Heal( hp );
                
                char s[3];
                
                sprintf( s, "%d", ( int )hp );
                
                AddPlayerPopupText( std::string( s ), orbEnt->GetPos().GetX(), orbEnt->GetPos().GetY(), 128.0f, 255.0f, 128.0f );
                
                
                
            } else {
                
                int exp = Util::RandomNumber( 5, 12 ) * mul;
                m_PlayerEXP += exp;
                
                char s[3];
                
                sprintf( s, "%d", exp );
                
                AddPlayerPopupText( std::string( s ), orbEnt->GetPos().GetX(), orbEnt->GetPos().GetY(), 19.0f, 212.0f, 233.0f );

            
            }
            
            m_pEntityManager->DeleteEntity( orbEnt );
            
        } else
		{
            
            entA->SetLastHurtTime( SDL_GetTicks() );
            
            if( HasUpgrade( 6 ) )
                entA->Damage( 2 );
            else if( HasUpgrade( 2 ) )
                entA->Damage( 5 );
            else
                entA->Damage( 10 );
            
            if( shipEnt->GetShipType() != 9 ) {
                
                shipEnt->Damage( 30 );
                
                if( shipEnt->GetHealth() <= 0 )
                    m_PlayerKillCount++;
            
            }
            
		}

	} else if( bulletCollision )
    {
        
        if( playerBullet ) {
            
            float y = entA->GetY();
            int height = entA->GetSize().GetY();
            
            if( y > -height - 20 ) {

                entA->Damage( bulletAI->GetDamage() );
                m_pEntityManager->DeleteEntity( bulletEnt );
                
                if( entA->GetHealth() <= 0 )
                    m_PlayerKillCount++;

                return;
            
            }

        }
        
    } else if( explosionCollision ) {
     
        if( explosionEnt->GetClassType() == "XPL" )
            if( entA->GetPos().GetY() > -10.0f && entA->GetShipType() != 9 )
                entA->Damage( 1 );
        
    }

}

void CTOFNContext::CreateStarBackground()
{
    
    int winWidth, winHeight;

    m_pGraphicsContext->GetWindowSize( &winWidth, &winHeight );
    m_StarFieldSpeedMul = 1.0f;
    m_StarFieldSlowFillIndex = 0;

    for( int j = 0; j < MAX_STARS; j++ )
    {
        
        CStar * s = NULL;

        if( m_bCreatedStarField )
            s = &m_pStars.at( j );
        else
            s = new CStar;

        if( m_bStarFieldUpgradeSelect ) {

            s->m_X = winWidth * .5f;
            s->m_Y = winHeight * .5f;
            s->m_Angle = Util::RandomNumber( 0, 359 );
            
            if( !m_bStarFieldSlowFill ) {
                
                float dist = Util::RandomNumber( 0, 600 );
                
                s->m_X += std::cos( s->m_Angle * DEG2RAD ) * dist;
                s->m_Y += std::sin( s->m_Angle * DEG2RAD ) * dist;
                
            }
            
        } else {
            
            s->m_X = Util::RandomNumber( 0, winWidth );
            s->m_Y = Util::RandomNumber( -500, winHeight );
            s->m_Angle = 0.0f;

        }
                
		RandomizeStar( s );
        
        if( m_bStarFieldSlowFill )
            s->m_Speed = 0.0f;

        if( !m_bCreatedStarField )
            m_pStars.push_back( s );

    }
    
    if( m_bStarFieldSlowFill )
        m_StarFieldSlowFillNextTime = SDL_GetTicks() + 1;

    m_bCreatedStarField = true;
    
    
}

void CTOFNContext::RandomizeStar( CStar * s )
{

	s->m_Size = Util::RandomNumber( 1, 4 );

	int c = Util::RandomNumber( 0, 25 );

	s->m_R = 1.0f;
	s->m_G = 1.0f;
	s->m_B = 1.0f;
    s->m_A = ( float )Util::RandomNumber( 20, 180 ) / 255.0f;


	if( c < 7 )
	{

        s->m_G = Util::RandomNumber( 160, 255 ) / 255.0f;
        s->m_B = Util::RandomNumber( 160, 255 ) / 255.0f;

	} else if( c < 14 )
	{

        s->m_R = Util::RandomNumber( 160, 255 ) / 255.0f;
        s->m_G = Util::RandomNumber( 160, 255 ) / 255.0f;

    }
    
    if( m_bStarFieldUpgradeSelect && c < 17 ) {
     
        s->m_R = Util::RandomNumber( 100, 255 ) / 255.0f;
        s->m_G = Util::RandomNumber( 100, 255 ) / 255.0f;
        s->m_B = Util::RandomNumber( 100, 255 ) / 255.0f;
        s->m_A = Util::RandomNumber( 120, 255 ) / 255.0f;

        
    }

	s->m_Speed = Util::RandomNumber( 50, 200 );
    
    if( m_CurrentMission == 4 ) {
     
        if( m_bEnding )
            s->m_Speed = Util::RandomNumber( 90, 135 );
        else
            s->m_Speed = Util::RandomNumber( STAR_SPEED_FINALE, STAR_SPEED_FINALE * 1.5f );
        
    }

    if( m_bStarFieldUpgradeSelect )
        s->m_Speed *= 2.0f;
    
}

void CTOFNContext::AddEnemyToGenQueue( int i, float p ) {
 
    m_EnemyGenQueue.push_back( CEnemyGenQueue( i, p ) );
    m_MaxGenQueueProbability += p * 10.0f;
    
}

void CTOFNContext::ClearGenQueue() {
 
    m_EnemyGenQueue.clear();
    m_MaxGenQueueProbability = 0;
    
}

void CTOFNContext::UpdateAllEntities()
{

    boost::ptr_vector< CEntityObject > * entityObjs = &m_pEntityManager->GetEntityObjects();
    bIter = true;
	for( boost::ptr_vector< CEntityObject >::iterator i = entityObjs->begin();
		 i != entityObjs->end(); i++ )
    {

        if( ( *i ).GetContent()->IsActive() )
        {

            ( *i ).GetContent()->Update();

            int type = ( *i ).GetContent()->GetClassTypeID();

            if( type & ( ENTTYPE_PLAYER | ENTTYPE_ENEMY ) )
            {

                CShipEntity * pShip = static_cast< CShipEntity * >( ( *i ).GetContent() );
                
                if( type & ENTTYPE_PLAYER ) {
                 
                    if( pShip->GetLastHurtTime() > 0 ) {
                        
                        if( pShip->GetLastHurtTime() + 150 > SDL_GetTicks() ) {
                         
                            pShip->SetColor( 1.0f, 0.0f, 0.0f, 1.0f );
                            
                        } else {
                            
                            pShip->SetColor( 1.0f, 1.0f, 1.0f, 1.0f );
                            pShip->SetLastHurtTime( -1 );
                            
                        }
                        
                    }
                    
                }

                if( pShip->GetHealth() <= 0 )
                {
                    
                    if( pShip->GetShipType() != 9 ) {
                        
                        bool destroy = true;
                        
                        if( type & ENTTYPE_ENEMY ) {
                        
                            int t = ( pShip->GetShipType() == 6 )? 0 : Util::RandomNumber( 0, 1 );
                            int count = ( pShip->GetShipType() == 6 )? 45 : Util::RandomNumber( 3, 6 );
                            
                            CreateOrbs( t, count, pShip->GetX(), pShip->GetY() );
                        
                        }
                        
                        if( type & ENTTYPE_PLAYER && m_bPlayerInvincible ) {
                         
                            destroy = false;
                            
                        }
                        
                        if( destroy ) {
        
                            DestroyShip( pShip, false );
                            
                            if( type & ENTTYPE_PLAYER )
                                m_pPlayerEntity = NULL;
                            
                        }
                        
                    } else {
                     
                        pShip->SetAIEnabled( false );
                        m_bPlayerInvincible = true;
                    }

                }

                if( pShip->GetPos().GetY() > m_pGraphicsContext->GetWindowHeight() )
                {

                    DestroyShip( pShip, true );

                }

            } else if( type & ( ENTTYPE_ENBULLET | ENTTYPE_PLYBULLET ) )
			{

				CAIEntity * pBullet = static_cast< CAIEntity * >( ( *i ).GetContent() );

				if( ( pBullet->GetPos().GetY() < 0 && type & ENTTYPE_PLYBULLET ) ||
                    ( pBullet->GetPos().GetY() < m_pGraphicsContext->GetWindowHeight() * -.5f && type & ENTTYPE_ENBULLET ) ||
                    pBullet->GetPos().GetY() > m_pGraphicsContext->GetWindowHeight() )
					m_pEntityManager->DeleteEntity( pBullet );

			}

        }

    }
    bIter = false;
    for( int j = 0; j < bulletvec.size(); j++ ) {
     
        m_QuadTree.AddEntity( ( CWorldEntity * )bulletvec[j] );
        m_pEntityManager->AddEntity( bulletvec[j] );
        
    }
    
    bulletvec.clear();

}

void CTOFNContext::DrawStarBackground()
{

    m_pTextureFactory->GetObjectContent( "star.png" )->Bind();

	int width, height;

	m_pGraphicsContext->GetWindowSize( &width, &height );

    if( !m_bGameFrozen ) {
        
        if( m_bStarFieldSlowFill ) {
            
            if( m_StarFieldSlowFillIndex < MAX_STARS ) {
                
                if( SDL_GetTicks() > m_StarFieldSlowFillNextTime ) {
                 
                    for( int j = 0; j < 3; j++ ) {
                        if( m_StarFieldSlowFillIndex < MAX_STARS ) {
                            
                            CStar * s = &m_pStars[m_StarFieldSlowFillIndex++];
                            s->m_Speed = Util::RandomNumber( 400, 600 );
                            
                        }
                    }
                    m_StarFieldSlowFillNextTime = SDL_GetTicks() + 1;
                    
                }
                
            }
            
        }

        
        int n = 0;
        for( ; n < MAX_STARS; n++ )
        {
            
            CStar * s = &m_pStars[n];
            
            m_StarEngine.SetParticleSize( n, s->m_Size, s->m_Size );
            m_StarEngine.SetParticleColor( n, s->m_R, s->m_G, s->m_B, s->m_A );
            m_StarEngine.SetParticlePos( n, s->m_X, s->m_Y );

            //We'll update in the same loop to save some cpu cycles
            
            if( m_bStarFieldUpgradeSelect ) {
            
                //s->m_Size += s->m_Speed * m_FrameDelta * .2f;
                s->m_X += std::cos( s->m_Angle * DEG2RAD ) * s->m_Speed * m_FrameDelta * m_StarFieldSpeedMul;
                s->m_Y += std::sin( s->m_Angle * DEG2RAD ) * s->m_Speed * m_FrameDelta * m_StarFieldSpeedMul;
                
            } else
                s->m_Y += s->m_Speed * m_FrameDelta * m_StarFieldSpeedMul;
            
            
            if( m_bStarFieldUpgradeSelect ) {
             
                if( s->m_Y > SCREEN_HEIGHT ||
                    s->m_X < 0.0f ||
                    s->m_X > SCREEN_WIDTH ||
                    s->m_Y < 0.0f )
                {
                    
                    s->m_X = SCREEN_WIDTH * .5f;
                    s->m_Y = SCREEN_HEIGHT * .5f;
                    s->m_Angle = Util::RandomNumber( 0, 359 );
                    
                    RandomizeStar( s );
                    
                }
                
            } else {
             
                if( s->m_Y > SCREEN_HEIGHT )
                {
                    
                    s->m_X = Util::RandomNumber( 0, width );
                    s->m_Y = Util::RandomNumber( -height, -5 );
                    
                    RandomizeStar( s );
                    
                }
                
            }


        }
    
    }
    
    m_StarEngine.BindVertexBuffers();

    m_pDrawContext->UseShaderProgram( m_pGraphicsContext->GetShaderIDFromIndex( 1 ) );
    
    m_StarEngine.Draw();
    
	m_pDrawContext->UseShaderProgram( m_pGraphicsContext->GetShaderIDFromIndex( 0 ) );
    
    
    m_pDrawContext->Bind2DVertexBuffer();

}

void CTOFNContext::DebugDrawQuadTree() {
    
    m_pDrawContext->SetTexture( GetTexture( "pixel.png" )->GetFrame( 0 ).GetTexture() );
    m_QuadTree.Draw( m_pGraphicsContext->GetDrawContext() );
    
}


void CTOFNContext::DrawSmoke() {
    
    m_pTextureFactory->GetObjectContent( "smoke.png" )->Bind();
    
    m_pDrawContext->UseShaderProgram( m_pGraphicsContext->GetShaderIDFromIndex( 1 ) );
    
    
    boost::ptr_vector< CSmokePlume >::iterator iter = m_pSmokePlumes.begin();
    for( ; iter != m_pSmokePlumes.end(); iter++ ) {
        
        ( *iter ).Draw();
    
    }
    
    m_pDrawContext->UseShaderProgram( m_pGraphicsContext->GetShaderIDFromIndex( 0 ) );
    
    m_pDrawContext->Bind2DVertexBuffer();
    
    
}


void CTOFNContext::DrawExplosions() {
    
    m_pDrawContext->UseShaderProgram( m_pGraphicsContext->GetShaderIDFromIndex( 4 ) );
    
    m_pTextureFactory->GetObjectContent( "pixel.png" )->Bind();
    
    boost::ptr_vector< CParticleExplosion >::iterator iter2 = m_pExplosions.begin();
    for( ; iter2 != m_pExplosions.end(); iter2++ ) {
        
        ( *iter2 ).Draw();
    }
    
    m_pDrawContext->UseShaderProgram( m_pGraphicsContext->GetShaderIDFromIndex( 0 ) );
    
    m_pDrawContext->Bind2DVertexBuffer();

    
}

void CTOFNContext::UpdateExplosions() {

    boost::ptr_vector< CSmokePlume >::iterator iter = m_pSmokePlumes.begin();
    
    for( ;iter != m_pSmokePlumes.end(); ) {
        
        if( ( *iter ).KillMe() ) {
            iter = m_pSmokePlumes.erase( iter );
        } else {
            
            ( *iter ).Think( GetFrameDelta() );
            iter++;
            
        }
    }
    
    boost::ptr_vector< CParticleExplosion >::iterator iter2 = m_pExplosions.begin();
    
    for( ;iter2 != m_pExplosions.end(); ) {

        if( ( *iter2 ).KillMe() ) {
            m_pEntityManager->DeleteEntity( ( *iter2 ).GetExplosionEntity() );
            iter2 = m_pExplosions.erase( iter2 );
        } else {
         
            ( *iter2 ).Think( GetFrameDelta() );
            iter2++;
            
        }
    }
    
}

void CTOFNContext::DrawFunkyBackground1() {
    
    static float funk = 0.0f;
    static float theta = 0.0f;
    static float thetaMul = 1.0f;
    
    funk = Util::MaxF( funk, 20.0f * GetFrameDelta(), 300.0f );
    
    
    theta += 50.0f * thetaMul * GetFrameDelta();
    
    if( theta > 360.0f ) {
        
        theta = 360.0f;
        thetaMul = -1.0f;
        
    }
    
    if( theta < -360.0f ) {
        
        theta = -360.0f;
        thetaMul = 1.0f;
        
    }
    
    
    DrawFunkyBackground( 6, funk, theta, thetaMul );
    
}

void CTOFNContext::DrawFunkyBackground2() {
 
    static float funk = 0.0f;
    static float theta = 0.0f;
    static float thetaMul = 1.0f;
    
    funk = Util::MaxF( funk, 40.0f * GetFrameDelta(), 255.0f );
    
    
    theta += 50.0f * thetaMul * GetFrameDelta();
    
    if( theta > 360.0f )
        theta -= 360.0f;
    
    DrawFunkyBackground( 7, funk, theta, thetaMul );
    
}

void CTOFNContext::DrawSpaceFog( GLuint explosionTex ) {
 
    if( m_pSpaceFogFBO ) {
     
        static float fogR = .8f, fogG = .8f, fogB = .8f;
        static float stadd = 0.0f;
        float stalpha = .35f;
        
        if( !m_bGameFrozen ) {
            
            float speed = .25f;
            
            if( m_CurrentMission == 4 && !m_bEnding )
                speed = FOG_STSPEED_FINALE;
            
            stadd += speed * GetFrameDelta();
            
        }
        
        if( m_CurrentMission == 4 && !m_bEnding ) {
         
            static float alphaWave = 0.0f;
            static float rgbWave = 0.0f;
            
            rgbWave += 5.0f * m_FrameDelta;
            alphaWave += 1.0f * m_FrameDelta;
            
            if( alphaWave > 360.0f )
                alphaWave -= 360.0f;
            
            stalpha = .33f + .05f * cos( alphaWave );
        
    
            fogR = fabs( cos( rgbWave ) );
            fogG = 1.0f - fabs( cos( rgbWave ) );
            fogB = fabs( sin( rgbWave ) );
            
            
            
        }
        
        if( stadd > 1.0f ) {
         
            stadd -= 1.0f;
            
        }

        
        DrawContext()->UseShaderProgram( GraphicsContext()->GetShaderIDFromIndex( 9 ) );
        
        glActiveTexture( GL_TEXTURE1 );
        glBindTexture( GL_TEXTURE_2D, explosionTex );
        int l = glGetUniformLocation( GraphicsContext()->GetShaderIDFromIndex( 9 ), "texUnit2" );
        glUniform1i( l, 1 );
        
        glActiveTexture( GL_TEXTURE0 );
     
        int f = glGetUniformLocation( GraphicsContext()->GetShaderIDFromIndex( 9 ), "STAdd" );
        glUniform1f( f, stadd );
        CMatrix< float > mat;
        mat.Identity();
        mat.Translate( 0.0f, SCREEN_HEIGHT, 0.0f );
        mat.Scale( SCREEN_WIDTH, -SCREEN_HEIGHT, 1.0f );
        
        DrawContext()->SetDrawColor( fogR, fogB, fogB, stalpha );
        
        DrawContext()->Bind2DVertexArray();
        m_pSpaceFogFBO->DrawTextureDontForceSize( DrawContext(), &mat );
        DrawContext()->UseShaderProgram( GraphicsContext()->GetShaderIDFromIndex( 0 ) );
        

        
    }
    
}

void CTOFNContext::DrawFunkyBackground( int s, float & funk, float & theta, float & thetaMul ) {
    
    DrawContext()->UseShaderProgram( GraphicsContext()->GetShaderIDFromIndex( s ) );
    DrawContext()->Bind2DVertexBuffer();

    glActiveTexture( GL_TEXTURE1 );
    
    int t = glGetUniformLocation( GraphicsContext()->GetShaderIDFromIndex( s ), "noiseImage" );
    glUniform1i( t, 1 );
    
    t = glGetUniformLocation( GraphicsContext()->GetShaderIDFromIndex( s ), "scientistImage" );
    glUniform1i( t, 2 );
    
    t = glGetUniformLocation(GraphicsContext()->GetShaderIDFromIndex( s ), "funkFactor" );
    glUniform1f( t, funk );
    
    t = glGetUniformLocation( GraphicsContext()->GetShaderIDFromIndex( s ), "theta" );
    glUniform1f( t, theta * DEG2RAD );
    
    m_pFunkyBGFBO->BindTexture();
    
    glActiveTexture( GL_TEXTURE2 );
    CTextureImage * img = TextureFactory()->GetObjectContent( "scientistface2.png" );
    img->Bind();
    
    glActiveTexture( GL_TEXTURE0 );
    
    DrawContext()->DrawMaterial( *m_pPixelMat, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f, 1.0f, 1.0f, 1.0f );
    
    DrawContext()->UseShaderProgram( GraphicsContext()->GetShaderIDFromIndex( 0 ) );
    DrawContext()->Bind2DVertexBuffer();
    
    
}

void CTOFNContext::GameLogic()
{

    if( !m_bGameFrozen ) {
    
     //   m_PhysicsWorld.Update();
        
        SoundContext()->Update();
        
        m_CollisionEngine.CheckForQuadTreeCollisions( &m_QuadTree, &m_CollisionCallback );

        UpdateExplosions();
        
        UpdateAllEntities();

        UpdateTextPopups();
        
        DoEnemyGenerator();
    
    }
    
    m_QuadTree.Think();
        
    if( m_pPlayerEntity )
        m_Lua.CallEngineFunction( "GameLogic" );
    
    //In normal operations, entities should be "deleted", when this happens they aren't actually removed from the entity manager until after all update logic is finished.
    m_pEntityManager->RemoveAllDeletedEntities();
    
    m_pEntityManager->AddAllQueuedEntities();

}
