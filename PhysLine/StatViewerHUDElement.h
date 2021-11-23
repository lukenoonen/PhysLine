#ifndef STATVIEWERHUDELEMENT_H
#define STATVIEWERHUDELEMENT_H

#include "Global.h"
#include "BaseHUDElement.h"
#include "TextHUDElement.h"

class CStatViewerHUDElement : public CBaseHUDElement
{
public:
	DECLARE_CLASS( CStatViewerHUDElement, CBaseHUDElement );

	CStatViewerHUDElement();
	virtual ~CStatViewerHUDElement();

	virtual bool IsPermanent( void );
	
	virtual void CopyToRender( void );

private:
	CTextHUDElement *m_pFPSTracker;
	CTextHUDElement *m_pFrameCounter;
	CTextHUDElement *m_pCameraTracker;
	CTextHUDElement *m_pZoomTracker;
};

#endif // STATVIEWERHUDELEMENT_H