#include "RenderManager.h"
#include "PhysEntity.h"
#include <stdio.h>

bool CC_SetScreenSize( const CLinkedList<const char *> &command )
{
	if (command.Count() < 2)
		return false;

	CIterator<const char *> iterator = command.StartIterating();

	Vector vecNewScreenSize;
	vecNewScreenSize.x = UTIL_atod( iterator.Iterate() );
	vecNewScreenSize.y = UTIL_atod( iterator.Iterate() );
	pRenderManager->SetScreenSize( vecNewScreenSize );
	return true;
}
CConCommand cc_setscreensize( "setss", CC_SetScreenSize, "setss [screen x] [screen y]" );

bool CC_SetZoom( const CLinkedList<const char *> &command )
{
	if (command.Count() < 1)
		return false;

	pRenderManager->SetRenderZoom( UTIL_atod( command.GetHead() ) );
	return true;
}
CConCommand cc_setzoom( "setzoom", CC_SetZoom, "setzoom [zoom]" );

bool CC_ZoomBy( const CLinkedList<const char *> &command )
{
	if (command.Count() < 1)
		return false;

	pRenderManager->ScaleRenderZoom( UTIL_atod( command.GetHead() ) );
	return true;
}
CConCommand cc_zoomby( "zoomby", CC_ZoomBy, "zoomby [amount]" );

bool CC_SetCamPos( const CLinkedList<const char *> &command )
{
	if (command.Count() < 2)
		return false;
	
	CIterator<const char *> iterator = command.StartIterating();
	Vector vecNewCamPos;
	vecNewCamPos.x = UTIL_atod( iterator.Iterate() );
	vecNewCamPos.y = UTIL_atod( iterator.Iterate() );
	pRenderManager->SetScreenPosition( vecNewCamPos );
	return true;
}
CConCommand cc_setcampos( "setcp", CC_SetCamPos, "setcp [pos x] [pos y]" );

bool CC_LockCamera( void )
{
	pRenderManager->SetScreenLocked( true );
	return true;
}
CConCommand cc_lockcamera( "lockc", CC_LockCamera );

bool CC_UnlockCamera( void )
{
	pRenderManager->SetScreenLocked( false );
	return true;
}
CConCommand cc_unlockcamera( "unlockc", CC_UnlockCamera );

CConBool cb_debug( "debug", false, "enables visual debugging tools" );

bool CC_AddFont( const CLinkedList<const char *> &command )
{
	if (command.Count() < 2)
		return false;

	CIterator<const char *> iterator = command.StartIterating();
	const char *sFontType = iterator.Iterate();

	FontType_t tFontType = FONTTYPE_INVALID;
	if (UTIL_strcmp( "nrm", sFontType ) == 0)
		tFontType = FONTTYPE_NORMAL;
	else if (UTIL_strcmp( "bld", sFontType ) == 0)
		tFontType = FONTTYPE_BOLD;
	else if (UTIL_strcmp( "itl", sFontType ) == 0)
		tFontType = FONTTYPE_ITALIC;
	else if (UTIL_strcmp( "sp0", sFontType ) == 0)
		tFontType = FONTTYPE_SPECIAL_0;
	else if (UTIL_strcmp( "sp1", sFontType ) == 0)
		tFontType = FONTTYPE_SPECIAL_1;
	else if (UTIL_strcmp( "sp2", sFontType ) == 0)
		tFontType = FONTTYPE_SPECIAL_2;

	if (tFontType == FONTTYPE_INVALID)
		return false;

	pRenderManager->AddFont( iterator.Iterate(), tFontType );
	return true;
}
CConCommand cc_addfont( "addfont", CC_AddFont, "addfont [nrm;bld;itl;sp0;sp1;sp2] [font address]" );

void UTIL_DrawCircle( SDL_Renderer *pRenderer, int iCenterX, int iCenterY, int iRadius )
{
	const Vector &vecScreenSize = pRenderManager->GetScreenSize();
	if (!(iCenterX > 0 && iCenterX < (int)vecScreenSize.x && iCenterY > 0 && iCenterY < (int)vecScreenSize.y) || iRadius > vecScreenSize.x || iRadius > vecScreenSize.y)
		return;

	const int iDiameter = (iRadius * 2);

	int x = (iRadius - 1);
	int y = 0;
	int tx = 1;
	int ty = 1;
	int iError = (tx - iDiameter);

	while (x >= y)
	{
		//  Each of the following renders an octant of the circle
		SDL_RenderDrawPoint( pRenderer, iCenterX + x, iCenterY - y );
		SDL_RenderDrawPoint( pRenderer, iCenterX + x, iCenterY + y );
		SDL_RenderDrawPoint( pRenderer, iCenterX - x, iCenterY - y );
		SDL_RenderDrawPoint( pRenderer, iCenterX - x, iCenterY + y );
		SDL_RenderDrawPoint( pRenderer, iCenterX + y, iCenterY - x );
		SDL_RenderDrawPoint( pRenderer, iCenterX + y, iCenterY + x );
		SDL_RenderDrawPoint( pRenderer, iCenterX - y, iCenterY - x );
		SDL_RenderDrawPoint( pRenderer, iCenterX - y, iCenterY + x );

		if (iError <= 0)
		{
			y++;
			iError += ty;
			ty += 2;
		}

		if (iError > 0)
		{
			x--;
			tx += 2;
			iError += (tx - iDiameter);
		}
	}
}

CRenderManager::CRenderManager()
{
	m_pWindow = NULL;
	m_pRenderer = NULL;
	m_pInvalidTexture = NULL;

	m_dRenderZoom = 16.0;
	m_vecRenderOffset = vec_zero;
	m_bScreenLocked = false;

	for (int i = 0; i < MAX_TEXTURES; i++)
		m_pTextures[i] = NULL;

	for (int i = 0; i < MAX_ANIMATIONS; i++)
		m_pAnimations[i] = NULL;

	m_iTextureCount = 0;
	m_iAnimationCount = 0;

	m_vecScreenSize.Init( 1920, 1080 );

	CreateWindow();
}

CRenderManager::~CRenderManager()
{
	DestroyWindow();
}

bool CRenderManager::CreateWindow( void )
{
	if (m_pWindow)
		return false;

	if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 || IMG_Init( IMG_INIT_PNG ) < 0 || TTF_Init() < 0)
		return false;

	m_pWindow = SDL_CreateWindow( "PhysLine Situation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int)m_vecScreenSize.x, (int)m_vecScreenSize.y, SDL_WINDOW_SHOWN );
	if (!m_pWindow)
		return false;

	m_pRenderer = SDL_CreateRenderer( m_pWindow, -1, SDL_RENDERER_ACCELERATED );

	m_pInvalidTexture = GenerateTexture( "Resources/invalid.png" );
	if (!m_pInvalidTexture)
		return false;

	m_pCursors[CURSORTYPE_DEFAULT] = SDL_CreateSystemCursor( SDL_SYSTEM_CURSOR_ARROW );
	m_pCursors[CURSORTYPE_BUTTON] = SDL_CreateSystemCursor( SDL_SYSTEM_CURSOR_HAND );
	m_pCursors[CURSORTYPE_IBEAM] = SDL_CreateSystemCursor( SDL_SYSTEM_CURSOR_IBEAM );
	m_pCursors[CURSORTYPE_NO] = SDL_CreateSystemCursor( SDL_SYSTEM_CURSOR_NO );
	SetCursor( CURSORTYPE_DEFAULT );
	
	// These are the default fonts. These are initialized here so that there are no issues with rendering text before the first fonts are added through 
	AddFont( DEFAULT_FONTTYPE_NORMAL, FONTTYPE_NORMAL );
	AddFont( DEFAULT_FONTTYPE_BOLD, FONTTYPE_ITALIC );
	AddFont( DEFAULT_FONTTYPE_ITALIC, FONTTYPE_BOLD );
	AddFont( DEFAULT_FONTTYPE_SPECIAL_0, FONTTYPE_SPECIAL_0 );
	AddFont( DEFAULT_FONTTYPE_SPECIAL_1, FONTTYPE_SPECIAL_1 );
	AddFont( DEFAULT_FONTTYPE_SPECIAL_2, FONTTYPE_SPECIAL_2 );

	SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );

	return true;
}

void CRenderManager::DestroyWindow( void )
{
	DestroyGraphics();

	for (int i = 0; i < CURSORTYPE_COUNT; i++)
		SDL_FreeCursor( m_pCursors[i] );

	for (int i = 0; i < FONTTYPE_COUNT; i++)
	{
		if (m_pFonts[i])
		{
			TTF_CloseFont( m_pFonts[i] );
			m_pFonts[i] = NULL;
		}
	}

	SDL_DestroyTexture( m_pInvalidTexture );
	m_pInvalidTexture = NULL;

	SDL_DestroyRenderer( m_pRenderer );
	m_pRenderer = NULL;

	SDL_DestroyWindow( m_pWindow );
	m_pWindow = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void CRenderManager::DestroyGraphics( void )
{
	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		if (m_pTextures[i])
		{
			SDL_DestroyTexture( m_pTextures[i] );
			m_pTextures[i] = NULL;
		}
	}

	m_iTextureCount = 0;

	for (int i = 0; i < MAX_ANIMATIONS; i++)
	{
		if (m_pAnimations[i])
		{
			delete m_pAnimations[i];
			m_pAnimations[i] = NULL;
		}
	}

	m_iAnimationCount = 0;
}

void CRenderManager::SetRenderZoom( double dRenderZoom )
{
	if (!m_bScreenLocked)
		m_dRenderZoom = dRenderZoom;
}

void CRenderManager::ScaleRenderZoom( double dFactor )
{
	if (!m_bScreenLocked)
		m_dRenderZoom *= dFactor;
}

void CRenderManager::CopyToRender( CBaseEntity *pEntity )
{
	if (!pEntity->IsVisible())
		return;

	Vector vecSize;
	Vector vecPosition;
	pEntity->CalculateScreenSizeAndPosition( vecSize, vecPosition );

	vecSize *= m_dScreenScale;
	vecPosition *= m_dScreenScale;

	SDL_Rect rEntityRect;
	rEntityRect.x = (int)(vecPosition.x);
	rEntityRect.y = (int)(vecPosition.y);
	rEntityRect.w = (int)(vecSize.x);
	rEntityRect.h = (int)(vecSize.y);

	SDL_Texture *pTexture = pEntity->GetTexture();
	if (!pTexture)
	{
		int iTextureIndex = pEntity->GetTextureIndex();
		if (iTextureIndex >= 0 && iTextureIndex < MAX_TEXTURES)
			pTexture = m_pTextures[iTextureIndex];
		if (!pTexture)
			pTexture = m_pInvalidTexture;
	}

	const SDL_Color &cModulation = pEntity->GetModulation();
	SDL_SetTextureColorMod( pTexture, cModulation.r, cModulation.g, cModulation.b );
	SDL_SetTextureAlphaMod( pTexture, cModulation.a );

	SDL_RenderCopyEx( m_pRenderer, pTexture, NULL, &rEntityRect, -pEntity->GetAngPosition() * 180.0 / M_PI, NULL, SDL_FLIP_NONE );

	if (cb_debug.GetValue())
	{
		CPhysEntity *pPhysEntity = dynamic_cast<CPhysEntity *>(pEntity);
		if (pPhysEntity)
		{
			CPhysObject *pPhysObject = pPhysEntity->GetPhysObject();
			if (pPhysObject)
			{
				SDL_SetRenderDrawColor( m_pRenderer, c_blue.r, c_blue.g, c_blue.b, c_blue.a );

				int iCenterX = (int)(rEntityRect.x + vecSize.x / 2);
				int iCenterY = (int)(rEntityRect.y + vecSize.y / 2);
				UTIL_DrawCircle( m_pRenderer, iCenterX, iCenterY, (int)(pPhysObject->GetColRadius() * m_dRenderZoom) );

				Vector vecVelocity = pPhysObject->GetVelocity();
				WorldToScreenDelta( vecVelocity );
				SDL_RenderDrawLine( m_pRenderer, iCenterX, iCenterY, iCenterX + (int)vecVelocity.x, iCenterY + (int)vecVelocity.y );

				SDL_SetRenderDrawColor( m_pRenderer, c_black.r, c_black.g, c_black.b, c_black.a );
			}
		}
	}
}

void CRenderManager::Render( void )
{
	SDL_RenderPresent( m_pRenderer );
	SDL_RenderClear( m_pRenderer );
}

bool CRenderManager::GenerateTexture( const char *sPath, int iIndex )
{
	if (iIndex < 0 || iIndex >= MAX_TEXTURES)
		return false;

	if (iIndex >= m_iTextureCount)
		m_iTextureCount = iIndex + 1;

	m_pTextures[iIndex] = GenerateTexture( sPath );
	
	return m_pTextures[iIndex] != NULL;
}

bool CRenderManager::AddAnimation( CAnimation *pAnimation, int iIndex )
{
	if (iIndex < 0 || iIndex >= MAX_ANIMATIONS)
		return false;

	if (iIndex >= m_iAnimationCount)
		m_iAnimationCount = iIndex + 1;

	m_pAnimations[iIndex] = pAnimation;
	return true;
}

bool CRenderManager::AddFont( const char *sFontAddresss, FontType_t tFont )
{
	if (m_pFonts[tFont])
		TTF_CloseFont( m_pFonts[tFont] );

	m_pFonts[tFont] = TTF_OpenFont( sFontAddresss, TEXT_SIZE );
	return m_pFonts[tFont] != NULL;
}

bool CRenderManager::IsFontInitialized( FontType_t tFont )
{
	return m_pFonts[tFont] != NULL;
}

SDL_Texture *CRenderManager::GenerateTextTexture( FontType_t tFont, const SDL_Color &cColor, const char *sText )
{
	TTF_Font *pFont = m_pFonts[tFont];
	if (!pFont)
		return NULL;

	SDL_Surface *pSurface = TTF_RenderText_Solid( pFont, sText, cColor );
	if (!pSurface)
		return NULL;

	SDL_Texture *pTexture = SDL_CreateTextureFromSurface( m_pRenderer, pSurface );
	SDL_FreeSurface( pSurface );

	return pTexture;
}

TTF_Font *CRenderManager::GetFont( FontType_t tFont )
{
	return m_pFonts[tFont];
}

CAnimation *CRenderManager::GetAnimation( int iIndex )
{
	if (iIndex < 0 || iIndex >= MAX_ANIMATIONS)
		return NULL;

	return m_pAnimations[iIndex];
}

double CRenderManager::GetRenderZoom( void )
{
	return m_dRenderZoom;
}

int CRenderManager::GetTextureCount( void )
{
	return m_iTextureCount;
}

int CRenderManager::GetAnimationCount( void )
{
	return m_iAnimationCount;
}

void CRenderManager::WorldToScreen( Vector &vecWorld )
{
	vecWorld -= m_vecRenderOffset * m_dScreenScale;

	vecWorld *= m_dRenderZoom;
	vecWorld.y *= -1.0;

	vecWorld += (m_vecScreenSize / (m_dScreenScale * 2.0));
}

void CRenderManager::ScreenToWorld( Vector &vecScreen )
{
	vecScreen -= (m_vecScreenSize / (m_dScreenScale * 2.0));

	vecScreen /= m_dRenderZoom;
	vecScreen.y *= -1.0f;

	vecScreen += m_vecRenderOffset * m_dScreenScale;
}

void CRenderManager::WorldToScreenSize( Vector &vecWorld )
{
	vecWorld *= m_dRenderZoom;
}

void CRenderManager::ScreenToWorldSize( Vector &vecScreen )
{
	vecScreen /= m_dRenderZoom;
}

void CRenderManager::WorldToScreenDelta( Vector &vecWorld )
{
	vecWorld *= m_dRenderZoom;
	vecWorld.y *= -1.0;
}

void CRenderManager::ScreenToWorldDelta( Vector &vecScreen )
{
	vecScreen /= m_dRenderZoom;
	vecScreen.y *= -1.0;
}

void CRenderManager::MoveScreenBy( const Vector &vecDisplacement )
{
	if (!m_bScreenLocked)
		m_vecRenderOffset += vecDisplacement;
}

void CRenderManager::SetScreenPosition( const Vector &vecPosition )
{
	if (!m_bScreenLocked)
		m_vecRenderOffset = vecPosition;
}

const Vector &CRenderManager::GetScreenPosition( void )
{
	return m_vecRenderOffset;
}

void CRenderManager::SetScreenLocked( bool bScreenLocked )
{
	m_bScreenLocked = bScreenLocked;
}

void CRenderManager::SetScreenSize( const Vector &vecScreenSize )
{
	m_vecScreenSize = vecScreenSize;

	m_dScreenScale = vecScreenSize.x / 1920.0;

	if (m_pWindow)
		SDL_SetWindowSize( m_pWindow, (int)m_vecScreenSize.x, (int)m_vecScreenSize.y );
}

const Vector &CRenderManager::GetScreenSize( void )
{
	return m_vecScreenSize;
}

double CRenderManager::GetScreenScale( void )
{
	return m_dScreenScale;
}

void CRenderManager::SetCursor( CursorType_t tCursorType )
{
	SDL_SetCursor( m_pCursors[tCursorType] );
}

void CRenderManager::Reset( void )
{
	DestroyGraphics();

	SetCursor( CURSORTYPE_DEFAULT );
}

SDL_Texture *CRenderManager::GenerateTexture( const char *sPath )
{
	SDL_Texture *newTexture = NULL;
	SDL_Surface *loadedSurface = IMG_Load( sPath );
	if (!loadedSurface)
		return m_pInvalidTexture;

	newTexture = SDL_CreateTextureFromSurface( m_pRenderer, loadedSurface );
	SDL_FreeSurface( loadedSurface );

	if (!newTexture)
		return m_pInvalidTexture;

	return newTexture;
}