#ifndef ANIMATIONCONTROLLER_H
#define ANIMATIONCONTROLLER_H

#include "Global.h"
#include "Animation.h"

class CAnimationController
{
public:
	DECLARE_CLASS_NOBASE( CAnimationController );

	CAnimationController( int iAnimationIndex, double dFrame );
	CAnimationController( CAnimationController *pOther );

	int GetFrame( void );

	CAnimation *GetCurrentAnimation( void );
	double GetDecimalFrame( void );
	int GetAnimationIndex( void );

	void SetDecimalFrame( double dFrame );

private:
	CAnimation *m_pCurrentAnimation;
	double m_dFrame;
	int m_iAnimationIndex;
};

#endif // ANIMATIONCONTROLLER_H