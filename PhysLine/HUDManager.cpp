#include "HUDManager.h"
#include <stdio.h>
#include "EntityManager.h"

bool CC_ToggleConsole( void )
{
	pHUDManager->ToggleConsole();
	return true;
}
CConCommand cc_toggleconsole( "togglec", CC_ToggleConsole );

bool CC_ToggleStats( void )
{
	pHUDManager->ToggleStats();
	return true;
}
CConCommand cc_togglestats( "toggles", CC_ToggleStats );

bool CC_Echo( const CLinkedList<const char *> &command )
{
	if (command.Count() < 1)
		return false;

	pHUDManager->AddConsoleEntry( FONTTYPE_NORMAL, c_white, command.GetHead() );
	return true;
}
CConCommand cc_echo( "echo", CC_Echo, "echo [message]" );

bool CC_SelectConsoleInput( void )
{
	pHUDManager->SelectConsoleInput();
	return true;
}
CConCommand cc_selectci( "selectci", CC_SelectConsoleInput );

CHUDManager::CHUDManager()
{
	CreateHUDElements();
}

void CHUDManager::CreateHUDElements( void )
{
	m_pConsole = new CConsoleHUDElement();
	pEntityManager->AddEntity( m_pConsole );

	m_pStatTracker = new CStatViewerHUDElement();
	pEntityManager->AddEntity( m_pStatTracker );
}

void CHUDManager::AddConsoleEntry( FontType_t tFontType, const SDL_Color &cColor, const char *sFormat, ... )
{
	char sText[256];

	va_list pArgs;
	va_start( pArgs, sFormat );
	vsnprintf_s( sText, sizeof(sText), sFormat, pArgs );
	va_end( pArgs );

	m_pConsole->AddEntry( tFontType, cColor, sText );
}

void CHUDManager::ToggleConsole( void )
{
	m_pConsole->SetVisible( !m_pConsole->IsVisible() );
}

void CHUDManager::ToggleStats( void )
{
	m_pStatTracker->SetVisible( !m_pStatTracker->IsVisible() );
}

void CHUDManager::SelectConsoleInput( void )
{
	m_pConsole->SelectInput();
	m_pConsole->SetVisible( true );
}