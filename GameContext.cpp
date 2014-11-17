#include "GameContext.h"


#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

CTOFNContext::CTOFNContext() : CLuaContext(), m_pPlayerEntity( NULL ), m_MaxEnemyCount( 30 ), m_CurEnemyCount( 0 )
{

}

void CTOFNContext::InitializePhysicsWorld()
{

    m_PhysicsWorld.Initialize( 0.0f, 0.0f );
    m_CollisionCallback.SetContext( this );
    m_PhysicsWorld.SetContactListener( &m_CollisionCallback );

}

void CTOFNContext::InitializeLua()
{

    m_Lua.Initialize();
    SetLuaContext( this );

}


void CTOFNContext::InitializeGraphics()
{

    int id = m_pGraphicsContext->LoadShaderProgram( "star.v", "star.f" );
    int width, height;

     m_pGraphicsContext->GetWindowSize( &width, &height );
    
    glGenBuffers( 1, &m_InstancedBuffer );
    
	glBindBuffer( GL_ARRAY_BUFFER, m_InstancedBuffer );
 
	for( int j = 0; j < 4; j++ )
	{
		glEnableVertexAttribArray( j + 2 );
        
		glVertexAttribPointer( j + 2, 4, GL_FLOAT, GL_FALSE, sizeof( float ) * 16, ( const GLvoid * )( sizeof( float ) * 4 * j ) );
		
        glVertexAttribDivisor( j + 2, 1 );

	}

    glGenBuffers( 1, &m_InstancedRGBABuffer );
	glBindBuffer( GL_ARRAY_BUFFER, m_InstancedRGBABuffer );

	glEnableVertexAttribArray( 6 );
	glVertexAttribPointer( 6, 4, GL_FLOAT, GL_FALSE, 0, ( void * )0 );
	glVertexAttribDivisor( 6, 1 );

    for( int j = 1; j >= 0; j-- )
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

		if( doc[j].HasMember( "gun" ) )
		{

			rapidjson::Value & gunarray = doc[j]["gun"];

			if( gunarray.IsArray() )
			{

				rapidjson::SizeType nGun = doc[j]["gun"].Size();
				for( rapidjson::SizeType i = 0; i < nGun; i++ )
				{

					rapidjson::Value & v = doc[j]["gun"][i];
					d.m_GunPos.push_back( Vector2< float >( v["x"].GetDouble(), v["y"].GetDouble() ) );

				}

			}

		}

		m_EnemyData.push_back( d );

    }

    Log::Debug( "Done reading enemy data" );

}

CShipEntity * CTOFNContext::CreatePlayerEntity()
{

    static const float defaultPlayerPosX = 350.0f;
    static const float defaultPlayerPosY = 500.0f;

	Log::Debug( "Creating player entity" );

    CShipEntity * ent = new CShipEntity;
	ent->SetContext( this );
    ent->SetClassTypeID( ENTTYPE_PLAYER );
	ent->CreatePhysicsBody( m_PhysicsWorld.GetPhysicsWorld(), 64, 64 );
    ent->SetMaterial( m_pTextureFactory->GetObjectContent( "player2.png" ) );
	ent->SetSize( 80, 80 );
    ent->DisablePhysicsMovement();
    ent->SetGravity( 0 );
    ent->SetPos( defaultPlayerPosX, defaultPlayerPosY );
    ent->SetDrawDepth( 1 );

    ent->AddGun( .3f, .23f );
	ent->AddGun( .7f, .23f );

    if( m_pPlayerEntity )
        m_pEntityManager->RemoveEntity( m_pPlayerEntity );

    m_pPlayerEntity = ent;

    m_pEntityManager->AddEntity( m_pPlayerEntity );

	Log::Debug( "Created player entity" );

    return m_pPlayerEntity;

}

void CTOFNContext::FireBulletFrom( int type, CShipEntity * pShip, int dmg, float speed )
{

    const std::vector< Vector3< float > > & GunPos = pShip->GetGunPositions();
    float pX, pY;

    pShip->GetPos().Get( &pX, &pY );

    for( int j = 0; j < GunPos.size(); j++ )
    {

        float x, y;
        GunPos[j].Get( &x, &y );

        FireBulletFrom( type, pX + x, pY + y, dmg, speed );

    }

}

CAnimEntity * CTOFNContext::CreateExplosion( int type, float x, float y )
{
    /*

    CAnimEntity * ent = new CAnimEntity;

	ent->SetContext( this );
    ent->SetClassTypeID( ENTTYPE_EXPLOSION );
    ent->SetClassType( "XPL" );

    std::string sprite = "explosion.png";

    switch( type )
    {
    case 1:
        //sprite = "";
        break;

    }

    ent->SetMaterial( m_pTextureFactory->GetObjectContent( sprite ) );

    ent->SetPos( x, y );
    ent->SetAnimType( ANIMTYPE_KILLONEND );

    ent->SetDrawDepth( 5 );

    m_pEntityManager->AddEntity( ent );
    
    return ent; */
    
    return NULL;

}

CAIEntity * CTOFNContext::FireBulletFrom( int type, float x, float y, int dmg, float speed )
{

    CAIEntity * ent = new CAIEntity;
    CBulletAI * aic = new CBulletAI;

    aic->SetSpeed( speed );
    aic->SetDamage( dmg );
    aic->SetDirection( ( type & ENTTYPE_ENBULLET )? DIR_DOWN : DIR_UP );

	ent->SetContext( this );
    ent->SetClassTypeID( type );
    ent->SetClassType( "BLT" );
	ent->CreatePhysicsBody( m_PhysicsWorld.GetPhysicsWorld(), 5, 5 );
    ent->SetMaterial( m_pTextureFactory->GetObjectContent( "bullet.png" ) );
    ent->DisablePhysicsMovement();
    ent->SetGravity( 0 );
    ent->SetPos( x, y );

    aic->SetTargetEntity( ent );
    ent->SetAIController( aic );
    
    if( bIter )
        bulletvec.push_back( ent );
    else
    m_pEntityManager->AddEntity( ent );

    return ent;

}

CShipEntity * CTOFNContext::CreateRandomEnemyEntity()
{

	int width, height;

	m_pGraphicsContext->GetWindowSize( &width, &height );

	int type = Util::RandomNumber( 0, m_EnemyData.size() - 1 );
	float x = Util::RandomNumber( 10, width - 100 );
	float y = Util::RandomNumber( -500, -100 );

	return CreateEnemyEntity( type, x, y );

}

CShipEntity * CTOFNContext::CreateEnemyEntity( int type, float x, float y )
{

    CShipEntity * ent = new CShipEntity;
    CEnemyAI * aic = new CEnemyAI;

	CEnemyData & d = m_EnemyData[type];

	ent->SetContext( this );
    ent->SetClassTypeID( ENTTYPE_ENEMY );
    ent->SetClassType( "EN" );
    ent->CreatePhysicsBody( m_PhysicsWorld.GetPhysicsWorld(), d.m_CollisionBoxWidth, d.m_CollisionBoxHeight );
    ent->SetMaterial( m_pTextureFactory->GetObjectContent( d.m_Sprite ) );
	ent->SetSize( d.m_Width, d.m_Height );
    ent->DisablePhysicsMovement();
    ent->SetGravity( 0 );
	ent->SetHealth( d.m_Health );
    ent->SetPos( x, y );
    ent->SetDrawDepth( 1 );

    aic->SetEntityContext( this );
	aic->SetSpeed( Util::RandomNumber( d.m_SpeedMin, d.m_SpeedMax ) );

	std::vector< Vector2< float > > & v = d.m_GunPos;

	for( int j = 0; j < v.size(); j++ )
	{

		ent->AddGun( v[j].GetX(), v[j].GetY() );

	}

    aic->SetTargetEntity( ent );
    ent->SetAIController( aic );

    m_pEntityManager->AddEntity( ent );
    m_pEntityManager->TrackEntity( "EN", ent );

    return ent;

}

void CTOFNContext::DoEnemyGenerator()
{

    int n = ( m_MaxEnemyCount - m_CurEnemyCount );
    for( int j = 0; j < n; j++ )
    {

        CShipEntity * e = CreateRandomEnemyEntity();
        e->SetCountAsEnemy( true );

    }

    m_CurEnemyCount = m_MaxEnemyCount;

}

void CTOFNContext::DestroyShip( CShipEntity * e, bool quiet )
{

    if( !quiet )
    {

        const Vector2< float > & pos = e->GetPos();

        CreateExplosion( 0, pos.GetX(), pos.GetY() );

    }

    m_pEntityManager->DeleteEntity( e );

    if( e->CountAsEnemy() )
        m_CurEnemyCount--;

}

void CTOFNContext::HandleEntityContact( void * pEntityA, int entTypeA, void * pEntityB, int entTypeB )
{

    //Due to the way our collision callback functions, entA is always a player ship or enemy ship
    CShipEntity * entA = static_cast< CShipEntity * >( pEntityA );

	CAIEntity * bulletEnt = NULL;
	CBulletAI * bulletAI = NULL;
	CShipEntity * shipEnt = NULL;

	bool playerInvolved = ( entTypeA & ( ENTTYPE_PLAYER ) );
	bool bulletCollision = ( entTypeB & ( ENTTYPE_PLYBULLET | ENTTYPE_PLYBULLET ) );
	bool playerBullet = ( entTypeB & ( ENTTYPE_PLYBULLET ) );

	if( bulletCollision )
	{

		bulletEnt = static_cast< CAIEntity * >( pEntityB );
		bulletAI = static_cast< CBulletAI * >( bulletEnt->GetAIController() );

	} else
	{

		shipEnt = static_cast< CShipEntity * >( pEntityB );

	}
    
    if( !entA->IsActive() || ( bulletCollision && !bulletEnt->IsActive() ) || ( !bulletCollision && !shipEnt->IsActive() ) )
        return;

	if( playerInvolved )
	{

		if( bulletCollision )
		{

			if( !playerBullet )
			{

				entA->Damage( bulletAI->GetDamage() );
				m_pEntityManager->DeleteEntity( bulletEnt );

			}

		} else if( entTypeB & ENTTYPE_POWERUP )
		{




		} else
		{

		}

	} else if( bulletCollision )
    {

        entA->Damage( bulletAI->GetDamage() );
        m_pEntityManager->DeleteEntity( bulletEnt );

		return;

    }

}

void CTOFNContext::CreateStarBackground()
{

    int winWidth, winHeight;

    m_pGraphicsContext->GetWindowSize( &winWidth, &winHeight );

    for( int j = 0; j < MAX_STARS; j++ )
    {

        CStar * s = new CStar;

        s->m_X = Util::RandomNumber( 0, winWidth );
        s->m_Y = Util::RandomNumber( -500, winHeight );

		RandomizeStar( s );

        m_pStars.push_back( s );

    }

}

void CTOFNContext::RandomizeStar( CStar * s )
{

	s->m_Size = Util::RandomNumber( 1, 3 );

	int c = Util::RandomNumber( 0, 25 );

	s->m_R = 1.0f;
	s->m_G = 1.0f;
	s->m_B = 1.0f;

	if( c == 5 )
	{

		s->m_G = 0.7f;
		s->m_B = 0.7f;

	} else if( c == 12 )
	{

		s->m_R = 0.7f;
		s->m_G = 0.7f;

	}

	s->m_A = ( float )Util::RandomNumber( 20, 180 ) / 255.0f;

	s->m_Speed = Util::RandomNumber( 50, 200 );

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

                if( pShip->GetHealth() <= 0 )
                {

                    DestroyShip( pShip, false );

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
     
        m_pEntityManager->AddEntity( bulletvec[j] );
        
    }
    
    bulletvec.clear();

}

void CTOFNContext::DrawStarBackground()
{

    m_pTextureFactory->GetObjectContent( "star.png" )->Bind();

	int width, height;

	m_pGraphicsContext->GetWindowSize( &width, &height );

	float starMat[MAX_STARS][16];
    float starColor[MAX_STARS][4];
    int n = 0;

    for( ; n < MAX_STARS; n++ )
    {

        CStar * s = &m_pStars[n];

		CMatrix< float > mat;
		mat.Identity();
        mat.SetTranslate( s->m_X, s->m_Y, 0.0f );
		mat.Scale( s->m_Size, s->m_Size, 1.0f );

		float * rawStarMat = mat.GetRawMatrix();
		std::copy( rawStarMat, rawStarMat + 16, starMat[n] );

        starColor[n][0] = s->m_R;
		starColor[n][1] = s->m_G;
		starColor[n][2] = s->m_B;
		starColor[n][3] = s->m_A;

        //We'll update in the same loop to save some cpu cycles
        s->m_Y += s->m_Speed * m_FrameDelta;

        if( s->m_Y > 800.0f )
        {

            s->m_X = Util::RandomNumber( 0, width );
            s->m_Y = Util::RandomNumber( -height, -5 );

			RandomizeStar( s );

        }


    }

    glBindBuffer( GL_ARRAY_BUFFER, m_InstancedBuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * n * 16, starMat, GL_DYNAMIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, m_InstancedRGBABuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * n * 4, starColor, GL_DYNAMIC_DRAW );

    m_pDrawContext->UseShaderProgram( m_pGraphicsContext->GetShaderIDFromIndex( 1 ) );
    
    glDrawElementsInstancedBaseVertex( GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_SHORT, ( void * )0, n, 0 );

	m_pDrawContext->UseShaderProgram( m_pGraphicsContext->GetShaderIDFromIndex( 0 ) );
    

}

void CTOFNContext::GameLogic()
{

    m_PhysicsWorld.Update();

    UpdateAllEntities();

    DoEnemyGenerator();

    m_Lua.CallEngineFunction( "GameLogic" );

    //In normal operations, entities should be "deleted", when this happens they aren't actually removed from the entity manager until after all update logic is finished.
    m_pEntityManager->RemoveAllDeletedEntities();

}
