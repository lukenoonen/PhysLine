#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include "PreGlobal.h"

typedef bool ( *FnCommandCallbackVoid_t )( void );
typedef bool ( *FnCommandCallback_t )( const CLinkedList<const char *> &command );

class CBaseConCommand
{
public:
	DECLARE_CLASS_NOBASE( CBaseConCommand );

	CBaseConCommand( const char *sName, const char *sHelpString );

	const char *GetName( void );

	virtual void Dispatch( const CLinkedList<const char *> &command );
	void Help( void );

private:
	const char *m_sName;
	const char *m_sHelpString;
};

class CConDouble : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConDouble, CBaseConCommand );

	CConDouble( const char *sName, double dDefaultValue, const char *sHelpString = NULL );

	virtual void Dispatch( const CLinkedList<const char *> &command );

	double GetValue( void );

private:

	double m_dValue;
};

class CConInt : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConInt, CBaseConCommand );

	CConInt( const char *sName, int iDefaultValue, const char *sHelpString = NULL );

	virtual void Dispatch( const CLinkedList<const char *> &command );

	int GetValue( void );

private:

	int m_iValue;
};

class CConBool : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConBool, CBaseConCommand );

	CConBool( const char *sName, bool bDefaultValue, const char *sHelpString = NULL );

	virtual void Dispatch( const CLinkedList<const char *> &command );

	bool GetValue( void );

private:

	bool m_bValue;
};

class CConVector : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConVector, CBaseConCommand );

	CConVector( const char *sName, const Vector &vecDefaultValue, const char *sHelpString = NULL );

	virtual void Dispatch( const CLinkedList<const char *> &command );

	const Vector &GetValue( void );

private:

	Vector m_vecValue;
};

class CConCommand : public CBaseConCommand
{
public:
	DECLARE_CLASS( CConCommand, CBaseConCommand );

	CConCommand( const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid, const char *sHelpString = NULL );
	CConCommand( const char *sName, FnCommandCallback_t fnCommandCallback, const char *sHelpString = NULL );

	virtual void Dispatch( const CLinkedList<const char *> &command );

private:

	FnCommandCallbackVoid_t m_fnCommandCallbackVoid;
	FnCommandCallback_t m_fnCommandCallback;
};

class CCommandProcesser
{
public:
	DECLARE_CLASS_NOBASE( CCommandProcesser );

	static void ProcessCommand( const char *sCommand );

	static void AddCommand( CBaseConCommand *pCommand );

private:
	static void ProcessSingleCommand( char *sCommand );
	static void FixCommand( char *sCommand, CLinkedList<const char *> &command );

private:
	static CLinkedList<CBaseConCommand *> *s_pCommands;

private:
	// This is here since there shouldn't be a way to make an instance of this class
	CCommandProcesser() {}
};

#endif // COMMANDMANAGER_H