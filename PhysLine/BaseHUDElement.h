#ifndef BASEHUDELEMENT_H
#define BASEHUDELEMENT_H

#include "Global.h"
#include "BaseEntity.h"

#define DURATION_INFINITE	-1.0

class CBaseHUDElement : public CBaseEntity
{
public:
	DECLARE_CLASS( CBaseHUDElement, CBaseEntity );

	CBaseHUDElement( int iAnimationIndex, double dFrame, int iRenderOrder, bool bVisible, bool bSelectable, const Vector &vecSize, const Vector &vecPosition, bool bWorld, double dDuration, Justification_t tJustification );
	
	virtual const Vector &GetSize( void );
	virtual const Vector &GetPosition( void );

	virtual const SDL_Color &GetModulation( void );

	virtual bool ShouldDelete( void );

	void SetSize( const Vector &vecSize );
	void SetPosition( const Vector &vecPosition );

	void SetModulation( const SDL_Color &cModulation );

	virtual bool IsWorld( void );
	virtual Justification_t GetJustification( void );

	double GetDuration( void );

private:
	bool m_bWorld;
	double m_dExpireTime;
	Justification_t m_tJustification;
	SDL_Color m_cModulation;

	Vector m_vecSize;
	Vector m_vecPosition;
};

#endif // BASEHUDELEMENT_H