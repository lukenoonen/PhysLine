#include "TextButtonHUDElement.h"
#include <stdio.h>

CTextButtonHUDElement::CTextButtonHUDElement( int iAnimationIndex, double dFrame, int iRenderOrder, bool bVisible, const Vector &vecSize, const Vector &vecPosition, bool bWorld, double dDuration, Justification_t tJustification, const char *sCommand, bool bDeleteOnSelect, double dScale, FontType_t tFont, const SDL_Color &cColor, const char *sText ) : BaseClass( iAnimationIndex, dFrame, iRenderOrder, bVisible, vecSize, vecPosition, bWorld, dDuration, tJustification, sCommand, bDeleteOnSelect )
{
	m_pTextElement = new CTextHUDElement( 0, true, vec_zero, bWorld, DURATION_INFINITE, JUSTIFICATION_CENTER, dScale, tFont, cColor, sText );
}

CTextButtonHUDElement::~CTextButtonHUDElement()
{
	delete m_pTextElement;
}

void CTextButtonHUDElement::CopyToRender( void )
{
	BaseClass::CopyToRender();

	if (IsVisible())
	{
		m_pTextElement->SetPosition( GetPosition() );
		m_pTextElement->CopyToRender();
	}
}

void CTextButtonHUDElement::OnStartHover( void )
{
	BaseClass::OnStartHover();
	m_pTextElement->SetModulation( { 140, 140, 140, 240 } );
}

void CTextButtonHUDElement::OnEndHover( void )
{
	BaseClass::OnEndHover();
	m_pTextElement->SetModulation( c_white );
}

CTextHUDElement *CTextButtonHUDElement::GetTextElement( void )
{
	return m_pTextElement;
}