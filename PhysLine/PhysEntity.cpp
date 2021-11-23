#include "PhysEntity.h"
#include "PlayerManager.h"

CPhysEntity::CPhysEntity( int iAnimationIndex, double dFrame, int iRenderOrder, bool bVisible, const Vector &vecSize, double dMass, double dElasticity, bool bStatic, const Vector &vecPosition, const Vector &vecVelocity, double dAngPosition, double dDragCoefficient ) : BaseClass( iAnimationIndex, dFrame, iRenderOrder, bVisible, true )
{
	m_pPhysObject = new CPhysObject( dMass, bStatic );
	
	SetSize( vecSize );

	m_pPhysObject->SetPosition( vecPosition );
	m_pPhysObject->SetVelocity( vecVelocity );
	
	m_pPhysObject->SetAngPosition( dAngPosition );

	m_pPhysObject->SetDragCoefficient( dDragCoefficient );

	m_pPhysObject->SetElasticity( dElasticity );

	m_pPhysObject->SetCollisionAudio( "Resources/Audio/collide01.wav" );
}

CPhysEntity::CPhysEntity( CPhysEntity *pOther, const Vector &vecPosition, const Vector &vecVelocity ) : CBaseEntity( pOther )
{
	CPhysObject *pOtherObject = pOther->GetPhysObject();
	m_pPhysObject = new CPhysObject( pOther->GetPhysObject() );

	SetSize( pOther->GetSize() );
	
	m_pPhysObject->SetPosition( vecPosition );
	m_pPhysObject->SetVelocity( vecVelocity );

	m_pPhysObject->SetAngPosition( pOtherObject->GetAngPosition() );

	m_pPhysObject->SetDragCoefficient( pOtherObject->GetDragCoefficient() );

	m_pPhysObject->SetElasticity( pOtherObject->GetElasticity() );
	
	m_pPhysObject->SetCollisionAudio( "Resources/Audio/collide01.wav" );
}

CPhysEntity::~CPhysEntity()
{
	delete m_pPhysObject;
}

CPhysObject *CPhysEntity::GetPhysObject( void )
{
	return m_pPhysObject;
}

void CPhysEntity::Simulate( void )
{
	m_pPhysObject->Simulate();
}

void CPhysEntity::Move( void )
{
	m_pPhysObject->Move();
}

void CPhysEntity::HandleCollisionsStatic( void )
{
	m_pPhysObject->HandleCollisionsStatic();
}

void CPhysEntity::HandleCollisions( void )
{
	m_pPhysObject->HandleCollisions();
}

const Vector &CPhysEntity::GetPosition( void )
{
	return m_pPhysObject->GetPosition();
}

double CPhysEntity::GetAngPosition( void )
{
	return m_pPhysObject->GetAngPosition();
}

const Vector &CPhysEntity::GetSize( void )
{
	return m_vecSize;
}

void CPhysEntity::OnSelected( void )
{
	pPlayerManager->SelectEntity( this );
}

bool CPhysEntity::IsPointInside( const Vector &vecPoint )
{
	return m_pPhysObject->IsPointInside( vecPoint );
}

void CPhysEntity::SetSize( const Vector &vecSize )
{
	m_pPhysObject->SetColRadius( UTIL_Magnitude( vecSize ) / 2.0 + M_MAXERROR );

	m_pPhysObject->RemoveColliders();
	m_pPhysObject->AddCollider( { -vecSize.x / 2.0, -vecSize.y / 2.0 }, { -vecSize.x / 2.0, vecSize.y / 2.0 }, false );
	m_pPhysObject->AddCollider( { -vecSize.x / 2.0, vecSize.y / 2.0 }, { vecSize.x / 2.0, vecSize.y / 2.0 }, true );
	m_pPhysObject->AddCollider( { vecSize.x / 2.0, vecSize.y / 2.0 }, { vecSize.x / 2.0, -vecSize.y / 2.0 }, true );
	m_pPhysObject->AddCollider( { vecSize.x / 2.0, -vecSize.y / 2.0 }, { -vecSize.x / 2.0, -vecSize.y / 2.0 }, false );
	m_pPhysObject->UpdateColliderPositions();

	m_vecSize = vecSize;
}