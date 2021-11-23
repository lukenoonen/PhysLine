#ifndef COLLIDERS_H
#define COLLIDERS_H

#include "Global.h"
#include "PhysGlobal.h"

class CCollision
{
public:
	DECLARE_CLASS_NOBASE( CCollision );

	CCollision( const Vector &vecDisplacement );

	void SetValues( const Vector &vecDisplacement, const Vector &vecNormal, CCollider *pCollidedCollider, CCollider *pThisCollider );
	bool RollOver( void );
	
	const Vector &GetDisplacement( void ) const;
	const Vector &GetOriginalDisplacement( void ) const;
	
	const Vector &GetNormal( void ) const;

	CCollider *GetCollidedCollider( void ) const;
	CCollider *GetThisCollider( void ) const;

private:
	Vector m_vecOriginalDisplacement;
	Vector m_vecDisplacement;
	Vector m_vecDirection;

	Vector m_vecPrevNormal;
	Vector m_vecNormal;

	CCollider *m_pCollidedCollider;
	CCollider *m_pThisCollider;
};

class CStoredCollision
{
public:
	DECLARE_CLASS_NOBASE( CStoredCollision );
	
	CStoredCollision( const Vector &vecNormal, CCollider *pCollidedCollider, bool bShouldCalculate );
	void SetCurrentCollision( void );
	
	const Vector &GetNormal( void ) const;
	double GetLikeness( void ) const;
	CCollider *GetCollidedCollider( void ) const;
	bool ShouldCalculate( void ) const;

	bool ShouldAdd( const Vector &vecNormal );

	bool operator==(const CStoredCollision &other) const;

private:
	Vector m_vecNormal;
	double m_dLikeness;
	CCollider *m_pCollidedCollider;
	bool m_bShouldCalculate;
};

class CCollider
{
public:
	DECLARE_CLASS_NOBASE( CCollider );

	CCollider( CPhysObject *pParent, const Vector& vecP1, const Vector& vecP2, bool bDirection );

	void UpdatePosition( void );

	const Vector &GetP1( void );
	const Vector &GetP2( void );
	const Vector &GetNormal( void );

	void CheckCollisionWithObject( CPhysObject *pObject, CCollision &collision );

	CPhysObject *GetParent( void );

private:

	Vector m_vecParentOffsetP1;
	Vector m_vecParentOffsetP2;
	Vector m_vecOriginalNormal;

	Vector m_vecP1;
	Vector m_vecP2;
	Vector m_vecNormal;

	double m_dAngPosition;

	CPhysObject *m_pParent;
};

#endif // COLLIDERS_H