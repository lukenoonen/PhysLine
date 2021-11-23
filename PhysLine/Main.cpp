#include "Global.h"
#include "FileManager.h"
#include "InputManager.h"
#include "PlayerManager.h"
#include "EntityManager.h"
#include "HUDManager.h"
#include "RenderManager.h"
#include <Windows.h>

int main( int argc, char * argv[] )
{
	ShowWindow( GetConsoleWindow(), SW_HIDE );

	CreateGlobals();

	pFileManager->Load( "Resources/Config/autoexec.cfg" );

	while (!pGlobalValues->MarkedForQuit())
	{
		while (!pGlobalValues->ShouldLoop());

		pGlobalValues->OnLoop();
		pInputManager->OnLoop();
		pPlayerManager->OnLoop();
		pEntityManager->OnLoop();
		pRenderManager->Render();
	}

	DestroyGlobals();

	return 0;
}