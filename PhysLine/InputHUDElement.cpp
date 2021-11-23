#include "InputHUDElement.h"
#include "RenderManager.h"
#include "InputManager.h"

CInputHUDElement::CInputHUDElement( int iRenderOrder, bool bVisible, const Vector &vecPosition, bool bWorld, double dDuration, Justification_t tJustification, double dScale, FontType_t tFont, const SDL_Color &cColor, const char *sDefaultText, bool bNumbersOnly, int iMaxCharacters ) : BaseClass( -1, 0.0, iRenderOrder, bVisible, true, vec_zero, vecPosition, bWorld, dDuration, tJustification )
{
	m_pTextInput = new CTextInput();
	m_dScale = dScale;
	m_tFont = tFont;
	m_cColor = cColor;
	
	m_pTextTexture = NULL;
	m_sDefaultText = sDefaultText ? UTIL_stredit( sDefaultText ) : NULL;
	SetText( sDefaultText );

	m_pTextCursor = new CTextHUDElement( 0, false, vec_zero, false, DURATION_INFINITE, tJustification, dScale, tFont, cColor, "|" );

	m_bHasFocus = false;

	m_sStoredText = NULL;

	SetNumbersOnly( bNumbersOnly );

	SetMaxCharacters( iMaxCharacters );
}

CInputHUDElement::~CInputHUDElement()
{
	delete m_pTextInput;
	SDL_DestroyTexture( m_pTextTexture );
	delete m_pTextCursor;

	if (m_sDefaultText)
		delete[] m_sDefaultText;
}

void CInputHUDElement::OnLoop( void )
{
	if (m_bHasFocus)
	{
		if (m_pTextInput->HasChanged())
		{
			SetText( m_pTextInput->GetString() );
			m_pTextCursor->SetPosition( GetPosition() + vec_right * (m_pTextInput->GetDistanceToIndex( m_tFont ) * m_dScale - m_pTextCursor->GetSize().x / 2.0) );
			m_pTextCursor->SetVisible( true );
		}

		if (m_dBlinkTime < pGlobalValues->GetTimeNoScale())
		{
			m_pTextCursor->SetVisible( !m_pTextCursor->IsVisible() );
			m_dBlinkTime = pGlobalValues->GetTimeNoScale() + TEXT_CURSOR_BLINK_TIME;
		}

		if (m_pTextInput->IsSuccess())
		{
			if (m_sStoredText)
				delete[] m_sStoredText;

			m_sStoredText = UTIL_stredit( m_pTextInput->GetString() );

			m_pTextInput->Reset();
			m_pTextInput->Clear();
			m_bHasFocus = false;
			m_pTextCursor->SetVisible( false );
			SetText( m_sDefaultText );
		}
		else if (m_pTextInput->IsFailed())
		{
			m_pTextInput->Reset();
			m_bHasFocus = false;
			m_pTextCursor->SetVisible( false );
			
			if (!*m_pTextInput->GetString())
				SetText( m_sDefaultText );
		}
	}
}

void CInputHUDElement::CopyToRender( void )
{
	BaseClass::CopyToRender();

	if (IsVisible())
		m_pTextCursor->CopyToRender();
}

void CInputHUDElement::OnStartHover( void )
{
	pRenderManager->SetCursor( CURSORTYPE_IBEAM );
}

void CInputHUDElement::OnEndHover( void )
{
	pRenderManager->SetCursor( CURSORTYPE_DEFAULT );
}

void CInputHUDElement::OnSelected( void )
{
	if (!m_bHasFocus)
	{
		m_pTextCursor->SetVisible( true );
		m_dBlinkTime = pGlobalValues->GetTimeNoScale() + TEXT_CURSOR_BLINK_TIME;
		pInputManager->SetTextInput( m_pTextInput );
		m_bHasFocus = true;
	}

	m_pTextInput->SetIndexToDistance( m_tFont, (pInputManager->GetMousePosition().x - GetPosition().x) / m_dScale + TEXT_SIZE_D / 4.0 );
}

SDL_Texture *CInputHUDElement::GetTexture( void )
{
	return m_pTextTexture;
}

const char *CInputHUDElement::GetString( void )
{
	return m_sStoredText;
}

void CInputHUDElement::ClearString( void )
{
	if (m_sStoredText)
	{
		delete[] m_sStoredText;
		m_sStoredText = NULL;
	}
}

void CInputHUDElement::SetNumbersOnly( bool bNumbersOnly )
{
	m_pTextInput->SetNumbersOnly( bNumbersOnly );
}

void CInputHUDElement::SetMaxCharacters( int iMaxCharacters )
{
	m_pTextInput->SetMaxCharacters( iMaxCharacters );
}

void CInputHUDElement::SetText( const char *sText )
{
	if (m_pTextTexture)
		SDL_DestroyTexture( m_pTextTexture );
		
	m_pTextTexture = pRenderManager->GenerateTextTexture( m_tFont, m_cColor, sText );
	
	if (!m_pTextTexture)
	{
		SetSize( vec_zero );
		return;
	}

	int iWidth;
	int iHeight;
	SDL_QueryTexture( m_pTextTexture, NULL, NULL, &iWidth, &iHeight );
	Vector vecNewSize( (double)iWidth, (double)iHeight );
	vecNewSize *= m_dScale;
	if (IsWorld())
		vecNewSize /= TEXT_SIZE_D;

	SetSize( vecNewSize );
}