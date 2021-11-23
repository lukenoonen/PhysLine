#include "BaseEntity.h"
#include "EntityManager.h"
#include "RenderManager.h"

CBaseEntity::CBaseEntity( int iAnimationIndex, double dFrame, int iRenderOrder, bool bVisible, bool bSelectable )
{
	m_pAnimationController = NULL;
	SetAnimation( iAnimationIndex, dFrame );
	m_iRenderOrder = iRenderOrder;
	m_bVisible = bVisible;
	m_bSelectable = bSelectable;

	m_bShouldDelete = false;
}

CBaseEntity::CBaseEntity( CBaseEntity *pOther )
{
	m_pAnimationController = NULL;
	SetAnimation( pOther->GetAnimationController() );
	m_iRenderOrder = pOther->GetRenderOrder();
	m_bVisible = pOther->IsVisible();
	m_bSelectable = pOther->IsSelectable();

	m_bShouldDelete = false;
}

CBaseEntity::~CBaseEntity()
{
	delete m_pAnimationController;
}

void CBaseEntity::SetRenderOrder( int iRenderOrder )
{
	if (m_iRenderOrder == iRenderOrder)
		return;

	m_iRenderOrder = iRenderOrder;

	pEntityManager->OnUpdateRenderOrder( this );
}

void CBaseEntity::SetVisible( bool bVisible )
{
	m_bVisible = bVisible;
}

bool CBaseEntity::IsVisible( void )
{
	return m_bVisible;
}

bool CBaseEntity::IsSelectable( void )
{
	return m_bSelectable;
}

const Vector &CBaseEntity::GetSize( void )
{
	return vec_zero;
}

const Vector &CBaseEntity::GetPosition( void )
{
	return vec_zero;
}

double CBaseEntity::GetAngPosition( void )
{
	return 0.0;
}

bool CBaseEntity::IsWorld( void )
{
	return true;
}

SDL_Texture *CBaseEntity::GetTexture( void )
{
	return NULL;
}

Justification_t CBaseEntity::GetJustification( void )
{
	return JUSTIFICATION_CENTER;
}

const SDL_Color &CBaseEntity::GetModulation( void )
{
	return c_white;
}

void CBaseEntity::SetAnimation( int iAnimationIndex, double dFrame )
{
	if (m_pAnimationController)
		delete m_pAnimationController;
	
	m_pAnimationController = new CAnimationController( iAnimationIndex, dFrame );
}

void CBaseEntity::SetAnimation( CAnimationController *pAnimationController )
{
	if (m_pAnimationController)
		delete m_pAnimationController;
	
	m_pAnimationController = new CAnimationController( *pAnimationController );
}

CAnimationController *CBaseEntity::GetAnimationController( void )
{
	return m_pAnimationController;
}

int CBaseEntity::GetTextureIndex( void )
{
	return m_pAnimationController->GetFrame();
}

int CBaseEntity::GetRenderOrder( void )
{
	return m_iRenderOrder;
}

void CBaseEntity::MarkForDelete( void )
{
	m_bShouldDelete = true;
}

bool CBaseEntity::ShouldDelete( void )
{
	return m_bShouldDelete;
}

bool CBaseEntity::IsPermanent( void )
{
	return false;
}

void CBaseEntity::CalculateScreenSizeAndPosition( Vector &vecSize, Vector &vecPosition )
{
	vecSize = GetSize();
	vecPosition = GetPosition();
	if (IsWorld())
	{
		pRenderManager->WorldToScreenSize( vecSize );
		pRenderManager->WorldToScreen( vecPosition );
	}
	
	switch (GetJustification())
	{
	case JUSTIFICATION_LEFT:
		vecPosition.x = vecPosition.x;
		vecPosition.y = vecPosition.y - vecSize.y / 2;
		break;
	case JUSTIFICATION_LEFT_UP:
		vecPosition.x = vecPosition.x;
		vecPosition.y = vecPosition.y;
		break;
	case JUSTIFICATION_LEFT_DOWN:
		vecPosition.x = vecPosition.x;
		vecPosition.y = vecPosition.y - vecSize.y;
		break;
	case JUSTIFICATION_CENTER:
		vecPosition.x = vecPosition.x - vecSize.x / 2;
		vecPosition.y = vecPosition.y - vecSize.y / 2;
		break;
	case JUSTIFICATION_CENTER_UP:
		vecPosition.x = vecPosition.x - vecSize.x / 2;
		vecPosition.y = vecPosition.y;
		break;
	case JUSTIFICATION_CENTER_DOWN:
		vecPosition.x = vecPosition.x - vecSize.x / 2;
		vecPosition.y = vecPosition.y - vecSize.y;
		break;
	case JUSTIFICATION_RIGHT:
		vecPosition.x = vecPosition.x - vecSize.x;
		vecPosition.y = vecPosition.y - vecSize.y / 2;
		break;
	case JUSTIFICATION_RIGHT_UP:
		vecPosition.x = vecPosition.x - vecSize.x;
		vecPosition.y = vecPosition.y;
		break;
	case JUSTIFICATION_RIGHT_DOWN:
		vecPosition.x = vecPosition.x - vecSize.x;
		vecPosition.y = vecPosition.y - vecSize.y;
		break;
	}
}

CBaseEntity *CBaseEntity::GetEntityAtPoint( const Vector &vecPoint )
{
	if (m_bSelectable && IsPointInside( vecPoint ))
		return this;

	return NULL;
}

void CBaseEntity::OnLoop( void )
{

}

void CBaseEntity::CopyToRender( void )
{
	pRenderManager->CopyToRender( this );
}

void CBaseEntity::OnStartHover( void )
{

}

void CBaseEntity::OnEndHover( void )
{

}

void CBaseEntity::OnSelected( void )
{

}

void CBaseEntity::OnDeselected( void )
{

}

bool CBaseEntity::IsPointInside( const Vector &vecPoint )
{
	Vector vecMin;
	Vector vecMax;
	CalculateMinBounds( vecMin );
	CalculateMaxBounds( vecMax );
	return UTIL_IsWithin( vecMin, vecMax, vecPoint );
}

void CBaseEntity::CalculateMinBounds( Vector &vecMin )
{
	if (IsWorld())
	{
		CalculateMinBounds( vecMin, GetSize(), GetPosition() );
	}
	else
	{
		Vector vecSize = GetSize();
		Vector vecPosition = GetPosition();
		pRenderManager->ScreenToWorldSize( vecSize );
		pRenderManager->ScreenToWorld( vecPosition );
		CalculateMinBounds( vecMin, vecSize, vecPosition );
	}
}

void CBaseEntity::CalculateMaxBounds( Vector &vecMax )
{
	if (IsWorld())
	{
		CalculateMaxBounds( vecMax, GetSize(), GetPosition() );
	}
	else
	{
		Vector vecSize = GetSize();
		Vector vecPosition = GetPosition();
		pRenderManager->ScreenToWorldSize( vecSize );
		pRenderManager->ScreenToWorld( vecPosition );
		CalculateMaxBounds( vecMax, vecSize, vecPosition );
	}
}

void CBaseEntity::CalculateMinBounds( Vector &vecMin, const Vector &vecSize, const Vector &vecPosition )
{
	switch (GetJustification())
	{
	case JUSTIFICATION_LEFT:
		vecMin.x = vecPosition.x;
		vecMin.y = vecPosition.y - vecSize.y / 2;
		break;
	case JUSTIFICATION_LEFT_UP:
		vecMin.x = vecPosition.x;
		vecMin.y = vecPosition.y - vecSize.y;
		break;
	case JUSTIFICATION_LEFT_DOWN:
		vecMin.x = vecPosition.x;
		vecMin.y = vecPosition.y;
		break;
	case JUSTIFICATION_CENTER:
		vecMin.x = vecPosition.x - vecSize.x / 2;
		vecMin.y = vecPosition.y - vecSize.y / 2;
		break;
	case JUSTIFICATION_CENTER_UP:
		vecMin.x = vecPosition.x - vecSize.x / 2;
		vecMin.y = vecPosition.y - vecSize.y;
		break;
	case JUSTIFICATION_CENTER_DOWN:
		vecMin.x = vecPosition.x - vecSize.x / 2;
		vecMin.y = vecPosition.y;
		break;
	case JUSTIFICATION_RIGHT:
		vecMin.x = vecPosition.x - vecSize.x;
		vecMin.y = vecPosition.y - vecSize.y / 2;
		break;
	case JUSTIFICATION_RIGHT_UP:
		vecMin.x = vecPosition.x - vecSize.x;
		vecMin.y = vecPosition.y - vecSize.y;
		break;
	case JUSTIFICATION_RIGHT_DOWN:
		vecMin.x = vecPosition.x - vecSize.x;
		vecMin.y = vecPosition.y;
		break;
	}
}

void CBaseEntity::CalculateMaxBounds( Vector &vecMax, const Vector &vecSize, const Vector &vecPosition )
{
	switch (GetJustification())
	{
	case JUSTIFICATION_LEFT:
		vecMax.x = vecPosition.x + vecSize.x;
		vecMax.y = vecPosition.y + vecSize.y / 2;
		break;
	case JUSTIFICATION_LEFT_UP:
		vecMax.x = vecPosition.x + vecSize.x;
		vecMax.y = vecPosition.y;
		break;
	case JUSTIFICATION_LEFT_DOWN:
		vecMax.x = vecPosition.x + vecSize.x;
		vecMax.y = vecPosition.y + vecSize.y;
		break;
	case JUSTIFICATION_CENTER:
		vecMax.x = vecPosition.x + vecSize.x / 2;
		vecMax.y = vecPosition.y + vecSize.y / 2;
		break;
	case JUSTIFICATION_CENTER_UP:
		vecMax.x = vecPosition.x + vecSize.x / 2;
		vecMax.y = vecPosition.y;
		break;
	case JUSTIFICATION_CENTER_DOWN:
		vecMax.x = vecPosition.x + vecSize.x / 2;
		vecMax.y = vecPosition.y + vecSize.y;
		break;
	case JUSTIFICATION_RIGHT:
		vecMax.x = vecPosition.x;
		vecMax.y = vecPosition.y + vecSize.y / 2;
		break;
	case JUSTIFICATION_RIGHT_UP:
		vecMax.x = vecPosition.x;
		vecMax.y = vecPosition.y;
		break;
	case JUSTIFICATION_RIGHT_DOWN:
		vecMax.x = vecPosition.x;
		vecMax.y = vecPosition.y + vecSize.y;
		break;
	}
}