#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "Global.h"
#include "Animation.h"
#include "BaseEntity.h"

#define MAX_TEXTURES			256
#define MAX_ANIMATIONS			64

#define TEXT_SIZE				30
#define TEXT_SIZE_D				TEXT_SIZE.0

const SDL_Color c_white{ 255, 255, 255, 255 };
const SDL_Color c_black{ 0, 0, 0, 255 };
const SDL_Color c_red{ 255, 0, 0, 255 };
const SDL_Color c_green{ 0, 255, 0, 255 };
const SDL_Color c_blue{ 0, 0, 255, 255 };

#define DEFAULT_FONTTYPE_NORMAL		"Resources/Fonts/arial.ttf"
#define DEFAULT_FONTTYPE_BOLD		"Resources/Fonts/arialbd.ttf"
#define DEFAULT_FONTTYPE_ITALIC		"Resources/Fonts/ariali.ttf"
#define DEFAULT_FONTTYPE_SPECIAL_0	"Resources/Fonts/ARIALN.ttf"
#define DEFAULT_FONTTYPE_SPECIAL_1	"Resources/Fonts/ARIALNB.ttf"
#define DEFAULT_FONTTYPE_SPECIAL_2	"Resources/Fonts/ARIALNI.ttf"

enum FontType_t
{
	FONTTYPE_NORMAL = 0,
	FONTTYPE_BOLD,
	FONTTYPE_ITALIC,
	FONTTYPE_SPECIAL_0,
	FONTTYPE_SPECIAL_1,
	FONTTYPE_SPECIAL_2,
	FONTTYPE_COUNT,
	FONTTYPE_INVALID = FONTTYPE_COUNT,
};

enum CursorType_t
{
	CURSORTYPE_DEFAULT = 0,
	CURSORTYPE_BUTTON,
	CURSORTYPE_IBEAM,
	CURSORTYPE_NO,
	CURSORTYPE_COUNT,
};

void	UTIL_DrawCircle( SDL_Renderer *pRenderer, int iCenterX, int iCenterY, int iRadius );

class CRenderManager
{
public:
	DECLARE_CLASS_NOBASE( CRenderManager );

	CRenderManager();
	~CRenderManager();

	bool CreateWindow( void );
	void DestroyWindow( void );
	void DestroyGraphics( void );

	void SetRenderZoom( double dRenderZoom );
	void ScaleRenderZoom( double dFactor );

	void CopyToRender( CBaseEntity *pEntity );
	void Render( void );

	bool GenerateTexture( const char *sPath, int iIndex );
	bool AddAnimation( CAnimation *pAnimation, int iIndex );

	bool AddFont( const char *sFontAddresss, FontType_t tFont );
	bool IsFontInitialized( FontType_t tFont );
	SDL_Texture *GenerateTextTexture( FontType_t tFont, const SDL_Color &cColor, const char *sText );
	TTF_Font *GetFont( FontType_t tFont );

	CAnimation *GetAnimation( int iIndex );

	double GetRenderZoom( void );

	int GetTextureCount( void );
	int GetAnimationCount( void );

	void WorldToScreen( Vector &vecWorld );
	void ScreenToWorld( Vector &vecScreen );
	void WorldToScreenSize( Vector &vecWorld );
	void ScreenToWorldSize( Vector &vecScreen );
	void WorldToScreenDelta( Vector &vecWorld );
	void ScreenToWorldDelta( Vector &vecScreen );

	void MoveScreenBy( const Vector &vecDisplacement );
	void SetScreenPosition( const Vector &vecPosition );
	const Vector &GetScreenPosition( void );
	void SetScreenLocked( bool bScreenLocked );

	void SetScreenSize( const Vector &vecScreenSize );
	const Vector &GetScreenSize( void );

	double GetScreenScale( void );

	void SetCursor( CursorType_t tCursorType );

	void Reset( void );

private:
	SDL_Texture *GenerateTexture( const char *sPath );

private:
	SDL_Window *m_pWindow;
	SDL_Renderer *m_pRenderer;

	SDL_Texture *m_pInvalidTexture;

	int m_iTextureCount;
	SDL_Texture *m_pTextures[MAX_TEXTURES];
	int m_iAnimationCount;
	CAnimation *m_pAnimations[MAX_ANIMATIONS];

	TTF_Font *m_pFonts[FONTTYPE_COUNT];
	SDL_Cursor *m_pCursors[CURSORTYPE_COUNT];

	double m_dRenderZoom;
	Vector m_vecRenderOffset;
	bool m_bScreenLocked;
	Vector m_vecScreenSize;
	double m_dScreenScale;
};

#endif // RENDERMANAGER_H