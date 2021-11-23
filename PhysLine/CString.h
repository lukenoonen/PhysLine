#ifndef CSTRING_H
#define CSTRING_H

#include "DataStructures.h"
#include <istream>

//===========================================================================================
// CString
//===========================================================================================

class CString
{
public:
	CString();
	CString( char cChar );
	CString( const char *sStr );
	~CString();

	void Clear( void );

	void Add( const char *sStr );
	void Add( char cChar );

	void Add( const char *sStr, int iIndex );
	void Add( char cChar, int iIndex );
	
	void Set( const char *sStr );
	void Set( char cChar );

	void Backspace( int iIndex );
	void Delete( int iIndex );

	bool Equals( const char *sStr );
	bool EqualsN( const char *sStr );

	int Length( void );

	const char *Get( void );

	bool operator==( const char *sStr );
	bool operator!=( const char *sStr );

private:
	CLinkedList<char> m_Str;
	char *m_sStoredStr;
	
public:
	friend std::istream &operator>>( std::istream &input, CString &sStr );
};

#endif // CSTRING_H