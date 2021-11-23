#ifndef BASEENTITY_H
#define BASEENTITY_H

#include "Global.h"
#include "AnimationController.h"

enum Justification_t
{
	JUSTIFICATION_LEFT = 0,
	JUSTIFICATION_LEFT_UP,
	JUSTIFICATION_LEFT_DOWN,
	JUSTIFICATION_CENTER,
	JUSTIFICATION_CENTER_UP,
	JUSTIFICATION_CENTER_DOWN,
	JUSTIFICATION_RIGHT,
	JUSTIFICATION_RIGHT_UP,
	JUSTIFICATION_RIGHT_DOWN,
};

class CBaseEntity
{
public:
	DECLARE_CLASS_NOBASE( CBaseEntity );

	CBaseEntity( int iAnimationIndex, double dFrame, int iRenderOrder, bool bVisible, bool bSelectable );
	CBaseEntity( CBaseEntity *pOther );
	~CBaseEntity();

	void SetRenderOrder( int iRenderOrder );

	void SetVisible( bool bVisible );
	bool IsVisible( void );

	bool IsSelectable( void );

	virtual const Vector &GetSize( void );
	virtual const Vector &GetPosition( void );
	virtual double GetAngPosition( void );
	virtual bool IsWorld( void );
	virtual SDL_Texture *GetTexture( void );
	virtual Justification_t GetJustification( void );
	virtual const SDL_Color &GetModulation( void );

	void SetAnimation( int iAnimationIndex, double dFrame );
	void SetAnimation( CAnimationController *pAnimationController );
	CAnimationController *GetAnimationController( void );
	int GetTextureIndex( void );
	int GetRenderOrder( void );

	void MarkForDelete( void );
	virtual bool ShouldDelete( void );
	virtual bool IsPermanent( void );

	void CalculateScreenSizeAndPosition( Vector &vecSize, Vector &vecPosition );
	virtual CBaseEntity *GetEntityAtPoint( const Vector &vecPoint );
	
	// Think functions
	virtual void OnLoop( void );
	virtual void CopyToRender( void );

	// Events
	virtual void OnStartHover( void );
	virtual void OnEndHover( void );
	virtual void OnSelected( void );
	virtual void OnDeselected( void );

protected:
	virtual bool IsPointInside( const Vector &vecPoint );
	void CalculateMinBounds( Vector &vecMin );
	void CalculateMaxBounds( Vector &vecMax );

	void CalculateMinBounds( Vector &vecMin, const Vector &vecSize, const Vector &vecPosition );
	void CalculateMaxBounds( Vector &vecMax, const Vector &vecSize, const Vector &vecPosition );
	
private:
	CAnimationController *m_pAnimationController;
	int m_iRenderOrder;
	bool m_bVisible;
	bool m_bSelectable;

	bool m_bShouldDelete;
};

#endif // BASEENTITY_H