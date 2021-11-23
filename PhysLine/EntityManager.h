#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Global.h"
#include "BaseEntity.h"
#include "PhysEntity.h"

class CEntityManager
{
public:
	DECLARE_CLASS_NOBASE( CEntityManager );

	CEntityManager();
	~CEntityManager();

	void OnLoop( void );

	void AddEntity( CBaseEntity *pNewEntity );
	void DestroyAllEntities( void );

	CBaseEntity *GetEntityAtMouse( void );

	bool TrySelectEntity( void );
	bool TryDeselectEntity( void );

	void OnUpdateRenderOrder( CBaseEntity *pEntity );

	CIterator<CBaseEntity *> GetEntityIterator( void );
	CIterator<CPhysEntity *> GetPhysEntityIterator( void );

	void Reset( void );

private:
	void CleanUpEntities( void );
	void UpdatePhysEntities( void );
	void UpdateEntities( void );
	void RenderEntities( void );
	void UpdateHoveredEntity( void );
	int GetDesiredIndex( CBaseEntity *pEntity );

private:
	CLinkedList<CBaseEntity *> m_pEntities;
	CBaseEntity *m_pHoveredEntity;
	CBaseEntity *m_pSelectedEntity;

	CLinkedList<CPhysEntity *> m_pPhysEntities;
};

#endif // ENTITYMANAGER_H