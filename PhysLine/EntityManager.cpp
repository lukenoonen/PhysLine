#include "EntityManager.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "PlayerManager.h"

CEntityManager::CEntityManager()
{
	m_pHoveredEntity = NULL;
	m_pSelectedEntity = NULL;
}

CEntityManager::~CEntityManager()
{
	DestroyAllEntities();
}

void CEntityManager::OnLoop( void )
{
	CleanUpEntities();
	UpdatePhysEntities();
	UpdateEntities();
	RenderEntities();
}
void CEntityManager::AddEntity( CBaseEntity *pNewEntity )
{
	m_pEntities.Add( pNewEntity, GetDesiredIndex( pNewEntity ) );

	CPhysEntity *pPhysEntity = dynamic_cast<CPhysEntity *>(pNewEntity);
	if (pPhysEntity)
		m_pPhysEntities.AddToTail( pPhysEntity );
}

void CEntityManager::DestroyAllEntities( void )
{
	int iIndex = 0;
	CIterator<CBaseEntity *> iterator = m_pEntities.StartIterating();
	while (iterator.Iterating())
	{
		CBaseEntity *pEntity = iterator.Iterate();
		if (!pEntity->IsPermanent())
		{
			CPhysEntity *pPhysEntity = dynamic_cast<CPhysEntity *>(pEntity);
			if (pPhysEntity)
				m_pPhysEntities.Remove( m_pPhysEntities.Find( pPhysEntity ) );

			delete pEntity;
			m_pEntities.Remove( iIndex );
			continue;
		}

		iIndex++;
	}
}

CBaseEntity *CEntityManager::GetEntityAtMouse( void )
{
	Vector vecMousePosition = pInputManager->GetMousePosition();
	pRenderManager->ScreenToWorld( vecMousePosition );

	CBaseEntity *pSelectedEntity = NULL;
	CIterator<CBaseEntity *> iterator = m_pEntities.StartIterating();
	while (iterator.Iterating())
	{
		CBaseEntity *pNewSelectedEntity = iterator.Iterate()->GetEntityAtPoint( vecMousePosition );
		if (pNewSelectedEntity)
			pSelectedEntity = pNewSelectedEntity;
	}

	return pSelectedEntity;
}

bool CEntityManager::TrySelectEntity( void )
{
	if (m_pHoveredEntity)
	{
		m_pSelectedEntity = m_pHoveredEntity;
		m_pHoveredEntity->OnSelected();
		return true;
	}

	return false;
}

bool CEntityManager::TryDeselectEntity( void )
{
	if (!m_pHoveredEntity || !m_pSelectedEntity)
		return false;

	if (m_pHoveredEntity != m_pSelectedEntity)
		return false;

	m_pSelectedEntity->OnDeselected();
	m_pSelectedEntity = NULL;

	return true;
}

void CEntityManager::OnUpdateRenderOrder( CBaseEntity *pEntity )
{
	m_pEntities.Move( m_pEntities.Find( pEntity ), GetDesiredIndex( pEntity ) );
}

CIterator<CBaseEntity *> CEntityManager::GetEntityIterator( void )
{
	return m_pEntities.StartIterating();
}

CIterator<CPhysEntity *> CEntityManager::GetPhysEntityIterator( void )
{
	return m_pPhysEntities.StartIterating();
}

void CEntityManager::Reset( void )
{
	DestroyAllEntities();

	m_pHoveredEntity = NULL;
	m_pSelectedEntity = NULL;
}

void CEntityManager::CleanUpEntities( void )
{
	int iCurrentEntityIndex = 0;
	CIterator<CBaseEntity *> iterator = m_pEntities.StartIterating();
	while (iterator.Iterating())
	{
		CBaseEntity *pCurrentEntity = iterator.Iterate();
		if (pCurrentEntity->ShouldDelete())
		{
			if (m_pHoveredEntity == pCurrentEntity)
				m_pHoveredEntity = NULL;
			if (m_pSelectedEntity == pCurrentEntity)
				m_pHoveredEntity = NULL;
			
			CPhysEntity *pPhysEntity = dynamic_cast<CPhysEntity *>(pCurrentEntity);
			if (pPhysEntity)
				m_pPhysEntities.Remove( m_pPhysEntities.Find( pPhysEntity ) );

			delete pCurrentEntity;
			m_pEntities.Remove( iCurrentEntityIndex );
			continue;
		}
		iCurrentEntityIndex++;
	}
}

void CEntityManager::UpdatePhysEntities( void )
{
	CIterator<CPhysEntity *> iteratorPhys = m_pPhysEntities.StartIterating();
	while (iteratorPhys.Iterating())
		iteratorPhys.Iterate()->Simulate();

	iteratorPhys = m_pPhysEntities.StartIterating();
	while (iteratorPhys.Iterating())
		iteratorPhys.Iterate()->Move();

	iteratorPhys = m_pPhysEntities.StartIterating();
	while (iteratorPhys.Iterating())
		iteratorPhys.Iterate()->HandleCollisionsStatic();

	iteratorPhys = m_pPhysEntities.StartIterating();
	while (iteratorPhys.Iterating())
		iteratorPhys.Iterate()->HandleCollisions();
}

void CEntityManager::UpdateEntities( void )
{
	CIterator<CBaseEntity *> iterator = m_pEntities.StartIterating();
	while (iterator.Iterating())
		iterator.Iterate()->OnLoop();

	UpdateHoveredEntity();
}

void CEntityManager::RenderEntities( void )
{
	CIterator<CBaseEntity *> iterator = m_pEntities.StartIterating();
	while (iterator.Iterating())
		iterator.Iterate()->CopyToRender();
}

void CEntityManager::UpdateHoveredEntity( void )
{
	CBaseEntity *pNewHoveredEntity = GetEntityAtMouse();
	if (m_pHoveredEntity != pNewHoveredEntity)
	{
		if (m_pHoveredEntity)
			m_pHoveredEntity->OnEndHover();

		if (pNewHoveredEntity)
			pNewHoveredEntity->OnStartHover();

		m_pHoveredEntity = pNewHoveredEntity;
	}
}

int CEntityManager::GetDesiredIndex( CBaseEntity *pEntity )
{
	int iRenderOrder = pEntity->GetRenderOrder();
	int iIndex = 0;
	CIterator<CBaseEntity *> iterator = m_pEntities.StartIterating();
	while (iterator.Iterating())
	{
		if (iterator.Iterate()->GetRenderOrder() > iRenderOrder)
			break;

		iIndex++;
	}

	return iIndex;
}