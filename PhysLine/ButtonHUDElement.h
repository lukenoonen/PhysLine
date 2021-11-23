#ifndef BUTTONHUDELEMENT_H
#define BUTTONHUDELEMENT_H

#include "Global.h"
#include "BaseHUDElement.h"

class CButtonHUDElement : public CBaseHUDElement
{
public:
	DECLARE_CLASS( CButtonHUDElement, CBaseHUDElement );

	CButtonHUDElement( int iAnimationIndex, double dFrame, int iRenderOrder, bool bVisible, const Vector &vecSize,const Vector &vecPosition, bool bWorld, double dDuration, Justification_t tJustification, const char *sCommand, bool bDeleteOnSelect );
	virtual ~CButtonHUDElement();
	
	virtual void OnStartHover( void );
	virtual void OnEndHover( void );
	virtual void OnSelected( void );

	const char *GetCommand( void );
	bool GetDeleteOnSelect( void );

private:
	char *m_sCommand;
	bool m_bDeleteOnSelect;
};

#endif // BUTTONHUDELEMENT_H