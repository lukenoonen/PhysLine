#ifndef INPUTHUDELEMENT_H
#define INPUTHUDELEMENT_H

#include "Global.h"
#include "BaseHUDElement.h"
#include "RenderManager.h"
#include "TextInput.h"
#include "TextHUDElement.h"

#define TEXT_CURSOR_BLINK_TIME 0.53

class CInputHUDElement : public CBaseHUDElement
{
public:
	DECLARE_CLASS( CInputHUDElement, CBaseHUDElement );

	CInputHUDElement( int iRenderOrder, bool bVisible, const Vector &vecPosition, bool bWorld, double dDuration, Justification_t tJustification, double dScale, FontType_t tFont, const SDL_Color &cColor, const char *sDefaultText, bool bNumbersOnly, int iMaxCharacters );
	virtual ~CInputHUDElement();

	virtual void OnLoop( void );
	virtual void CopyToRender( void );
	
	virtual void OnStartHover( void );
	virtual void OnEndHover( void );
	virtual void OnSelected( void );
	
	virtual SDL_Texture *GetTexture( void );

	const char *GetString( void );
	void ClearString( void );

	void SetNumbersOnly( bool bNumbersOnly );
	void SetMaxCharacters( int iMaxCharacters );

private:
	void SetText( const char *sText );

private:
	CTextInput *m_pTextInput;
	SDL_Texture *m_pTextTexture;

	double m_dScale;
	FontType_t m_tFont;
	SDL_Color m_cColor;

	char *m_sDefaultText;

	CTextHUDElement *m_pTextCursor;
	double m_dBlinkTime;

	bool m_bHasFocus;

	char *m_sStoredText;
};

#endif // INPUTHUDELEMENT_H