#include "GameContext.h"

CTOFNContext::CTOFNContext() : CLuaContext(), m_pPlayerEntity( NULL ), m_MaxEnemyCount( 3 ), m_CurEnemyCount( 0 )
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

		glEnableVertexAttribArray( j + 1 );
		glVertexAttribPointer( j + 1, 4, GL_FLOAT, GL_FALSE, sizeof( float ) * 16, ( const GLvoid * )( sizeof( float ) * 4 * j ) );
		glVertexAttribDivisor( j + 1, 1 );

	}

    glGenBuffers( 1, &m_InstancedRGBABuffer );
	glBindBuffer( GL_ARRAY_BUFFER, m_InstancedRGBABuffer );

	glEnableVertexAttribArray( 5 );
	glVertexAttribPointer( 5, 4, GL_FLOAT, GL_FALSE, 0, ( void * )0 );
	glVertexAttribDivisor( 5, 1 );

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

    JsonObject jsonData;
    jsonData.Open( "enemy.txt" );

    rapidjson::Document & doc = jsonData.GetDocument();
    int nEnemy = doc.Size();

    for( int j = 0; j < nEnemy; j++ )
    {

        int health = doc[j]["health"].GetInt();
        int speedmin = doc[j]["speedmin"].GetInt();
        int speedmax = doc[j]["speedmax"].GetInt();
        int width = doc[j]["size"][0];
        int height = doc[j]["size"][1];

        std::string sprite = doc[j]["sprite"].GetString();

        int nGun = doc[j]["gun"].GetSize();
        for( int i = 0; i < nGun; i++ )
        {

            rapidjson::Value & v = doc[j]["gun"][i];
            //AddGun( v.["x"].GetDouble(), v.["y"].GetDouble() );

        }

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

    m_pEntityManager->AddEntity( ent );

    return ent;

}

CShipEntity * CTOFNContext::CreateRandomEnemyEntity()
{

    CShipEntity * ent = new CShipEntity;
    CEnemyAI * aic = new CEnemyAI;

	ent->SetContext( this );
    ent->SetClassTypeID( ENTTYPE_ENEMY );
    ent->SetClassType( "EN" );
    ent->CreatePhysicsBody( m_PhysicsWorld.GetPhysicsWorld(), 70, 70 );
    ent->SetMaterial( m_pTextureFactory->GetObjectContent( "Enemy.png" ) );
	ent->SetSize( 70, 70 );
    ent->DisablePhysicsMovement();
    ent->SetGravity( 0 );
	ent->SetHealth( 10 );
    ent->SetPos( Util::RandomNumber( 100, 810 ), Util::RandomNumber( -500, -100 ) );

    aic->SetTargetEntity( ent );
    ent->SetAIController( aic );

    m_pEntityManager->AddEntity( ent );
    m_pEntityManager->TrackEntity( "EN", ent );

    m_CurEnemyCount++;

    return ent;

}

void CTOFNContext::DoEnemyGenerator()
{

    int n = ( m_MaxEnemyCount - m_CurEnemyCount );
    for( int j = 0; j < n; j++ )
    {

        CreateRandomEnemyEntity();

    }

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

                if( pShip->GetHealth() <= 0 || pShip->GetPos().GetY() > 650 )
                {

                    m_pEntityManager->DeleteEntity( pShip );
					m_CurEnemyCount--;

                }

            } else if( type & ( ENTTYPE_ENBULLET | ENTTYPE_PLYBULLET ) )
			{

				CAIEntity * pBullet = static_cast< CAIEntity * >( ( *i ).GetContent() );

				if( pBullet->GetPos().GetY() > 600 || pBullet->GetPos().GetY() < 0 )
					m_pEntityManager->DeleteEntity( pBullet );

			}

        }

    }

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
