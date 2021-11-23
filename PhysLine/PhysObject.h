#ifndef PHYSOBJECT_H
#define PHYSOBJECT_H

#include "Global.h"
#include "PhysGlobal.h"

class CPhysObject
{
public:
	DECLARE_CLASS_NOBASE( CPhysObject );

	CPhysObject( double dMass, bool bIsStatic );
	CPhysObject( CPhysObject *pOther );
	~CPhysObject();

	// SET starts
	void AddCollider( const Vector &vecP1, const Vector &vecP2, bool bDirection );
	void RemoveColliders( void );

	void SetMass( double dMass );
	
	void SetColRadius( double dColRadius );
	void SetStatic( bool bIsStatic );

	void SetPosition( const Vector &vecPosition );
	void SetVelocity( const Vector &vecVelocity );

	void SetAngPosition( double dAngPosition );
	
	void SetDragCoefficient( double dDragCoefficient );

	void SetElasticity( double dElasticity );

	void SetCollisionAudio( const char *sCollisionAudio );

	// GET starts
	
	CIterator<CCollider *> GetColliderIterator( void );

	double GetColRadius( void );
	bool IsStatic( void );

	double GetMass( void );

	const Vector &GetPosition( void );
	const Vector &GetVelocity( void );
	
	double GetAngPosition( void );

	double GetDragCoefficient( void );

	double GetElasticity( void );
	
	// Physics calculations/simulations
	void AddVelocity( const Vector &vecVelocity );
	void AddAcceleration( const Vector &vecAcceleration );
	void AddForce( const Vector &vecForce );

	void Simulate( void );
	void Move( void );
	void HandleCollisionsStatic( void );
	void HandleCollisions( void );
	
	bool MoveBeforeOther( void );

	bool IsPointInside( const Vector &vecPoint );

	void AddStoredCollision( const Vector &vecNormal, CCollider *pCollidedCollider, bool bShouldCalculate );
	void AddStoredCollisionStatic( const Vector &vecNormal, CCollider *pCollidedCollider );
	bool IsNewStoredCollision( CStoredCollision &storedCollision );
	bool IsNewStoredCollisionStatic( CStoredCollision &storedCollision );

	bool ShouldAddCollisionFromObject( CPhysObject *pObject );

	void CompileCollisions( const Vector &vecNormal, CLinkedList<CStoredCollision &> & storedCollisions );

	void AdjustVelocity( Vector &vecVelocity );
	
	void UpdateColliderPositions( void );

private:
	void CheckCollisionWithObject( CPhysObject *pObject, CCollision &collision );
	void OnCollision( CCollision &collision );

private:
	CLinkedList<CCollider *> m_Colliders;

	CLinkedList<CStoredCollision> m_StoredCollisions;
	CLinkedList<CStoredCollision> m_PrevStoredCollisions;

	CLinkedList<CStoredCollision> m_StoredCollisionsStatic;
	CLinkedList<CStoredCollision> m_PrevStoredCollisionsStatic;

	// Optimization variables
	double m_dColRadius;
	bool m_bIsStatic;

	// Mass
	double m_dMass;

	// Linear values
	Vector m_vecPosition;
	Vector m_vecVelocity;
	
	Vector m_vecNetForce;

	double m_dAngPosition;

	double m_dDragCoefficient;

	double m_dElasticity;

	int m_iCollisionAudio;

	bool m_bHasMoved;
	bool m_bHasHandeledCollisions;
};

#endif // PHYSOBJECT_H