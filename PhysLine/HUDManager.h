#ifndef HUDMANAGER_H
#define HUDMANAGER_H

#include "Global.h"
#include "RenderManager.h"
#include "ConsoleHUDElement.h"
#include "StatViewerHUDElement.h"

class CHUDManager
{
public:
	DECLARE_CLASS_NOBASE( CHUDManager );

	CHUDManager();

	void CreateHUDElements( void );

	void AddConsoleEntry( FontType_t tFontType, const SDL_Color &cColor, const char *sFormat, ... );
	void ToggleConsole( void );
	void ToggleStats( void );
	void SelectConsoleInput( void );

private:
	CConsoleHUDElement *m_pConsole;
	CStatViewerHUDElement *m_pStatTracker;
};

#endif // HUDMANAGER_H