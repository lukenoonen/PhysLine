#include "AnimationController.h"
#include "RenderManager.h"

CAnimationController::CAnimationController( int iAnimationIndex, double dFrame )
{
	m_pCurrentAnimation = pRenderManager->GetAnimation( iAnimationIndex );
	m_dFrame = dFrame;
	m_iAnimationIndex = iAnimationIndex;
}

CAnimationController::CAnimationController( CAnimationController *pOther )
{
	m_pCurrentAnimation = pOther->GetCurrentAnimation();
	m_dFrame = pOther->GetDecimalFrame();
	m_iAnimationIndex = pOther->GetAnimationIndex();
}

int CAnimationController::GetFrame( void )
{
	if (!m_pCurrentAnimation)
		return -1;

	m_dFrame += m_pCurrentAnimation->GetFrameDelta();
	if ((int)m_dFrame >= m_pCurrentAnimation->GetFrameCount())
	{
		m_iAnimationIndex = m_pCurrentAnimation->GetNextAnimation();
		CAnimation *pNewAnimation = pRenderManager->GetAnimation( m_iAnimationIndex );
		if (pNewAnimation)
			m_pCurrentAnimation = pNewAnimation;

		m_dFrame = 0.0;
	}

	return m_pCurrentAnimation->GetFrame( (int)m_dFrame );
}

CAnimation *CAnimationController::GetCurrentAnimation( void )
{
	return m_pCurrentAnimation;
}

double CAnimationController::GetDecimalFrame( void )
{
	return m_dFrame;
}

int CAnimationController::GetAnimationIndex( void )
{
	return m_iAnimationIndex;
}

void CAnimationController::SetDecimalFrame( double dFrame )
{
	m_dFrame = dFrame;
}