#include "StatViewerHUDElement.h"
#include "RenderManager.h"
#include <stdio.h>

CStatViewerHUDElement::CStatViewerHUDElement() : BaseClass( -1, 0.0, 10000, true, false, vec_one * 270.0, vec_zero, false, DURATION_INFINITE, JUSTIFICATION_RIGHT_UP )
{
	Vector vecPosition( pRenderManager->GetScreenSize().x, 0 );
	m_pFPSTracker = new CTextHUDElement( 0, true, vecPosition, false, DURATION_INFINITE, JUSTIFICATION_RIGHT_UP, 0.75, FONTTYPE_NORMAL, c_white, "0 FPS" );
	m_pFrameCounter = new CTextHUDElement( 0, true, vecPosition + vec_up * TEXT_SIZE_D * 0.75, false, DURATION_INFINITE, JUSTIFICATION_RIGHT_UP, 0.75, FONTTYPE_NORMAL, c_white, "Frame 0" );
	m_pCameraTracker = new CTextHUDElement( 0, true, vecPosition + vec_up * 2.0 * TEXT_SIZE_D * 0.75, false, DURATION_INFINITE, JUSTIFICATION_RIGHT_UP, 0.75, FONTTYPE_NORMAL, c_white, "Pos 0.00 0.00" );
	m_pZoomTracker = new CTextHUDElement( 0, true, vecPosition + vec_up * 3.0 * TEXT_SIZE_D * 0.75, false, DURATION_INFINITE, JUSTIFICATION_RIGHT_UP, 0.75, FONTTYPE_NORMAL, c_white, "Zoom 32.00x" );
}

CStatViewerHUDElement::~CStatViewerHUDElement()
{
	delete m_pFPSTracker;
	delete m_pFrameCounter;
	delete m_pCameraTracker;
	delete m_pZoomTracker;
}

bool CStatViewerHUDElement::IsPermanent( void )
{
	return true;
}

void CStatViewerHUDElement::CopyToRender( void )
{
	if (IsVisible())
	{
		char sText[48];

		sprintf_s( sText, sizeof(sText), "%d FPS", (int)(1.0 / pGlobalValues->GetFrameTimeNoScale()) );
		m_pFPSTracker->SetText( FONTTYPE_NORMAL, c_white, sText );
	
		sprintf_s( sText, sizeof(sText), "Frame %d", pGlobalValues->GetFrameCount() );
		m_pFrameCounter->SetText( FONTTYPE_NORMAL, c_white, sText );
		
		const Vector &vecScreenPos = pRenderManager->GetScreenPosition();
		sprintf_s( sText, sizeof(sText), "Pos %.2f %.2f", vecScreenPos.x, vecScreenPos.y );
		m_pCameraTracker->SetText( FONTTYPE_NORMAL, c_white, sText );
		
		sprintf_s( sText, sizeof(sText), "Zoom %.2f x", pRenderManager->GetRenderZoom() );
		m_pZoomTracker->SetText( FONTTYPE_NORMAL, c_white, sText );

		m_pFPSTracker->CopyToRender();
		m_pFrameCounter->CopyToRender();
		m_pCameraTracker->CopyToRender();
		m_pZoomTracker->CopyToRender();
	}
}