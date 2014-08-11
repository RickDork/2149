#include "GameContext.h"

CTOFNContext::CTOFNContext() : CLuaContext(), m_pPlayerEntity( NULL ), m_MaxEnemyCount( 10 ), m_CurEnemyCount( 0 ),
                               m_MaxStars( 1000 )
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
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, ( void * )0 );
	glVertexAttribDivisor( 1, 1 );

    glGenBuffers( 1, &m_InstancedRGBABuffer );
	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, 0, ( void * )0 );
	glVertexAttribDivisor( 2, 1 );

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

CShipEntity * CTOFNContext::CreatePlayerEntity()
{

    static const float defaultPlayerPosX = 350.0f;
    static const float defaultPlayerPosY = 500.0f;

	Log::Debug( "Creating player entity" );

    CShipEntity * ent = new CShipEntity;
	ent->SetContext( this );
    ent->SetClassTypeID( ENTTYPE_PLAYER );
	ent->CreatePhysicsBody( m_PhysicsWorld.GetPhysicsWorld(), 64, 64 );
    ent->SetMaterial( m_pTextureFactory->GetObjectContent( "PLAYERSPRITE.png" ) );
    ent->DisablePhysicsMovement();
    ent->SetGravity( 0 );
    ent->SetPos( defaultPlayerPosX, defaultPlayerPosY );

    ent->AddGun( 32.0f, 12.0f );

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

    ent->SetClassTypeID( type );
    ent->SetClassType( "BLT" );
	ent->CreatePhysicsBody( m_PhysicsWorld.GetPhysicsWorld(), 5, 5 );
    ent->SetMaterial( m_pTextureFactory->GetObjectContent( "BULLET.png" ) );
    ent->DisablePhysicsMovement();
    ent->SetGravity( 0 );
    ent->SetPos( x, y );

    aic->SetTargetEntity( ent );
    ent->SetAIController( aic );

    m_pEntityManager->AddEntity( ent );
    m_pEntityManager->TrackEntity( "BLT", ent );

    return ent;

}

CShipEntity * CTOFNContext::CreateRandomEnemyEntity()
{

    CShipEntity * ent = new CShipEntity;
    CEnemyAI * aic = new CEnemyAI;

    ent->SetClassTypeID( ENTTYPE_ENEMY );
    ent->SetClassType( "EN" );
    ent->CreatePhysicsBody( m_PhysicsWorld.GetPhysicsWorld(), 64, 64 );
    ent->SetMaterial( m_pTextureFactory->GetObjectContent( "ENEMYSPRITE.png" ) );
    ent->DisablePhysicsMovement();
    ent->SetGravity( 0 );
    ent->SetPos( Util::RandomNumber( 100, 810 ), Util::RandomNumber( -500, -200 ) );

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

    if( entTypeB & ( ENTTYPE_PLYBULLET | ENTTYPE_PLYBULLET ) )
    {

		CAIEntity * bulletEnt = static_cast< CAIEntity * >( pEntityB );
		CBulletAI * bulletAI = static_cast< CBulletAI * >( bulletEnt->GetAIController() );

        entA->Damage( bulletAI->GetDamage() );
        m_pEntityManager->DeleteEntity( bulletEnt );

		return;

    } else if( entTypeA & ENTTYPE_PLAYER && entTypeB & ENTTYPE_POWERUP )
    {




    } else if( entTypeB & ENTTYPE_ENEMY )
    {

        CShipEntity * shipEnt = static_cast< CShipEntity * >( pEntityB );

        //If entity A is a player
        if( entTypeA & ENTTYPE_PLAYER )
        {



        //If entity A is an enemy
        } else if( entTypeA & ENTTYPE_ENEMY )
        {


        }

    }

}

void CTOFNContext::CreateStarBackground()
{

    int winWidth, winHeight;

    m_pGraphicsContext->GetWindowSize( &winWidth, &winHeight );

    for( int j = 0; j < m_MaxStars; j++ )
    {

        CStar * s = new CStar;

        s->m_X = Util::RandomNumber( 0, winWidth );
        s->m_Y = Util::RandomNumber( 0, winHeight );

        s->m_R = 1.0f;
        s->m_G = 1.0f;
        s->m_B = 1.0f;
        s->m_A = 1.0f;

        s->m_Speed = Util::RandomNumber( 300, 1400 );

        m_pStars.push_back( s );

    }

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

                if( pShip->GetHealth() <= 0 )
                {

                    m_pEntityManager->DeleteEntity( pShip );

                }

            }

        }

    }

}

void CTOFNContext::DrawStarBackground()
{

    m_pTextureFactory->GetObjectContent( "star.png" )->Bind();

    Vector2< float > starVert[m_MaxStars];
    Vector4< float > startColor[m_MaxStars];
    int n = 0;

    for( ; n < m_MaxStars; n++ )
    {

        CStar * s = m_pStars[n];
        starVert[n].Set( s->m_X, s->m_Y );
        starColor[n].Set( s->m_R, s->m_G, s->m_B, s->m_A );

        //We'll update in the same loop to save some cpu cycles
        s->m_Y += s->m_Speed * m_FrameDelta;

        if( s->m_Y > 800.0f )
        {

            s->m_X = Util::RandomNumber( 0, width );
            s->m_Y = Util::RandomNumber( -height, -5 );

        }

    }

    glBindBuffer( GL_ARRAY_BUFFER, m_InstancedBuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof( Vertex2 ) * n, starVert, GL_DYNAMIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, m_InstancedRGBABuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof( Vertex4 ) * n, starColor, GL_DYNAMIC_DRAW );

    m_pDrawContext->GetShaderIDFromIndex( 1 );

    glDrawElementsInstancedBaseVertex( GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_SHORT, ( void * )0, n, 0 );

    m_pDrawContext->GetShaderIDFromIndex( 0 );

}

void CTOFNContext::GameLogic()
{

    m_PhysicsWorld.Update();

    UpdateAllEntities();

    //DoEnemyGenerator();

    m_Lua.CallEngineFunction( "GameLogic" );

    //In normal operations, entities should be "deleted", when this happens they aren't actually removed from the entity manager until after all update logic is finished.
    m_pEntityManager->RemoveAllDeletedEntities();

}
