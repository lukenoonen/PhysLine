#include "BaseHUDElement.h"
#include "RenderManager.h"

CBaseHUDElement::CBaseHUDElement( int iAnimationIndex, double dFrame, int iRenderOrder, bool bVisible, bool bSelectable, const Vector &vecSize, const Vector &vecPosition, bool bWorld, double dDuration, Justification_t tJustification ) : BaseClass( iAnimationIndex, dFrame, iRenderOrder, bVisible, bSelectable )
{
	m_bWorld = bWorld;
	m_dExpireTime = dDuration != DURATION_INFINITE ? (dDuration + (bWorld ? pGlobalValues->GetTime() : pGlobalValues->GetTimeNoScale())) : DURATION_INFINITE;
	m_tJustification = tJustification;
	m_cModulation = c_white;

	m_vecSize = vecSize;
	m_vecPosition = vecPosition;
}

const Vector &CBaseHUDElement::GetSize( void )
{
	return m_vecSize;
}

const Vector &CBaseHUDElement::GetPosition( void )
{
	return m_vecPosition;
}

const SDL_Color &CBaseHUDElement::GetModulation( void )
{
	return m_cModulation;
}

bool CBaseHUDElement::ShouldDelete( void )
{
	return ((m_dExpireTime != DURATION_INFINITE) && (IsWorld() ? pGlobalValues->GetTime() : pGlobalValues->GetTimeNoScale() > m_dExpireTime)) || BaseClass::ShouldDelete();
}

void CBaseHUDElement::SetSize( const Vector &vecSize )
{
	m_vecSize = vecSize;
}

void CBaseHUDElement::SetPosition( const Vector &vecPosition )
{
	m_vecPosition = vecPosition;
}

void CBaseHUDElement::SetModulation( const SDL_Color &cModulation )
{
	m_cModulation = cModulation;
}

bool CBaseHUDElement::IsWorld( void )
{
	return m_bWorld;
}

Justification_t CBaseHUDElement::GetJustification( void )
{
	return m_tJustification;
}

double CBaseHUDElement::GetDuration( void )
{
	return m_dExpireTime != DURATION_INFINITE ? (m_dExpireTime - (IsWorld() ? pGlobalValues->GetTime() : pGlobalValues->GetTimeNoScale())) : DURATION_INFINITE;
}