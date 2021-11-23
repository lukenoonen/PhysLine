#ifndef TEXTINPUT_H
#define TEXTINPUT_H

#include "Global.h"
#include "CString.h"
#include "RenderManager.h"

class CTextInput
{
public:
	DECLARE_CLASS_NOBASE( CTextInput );

	CTextInput();

	void AddString( const char *sString );
	void Backspace( void );
	void Delete( void );
	void Move( int iDirection );
	void Clear( void );

	bool HasChanged( void );
	const char *GetString( void );

	void MarkComplete( void );
	void MarkFailed( void );
	bool IsFailed( void );
	bool IsSuccess( void );
	void Reset( void );

	double GetDistanceToIndex( FontType_t tFontType );
	void SetIndexToDistance( FontType_t tFontType, double dDistance );
	
	void SetNumbersOnly( bool bNumbersOnly );

	void SetMaxCharacters( int iMaxCharacters );

private:
	bool m_bChanged;
	CString m_sString;
	int m_iIndex;
	bool m_bComplete;
	bool m_bFailed;
	bool m_bNumbersOnly;
	int m_iMaxCharacters;
};

#endif // TEXTINPUT_H