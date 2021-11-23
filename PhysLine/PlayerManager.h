#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include "Global.h"
#include "PhysEntity.h"

class CPlayerManager
{
public:
	DECLARE_CLASS_NOBASE( CPlayerManager );

	CPlayerManager();

	void OnLoop( void );

	void Reset( void );

	void SetMovingMouse( bool bMovingMouse );
	void ToggleEntityCamera( void );
	void DuplicateSelectedEntity( void );
	void DeleteSelectedEntity( void );
	void SetEntityPosition( const Vector &vecPosition );
	void SetEntityVelocity( const Vector &vecVelocity );
	void SetEntityMass( double dMass );
	void SetEntityAngle( double dAngle );
	void SetEntityElasticity( double dElasticity );
	void SetEntityDragCoefficient( double dDragCoefficient );
	void SetEntityStatic( bool bStatic );
	void SetEntitySize( const Vector &vecVelocity );
	void SetEntityAnimation( int iAnimationIndex, double dFrame );
	void SetEntityFrame( double dFrame );

	void SelectEntity( CPhysEntity *pEntity );
	void DeselectEntity( void );

private:
	CPhysEntity *m_pSelectedEntity;
	bool m_bGrabbingEntity;
	Vector m_vecRelativeGrabPosition;
	bool m_bEntityCamera;

	bool m_bMovingMouse;
};

#endif // PLAYERMANAGER_H