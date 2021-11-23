#include "TextHUDElement.h"
#include <stdio.h>

CTextHUDElement::CTextHUDElement( int iRenderOrder, bool bVisible, const Vector &vecPosition, bool bWorld, double dDuration, Justification_t tJustification, double dScale, FontType_t tFont, const SDL_Color &cColor, const char *sText ) : BaseClass( -1, 0.0, iRenderOrder, bVisible, false, vec_zero, vecPosition, bWorld, dDuration, tJustification )
{
	m_pTextTexture = NULL;
	m_dScale = dScale;
	m_sText = NULL;
	SetText( tFont, cColor, sText );
}

CTextHUDElement::~CTextHUDElement()
{
	SDL_DestroyTexture( m_pTextTexture );
	delete m_sText;
}

SDL_Texture *CTextHUDElement::GetTexture( void )
{
	return m_pTextTexture;
}

void CTextHUDElement::SetText( FontType_t tFont, const SDL_Color &cColor, const char *sText )
{
	if (m_sText)
		delete m_sText;

	m_sText = UTIL_stredit( sText );

	if (m_pTextTexture)
		SDL_DestroyTexture( m_pTextTexture );

	m_pTextTexture = pRenderManager->GenerateTextTexture( tFont, cColor, sText );
	
	if (!m_pTextTexture)
	{
		SetSize( vec_zero );
		return;
	}
	
	int iWidth;
	int iHeight;
	SDL_QueryTexture( m_pTextTexture, NULL, NULL, &iWidth, &iHeight );
	Vector vecNewSize( (double)iWidth, (double)iHeight );
	vecNewSize *= m_dScale;
	if (IsWorld())
		vecNewSize /= TEXT_SIZE_D;

	SetSize( vecNewSize );
}

double CTextHUDElement::GetScale( void )
{
	return m_dScale;
}

const char *CTextHUDElement::GetText( void )
{
	return m_sText;
}