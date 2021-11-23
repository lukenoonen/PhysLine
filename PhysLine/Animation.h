#ifndef ANIMATION_H
#define ANIMATION_H

#include "Global.h"

class CAnimation
{
public:
	DECLARE_CLASS_NOBASE( CAnimation );

	CAnimation();
	~CAnimation();

	void SetFrameCount( int iFrameCount );
	void SetFrame( int iFrameIndex, int iFrame );
	void SetNextAnimation( int iNextAnimation );
	void SetFrameRate( double dFrameRate );

	int GetFrameCount( void );
	int GetFrame( int iIndex );
	int GetNextAnimation( void );
	double GetFrameDelta( void );

private:
	int m_iFrameCount;
	int *m_iFrames;

	int m_iNextAnimation;

	double m_dFrameRate;
};

#endif // ANIMATION_H