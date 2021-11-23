#include "ConsoleHUDElement.h"
#include "RenderManager.h"

CConInt ci_maxconsoleentries( "maxce", 5, "the maximum amount of console entries on screen" );
CConDouble cd_consoleentrylifetime( "celifetime", 5.0, "the time until a console entry is removed" );

CConsoleHUDElement::CConsoleHUDElement() : BaseClass( -1, 0.0, 10000, true, false, vec_one * 270.0, vec_zero, false, DURATION_INFINITE, JUSTIFICATION_LEFT_UP )
{
	Vector vecPosition( 0, ci_maxconsoleentries.GetValue() * TEXT_SIZE_D * 0.75 );
	m_pInputElement = new CInputHUDElement( 0, true, vecPosition, false, DURATION_INFINITE, JUSTIFICATION_LEFT_UP, 0.75, FONTTYPE_NORMAL, c_white, COMMAND_PROMPT, false, -1 );
}

CConsoleHUDElement::~CConsoleHUDElement()
{
	CIterator<CTextHUDElement *> iterator = m_TextElements.StartIterating();
	while (iterator.Iterating())
		delete iterator.Iterate();

	m_TextElements.RemoveAll();
}
	
void CConsoleHUDElement::AddEntry( FontType_t tFontType, const SDL_Color &cColor, const char *sText )
{
	if (!pRenderManager->IsFontInitialized( tFontType ))
		return;

	if (!IsVisible())
		return;

	Vector vecPosition( 0, (double)m_TextElements.Count() * TEXT_SIZE_D * 0.75 );

	CTextHUDElement *pTextElement = new CTextHUDElement( 0, true, vecPosition, false, cd_consoleentrylifetime.GetValue(), JUSTIFICATION_LEFT_UP, 0.75, tFontType, cColor, sText );
	m_TextElements.AddToTail( pTextElement );
}

bool CConsoleHUDElement::IsPermanent( void )
{
	return true;
}

CBaseEntity *CConsoleHUDElement::GetEntityAtPoint( const Vector &vecPoint )
{
	return m_pInputElement->GetEntityAtPoint( vecPoint );
}

void CConsoleHUDElement::CopyToRender( void )
{
	if (IsVisible())
	{
		bool bChanged = false;
		while (m_TextElements.Count() > ci_maxconsoleentries.GetValue())
		{
			if (!bChanged)
				bChanged = true;

			delete m_TextElements.GetHead();
			m_TextElements.RemoveFromHead();
		}

		while (m_TextElements.Count() > 0 && m_TextElements.GetHead()->ShouldDelete())
		{
			if (!bChanged)
				bChanged = true;

			delete m_TextElements.GetHead();
			m_TextElements.RemoveFromHead();
		}

		if (bChanged)
		{
			int iIndex = 0;
			CIterator<CTextHUDElement *> iterator = m_TextElements.StartIterating();
			while (iterator.Iterating())
			{
				CTextHUDElement *pTextElement = iterator.Iterate();
				Vector vecPosition( 0, (double)iIndex * TEXT_SIZE_D * 0.75 );
				pTextElement->SetPosition( vecPosition );
				iIndex++;
			}
		}
	
		Vector vecPosition( 0, ci_maxconsoleentries.GetValue() * TEXT_SIZE_D * 0.75 );
		m_pInputElement->SetPosition( vecPosition );
		m_pInputElement->OnLoop();

		const char *sCommand = m_pInputElement->GetString();
		if (sCommand)
		{
			AddEntry( FONTTYPE_NORMAL, c_white, sCommand );
			CCommandProcesser::ProcessCommand( sCommand );
			m_pInputElement->ClearString();
		}

		CIterator<CTextHUDElement *> iterator = m_TextElements.StartIterating();
		while (iterator.Iterating())
			iterator.Iterate()->CopyToRender();

		m_pInputElement->CopyToRender();
	}
}

void CConsoleHUDElement::SelectInput( void )
{
	m_pInputElement->OnSelected();
}