#include "CommandProcessor.h"
#include "HUDManager.h"

CBaseConCommand::CBaseConCommand( const char *sName, const char *sHelpString )
{
	m_sName = sName;
	m_sHelpString = sHelpString;
}

const char *CBaseConCommand::GetName( void )
{
	return m_sName;
}

void CBaseConCommand::Dispatch( const CLinkedList<const char *> &command )
{

}

void CBaseConCommand::Help( void )
{
	if (m_sHelpString)
		pHUDManager->AddConsoleEntry( FONTTYPE_NORMAL, c_red, "'%s' Usage: %s", m_sName, m_sHelpString );
}

CConDouble::CConDouble( const char *sName, double dDefaultValue, const char *sHelpString /*= NULL*/ ) : BaseClass( sName, sHelpString )
{
	m_dValue = dDefaultValue;

	CCommandProcesser::AddCommand( this );
}

void CConDouble::Dispatch( const CLinkedList<const char *> &command )
{
	if (command.Count() > 0)
		m_dValue = UTIL_atod( command.GetHead() );
	else
		Help();
}

double CConDouble::GetValue( void )
{
	return m_dValue;
}

CConInt::CConInt( const char *sName, int iDefaultValue, const char *sHelpString /*= NULL*/ ) : BaseClass( sName, sHelpString )
{
	m_iValue = iDefaultValue;

	CCommandProcesser::AddCommand( this );
}

void CConInt::Dispatch( const CLinkedList<const char *> &command )
{
	if (command.Count() > 0)
		m_iValue = UTIL_atoi( command.GetHead() );
	else
		Help();
}

int CConInt::GetValue( void )
{
	return m_iValue;
}

CConBool::CConBool( const char *sName, bool bDefaultValue, const char *sHelpString /*= NULL*/ ) : BaseClass( sName, sHelpString )
{
	m_bValue = bDefaultValue;

	CCommandProcesser::AddCommand( this );
}

void CConBool::Dispatch( const CLinkedList<const char *> &command )
{
	if (command.Count() > 0)
		m_bValue = UTIL_atoi( command.GetHead() ) != 0;
	else
		Help();
}

bool CConBool::GetValue( void )
{
	return m_bValue;
}

CConVector::CConVector( const char *sName, const Vector &bDefaultValue, const char *sHelpString /*= NULL*/ ) : BaseClass( sName, sHelpString )
{
	m_vecValue = bDefaultValue;

	CCommandProcesser::AddCommand( this );
}

void CConVector::Dispatch( const CLinkedList<const char *> &command )
{
	if (command.Count() > 1)
	{
		CIterator<const char *> iterator = command.StartIterating();
		m_vecValue.x = UTIL_atod( iterator.Iterate() );
		m_vecValue.y = UTIL_atod( iterator.Iterate() );
	}
	else
	{
		Help();
	}
}

const Vector &CConVector::GetValue( void )
{
	return m_vecValue;
}

CConCommand::CConCommand(const char *sName, FnCommandCallbackVoid_t fnCommandCallbackVoid, const char *sHelpString /*= NULL*/) : BaseClass( sName, sHelpString )
{
	m_fnCommandCallbackVoid = fnCommandCallbackVoid;
	m_fnCommandCallback = NULL;
	
	CCommandProcesser::AddCommand( this );
}

CConCommand::CConCommand( const char *sName, FnCommandCallback_t fnCommandCallback, const char *sHelpString /*= NULL*/ ) : BaseClass( sName, sHelpString )
{
	m_fnCommandCallbackVoid = NULL;
	m_fnCommandCallback = fnCommandCallback;

	CCommandProcesser::AddCommand( this );
}

void CConCommand::Dispatch( const CLinkedList<const char *> &command )
{
	if (m_fnCommandCallbackVoid)
	{
		if (!(*m_fnCommandCallbackVoid)())
			Help();
	}
	else if (m_fnCommandCallback)
	{
		if (!(*m_fnCommandCallback)( command ))
			Help();
	}
}

CLinkedList<CBaseConCommand *> *CCommandProcesser::s_pCommands = NULL;

void CCommandProcesser::ProcessCommand( const char *sCommand )
{
	char *sCommandEditable = UTIL_stredit( sCommand );

	char *sSearch = sCommandEditable;
	char *sChar = sCommandEditable;

	bool bSkip = false;

	while (true)
	{
		sChar = UTIL_strchr( sChar, "\";\n" );

		if (!sChar)
			break;

		if (*sChar == '\"')
		{
			bSkip = !bSkip;
			sChar++;
		}
		else
		{
			if (!bSkip)
			{
				*sChar++ = '\0';

				if (*sSearch)
					ProcessSingleCommand( sSearch );

				sSearch = sChar;
			}
			else
			{
				sChar++;
			}
		}
	}
	
	if (*sSearch)
		ProcessSingleCommand( sSearch );

	delete[] sCommandEditable;
}

void CCommandProcesser::AddCommand( CBaseConCommand *pCommand )
{
	if (!s_pCommands)
		s_pCommands = new CLinkedList<CBaseConCommand *>();

	CIterator<CBaseConCommand *> iterator = s_pCommands->StartIterating();
	while (iterator.Iterating())
	{
		if (iterator.Iterate()->GetName() == pCommand->GetName())
			return;
	}

	s_pCommands->AddToTail( pCommand );
}

void CCommandProcesser::ProcessSingleCommand( char *sCommand )
{
	if (!s_pCommands)
	{
		pHUDManager->AddConsoleEntry( FONTTYPE_NORMAL, c_red, "Warning: no console commands!" );
		return;
	}

	CLinkedList<const char *> command;
	FixCommand( sCommand, command );

	CIterator<CBaseConCommand *> iterator = s_pCommands->StartIterating();
	while (iterator.Iterating())
	{
		CBaseConCommand *pBaseConCommand = iterator.Iterate();

		if (UTIL_strcmp( pBaseConCommand->GetName(), command.GetHead() ) == 0)
		{
			command.RemoveFromHead();
			pBaseConCommand->Dispatch( command );
			return;
		}
	}

	pHUDManager->AddConsoleEntry( FONTTYPE_NORMAL, c_red, "Warning: unknown command '%s'", command.GetHead() );
}

void CCommandProcesser::FixCommand( char *sCommand, CLinkedList<const char *> &command )
{
	const char *sToAdd;
	bool bPreviousWasQuote = false;
	while (true)
	{
		char *sChar;
		if (bPreviousWasQuote)
			sChar = UTIL_strchr( sCommand, '\"' );
		else
			sChar = UTIL_strchr( sCommand, " \"" );

		if (!sChar)
			break;

		if (*sChar == '\"' && !bPreviousWasQuote)
			bPreviousWasQuote = true;
		else
			bPreviousWasQuote = false;

		*sChar++ = '\0';

		if (*sCommand)
		{
			sToAdd = sCommand;
			command.AddToTail( sToAdd );
		}

		sCommand = sChar;
	}

	if (*sCommand)
	{
		sToAdd = sCommand;
		command.AddToTail( sToAdd );
	}
}