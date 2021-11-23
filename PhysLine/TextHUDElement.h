#ifndef TEXTHUDELEMENT_H
#define TEXTHUDELEMENT_H

#include "Global.h"
#include "BaseHUDElement.h"
#include "RenderManager.h"

class CTextHUDElement : public CBaseHUDElement
{
public:
	DECLARE_CLASS( CTextHUDElement, CBaseHUDElement );

	CTextHUDElement( int iRenderOrder, bool bVisible, const Vector &vecPosition, bool bWorld, double dDuration, Justification_t tJustification, double dScale, FontType_t tFont, const SDL_Color &cColor, const char *sText );
	virtual ~CTextHUDElement();
	
	virtual SDL_Texture *GetTexture( void );

	void SetText( FontType_t tFont, const SDL_Color &cColor, const char *sText );

	double GetScale( void );
	const char *GetText( void );

private:
	SDL_Texture *m_pTextTexture;

	double m_dScale;
	char *m_sText;
};

#endif // TEXTHUDELEMENT_H