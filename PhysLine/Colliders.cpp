#include "Colliders.h"
#include "PhysObject.h"

#define UTIL_PrimaryFullColCheck( a, b, c, d, e, f, g )		(UTIL_CheckCol( a, -b, c, d, &e ))
#define UTIL_SecondaryFullColCheck( a, b, c, d, e )			(UTIL_CheckCol( a, b, c, d, &e ))

//================================================================================================================================================================
// CCollision
//================================================================================================================================================================

CCollision::CCollision( const Vector &vecDisplacement )
{
	m_vecOriginalDisplacement = vecDisplacement;
	m_vecDisplacement = m_vecOriginalDisplacement;
	m_vecDirection = m_vecOriginalDisplacement;
	UTIL_Normalize( m_vecDirection );

	m_vecPrevNormal = -vecDisplacement;
	m_vecNormal = vec_zero;

	m_pCollidedCollider = NULL;
}

void CCollision::SetValues( const Vector &vecDisplacement, const Vector &vecNormal, CCollider *pCollidedCollider, CCollider *pThisCollider )
{
	m_vecDisplacement = m_vecDirection * (m_vecDirection * vecDisplacement);
	m_vecNormal = vecNormal;

	m_pCollidedCollider = pCollidedCollider;
	m_pThisCollider = pThisCollider;
}

bool CCollision::RollOver( void )
{
	if (UTIL_LessThanOrEqualError( m_vecPrevNormal * m_vecNormal, 0.0 ))
		return false;

	Vector vecRemainder = m_vecOriginalDisplacement - m_vecDisplacement;

	m_vecOriginalDisplacement = vecRemainder - m_vecNormal * ( m_vecNormal * vecRemainder );
	m_vecDisplacement = m_vecOriginalDisplacement;
	UTIL_Perpendicular( m_vecDirection, m_vecNormal );

	m_vecPrevNormal = m_vecNormal;
	m_vecNormal = vec_zero;
	
	m_pCollidedCollider = NULL;
	return true;
}

const Vector &CCollision::GetDisplacement( void ) const
{
	return m_vecDisplacement;
}

const Vector &CCollision::GetOriginalDisplacement( void ) const
{
	return m_vecOriginalDisplacement;
}

const Vector &CCollision::GetNormal( void ) const
{
	return m_vecNormal;
}

CCollider *CCollision::GetCollidedCollider( void ) const
{
	return m_pCollidedCollider;
}

CCollider *CCollision::GetThisCollider( void ) const
{
	return m_pThisCollider;
}

//================================================================================================================================================================
// CStoredCollision
//================================================================================================================================================================

CStoredCollision::CStoredCollision( const Vector &vecNormal, CCollider *pCollidedCollider, bool bShouldCalculate )
{
	m_vecNormal = vecNormal;
	m_dLikeness = 0.0;
	m_pCollidedCollider = pCollidedCollider;
	m_bShouldCalculate = bShouldCalculate;
}

void CStoredCollision::SetCurrentCollision( void )
{
	m_dLikeness = 1.0;
}

const Vector &CStoredCollision::GetNormal( void ) const
{
	return m_vecNormal;
}

double CStoredCollision::GetLikeness( void ) const
{
	return m_dLikeness;
}

CCollider *CStoredCollision::GetCollidedCollider( void ) const
{
	return m_pCollidedCollider;
}

bool CStoredCollision::ShouldCalculate( void ) const
{
	return m_bShouldCalculate;
}

bool CStoredCollision::ShouldAdd( const Vector &vecNormal )
{
	m_dLikeness = (m_vecNormal * vecNormal);
	return m_dLikeness > 0.0;
}

bool CStoredCollision::operator==( const CStoredCollision &other ) const
{
	return m_pCollidedCollider->GetParent() == other.m_pCollidedCollider->GetParent();
}

//================================================================================================================================================================
// CCollider
//================================================================================================================================================================

CCollider::CCollider( CPhysObject *pParent, const Vector& vecP1, const Vector& vecP2, bool bDirection )
{
	m_pParent = pParent;

	m_vecParentOffsetP1 = vecP1;
	m_vecParentOffsetP2 = vecP2;

	Vector vecDifference = vecP2 - vecP1;
	UTIL_Perpendicular( m_vecOriginalNormal, vecDifference );
	UTIL_Normalize( m_vecOriginalNormal );

	if (m_vecOriginalNormal.y > 0.0)
	{
		if (!bDirection)
			m_vecOriginalNormal *= -1.0;
	}
	else if (m_vecOriginalNormal.y < 0.0)
	{
		if (bDirection)
			m_vecOriginalNormal *= -1.0;
	}
	else
	{
		if (m_vecOriginalNormal.x > 0.0)
		{
			if (!bDirection)
				m_vecOriginalNormal *= -1.0;
		}
		else
		{
			if (bDirection)
				m_vecOriginalNormal *= -1.0;
		}
	}
}

void CCollider::UpdatePosition( void )
{
	m_dAngPosition = m_pParent->GetAngPosition();

	const Vector &vecParentPosition = m_pParent->GetPosition();
	m_vecP1 = m_vecParentOffsetP1 + vecParentPosition;
	m_vecP2 = m_vecParentOffsetP2 + vecParentPosition;
	m_vecNormal = m_vecOriginalNormal;
	UTIL_Rotate( m_vecP1, m_dAngPosition, vecParentPosition );
	UTIL_Rotate( m_vecP2, m_dAngPosition, vecParentPosition );
	UTIL_Rotate( m_vecNormal, m_dAngPosition );
}

const Vector &CCollider::GetP1( void )
{
	return m_vecP1;
}

const Vector &CCollider::GetP2( void )
{
	return m_vecP2;
}

const Vector &CCollider::GetNormal( void )
{
	return m_vecNormal;
}

void CCollider::CheckCollisionWithObject( CPhysObject *pObject, CCollision &collision )
{
	const Vector &vecDisplacement = collision.GetDisplacement();
	if (UTIL_LessThanOrEqualError( vecDisplacement * m_vecNormal, 0.0 ))
		return;

	CIterator<CCollider *> colliderIterator = pObject->GetColliderIterator();
	while (colliderIterator.Iterating())
	{
		CCollider *pCollider = colliderIterator.Iterate();
		const Vector &vecOtherNormal = pCollider->GetNormal();
		
		if (UTIL_GreaterThanOrEqualError( vecDisplacement * vecOtherNormal, 0.0 ))
			continue;

		const Vector &vecOtherP1 = pCollider->GetP1();
		const Vector &vecOtherP2 = pCollider->GetP2();

		Vector vecHitPoint;

		if (UTIL_CheckCol( vecOtherP1, -vecDisplacement, m_vecP1, m_vecP2, &vecHitPoint ))
		{
			if (!pObject->MoveBeforeOther() || UTIL_CheckCol( vecOtherP1, -vecDisplacement, m_vecP1, m_vecP2, &vecHitPoint ))
				collision.SetValues( vecOtherP1 - vecHitPoint, -m_vecNormal, pCollider, this );
		}
		if (UTIL_CheckCol( vecOtherP2, -vecDisplacement, m_vecP1, m_vecP2, &vecHitPoint ))
		{
			if (!pObject->MoveBeforeOther() || UTIL_CheckCol( vecOtherP2, -vecDisplacement, m_vecP1, m_vecP2, &vecHitPoint ))
				collision.SetValues( vecOtherP2 - vecHitPoint, -m_vecNormal, pCollider, this );
		}

		if (UTIL_CheckCol( m_vecP1, vecDisplacement, vecOtherP1, vecOtherP2, &vecHitPoint ))
		{
			if (!pObject->MoveBeforeOther() || UTIL_CheckCol( m_vecP1, vecDisplacement, vecOtherP1, vecOtherP2, &vecHitPoint ))
				collision.SetValues( vecHitPoint - m_vecP1, vecOtherNormal, pCollider, this );
		}
		if (UTIL_CheckCol( m_vecP2, vecDisplacement, vecOtherP1, vecOtherP2, &vecHitPoint ))
		{
			if (!pObject->MoveBeforeOther() || UTIL_CheckCol( m_vecP2, vecDisplacement, vecOtherP1, vecOtherP2, &vecHitPoint ))
				collision.SetValues( vecHitPoint - m_vecP2, vecOtherNormal, pCollider, this );
		}
	}
}

CPhysObject *CCollider::GetParent( void )
{
	return m_pParent;
}

/*
UTIL_CheckCol( vecOtherP1, -vecDisplacement, m_vecP1, m_vecP2, &vecHitPoint ) && UTIL_GreaterThanOrEqualError( UTIL_Dot( vecOtherP2 - vecOtherP1, m_vecNormal ), 0.0 ) && UTIL_CheckEdgeCases( vecHitPoint, vecOtherP2, vecOtherP1, m_vecP1, m_vecP2 )
UTIL_CheckCol( vecOtherP2, -vecDisplacement, m_vecP1, m_vecP2, &vecHitPoint ) && UTIL_GreaterThanOrEqualError( UTIL_Dot( vecOtherP1 - vecOtherP2, m_vecNormal ), 0.0 ) && UTIL_CheckEdgeCases( vecHitPoint, vecOtherP1, vecOtherP2, m_vecP1, m_vecP2 )
UTIL_CheckCol( m_vecP1, vecDisplacement, vecOtherP1, vecOtherP2, &vecHitPoint ) && !UTIL_IsEqualError( vecHitPoint, vecOtherP1 ) && !UTIL_IsEqualError( vecHitPoint, vecOtherP2 )
UTIL_CheckCol( m_vecP2, vecDisplacement, vecOtherP1, vecOtherP2, &vecHitPoint ) && !UTIL_IsEqualError( vecHitPoint, vecOtherP1 ) && !UTIL_IsEqualError( vecHitPoint, vecOtherP2 )
*/