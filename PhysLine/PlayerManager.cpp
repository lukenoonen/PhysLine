#include "PlayerManager.h"
#include "EntityManager.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "HUDManager.h"

bool CC_StartSelect( void )
{
	pEntityManager->TrySelectEntity();
	return true;
}
CConCommand cc_startselect( "+select", CC_StartSelect );

bool CC_StopSelect( void )
{
	pEntityManager->TryDeselectEntity();
	pPlayerManager->DeselectEntity();
	return true;
}
CConCommand cc_stopselect( "-select", CC_StopSelect );

bool CC_StartMove( void )
{
	pPlayerManager->SetMovingMouse( true );
	return true;
}
CConCommand cc_startmove( "+move", CC_StartMove );

bool CC_EndMove( void )
{
	pPlayerManager->SetMovingMouse( false );
	return true;
}
CConCommand cc_endmove( "-move", CC_EndMove );

bool CC_Ent_TogCam( void )
{
	pPlayerManager->ToggleEntityCamera();
	return true;
}
CConCommand cc_ent_togcam( "ent_togcam", CC_Ent_TogCam );

bool CC_Ent_Create( const CLinkedList<const char *> &command )
{
	if (command.Count() < 1)
		return false;
	
	CIterator<const char *> iterator = command.StartIterating();
	int iAnimationIndex = UTIL_atoi( iterator.Iterate() );
	double dFrame = 0.0;
	int iRenderOrder = 0;
	bool bVisible = true;
	Vector vecSize = vec_one;
	double dMass = 1.0;
	double dElasticity = 0.0;
	bool bStatic = false;
	Vector vecPosition = pInputManager->GetMousePosition();
	pRenderManager->ScreenToWorld( vecPosition );
	Vector vecVelocity = vec_zero;
	double dAngPosition = 0.0;
	double dDragCoefficient = 1.0;

	if (iterator.Iterating())
		dFrame = UTIL_atod( iterator.Iterate() );

	if (iterator.Iterating())
		iRenderOrder = UTIL_atoi( iterator.Iterate() );

	if (iterator.Iterating())
		bVisible = UTIL_atoi( iterator.Iterate() ) != 0;

	if (iterator.Iterating())
		vecSize.x = UTIL_atod( iterator.Iterate() );

	if (iterator.Iterating())
		vecSize.y = UTIL_atod( iterator.Iterate() );

	if (iterator.Iterating())
		dMass = UTIL_atod( iterator.Iterate() );

	if (iterator.Iterating())
		dElasticity = UTIL_atod( iterator.Iterate() );

	if (iterator.Iterating())
		bStatic = UTIL_atoi( iterator.Iterate() ) != 0;

	if (iterator.Iterating())
		vecPosition.x = UTIL_atod( iterator.Iterate() );

	if (iterator.Iterating())
		vecPosition.y = UTIL_atod( iterator.Iterate() );

	if (iterator.Iterating())
		vecVelocity.x = UTIL_atod( iterator.Iterate() );

	if (iterator.Iterating())
		vecVelocity.y = UTIL_atod( iterator.Iterate() );

	if (iterator.Iterating())
		dAngPosition = UTIL_atod( iterator.Iterate() ) * 180.0 / M_PI;

	if (iterator.Iterating())
		dDragCoefficient = UTIL_atod( iterator.Iterate() );

	CBaseEntity *pNewEntity = new CPhysEntity( iAnimationIndex, dFrame, iRenderOrder, bVisible, vecSize, dMass, dElasticity, bStatic, vecPosition, vecVelocity, dAngPosition, dDragCoefficient );
	pEntityManager->AddEntity( pNewEntity );
	return true;
}
CConCommand cc_ent_create( "ent_create", CC_Ent_Create, "ent_create [animation index] (frame) (render order) (visible) (size) (mass) (elasticity) (static) (position) (velocity) (angposition) (drag coef)" );

bool CC_Ent_Dupe( void )
{
	pPlayerManager->DuplicateSelectedEntity();
	return true;
}
CConCommand cc_ent_dupe( "ent_dupe", CC_Ent_Dupe );

bool CC_Ent_Del( void )
{
	pPlayerManager->DeleteSelectedEntity();
	return true;
}
CConCommand cc_ent_del( "ent_del", CC_Ent_Del );

bool CC_Ent_SetPos( const CLinkedList<const char *> &command )
{
	if (command.Count() < 2)
		return false;
	
	CIterator<const char *> iterator = command.StartIterating();
	Vector vecNewPos;
	vecNewPos.x = UTIL_atod( iterator.Iterate() );
	vecNewPos.y = UTIL_atod( iterator.Iterate() );
	pPlayerManager->SetEntityPosition( vecNewPos );
	return true;
}
CConCommand cc_ent_setpos( "ent_setpos", CC_Ent_SetPos );

bool CC_Ent_SetVel( const CLinkedList<const char *> &command )
{
	if (command.Count() < 2)
		return false;
	
	CIterator<const char *> iterator = command.StartIterating();
	Vector vecNewVel;
	vecNewVel.x = UTIL_atod( iterator.Iterate() );
	vecNewVel.y = UTIL_atod( iterator.Iterate() );
	pPlayerManager->SetEntityVelocity( vecNewVel );
	return true;
}
CConCommand cc_ent_setvel( "ent_setvel", CC_Ent_SetVel );

bool CC_Ent_SetMass( const CLinkedList<const char *> &command )
{
	if (command.Count() < 1)
		return false;

	pPlayerManager->SetEntityMass( UTIL_atod( command.GetHead() ) );
	return true;
}
CConCommand cc_ent_setmass( "ent_setmass", CC_Ent_SetMass );

bool CC_Ent_SetAng( const CLinkedList<const char *> &command )
{
	if (command.Count() < 1)
		return false;

	pPlayerManager->SetEntityAngle( UTIL_atod( command.GetHead() ) );
	return true;
}
CConCommand cc_ent_setang( "ent_setang", CC_Ent_SetAng );

bool CC_Ent_SetElas( const CLinkedList<const char *> &command )
{
	if (command.Count() < 1)
		return false;

	pPlayerManager->SetEntityElasticity( UTIL_atod( command.GetHead() ) );
	return true;
}
CConCommand cc_ent_setelas( "ent_setelas", CC_Ent_SetElas );

bool CC_Ent_SetDragCoef( const CLinkedList<const char *> &command )
{
	if (command.Count() < 1)
		return false;

	pPlayerManager->SetEntityDragCoefficient( UTIL_atod( command.GetHead() ) );
	return true;
}
CConCommand cc_ent_setdragcoef( "ent_setdragcoef", CC_Ent_SetDragCoef );

bool CC_Ent_SetStatic( const CLinkedList<const char *> &command )
{
	if (command.Count() < 1)
		return false;

	pPlayerManager->SetEntityStatic( UTIL_atoi( command.GetHead() ) != 0 );
	return true;
}
CConCommand cc_ent_setstatic( "ent_setstatic", CC_Ent_SetStatic );

bool CC_Ent_SetSize( const CLinkedList<const char *> &command )
{
	if (command.Count() < 2)
		return false;
	
	CIterator<const char *> iterator = command.StartIterating();
	Vector vecNewSize;
	vecNewSize.x = UTIL_atod( iterator.Iterate() );
	vecNewSize.y = UTIL_atod( iterator.Iterate() );
	pPlayerManager->SetEntitySize( vecNewSize );
	return true;
}
CConCommand cc_ent_setsize( "ent_setsize", CC_Ent_SetSize );

bool CC_Ent_SetAnim( const CLinkedList<const char *> &command )
{
	if (command.Count() < 1)
		return false;
	
	CIterator<const char *> iterator = command.StartIterating();

	int iAnimationIndex = UTIL_atoi( iterator.Iterate() );
	double dFrame = 0.0;
	if (iterator.Iterating())
		dFrame = UTIL_atod( iterator.Iterate() );
	
	pPlayerManager->SetEntityAnimation( iAnimationIndex, dFrame );
	return true;
}
CConCommand cc_ent_setanim( "ent_setanim", CC_Ent_SetAnim );

bool CC_Ent_SetFrame( const CLinkedList<const char *> &command )
{
	if (command.Count() < 1)
		return false;
	
	pPlayerManager->SetEntityFrame( UTIL_atod( command.GetHead() ) );
	return true;
}
CConCommand cc_ent_setframe( "ent_setframe", CC_Ent_SetFrame );

CConInt ci_pl_dragtype( "pl_dragtype", 0, "0 for force-based dragging, 1 for velocity-based dragging" );
CConDouble cd_pl_dragmul( "pl_dragmul", 50.0, "makes the dragged object move faster/slower" );

CPlayerManager::CPlayerManager()
{
	Reset();
}

void CPlayerManager::OnLoop( void )
{
	if (m_bMovingMouse)
	{
		Vector vecDifference = pInputManager->GetMouseDelta();
		pRenderManager->ScreenToWorldDelta( vecDifference );
		pRenderManager->MoveScreenBy( -vecDifference );
	}

	// GrabEntity controls
	if (m_pSelectedEntity)
	{
		// Moving the GrabEntity
		if (m_bGrabbingEntity)
		{
			CPhysObject *pGrabObject = m_pSelectedEntity->GetPhysObject();
			Vector vecCurrentGrabPosition = m_vecRelativeGrabPosition;
			UTIL_Rotate( vecCurrentGrabPosition, pGrabObject->GetAngPosition() );
			vecCurrentGrabPosition += pGrabObject->GetPosition();

			Vector vecForce = pInputManager->GetMousePosition();
			pRenderManager->ScreenToWorld( vecForce );

			vecForce -= vecCurrentGrabPosition;
			vecForce *= cd_pl_dragmul.GetValue();

			switch (ci_pl_dragtype.GetValue())
			{
			case 1:
				pGrabObject->SetVelocity( vecForce );
				break;
			default:
				pGrabObject->AddForce( vecForce );
				break;
			}
		}

		if (m_bEntityCamera)
			pRenderManager->SetScreenPosition( m_pSelectedEntity->GetPosition() );
	}
}

void CPlayerManager::Reset( void )
{
	m_pSelectedEntity = NULL;
	m_bGrabbingEntity = false;
	m_bEntityCamera = false;
	m_bMovingMouse = false;
}

void CPlayerManager::SetMovingMouse( bool bMovingMouse )
{
	m_bMovingMouse = bMovingMouse;
}

void CPlayerManager::ToggleEntityCamera( void )
{
	m_bEntityCamera = !m_bEntityCamera;
	pHUDManager->AddConsoleEntry( FONTTYPE_NORMAL, c_white, "Camera mode switched to %s mode", m_bEntityCamera ? "locked" : "free" );
}

void CPlayerManager::DuplicateSelectedEntity( void )
{
	if (!m_pSelectedEntity)
		return;

	Vector vecPosition = pInputManager->GetMousePosition();
	pRenderManager->ScreenToWorld( vecPosition );

	CPhysEntity *pDuplicateEntity = new CPhysEntity( m_pSelectedEntity, vecPosition, vec_zero );
	pEntityManager->AddEntity( pDuplicateEntity );

	pHUDManager->AddConsoleEntry( FONTTYPE_NORMAL, c_white, "Duplicated selected entity" );
}

void CPlayerManager::DeleteSelectedEntity( void )
{
	if (!m_pSelectedEntity)
		return;

	m_pSelectedEntity->MarkForDelete();
	m_pSelectedEntity = NULL;
	pHUDManager->AddConsoleEntry( FONTTYPE_NORMAL, c_white, "Deleted selected entity" );
}

void CPlayerManager::SetEntityPosition( const Vector &vecPosition )
{
	if (!m_pSelectedEntity)
		return;
	
	m_pSelectedEntity->GetPhysObject()->SetPosition( vecPosition );
}

void CPlayerManager::SetEntityVelocity( const Vector &vecVelocity )
{
	if (!m_pSelectedEntity)
		return;
	
	m_pSelectedEntity->GetPhysObject()->SetVelocity( vecVelocity );
}

void CPlayerManager::SetEntityMass( double dMass )
{
	if (!m_pSelectedEntity)
		return;

	m_pSelectedEntity->GetPhysObject()->SetMass( dMass );
}

void CPlayerManager::SetEntityAngle( double dAngle )
{
	if (!m_pSelectedEntity)
		return;
	
	m_pSelectedEntity->GetPhysObject()->SetAngPosition( dAngle * M_PI / 180.0 );
}

void CPlayerManager::SetEntityElasticity( double dElasticity )
{
	if (!m_pSelectedEntity)
		return;
	
	m_pSelectedEntity->GetPhysObject()->SetElasticity( dElasticity );
}

void CPlayerManager::SetEntityDragCoefficient( double dDragCoefficient )
{
	if (!m_pSelectedEntity)
		return;
	
	m_pSelectedEntity->GetPhysObject()->SetDragCoefficient( dDragCoefficient );
}

void CPlayerManager::SetEntityStatic( bool bStatic )
{
	if (!m_pSelectedEntity)
		return;
	
	m_pSelectedEntity->GetPhysObject()->SetStatic( bStatic );
}

void CPlayerManager::SetEntitySize( const Vector &vecSize )
{
	if (!m_pSelectedEntity)
		return;
	
	m_pSelectedEntity->SetSize( vecSize );
}

void CPlayerManager::SetEntityAnimation( int iAnimationIndex, double dFrame )
{
	if (!m_pSelectedEntity)
		return;
	
	m_pSelectedEntity->SetAnimation( iAnimationIndex, dFrame );
}

void CPlayerManager::SetEntityFrame( double dFrame )
{
	if (!m_pSelectedEntity)
		return;
	
	m_pSelectedEntity->GetAnimationController()->SetDecimalFrame( dFrame );
}

void CPlayerManager::SelectEntity( CPhysEntity *pEntity )
{
	if (m_pSelectedEntity != pEntity)
	{
		m_pSelectedEntity = pEntity;
		m_bEntityCamera = false;
		pHUDManager->AddConsoleEntry( FONTTYPE_NORMAL, c_white, "Selected new entity" );
	}
	
	Vector vecWorldMousePosition = pInputManager->GetMousePosition();
	pRenderManager->ScreenToWorld( vecWorldMousePosition );
	m_vecRelativeGrabPosition = vecWorldMousePosition - m_pSelectedEntity->GetPosition();
	UTIL_Rotate( m_vecRelativeGrabPosition, -m_pSelectedEntity->GetAngPosition() );
	m_bGrabbingEntity = true;
}

void CPlayerManager::DeselectEntity( void )
{
	m_bGrabbingEntity = false;
}