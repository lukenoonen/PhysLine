#ifndef CONSOLEHUDELEMENT_H
#define CONSOLEHUDELEMENT_H

#include "Global.h"
#include "BaseHUDElement.h"
#include "TextHUDElement.h"
#include "InputHUDElement.h"

#define COMMAND_PROMPT	"Enter Command..."

class CConsoleHUDElement : public CBaseHUDElement
{
public:
	DECLARE_CLASS( CConsoleHUDElement, CBaseHUDElement );

	CConsoleHUDElement();
	virtual ~CConsoleHUDElement();
	
	void AddEntry( FontType_t tFontType, const SDL_Color &cColor, const char *sText );

	virtual bool IsPermanent( void );

	virtual CBaseEntity *GetEntityAtPoint( const Vector &vecPoint );

	virtual void CopyToRender( void );

	void SelectInput( void );

private:
	CLinkedList<CTextHUDElement *> m_TextElements;
	CInputHUDElement *m_pInputElement;
};

#endif // CONSOLEHUDELEMENT_H