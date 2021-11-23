#include "ButtonHUDElement.h"
#include "RenderManager.h"

CButtonHUDElement::CButtonHUDElement( int iAnimationIndex, double dFrame, int iRenderOrder, bool bVisible, const Vector &vecSize, const Vector &vecPosition, bool bWorld, double dDuration, Justification_t tJustification, const char *sCommand, bool bDeleteOnSelect ) : BaseClass( iAnimationIndex, dFrame, iRenderOrder, bVisible, true, vecSize, vecPosition, bWorld, dDuration, tJustification )
{
	m_sCommand = UTIL_stredit( sCommand );
	m_bDeleteOnSelect = bDeleteOnSelect;
}

CButtonHUDElement::~CButtonHUDElement()
{
	if (m_sCommand)
		delete[] m_sCommand;
}

void CButtonHUDElement::OnStartHover( void )
{
	SetModulation( { 140, 140, 140, 240 } );
	pRenderManager->SetCursor( CURSORTYPE_BUTTON );
}

void CButtonHUDElement::OnEndHover( void )
{
	SetModulation( c_white );
	pRenderManager->SetCursor( CURSORTYPE_DEFAULT );
}

void CButtonHUDElement::OnSelected( void )
{
	CCommandProcesser::ProcessCommand( m_sCommand );
	if (m_bDeleteOnSelect)
		MarkForDelete();
}

const char *CButtonHUDElement::GetCommand( void )
{
	return m_sCommand;
}

bool CButtonHUDElement::GetDeleteOnSelect( void )
{
	return m_bDeleteOnSelect;
}