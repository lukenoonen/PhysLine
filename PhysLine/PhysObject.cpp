#include "PhysObject.h"
#include "Colliders.h"
#include "EntityManager.h"
#include "HUDManager.h"
#include "PhysEntity.h"
#include "AudioManager.h"

CConVector cv_gravity( "grv", vec_down * 9.81, "set the gravity" );
CConDouble cd_dragmul( "dragmul", 0.0, "set the drag multiplier" );
CConDouble cd_angdragmul( "angdragmul", 0.0, "set the angular drag multiplier" );
CConInt ci_maxphysstep( "maxps", 10, "max physics steps" );

CPhysObject::CPhysObject( double dMass, bool bIsStatic )
{
	SetMass( dMass );
	SetStatic( bIsStatic );

	m_vecPosition = vec_zero;
	m_vecVelocity = vec_zero;

	m_vecNetForce = vec_zero;

	m_dDragCoefficient = 1.0;

	m_dElasticity = 0.0;

	m_iCollisionAudio = -1;
}

CPhysObject::CPhysObject( CPhysObject *pOther )
{
	SetMass( pOther->GetMass() );
	SetColRadius( pOther->GetColRadius() );
	SetStatic( pOther->IsStatic() );

	m_vecPosition = vec_zero;
	m_vecVelocity = vec_zero;

	m_vecNetForce = vec_zero;

	m_dDragCoefficient = 1.0;

	m_dElasticity = 0.0;

	m_iCollisionAudio = -1;
}

CPhysObject::~CPhysObject()
{
	while (!m_Colliders.IsEmpty())
	{
		delete m_Colliders.GetHead();
		m_Colliders.RemoveFromHead();
	}
}

void CPhysObject::AddCollider( const Vector &vecP1, const Vector &vecP2, bool bDirection )
{
	CCollider *pNewCollider = new CCollider( this, vecP1, vecP2, bDirection );
	m_Colliders.AddToTail( pNewCollider );
}

void CPhysObject::RemoveColliders( void )
{
	while (!m_Colliders.IsEmpty())
	{
		delete m_Colliders.GetHead();
		m_Colliders.RemoveFromHead();
	}
}

void CPhysObject::SetMass( double dMass )
{
	if (dMass < 0.001)
		dMass = 0.001;

	m_dMass = dMass;
}

void CPhysObject::SetColRadius( double dColRadius )
{
	m_dColRadius = dColRadius;
}

void CPhysObject::SetStatic( bool bIsStatic )
{
	m_bIsStatic = bIsStatic;
	
	m_vecVelocity = vec_zero;

	m_vecNetForce = vec_zero;
}

void CPhysObject::SetPosition( const Vector &vecPosition )
{
	m_vecPosition = vecPosition;
	UpdateColliderPositions();
}

void CPhysObject::SetVelocity( const Vector &vecVelocity )
{
	m_vecVelocity = vecVelocity;
}

void CPhysObject::SetAngPosition( double dAngPosition )
{
	m_dAngPosition = dAngPosition;
	UpdateColliderPositions();
}

void CPhysObject::SetDragCoefficient( double dDragCoefficient )
{
	m_dDragCoefficient = dDragCoefficient;
}

void CPhysObject::SetElasticity( double dElasticity )
{
	m_dElasticity = dElasticity;
}

void CPhysObject::SetCollisionAudio( const char *sCollisionAudio )
{
	m_iCollisionAudio = pAudioManager->LoadAudio( sCollisionAudio );
}

CIterator<CCollider *> CPhysObject::GetColliderIterator( void )
{
	return m_Colliders.StartIterating();
}

double CPhysObject::GetColRadius( void )
{
	return m_dColRadius;
}

bool CPhysObject::IsStatic( void )
{
	return m_bIsStatic;
}

double CPhysObject::GetMass( void )
{
	return m_dMass;
}

const Vector &CPhysObject::GetPosition( void )
{
	return m_vecPosition;
}

const Vector &CPhysObject::GetVelocity( void )
{
	return m_vecVelocity;
}

double CPhysObject::GetAngPosition( void )
{
	return m_dAngPosition;
}

double CPhysObject::GetDragCoefficient( void )
{
	return m_dDragCoefficient;
}

double CPhysObject::GetElasticity( void )
{
	return m_dElasticity;
}

void CPhysObject::AddVelocity( const Vector &vecVelocity )
{
	m_vecVelocity += vecVelocity;
}

void CPhysObject::AddAcceleration( const Vector &vecAcceleration )
{
	AddForce( vecAcceleration * m_dMass );
}

void CPhysObject::AddForce( const Vector &vecForce )
{
	m_vecNetForce += vecForce;
}

void CPhysObject::Simulate( void )
{
	if (IsStatic())
		return;

	m_bHasMoved = false;
	m_bHasHandeledCollisions = false;
	
	AddAcceleration( cv_gravity.GetValue() );
	AddForce( m_vecVelocity * m_dDragCoefficient * -cd_dragmul.GetValue() );

	AddVelocity( (m_vecNetForce / m_dMass) * pGlobalValues->GetFrameTime() );

	m_vecNetForce = vec_zero;

	m_PrevStoredCollisions.CopyOther( m_StoredCollisions );
	m_PrevStoredCollisionsStatic.CopyOther( m_StoredCollisionsStatic );

	m_StoredCollisions.RemoveAllNoDelete();
	m_StoredCollisionsStatic.RemoveAllNoDelete();
}

void CPhysObject::Move( void )
{
	if (IsStatic())
		return;

	if (m_bHasMoved)
		return;

	m_bHasMoved = true;

	if (m_vecVelocity != vec_zero)
	{
		CCollision collision( m_vecVelocity * pGlobalValues->GetFrameTime() );

		int iMaxPhysSteps = ci_maxphysstep.GetValue();
		int iSteps = 0;
		while (++iSteps < iMaxPhysSteps)
		{
			CIterator<CPhysEntity *> iterator = pEntityManager->GetPhysEntityIterator();
			while (iterator.Iterating())
				CheckCollisionWithObject( iterator.Iterate()->GetPhysObject(), collision );

			m_vecPosition += collision.GetDisplacement();
			UpdateColliderPositions();

			OnCollision( collision );

			if (!collision.RollOver())
				break;
		}

		if (iSteps == iMaxPhysSteps)
			pHUDManager->AddConsoleEntry( FONTTYPE_NORMAL, c_red, "Unable to resolve position within %d steps!", iMaxPhysSteps );
	}
}

void CPhysObject::HandleCollisionsStatic( void )
{
	if (IsStatic())
		return;

	if (m_StoredCollisionsStatic.Count() == 0)
		return;

	CIterator<CStoredCollision> iterator = m_StoredCollisionsStatic.StartIterating();
	while (iterator.Iterating())
	{
		CStoredCollision &storedCollision = iterator.Iterate();

		const Vector &vecNormal = storedCollision.GetNormal();
		CPhysObject *pCollided = storedCollision.GetCollidedCollider()->GetParent();

		double dAverageElasticity = (m_dElasticity + pCollided->GetElasticity()) / 2.0;
		Vector vecVelocityThis = vecNormal * (vecNormal * m_vecVelocity);
		
		if (IsNewStoredCollisionStatic( storedCollision ))
		{
			double dAudioVolume = UTIL_Magnitude( vecVelocityThis ) / 500.0;
			dAudioVolume = UTIL_Clamp( dAudioVolume, 1.0, 0.0 );
			if (dAudioVolume > 0.005)
				pAudioManager->PlayAudio( m_vecPosition, m_iCollisionAudio, dAudioVolume );
		}

		if (iterator.Iteration() == 2 && UTIL_LessThanOrEqualError( vecNormal * m_StoredCollisionsStatic.GetHead().GetNormal(), 0.0 ))
			vecVelocityThis = m_vecVelocity;

		AddVelocity( vecVelocityThis * (-1.0 - dAverageElasticity) );
	}
}

void CPhysObject::HandleCollisions( void )
{
	if (IsStatic())
		return;

	if (m_StoredCollisions.Count() == 0)
		return;

	if (m_bHasHandeledCollisions)
		return;

	m_bHasHandeledCollisions = true;

	CIterator<CStoredCollision> iterator = m_StoredCollisions.StartIterating();
	while (iterator.Iterating())
	{
		CStoredCollision &storedCollision = iterator.Iterate();
		if (!storedCollision.ShouldCalculate())
			break;

		storedCollision.SetCurrentCollision();

		const Vector &vecNormal = storedCollision.GetNormal();
		CPhysObject *pCollided = storedCollision.GetCollidedCollider()->GetParent();
		if (UTIL_GreaterThanOrEqualError( vecNormal * (m_vecVelocity - pCollided->GetVelocity()), 0.0 ))
			continue;

		pCollided->HandleCollisions();

		Vector vecVelocityThis = vecNormal * (vecNormal * m_vecVelocity);
		Vector vecVelocityOther = vecNormal * (vecNormal * pCollided->GetVelocity());

		if (IsNewStoredCollision( storedCollision ))
		{
			double dAudioVolume = UTIL_Magnitude( vecVelocityThis - vecVelocityOther ) / 500.0;
			dAudioVolume = UTIL_Clamp( dAudioVolume, 1.0, 0.0 );
			if (dAudioVolume > 0.005)
				pAudioManager->PlayAudio( m_vecPosition, m_iCollisionAudio, dAudioVolume );
		}

		CLinkedList<CStoredCollision &> storedCollisions;
		storedCollisions.AddToTail( storedCollision );
		pCollided->CompileCollisions( vecNormal, storedCollisions );

		double dMassOther = 0.0;
		CIterator<CStoredCollision &> iteratorStoredCollisions = storedCollisions.StartIterating();
		while (iteratorStoredCollisions.Iterating())
		{
			const CStoredCollision &storedCollision = iteratorStoredCollisions.Iterate();
			dMassOther += storedCollision.GetLikeness() * storedCollision.GetCollidedCollider()->GetParent()->GetMass();
		}

		double dAverageElasticity = (m_dElasticity + pCollided->GetElasticity()) / 2.0;

		Vector vecElasticVelocityThis = (vecVelocityThis * (m_dMass - dMassOther) + vecVelocityOther * 2.0 * dMassOther) / (m_dMass + dMassOther);
		Vector vecElasticVelocityOther = vecElasticVelocityThis + vecVelocityThis - vecVelocityOther;

		Vector vecInelasticVelocity = (vecVelocityThis * m_dMass + vecVelocityOther * dMassOther) / (m_dMass + dMassOther);

		Vector vecNewVelocityThis;
		UTIL_Lerp( vecNewVelocityThis, vecInelasticVelocity, vecElasticVelocityThis, dAverageElasticity );

		Vector vecNewVelocityOther;
		UTIL_Lerp( vecNewVelocityOther, vecInelasticVelocity, vecElasticVelocityOther, dAverageElasticity );

		AddVelocity( vecNewVelocityThis - vecVelocityThis );

		Vector vecVelocityDelta = vecNewVelocityOther - vecVelocityOther;
		iteratorStoredCollisions = storedCollisions.StartIterating();
		while (iteratorStoredCollisions.Iterating())
		{
			CStoredCollision &storedCollision = iteratorStoredCollisions.Iterate();
			const Vector &vecStoredCollisionNormal = storedCollision.GetNormal();
			storedCollision.GetCollidedCollider()->GetParent()->AddVelocity( vecStoredCollisionNormal * (vecStoredCollisionNormal * vecVelocityDelta) );
		}
	}
}

bool CPhysObject::MoveBeforeOther( void )
{
	if (IsStatic())
		return false;

	if (m_bHasMoved)
		return false;

	Move();
	return true;
}

bool CPhysObject::IsPointInside( const Vector &vecPoint )
{
	int iUpCollided = 0;
	int iDownCollided = 0;
	CIterator<CCollider *> iterator = m_Colliders.StartIterating();
	while (iterator.Iterating())
	{
		Vector vecHitPoint;
		CCollider *pCollider = iterator.Iterate();
		if (UTIL_OneInfLineCol( vecPoint, vecPoint + vec_up, pCollider->GetP1(), pCollider->GetP2(), &vecHitPoint ))
		{
			if (vecPoint.y < vecHitPoint.y)
				iUpCollided += ((vec_up * pCollider->GetNormal()) > 0.0) ? 1 : -1;
			else
				iDownCollided += ((vec_up * pCollider->GetNormal()) < 0.0) ? 1 : -1;
		}
	}

	return iUpCollided > 0 && iDownCollided > 0;
}

void CPhysObject::AddStoredCollision( const Vector &vecNormal, CCollider *pCollidedCollider, bool bShouldCalculate )
{
	CStoredCollision newStoredCollision( vecNormal, pCollidedCollider, bShouldCalculate );
	if (bShouldCalculate)
		m_StoredCollisions.AddToHead( newStoredCollision );
	else
		m_StoredCollisions.AddToTail( newStoredCollision );
}

void CPhysObject::AddStoredCollisionStatic( const Vector &vecNormal, CCollider *pCollidedCollider )
{
	CStoredCollision newStoredCollision( vecNormal, pCollidedCollider, true );
	m_StoredCollisionsStatic.AddToTail( newStoredCollision );
}

bool CPhysObject::IsNewStoredCollision( CStoredCollision &storedCollision )
{
	return m_PrevStoredCollisions.Find( storedCollision ) == -1;
}

bool CPhysObject::IsNewStoredCollisionStatic( CStoredCollision &storedCollision )
{
	return m_PrevStoredCollisionsStatic.Find( storedCollision ) == -1;
}

bool CPhysObject::ShouldAddCollisionFromObject( CPhysObject *pObject )
{
	CIterator<CStoredCollision> iterator = m_StoredCollisions.StartIterating();
	while (iterator.Iterating())
	{
		if (iterator.Iterate().GetCollidedCollider()->GetParent() == pObject)
			return false;
	}
	return true;
}

void CPhysObject::CompileCollisions( const Vector &vecNormal, CLinkedList<CStoredCollision &> & storedCollisions )
{
	CIterator<CStoredCollision> iterator = m_StoredCollisions.StartIterating();
	while (iterator.Iterating())
	{
		CStoredCollision &storedCollision = iterator.Iterate();
		if (storedCollision.ShouldAdd( vecNormal ))
		{
			storedCollisions.AddToTail( storedCollision );
			storedCollision.GetCollidedCollider()->GetParent()->CompileCollisions( vecNormal, storedCollisions );
		}
	}
}

void CPhysObject::AdjustVelocity( Vector &vecVelocity )
{
	if (m_StoredCollisionsStatic.Count() == 0)
		return;

	if (m_StoredCollisionsStatic.Count() == 1)
	{
		CStoredCollision &storedCollision = m_StoredCollisionsStatic.GetHead();
		const Vector &vecNormal = storedCollision.GetNormal();
		if (UTIL_GreaterThanOrEqualError( (vecNormal * m_vecVelocity), 0.0 ))
			return;

		vecVelocity += vecNormal * (vecNormal * m_vecVelocity) * (-1.0 - (m_dElasticity + storedCollision.GetCollidedCollider()->GetParent()->GetElasticity()) / 2.0);
		return;
	}

	CStoredCollision &storedCollisionFirst = m_StoredCollisionsStatic.GetHead();
	CStoredCollision &storedCollisionSecond = m_StoredCollisionsStatic.GetTail();
	const Vector &vecNormalFirst = storedCollisionFirst.GetNormal();
	const Vector &vecNormalSecond = storedCollisionFirst.GetNormal();

	if (UTIL_GreaterThanOrEqualError( (vecNormalFirst * m_vecVelocity), 0.0) )
	{
		if (UTIL_GreaterThanOrEqualError( (vecNormalSecond * m_vecVelocity), 0.0 ))
			return;
		
		vecVelocity += vecNormalSecond * (vecNormalSecond * m_vecVelocity) * (-1.0 - (m_dElasticity + storedCollisionSecond.GetCollidedCollider()->GetParent()->GetElasticity()) / 2.0);
		return;
	}
	else if (UTIL_GreaterThanOrEqualError( (vecNormalSecond * m_vecVelocity), 0.0) )
	{
		vecVelocity += vecNormalFirst * (vecNormalFirst * m_vecVelocity) * (-1.0 - (m_dElasticity + storedCollisionFirst.GetCollidedCollider()->GetParent()->GetElasticity()) / 2.0);
		return;
	}
	
	vecVelocity += m_vecVelocity * (-1.0 - (m_dElasticity + storedCollisionFirst.GetCollidedCollider()->GetParent()->GetElasticity()) / 2.0);
}

void CPhysObject::UpdateColliderPositions( void )
{
	CIterator<CCollider *> iterator = m_Colliders.StartIterating();
	while (iterator.Iterating())
		iterator.Iterate()->UpdatePosition();
}

void CPhysObject::CheckCollisionWithObject( CPhysObject *pObject, CCollision &collision )
{
	if (pObject == this)
		return;

	double dSqrRadius = (GetColRadius() + pObject->GetColRadius());
	dSqrRadius *= dSqrRadius;
	
	if (UTIL_DistSqr( m_vecPosition, pObject->GetPosition() ) > dSqrRadius && UTIL_DistSqr( m_vecPosition + collision.GetDisplacement(), pObject->GetPosition()) > dSqrRadius)
		return;

	CIterator<CCollider *> iterator = GetColliderIterator();
	while (iterator.Iterating())
		iterator.Iterate()->CheckCollisionWithObject( pObject, collision );
}

void CPhysObject::OnCollision( CCollision &collision )
{
	CCollider *pCollidedCollider = collision.GetCollidedCollider();
	if (!pCollidedCollider)
		return;

	CPhysObject *pCollided = pCollidedCollider->GetParent();
	const Vector &vecNormal = collision.GetNormal();
	
	if (pCollided->IsStatic())
	{
		if (m_StoredCollisionsStatic.Count() == 2)
			return;

		AddStoredCollisionStatic( vecNormal, pCollidedCollider );
	}
	else
	{
		if (ShouldAddCollisionFromObject( pCollided ))
		{
			AddStoredCollision( vecNormal, pCollidedCollider, true );
			pCollided->AddStoredCollision( -vecNormal, collision.GetThisCollider(), false );
		}
	}
}