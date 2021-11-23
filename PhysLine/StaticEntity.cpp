#include "StaticEntity.h"

CStaticEntity::CStaticEntity( int iAnimationIndex, double dFrame, int iRenderOrder, bool bVisible, const Vector &vecSize, const Vector &vecPosition, double dAnglePosition ) : BaseClass( iAnimationIndex, dFrame, iRenderOrder, bVisible, false )
{
	m_vecPosition = vecPosition;
	m_vecSize = vecSize;
	m_dAngPosition = dAnglePosition;
}

const Vector &CStaticEntity::GetPosition( void )
{
	return m_vecPosition;
}

const Vector &CStaticEntity::GetSize( void )
{
	return m_vecSize;
}

double CStaticEntity::GetAngPosition( void )
{
	return m_dAngPosition;
}