#include "TextInput.h"

CTextInput::CTextInput()
{
	m_bChanged = false;
	m_iIndex = 0;
	m_bComplete = false;
	m_bFailed = false;
}

void CTextInput::AddString( const char *sString )
{
	if (m_bNumbersOnly)
	{
		for (const char *sSearch = sString; *sSearch; sSearch++)
		{
			if (*sSearch >= '0' && *sSearch <= '9')
			{
				m_sString.Add( sString, m_iIndex );
				Move( 1 );
			}
		}
	}
	else
	{
		m_sString.Add( sString, m_iIndex );
		Move( UTIL_strlen( sString ) );
	}

	if (m_iMaxCharacters > 0)
	{
		while (m_sString.Length() > m_iMaxCharacters)
			m_sString.Backspace( m_sString.Length() );
	}

	m_bChanged = true;
}

void CTextInput::Backspace( void )
{
	m_sString.Backspace( m_iIndex );
	m_bChanged = true;
	Move( -1 );
}

void CTextInput::Delete( void )
{
	m_sString.Delete( m_iIndex );
	m_bChanged = true;
}

void CTextInput::Move( int iDirection )
{
	m_iIndex += iDirection;
	if (m_iIndex < 0)
		m_iIndex = 0;
	else if (m_iIndex > m_sString.Length())
		m_iIndex = m_sString.Length();
	m_bChanged = true;
}

void CTextInput::Clear( void )
{
	m_sString.Clear();
	m_iIndex = 0;
	m_bChanged = true;
}

bool CTextInput::HasChanged( void )
{
	return m_bChanged;
}

const char *CTextInput::GetString( void )
{
	m_bChanged = false;
	return m_sString.Get();
}

void CTextInput::MarkComplete( void )
{
	m_bComplete = true;
}

void CTextInput::MarkFailed( void )
{
	m_bComplete = true;
	m_bFailed = true;
}

bool CTextInput::IsFailed( void )
{
	return m_bFailed && m_bComplete;
}

bool CTextInput::IsSuccess( void )
{
	return !m_bFailed && m_bComplete;
}

void CTextInput::Reset( void )
{
	m_bComplete = false;
	m_bFailed = false;
	m_bChanged = false;
}

double CTextInput::GetDistanceToIndex( FontType_t tFontType )
{
	int iWidth;
	int iHeight;
	char *sText = UTIL_stredit( GetString() );
	sText[m_iIndex] = '\0';
	TTF_SizeText( pRenderManager->GetFont( tFontType ), sText, &iWidth, &iHeight );
	delete[] sText;
	return (double)iWidth;
}

void CTextInput::SetIndexToDistance( FontType_t tFontType, double dDistance )
{
	m_iIndex = m_sString.Length();
	while (m_iIndex > 0)
	{
		if (GetDistanceToIndex( tFontType ) <= dDistance)
			break;

		m_iIndex--;
	}
	m_bChanged = true;
}

void CTextInput::SetNumbersOnly( bool bNumbersOnly )
{
	m_bNumbersOnly = bNumbersOnly;
}

void CTextInput::SetMaxCharacters( int iMaxCharacters )
{
	m_iMaxCharacters = iMaxCharacters;
}