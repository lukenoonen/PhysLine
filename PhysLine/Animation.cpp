#include "Animation.h"

CAnimation::CAnimation()
{
	m_iFrameCount = 0;
	m_iFrames = NULL;

	m_iNextAnimation = -1;

	m_dFrameRate = 0.0;
}

CAnimation::~CAnimation()
{
	if (m_iFrames)
		delete[] m_iFrames;
}

void CAnimation::SetFrameCount( int iFrameCount )
{
	if (iFrameCount < 1)
		return;

	if (m_iFrames)
		delete[] m_iFrames;

	m_iFrameCount = iFrameCount;
	if (iFrameCount > 0)
		m_iFrames = new int[m_iFrameCount];

	for (int i = 0; i < m_iFrameCount; i++)
		m_iFrames[i] = NULL;
}

void CAnimation::SetFrame( int iFrameIndex, int iFrame )
{
	if (iFrameIndex < 0 || iFrameIndex >= m_iFrameCount)
		return;

	m_iFrames[iFrameIndex] = iFrame;
}

void CAnimation::SetNextAnimation( int iNextAnimation )
{
	m_iNextAnimation = iNextAnimation;
}

void CAnimation::SetFrameRate( double dFrameRate )
{
	m_dFrameRate = dFrameRate;
}

int CAnimation::GetFrameCount( void )
{
	return m_iFrameCount;
}

int CAnimation::GetFrame( int iIndex )
{
	if (!m_iFrames)
		return -1;

	return m_iFrames[iIndex];
}

int CAnimation::GetNextAnimation( void )
{
	
	return m_iNextAnimation;
}

double CAnimation::GetFrameDelta( void )
{
	return m_dFrameRate * pGlobalValues->GetFrameTime();
}