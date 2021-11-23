#ifndef GLOBAL_H
#define GLOBAL_H

#include "PreGlobal.h"
#include "CommandProcessor.h"

void CreateGlobals( void );
void DestroyGlobals( void );

class CGlobalValues
{
public:
	DECLARE_CLASS_NOBASE( CGlobalValues );

	CGlobalValues();

	bool			ShouldLoop( void );
	unsigned long	GetFrameCount( void );
	double			GetFrameTime( void );
	double			GetFrameTimeNoScale( void );
	double			GetTime( void );
	double			GetTimeNoScale( void );

	bool			MarkedForQuit( void );

	void			OnLoop();
	void			MarkForQuit( void );

	private:
		double			m_dRealTime;
		double			m_dPrevRealTime;

	unsigned long	m_uFrameCount;

	double			m_dFrameTime;
	double			m_dFrameTimeNoScale;
	double			m_dTime;
	double			m_dTimeNoScale;

	bool			m_bMarkedForQuit;

	Uint64			m_u64Now;
	Uint64			m_u64Last;
};

extern CGlobalValues *pGlobalValues;
extern CRenderManager *pRenderManager;
extern CEntityManager *pEntityManager;
extern CInputManager *pInputManager;
extern CPlayerManager *pPlayerManager;
extern CHUDManager *pHUDManager;
extern CFileManager *pFileManager;
extern CAudioManager *pAudioManager;

#endif // GLOBAL_H