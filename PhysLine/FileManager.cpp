#include "FileManager.h"
#include <fstream>
#include "CString.h"
#include "RenderManager.h"
#include "PlayerManager.h"
#include "HUDManager.h"
#include "EntityManager.h"
#include "AudioManager.h"
#include "BaseEntity.h"
#include "StaticEntity.h"
#include "PhysEntity.h"
#include "TextButtonHUDElement.h"
#include <Windows.h>
#include <iomanip>

bool CC_Load( const CLinkedList<const char *> &command )
{
	if (command.Count() == 0)
	{
		char filename[MAX_PATH];
		ZeroMemory( &filename, sizeof( filename ) );

		OPENFILENAME ofn;
		ZeroMemory( &ofn, sizeof( ofn ) );
		ofn.lStructSize = sizeof( ofn );
		ofn.hwndOwner = NULL;
		ofn.lpstrFilter = "Situation Files\0*.sit\0Config Files\0*.cfg\0Graphics Files\0*.gph";
		ofn.lpstrFile = filename;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = "Select a File";
		ofn.lpstrDefExt = ".sit";
		ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA( &ofn ))
			pFileManager->Load( filename );
	}
	else
	{
		pFileManager->Load( command.GetHead() );
	}
	return true;
}
CConCommand cc_load( "load", CC_Load, "load (file name)" );

bool CC_Save( const CLinkedList<const char *> &command )
{
	if (command.Count() == 0)
	{
		char filename[MAX_PATH];
		ZeroMemory( &filename, sizeof( filename ) );

		OPENFILENAME sfn;
		ZeroMemory( &sfn, sizeof( sfn ) );
		sfn.lStructSize = sizeof( sfn );
		sfn.hwndOwner = NULL;
		sfn.lpstrFilter = "Situation Files\0*.sit";
		sfn.lpstrFile = filename;
		sfn.nMaxFile = MAX_PATH;
		sfn.lpstrTitle = "Save a File";
		sfn.lpstrDefExt = ".sit";
		sfn.Flags = OFN_DONTADDTORECENT | OFN_NOCHANGEDIR;

		if (GetSaveFileNameA( &sfn ))
			pFileManager->Save( filename );
	}
	else
	{
		pFileManager->Save( command.GetHead() );
	}
	return true;
}
CConCommand cc_save( "save", CC_Save, "save (file name)" );

bool CC_New( const CLinkedList<const char *> &command )
{
	if (command.Count() == 0)
	{
		char filename[MAX_PATH];
		ZeroMemory( &filename, sizeof( filename ) );

		OPENFILENAME sfn;
		ZeroMemory( &sfn, sizeof( sfn ) );
		sfn.lStructSize = sizeof( sfn );
		sfn.hwndOwner = NULL;
		sfn.lpstrFilter = "Situation Files\0*.sit";
		sfn.lpstrFile = filename;
		sfn.nMaxFile = MAX_PATH;
		sfn.lpstrTitle = "Create a File";
		sfn.lpstrDefExt = ".sit";
		sfn.Flags = OFN_DONTADDTORECENT | OFN_NOCHANGEDIR;

		if (GetSaveFileNameA( &sfn ))
			pFileManager->New( filename );
	}
	else
	{
		pFileManager->New( command.GetHead() );
	}
	return true;
}
CConCommand cc_new( "new", CC_New, "new (file name)" );

bool CC_Copy( const CLinkedList<const char *> &command )
{
	if (command.Count() == 0)
	{
		char srcfilename[MAX_PATH];
		ZeroMemory( &srcfilename, sizeof( srcfilename ) );

		OPENFILENAME sfn;
		ZeroMemory( &sfn, sizeof( sfn ) );
		sfn.lStructSize = sizeof( sfn );
		sfn.hwndOwner = NULL;
		sfn.lpstrFilter = "Situation Files\0*.sit";
		sfn.lpstrFile = srcfilename;
		sfn.nMaxFile = MAX_PATH;
		sfn.lpstrTitle = "Select a Source File";
		sfn.lpstrDefExt = ".sit";
		sfn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA( &sfn ))
		{
			char destfilename[MAX_PATH];
			ZeroMemory( &destfilename, sizeof( destfilename ) );

			sfn.lpstrFile = destfilename;
			sfn.lpstrTitle = "Create a File";
			sfn.Flags = OFN_DONTADDTORECENT | OFN_NOCHANGEDIR;

			if (GetSaveFileNameA( &sfn ))
				pFileManager->Copy( srcfilename, destfilename );
		}
	}
	else if (command.Count() == 1)
	{
		char destfilename[MAX_PATH];
		ZeroMemory( &destfilename, sizeof( destfilename ) );

		OPENFILENAME sfn;
		ZeroMemory( &sfn, sizeof( sfn ) );
		sfn.lStructSize = sizeof( sfn );
		sfn.hwndOwner = NULL;
		sfn.lpstrFilter = "Situation Files\0*.sit";
		sfn.lpstrFile = destfilename;
		sfn.nMaxFile = MAX_PATH;
		sfn.lpstrTitle = "Create a File";
		sfn.lpstrDefExt = ".sit";
		sfn.Flags = OFN_DONTADDTORECENT | OFN_NOCHANGEDIR;

		if (GetSaveFileNameA( &sfn ))
			pFileManager->Copy( command.GetHead(), destfilename );
	}
	else
	{
		CIterator<const char *> iterator = command.StartIterating();

		const char *sFileNameSrc = iterator.Iterate();
		const char *sFileNameDest = iterator.Iterate();
		pFileManager->Copy( sFileNameSrc, sFileNameDest );
	}
	return true;
}
CConCommand cc_copy( "copy", CC_Copy, "copy (source file name) (destination file name)" );

bool CC_Quicksave( void )
{
	pFileManager->Quicksave();
	return true;
}
CConCommand cc_quicksave( "quicksave", CC_Quicksave );

bool CC_ResetConfig( void )
{
	pFileManager->ResetConfigs();
	return true;
}
CConCommand cc_resetcfg( "resetcfg", CC_ResetConfig );

bool CC_ResetGraphics( void )
{
	pFileManager->ResetGraphics();
	return true;
}
CConCommand cc_resetgph( "resetgph", CC_ResetGraphics );

CFileManager::CFileManager()
{
	m_sSituationName = NULL;
}

CFileManager::~CFileManager()
{
	if (m_sSituationName)
		delete m_sSituationName;

	ResetConfigs();
	ResetGraphics();
}

void CFileManager::Load( const char *sFileName )
{
	const char *pExtension = UTIL_extn( sFileName );

	bool bSuccess = false;

	if (pExtension)
	{
		if (UTIL_strcmp( "cfg", pExtension ) == 0)
			bSuccess = LoadConfig( sFileName );
		else if (UTIL_strcmp( "sit", pExtension ) == 0)
			bSuccess = LoadSituation( sFileName );
		else if (UTIL_strcmp( "gph", pExtension ) == 0)
			bSuccess = LoadGraphics( sFileName );
	}

	if (!bSuccess)
		pHUDManager->AddConsoleEntry( FONTTYPE_NORMAL, c_red, "Unable to load file %s!", sFileName );
}

void CFileManager::Save( const char *sFileName )
{
	std::fstream fSaveFile( sFileName, std::ios::out );

	fSaveFile << std::setprecision( std::numeric_limits<double>::digits10 + 1 );

	fSaveFile << "head\n{\n\tconfig\n\t{\n";

	CIterator<char *> iterator = m_sSituationConfigs.StartIterating();
	while (iterator.Iterating())
		fSaveFile << "\t\t" << iterator.Iterate() << '\n';

	fSaveFile << "\t}\n\tgraphics\n\t{\n";

	iterator = m_sSituationGraphics.StartIterating();
	while (iterator.Iterating())
		fSaveFile << "\t\t" << iterator.Iterate() << '\n';

	const Vector &vecCameraPosition = pRenderManager->GetScreenPosition();
	double dCameraZoom = pRenderManager->GetRenderZoom();
	fSaveFile << "\t}\n\tcamera\n\t{\n\t\t-position " << vecCameraPosition.x << ' ' << vecCameraPosition.y << "\n\t\t-zoom " << dCameraZoom << "\n\t}\n}\nbody\n{\n";

	CIterator<CBaseEntity *> entityIterator = pEntityManager->GetEntityIterator();
	while (entityIterator.Iterating())
	{
		CBaseEntity *pEntity = entityIterator.Iterate();

		if (pEntity->IsPermanent())
			continue;

		int iAnimationIndex = pEntity->GetAnimationController()->GetAnimationIndex();
		double dFrame = pEntity->GetAnimationController()->GetDecimalFrame();
		int iRenderOrder = pEntity->GetRenderOrder();
		bool bVisible = pEntity->IsVisible();
		const Vector &vecSize = pEntity->GetSize();
		const Vector &vecPosition = pEntity->GetPosition();

		{
			CPhysEntity *pPhysEntity = dynamic_cast<CPhysEntity *>(pEntity);
			if (pPhysEntity)
			{
				CPhysObject *pPhysObject = pPhysEntity->GetPhysObject();

				double dMass = pPhysObject->GetMass();
				double dElasticity = pPhysObject->GetElasticity();
				bool bStatic = pPhysObject->IsStatic();
				const Vector &vecVelocity = pPhysObject->GetVelocity();
				double dAngPosition = pPhysObject->GetAngPosition() * 180.0 / M_PI;
				double dDragCoefficient = pPhysObject->GetDragCoefficient();

				fSaveFile << "\tphysics\n\t{\n\t\t-animation " << iAnimationIndex << "\n\t\t-frame " << dFrame << "\n\t\t-order " << iRenderOrder << "\n\t\t-visible " << bVisible << "\n\t\t-size " << vecSize.x << ' ' << vecSize.y << "\n\t\t-position " << vecPosition.x << ' ' << vecPosition.y;
				fSaveFile << "\n\t\t-mass " << dMass << "\n\t\t-elasticity " << dElasticity << "\n\t\t-static " << bStatic << "\n\t\t-velocity " << vecVelocity.x << ' ' << vecVelocity.y << "\n\t\t-angposition " << dAngPosition << "\n\t\t-dragcoeff " << dDragCoefficient;
				fSaveFile << "\n\t}\n";

				continue;
			}
		}

		{
			CStaticEntity *pStaticEntity = dynamic_cast<CStaticEntity *>(pEntity);
			if (pStaticEntity)
			{
				double dAngPosition = pStaticEntity->GetAngPosition();

				fSaveFile << "\tstatic\n\t{\n\t\t-animation " << iAnimationIndex << "\n\t\t-frame " << dFrame << "\n\t\t-order " << iRenderOrder << "\n\t\t-visible " << bVisible << "\n\t\t-size " << vecSize.x << ' ' << vecSize.y << "\n\t\t-position " << vecPosition.x << ' ' << vecPosition.y;
				fSaveFile << "\n\t\t-angposition " << dAngPosition;
				fSaveFile << "\n\t}\n";

				continue;
			}
		}

		{
			CTextButtonHUDElement *pTextButtonHUDElement = dynamic_cast<CTextButtonHUDElement *>(pEntity);
			if (pTextButtonHUDElement)
			{
				bool bWorld = pTextButtonHUDElement->IsWorld();
				double dDuration = pTextButtonHUDElement->GetDuration();
				const char *sCommand = pTextButtonHUDElement->GetCommand();
				bool bDeleteOnSelect = pTextButtonHUDElement->GetDeleteOnSelect();
				double dScale = pTextButtonHUDElement->GetTextElement()->GetScale();
				const char *sText = pTextButtonHUDElement->GetTextElement()->GetText();

				fSaveFile << "\tbutton\n\t{\n\t\t-animation " << iAnimationIndex << "\n\t\t-frame " << dFrame << "\n\t\t-order " << iRenderOrder << "\n\t\t-visible " << bVisible << "\n\t\t-size " << vecSize.x << ' ' << vecSize.y << "\n\t\t-position " << vecPosition.x << ' ' << vecPosition.y;
				fSaveFile << "\n\t\t-world " << bWorld << "\n\t\t-duration " << dDuration << "\n\t\t-command \"" << sCommand << "\"\n\t\t-deleteonselect " << bDeleteOnSelect << "\n\t\t-scale " << dScale << "\n\t\t-text \"" << sText << '\"';
				fSaveFile << "\n\t}\n";

				continue;
			}
		}
	}

	fSaveFile << '}';
	fSaveFile.close();
}

void CFileManager::New( const char *sFileName )
{
	Copy( "Resources/empty.sit", sFileName );
}

void CFileManager::Copy( const char *sFileNameSrc, const char *sFileNameDest )
{
	if (UTIL_strcmp( "sit", UTIL_extn( sFileNameSrc ) ) != 0)
		return;

	if (UTIL_strcmp( "sit", UTIL_extn( sFileNameDest ) ) != 0)
		return;

	std::fstream fSrcFile( sFileNameSrc, std::ios::in );
	std::fstream fDestFile( sFileNameDest, std::ios::out );

	fDestFile << fSrcFile.rdbuf();

	fSrcFile.close();
	fDestFile.close();
}

void CFileManager::Quicksave( void )
{
	if (m_sSituationName)
		Save( m_sSituationName );
}

void CFileManager::ResetConfigs( void )
{
	while (!m_sSituationConfigs.IsEmpty())
	{
		delete m_sSituationConfigs.GetHead();
		m_sSituationConfigs.RemoveFromHead();
	}
}

void CFileManager::ResetGraphics( void )
{
	while (!m_sSituationGraphics.IsEmpty())
	{
		delete m_sSituationGraphics.GetHead();
		m_sSituationGraphics.RemoveFromHead();
	}
}

bool CFileManager::LoadConfig( const char *sFileName )
{
	if (UTIL_strcmp( "cfg", UTIL_extn( sFileName ) ) != 0)
		return false;

	pHUDManager->AddConsoleEntry( FONTTYPE_NORMAL, c_white, "Loading config file %s...", sFileName );

	std::fstream fConfigFile( sFileName, std::ios::in );

	if (!fConfigFile.is_open())
		return false;

	int iLength = 0;
	while (fConfigFile.get() != EOF)
		iLength++;

	fConfigFile.clear();
	fConfigFile.seekg( 0, fConfigFile.beg );

	char *sFileContents = new char[iLength + 1];
	fConfigFile.read( sFileContents, iLength );
	sFileContents[iLength] = '\0';

	fConfigFile.close();

	CCommandProcesser::ProcessCommand( sFileContents );

	delete[] sFileContents;

	char *sStrToAdd = UTIL_stredit( sFileName );
	m_sSituationConfigs.AddToTail( sStrToAdd );
	return true;
}

bool CFileManager::LoadSituation(const char *sFileName)
{
	if (UTIL_strcmp("sit", UTIL_extn(sFileName)) != 0)
		return false;

	pHUDManager->AddConsoleEntry(FONTTYPE_NORMAL, c_white, "Loading situation file %s...", sFileName);

	std::fstream fSituationFile(sFileName, std::ios::in);

	if (!fSituationFile.is_open())
		return false;

	if (m_sSituationName)
		delete m_sSituationName;

	m_sSituationName = UTIL_stredit( sFileName );

	ResetConfigs();
	ResetGraphics();

	pRenderManager->Reset();
	pEntityManager->Reset();
	pPlayerManager->Reset();
	pAudioManager->Reset();

	CString sInput;

	bool bLoadedHead = false;
	bool bLoadedBody = false;

	while (fSituationFile >> sInput)
	{
		if (sInput.Length() == 0)
			continue;
		
		if (!bLoadedHead && sInput == "head")
		{
			bLoadedHead = true;
			while (fSituationFile >> sInput && sInput != "{");
			while (fSituationFile >> sInput && sInput != "}")
			{
				if (sInput == "config")
				{
					while (fSituationFile >> sInput && sInput != "{");
					while (fSituationFile >> sInput && sInput != "}")
					{
						if (sInput.Length() == 0)
							continue;

						const char *sStr = sInput.Get();
						LoadConfig( sStr );
					}
				}
				else if (sInput == "graphics")
				{
					while (fSituationFile >> sInput && sInput != "{");
					while (fSituationFile >> sInput && sInput != "}")
					{
						if (sInput.Length() == 0)
							continue;

						const char *sStr = sInput.Get();
						LoadGraphics( sStr );
					}
				}
				else if (sInput == "camera")
				{
					while (fSituationFile >> sInput && sInput != "{");
					while (fSituationFile >> sInput && sInput != "}")
					{
						if (sInput.Length() == 0)
							continue;

						if (sInput == "-position")
						{
							Vector vecPosition;
							fSituationFile >> vecPosition.x;
							fSituationFile >> vecPosition.y;
							pRenderManager->SetScreenPosition( vecPosition );
						}
						else if (sInput == "-zoom")
						{
							double dZoom;
							fSituationFile >> dZoom;
							pRenderManager->SetRenderZoom( dZoom );
						}
					}
				}
			}
		}
		else if (!bLoadedBody && sInput == "body")
		{
			bLoadedBody = true;
			while (fSituationFile >> sInput && sInput != "{");
			while (fSituationFile >> sInput && sInput != "}")
			{
				if (sInput == "physics")
				{
					int iAnimationIndex = -1;
					double dFrame = 0.0;
					int iRenderOrder = 0;
					bool bVisible = true;
					Vector vecSize = vec_one;
					double dMass = 1.0;
					double dElasticity = 0.0;
					bool bStatic = false;
					Vector vecPosition = vec_zero;
					Vector vecVelocity = vec_zero;
					double dAngPosition = 0.0;
					double dDragCoefficient = 1.0;

					while (fSituationFile >> sInput && sInput != "{");
					while (fSituationFile >> sInput && sInput != "}")
					{
						if (sInput.Length() == 0)
							continue;

						if (sInput == "-animation")
						{
							fSituationFile >> iAnimationIndex;
						}
						else if (sInput == "-frame")
						{
							fSituationFile >> dFrame;
						}
						else if (sInput == "-order")
						{
							fSituationFile >> iRenderOrder;
						}
						else if (sInput == "-visible")
						{
							fSituationFile >> bVisible;
						}
						else if (sInput == "-size")
						{
							fSituationFile >> vecSize.x;
							fSituationFile >> vecSize.y;
						}
						else if (sInput == "-mass")
						{
							fSituationFile >> dMass;
						}
						else if (sInput == "-elasticity")
						{
							fSituationFile >> dElasticity;
						}
						else if (sInput == "-static")
						{
							fSituationFile >> bStatic;
						}
						else if (sInput == "-position")
						{
							fSituationFile >> vecPosition.x;
							fSituationFile >> vecPosition.y;
						}
						else if (sInput == "-velocity")
						{
							fSituationFile >> vecVelocity.x;
							fSituationFile >> vecVelocity.y;
						}
						else if (sInput == "-angposition")
						{
							fSituationFile >> dAngPosition;
							dAngPosition *= M_PI / 180.0;
						}
						else if (sInput == "-dragcoef")
						{
							fSituationFile >> dDragCoefficient;
						}
					}

					CBaseEntity *pNewEntity = new CPhysEntity( iAnimationIndex, dFrame, iRenderOrder, bVisible, vecSize, dMass, dElasticity, bStatic, vecPosition, vecVelocity, dAngPosition, dDragCoefficient );
					pEntityManager->AddEntity( pNewEntity );
				}
				else if (sInput == "static")
				{
					int iAnimationIndex = -1;
					double dFrame = 0.0;
					int iRenderOrder = 0;
					bool bVisible = true;
					Vector vecSize = vec_one;
					Vector vecPosition = vec_zero;
					double dAngPosition = 0.0;

					while (fSituationFile >> sInput && sInput != "{");
					while (fSituationFile >> sInput && sInput != "}")
					{
						if (sInput.Length() == 0)
							continue;

						if (sInput == "-animation")
						{
							fSituationFile >> iAnimationIndex;
						}
						else if (sInput == "-frame")
						{
							fSituationFile >> dFrame;
						}
						else if (sInput == "-order")
						{
							fSituationFile >> iRenderOrder;
						}
						else if (sInput == "-visible")
						{
							fSituationFile >> bVisible;
						}
						else if (sInput == "-size")
						{
							fSituationFile >> vecSize.x;
							fSituationFile >> vecSize.y;
						}
						else if (sInput == "-position")
						{
							fSituationFile >> vecPosition.x;
							fSituationFile >> vecPosition.y;
						}
						else if (sInput == "-angposition")
						{
							fSituationFile >> dAngPosition;
							dAngPosition *= M_PI / 180.0;
						}
					}

					CBaseEntity *pNewEntity = new CStaticEntity( iAnimationIndex, dFrame, iRenderOrder, bVisible, vecSize, vecPosition, dAngPosition );
					pEntityManager->AddEntity( pNewEntity );
				}
				else if (sInput == "button")
				{
					int iAnimationIndex = -1;
					double dFrame = 0.0;
					int iRenderOrder = 0;
					bool bVisible = true;
					Vector vecSize = vec_one;
					Vector vecPosition = vec_zero;
					bool bWorld = false;
					double dDuration = DURATION_INFINITE;
					char *sCommand = NULL;
					bool bDeleteOnSelect = false;
					double dScale = 1.0;
					char *sText = NULL;

					while (fSituationFile >> sInput && sInput != "{");
					while (fSituationFile >> sInput && sInput != "}")
					{
						if (sInput.Length() == 0)
							continue;

						if (sInput == "-animation")
						{
							fSituationFile >> iAnimationIndex;
						}
						else if (sInput == "-frame")
						{
							fSituationFile >> dFrame;
						}
						else if (sInput == "-order")
						{
							fSituationFile >> iRenderOrder;
						}
						else if (sInput == "-visible")
						{
							fSituationFile >> bVisible;
						}
						else if (sInput == "-size")
						{
							fSituationFile >> vecSize.x;
							fSituationFile >> vecSize.y;
						}
						else if (sInput == "-position")
						{
							fSituationFile >> vecPosition.x;
							fSituationFile >> vecPosition.y;
						}
						else if (sInput == "-world")
						{
							fSituationFile >> bWorld;
						}
						else if (sInput == "-duration")
						{
							fSituationFile >> dDuration;
						}
						else if (sInput == "-command")
						{
							fSituationFile >> sInput;
							sCommand = UTIL_stredit( sInput.Get() );
						}
						else if (sInput == "-deleteonselect")
						{
							fSituationFile >> bDeleteOnSelect;
						}
						else if (sInput == "-scale")
						{
							fSituationFile >> dScale;
						}
						else if (sInput == "-text")
						{
							fSituationFile >> sInput;
							sText = UTIL_stredit( sInput.Get() ); 
						}
					}

					CTextButtonHUDElement *pNewButton = new CTextButtonHUDElement( iAnimationIndex, dFrame, iRenderOrder, bVisible, vecSize, vecPosition, bWorld, dDuration, JUSTIFICATION_CENTER, sCommand != NULL ? sCommand : "", bDeleteOnSelect, dScale, FONTTYPE_NORMAL, c_white, sText != NULL ? sText : "" );
					pEntityManager->AddEntity( pNewButton );

					if (sCommand)
						delete[] sCommand;

					if (sText)
						delete[] sText;
				}
			}
		}
	}

	fSituationFile.close();
	return true;
}

bool CFileManager::LoadGraphics( const char *sFileName )
{
	if (UTIL_strcmp( "gph", UTIL_extn( sFileName ) ) != 0)
		return false;

	std::fstream fGraphicsFile( sFileName, std::ios::in );

	if (!fGraphicsFile.is_open())
		return false;
	
	int iTextureOffset = pRenderManager->GetTextureCount();
	int iAnimationOffset = pRenderManager->GetAnimationCount();
	CString sInput;

	while (fGraphicsFile >> sInput)
	{
		if (sInput == "textures")
		{
			while (fGraphicsFile >> sInput && sInput != "{");
			while (fGraphicsFile >> sInput && sInput != "}")
			{
				if (sInput.Length() == 0)
					continue;

				int iIndex;
				fGraphicsFile >> iIndex;
				iIndex += iTextureOffset;

				pRenderManager->GenerateTexture( sInput.Get(), iIndex );
			}
		}
		else if (sInput == "animations")
		{
			while (fGraphicsFile >> sInput && sInput != "{");
			while (fGraphicsFile >> sInput && sInput != "}")
			{
				if (sInput.Length() == 0)
					continue;

				int iIndex = UTIL_atoi( sInput.Get() ) + iAnimationOffset;
				CAnimation *pNewAnimation = new CAnimation();

				while (fGraphicsFile >> sInput && sInput != "{");
				while (fGraphicsFile >> sInput && sInput != "}")
				{
					if (sInput == "-framecount")
					{
						int iFrameCount;
						fGraphicsFile >> iFrameCount;
						pNewAnimation->SetFrameCount( iFrameCount );
					}
					else if (sInput == "-frames")
					{
						if (pNewAnimation->GetFrameCount() != 0)
						{
							int iFrameIndex = 0;
							while (fGraphicsFile >> sInput && sInput != "{");
							while (fGraphicsFile >> sInput && sInput != "}")
							{
								if (sInput.Length() == 0)
									continue;

								pNewAnimation->SetFrame( iFrameIndex++, UTIL_atoi( sInput.Get() ) + iTextureOffset );
							}
						}
					}
					else if (sInput == "-nextanimation")
					{
						int iNextAnimation;
						fGraphicsFile >> iNextAnimation;
						pNewAnimation->SetNextAnimation( iNextAnimation + iAnimationOffset );
					}
					else if (sInput == "-framerate")
					{
						double dFrameRate;
						fGraphicsFile >> dFrameRate;
						pNewAnimation->SetFrameRate( dFrameRate );
					}
				}

				pRenderManager->AddAnimation( pNewAnimation, iIndex );
			}
		}
	}
	
	char *sStrToAdd = UTIL_stredit( sFileName );
	m_sSituationGraphics.AddToTail( sStrToAdd );

	fGraphicsFile.close();
	return true;
}