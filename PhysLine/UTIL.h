#ifndef UTIL_H
#define UTIL_H

// String stuff
char		UTIL_clower( char c );
int			UTIL_strlen( const char *sStr );
int			UTIL_strcmp( const char *sStr1, const char *sStr2 );
int			UTIL_strncmp( const char *sStr1, const char *sStr2 );
bool		UTIL_strinc( const char *sStr1, const char *sStr2 );
bool		UTIL_strninc( const char *sStr1, const char *sStr2 );
void		UTIL_strcpy( char *sDest, const char *sSource );
void		UTIL_strncpy( char *sDest, const char *sSource, int iSize );
char *		UTIL_stredit( const char *sSource );
void		UTIL_stradd( char *sDest, const char *sSource1, const char *sSource2 );
const char *UTIL_strstr( const char *sStr1, const char *sStr2 );
char *		UTIL_strstr( char *sStr1, const char *sStr2 );
const char *UTIL_strnstr( const char *sStr1, const char *sStr2 );
char *		UTIL_strnstr( char *sStr1, const char *sStr2 );
const char *UTIL_strchr( const char *sStr, const char cChar );
char *		UTIL_strchr( char *sStr, const char cChar );
const char *UTIL_strchr( const char *sStr, const char cChar, const char cIgnore );
char *		UTIL_strchr( char *sStr, const char cChar, const char cIgnore );
const char *UTIL_strchr( const char *sStr, const char *sChar );
char *		UTIL_strchr( char *sStr, const char *sChar );
const char *UTIL_strnchr( const char *sStr, const char cChar );
char *		UTIL_strnchr( char *sStr, const char cChar );
const char *UTIL_extn( const char *sFileName );
int			UTIL_atoi( const char *sStr );
int			UTIL_atoi( const char *sStr, int iLen );
double		UTIL_atod( const char *sStr );
double		UTIL_atod( const char *sStr, int iLen );
int			UTIL_distc( const char *sStr, const char cChar );

// Sorting stuff
template <class T>
void UTIL_swap( T &t1, T &t2 )
{
	T tTemp = t1;
	t1 = t2;
	t2 = tTemp;
}

#endif // UTIL_H