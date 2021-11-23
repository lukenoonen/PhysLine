#ifndef PHYSENTITY_H
#define PHYSENTITY_H

#include "BaseEntity.h"
#include "PhysObject.h"

class CPhysEntity : public CBaseEntity
{
public:
	DECLARE_CLASS( CPhysEntity, CBaseEntity );
	
	CPhysEntity( int iAnimationIndex, double dFrame, int iRenderOrder, bool bVisible, const Vector &vecSize, double dMass, double dElasticity, bool bStatic, const Vector &vecPosition, const Vector &vecVelocity, double dAngPosition, double dDragCoefficient );
	CPhysEntity( CPhysEntity *pOther, const Vector &vecPosition, const Vector &vecVelocity );
	virtual ~CPhysEntity();

	CPhysObject *GetPhysObject( void );

	void Simulate( void );
	void Move( void );
	void HandleCollisionsStatic( void );
	void HandleCollisions( void );

	virtual const Vector &GetSize( void );
	virtual const Vector &GetPosition( void );
	virtual double GetAngPosition( void );

	virtual void OnSelected( void );

	void SetSize( const Vector &vecSize );

protected:
	virtual bool IsPointInside( const Vector &vecPoint );

private:
	Vector m_vecSize;
	CPhysObject *m_pPhysObject;
};

#endif // PHYSENTITY_H