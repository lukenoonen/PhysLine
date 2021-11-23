#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Global.h"

class CFileManager
{
public:
	DECLARE_CLASS_NOBASE( CFileManager );

	CFileManager();
	~CFileManager();

	void Load( const char *sFileName );
	void Save( const char *sFileName );
	void New( const char *sFileName );
	void Copy( const char *sFileNameSrc, const char *sFileNameDest );
	void Quicksave( void );
	
	void ResetConfigs( void );
	void ResetGraphics( void );

private:
	bool LoadConfig( const char *sFileName );
	bool LoadSituation( const char *sFileName );
	bool LoadGraphics( const char *sFileName );

	char *m_sSituationName;
	CLinkedList<char *> m_sSituationConfigs;
	CLinkedList<char *> m_sSituationGraphics;
};

#endif // FILEMANAGER_H