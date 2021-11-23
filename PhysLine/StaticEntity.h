#ifndef STATICENTITY_H
#define STATICENTITY_H

#include "Global.h"
#include "BaseEntity.h"

class CStaticEntity : public CBaseEntity
{
public:
	DECLARE_CLASS( CStaticEntity, CBaseEntity );

	CStaticEntity( int iAnimationIndex, double dFrame, int iRenderOrder, bool bVisible, const Vector &vecSize, const Vector &vecPosition, double dAngPosition );
	
	virtual const Vector &GetSize( void );
	virtual const Vector &GetPosition( void );
	virtual double GetAngPosition( void );

private:
	Vector m_vecSize;
	Vector m_vecPosition;
	double m_dAngPosition;
};

#endif // STATICENTITY_H