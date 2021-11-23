#include "Global.h"
#include "RenderManager.h"
#include "EntityManager.h"
#include "InputManager.h"
#include "PlayerManager.h"
#include "HUDManager.h"
#include "FileManager.h"
#include "AudioManager.h"
#include <Windows.h>
#include <time.h>

CConDouble cd_fps( "fps", 60.0, "set the target fps" );
CConDouble cd_maxft( "maxft", 0.2, "set the max frame time" );
CConDouble cd_timescale( "ts", 1.0, "set the timescale" );

bool CC_Quit( void )
{
	pGlobalValues->MarkForQuit();
	return true;
}
CConCommand cc_quit( "quit", CC_Quit );

void CreateGlobals( void )
{
	srand( (unsigned int)time(NULL) );

	pGlobalValues = new CGlobalValues();
	pRenderManager = new CRenderManager();
	pEntityManager = new CEntityManager();
	pInputManager = new CInputManager();
	pPlayerManager = new CPlayerManager();
	pHUDManager = new CHUDManager();
	pFileManager = new CFileManager();
	pAudioManager = new CAudioManager();
}

void DestroyGlobals( void )
{
	delete pGlobalValues;
	delete pRenderManager;
	delete pEntityManager;
	delete pInputManager;
	delete pPlayerManager;
	delete pHUDManager;
	delete pFileManager;
	delete pAudioManager;
}

CGlobalValues::CGlobalValues()
{
	m_dRealTime = 0.0f;
	m_dPrevRealTime = 0.0f;

	m_uFrameCount = 0;

	m_dFrameTime = 0.0;
	m_dFrameTimeNoScale = 0.0;
	m_dTime = 0.0;
	m_dTimeNoScale = 0.0;

	m_u64Now = SDL_GetPerformanceCounter();
	m_u64Last = 0;

	m_bMarkedForQuit = false;
}

bool CGlobalValues::ShouldLoop( void )
{
	m_u64Last = m_u64Now;
	m_u64Now = SDL_GetPerformanceCounter();
	m_dRealTime += ((m_u64Now - m_u64Last) / (double)SDL_GetPerformanceFrequency());

	double dSecondsPerFrame = 1.0 / cd_fps.GetValue();
	if (dSecondsPerFrame < 0.0)
		return true;

	return m_dRealTime - m_dPrevRealTime > dSecondsPerFrame;
}

unsigned long CGlobalValues::GetFrameCount( void )
{
	return m_uFrameCount;
}

double CGlobalValues::GetFrameTime( void )
{
	return m_dFrameTime;
}

double CGlobalValues::GetFrameTimeNoScale( void )
{
	return m_dFrameTimeNoScale;
}

double CGlobalValues::GetTime( void )
{
	return m_dTime;
}

double CGlobalValues::GetTimeNoScale( void )
{
	return m_dTimeNoScale;
}

bool CGlobalValues::MarkedForQuit( void )
{
	return m_bMarkedForQuit;
}

void CGlobalValues::OnLoop( void )
{
	m_uFrameCount++;

	m_dFrameTimeNoScale = m_dRealTime - m_dPrevRealTime;
	m_dFrameTimeNoScale = UTIL_Min( m_dFrameTimeNoScale, cd_maxft.GetValue() );
	m_dFrameTime = m_dFrameTimeNoScale * cd_timescale.GetValue();

	m_dTime += m_dFrameTime;
	m_dTimeNoScale += m_dFrameTimeNoScale;

	m_dPrevRealTime = m_dRealTime;
}

void CGlobalValues::MarkForQuit( void )
{
	m_bMarkedForQuit = true;
}

CGlobalValues *pGlobalValues;
CRenderManager *pRenderManager;
CEntityManager *pEntityManager;
CInputManager *pInputManager;
CPlayerManager *pPlayerManager;
CHUDManager *pHUDManager;
CFileManager *pFileManager;
CAudioManager *pAudioManager;