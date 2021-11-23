#include "CString.h"
#include "UTIL.h"

CString::CString()
{
	m_sStoredStr = NULL;
}

CString::CString( char cChar )
{
	m_sStoredStr = NULL;
	Add( cChar );
}

CString::CString( const char *sStr )
{
	m_sStoredStr = NULL;
	Add( sStr );
}

CString::~CString()
{
	if (m_sStoredStr)
		delete[] m_sStoredStr;
}

void CString::Clear( void )
{
	m_Str.RemoveAll();

	if (m_sStoredStr)
	{
		delete[] m_sStoredStr;
		m_sStoredStr = NULL;
	}
}

void CString::Add( const char *sStr )
{
	while (*sStr)
	{
		Add( *sStr );
		sStr++;
	}
}

void CString::Add( char cChar )
{
	m_Str.AddToTail( cChar );

	if (m_sStoredStr)
	{
		delete[] m_sStoredStr;
		m_sStoredStr = NULL;
	}
}

void CString::Add( const char *sStr, int iIndex )
{
	while (*sStr)
	{
		Add( *sStr, iIndex++ );
		sStr++;
	}
}

void CString::Add( char cChar, int iIndex )
{
	m_Str.Add( cChar, iIndex );

	if (m_sStoredStr)
	{
		delete[] m_sStoredStr;
		m_sStoredStr = NULL;
	}
}

void CString::Set( const char *sStr )
{
	Clear();
	Add( sStr );
}

void CString::Set( char cChar )
{
	Clear();
	Add( cChar );
}

void CString::Backspace( int iIndex )
{
	if (!m_Str.Remove( iIndex - 1 ))
		return;

	if (m_sStoredStr)
	{
		delete[] m_sStoredStr;
		m_sStoredStr = NULL;
	}
}

void CString::Delete( int iIndex )
{
	if (!m_Str.Remove( iIndex ))
		return;

	if (m_sStoredStr)
	{
		delete[] m_sStoredStr;
		m_sStoredStr = NULL;
	}
}

bool CString::Equals( const char *sStr )
{
	CIterator<char> iterator = m_Str.StartIterating();
	while (iterator.Iterating())
	{
		if (iterator.Iterate() != *sStr++)
			return false;
	}

	return !*sStr;
}

bool CString::EqualsN( const char *sStr )
{
	CIterator<char> iterator = m_Str.StartIterating();
	while (iterator.Iterating())
	{
		if (UTIL_clower( iterator.Iterate() ) != UTIL_clower( *sStr++ ))
			return false;
	}

	return !*sStr;
}

int CString::Length( void )
{
	return m_Str.Count();
}

const char *CString::Get( void )
{
	if (!m_sStoredStr)
	{
		m_sStoredStr = new char[Length() + 1];

		int iIndex = 0;
		CIterator<char> iterator = m_Str.StartIterating();
		while (iterator.Iterating())
			m_sStoredStr[iIndex++] = iterator.Iterate();

		m_sStoredStr[iIndex] = '\0';
	}

	return m_sStoredStr;
}

bool CString::operator==( const char *sStr )
{
	return Equals( sStr );
}

bool CString::operator!=( const char *sStr )
{
	return !Equals( sStr );
}

std::istream &operator>>( std::istream &input, CString &sStr )
{
	bool bSkip = false;
	sStr.Clear();
	char c = input.get();
	while (c == '\n' || c == ' ' || c == '\t')
		c = input.get();
	while ((bSkip || (c != '\n' && c != ' ' && c != '\t') && c != '\0') && c != EOF)
	{
		if (c == '\"')
			bSkip = !bSkip;
		else
			sStr.Add( c );

		c = input.get();
	}
	return input;
}