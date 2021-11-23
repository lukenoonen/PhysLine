#ifndef TEXTBUTTONHUDELEMENT_H
#define TEXTBUTTONHUDELEMENT_H

#include "Global.h"
#include "ButtonHUDElement.h"
#include "TextHUDElement.h"

class CTextButtonHUDElement : public CButtonHUDElement
{
public:
	DECLARE_CLASS( CTextButtonHUDElement, CButtonHUDElement );

	CTextButtonHUDElement( int iAnimationIndex, double dFrame, int iRenderOrder, bool bVisible, const Vector &vecSize, const Vector &vecPosition, bool bWorld, double dDuration, Justification_t tJustification, const char *sCommand, bool bDeleteOnSelect, double dScale, FontType_t tFont, const SDL_Color &cColor, const char *sText );
	virtual ~CTextButtonHUDElement();

	virtual void CopyToRender( void );
	
	virtual void OnStartHover( void );
	virtual void OnEndHover( void );

	CTextHUDElement *GetTextElement( void );

private:
	CTextHUDElement *m_pTextElement;
};

#endif //TEXTBUTTONHUDELEMENT_H