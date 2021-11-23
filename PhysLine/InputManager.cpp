#include "InputManager.h"
#include "RenderManager.h"
#include "TextInput.h"

bool CC_Bind( const CLinkedList<const char *> &command )
{
	if (command.Count() < 2)
		return false;

	CIterator<const char *> iterator = command.StartIterating();

	KeyCodes_t tKeyCodes = UTIL_KeyStrToCode( iterator.Iterate() );
	if (tKeyCodes == KEYCODE_INVALID)
		return false;

	pInputManager->BindKey( tKeyCodes, iterator.Iterate() );

	return true;
}
CConCommand cc_bind( "bind", CC_Bind, "bind [key] [command]" );

bool CC_Unbind( const CLinkedList<const char *> &command )
{
	if (command.Count() < 1)
		return false;

	KeyCodes_t tKeyCodes = UTIL_KeyStrToCode( command.GetHead() );
	if (tKeyCodes == KEYCODE_INVALID)
		return false;

	pInputManager->UnbindKey( tKeyCodes );

	return true;
}
CConCommand cc_unbind( "unbind", CC_Unbind, "unbind [key]" );

const char *UTIL_KeyCodeToStr( KeyCodes_t tKeyCode )
{
	return sKeycodeNames[tKeyCode];
}

KeyCodes_t UTIL_KeyStrToCode( const char *sKeyCode )
{
	for (int i = 0; i < KEYCODE_COUNT; i++)
	{
		if (UTIL_strncmp( sKeyCode, sKeycodeNames[i] ) == 0)
			return (KeyCodes_t)i;
	}

	return KEYCODE_INVALID;
}

CKeyBind::CKeyBind()
{
	m_sCommand = NULL;
}

CKeyBind::~CKeyBind()
{
	if (m_sCommand)
		delete[] m_sCommand;
}

void CKeyBind::Bind( const char *sCommand )
{
	if (m_sCommand)
		delete[] m_sCommand;

	m_sCommand = UTIL_stredit( sCommand );
}

void CKeyBind::Unbind( void )
{
	if (m_sCommand)
	{
		delete[] m_sCommand;
		m_sCommand = NULL;
	}
}

void CKeyBind::DownDispatch( void )
{
	if (m_sCommand)
		CCommandProcesser::ProcessCommand( m_sCommand );
}

void CKeyBind::UpDispatch( void )
{
	if (m_sCommand && m_sCommand[0] == '+')
	{
		m_sCommand[0] = '-';
		CCommandProcesser::ProcessCommand( m_sCommand );
		m_sCommand[0] = '+';
	}
}

CInputManager::CInputManager()
{
	for (int i = 0; i < KEYCODE_COUNT; i++)
	{
		m_bKeyDown[i] = false;
		m_bPrevKeyDown[i] = false;
	}
}

void CInputManager::OnLoop( void )
{
	for (int i = 0; i < KEYCODE_COUNT; i++)
		m_bPrevKeyDown[i] = m_bKeyDown[i];

	SDL_Event event;
	while (SDL_PollEvent( &event ))
	{
		switch (event.type)
		{
		// Special cases for quit input
		case SDL_QUIT:
			pGlobalValues->MarkForQuit();
			break;

		// Text input
		case SDL_TEXTINPUT:
			if (m_pTextInput)
			{
				m_pTextInput->AddString( event.text.text );
				break;
			}

			break;

		// Keyboard keys
		case SDL_KEYDOWN:
			if (m_pTextInput)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_BACKQUOTE:
					CancelTextInput();
					break;
				case SDLK_ESCAPE:
					CancelTextInput();
					break;
				case SDLK_RETURN:
					EnterTextInput();
					break;
				case SDLK_BACKSPACE:
					m_pTextInput->Backspace();
					break;
				case SDLK_DELETE:
					m_pTextInput->Delete();
					break;
				case SDLK_LEFT:
					m_pTextInput->Move( -1 );
					break;
				case SDLK_RIGHT:
					m_pTextInput->Move( 1 );
					break;
				}

				break;
			}

			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				m_bKeyDown[KEYCODE_ESCAPE] = true;
				break;
			case SDLK_DELETE:
				m_bKeyDown[KEYCODE_DELETE] = true;
				break;
			case SDLK_BACKSPACE:
				m_bKeyDown[KEYCODE_BACKSPACE] = true;
				break;
			case SDLK_LSHIFT:
				m_bKeyDown[KEYCODE_LSHIFT] = true;
				break;
			case SDLK_RSHIFT:
				m_bKeyDown[KEYCODE_RSHIFT] = true;
				break;
			case SDLK_RETURN:
				m_bKeyDown[KEYCODE_ENTER] = true;
				break;
			case SDLK_TAB:
				m_bKeyDown[KEYCODE_TAB] = true;
				break;
			case SDLK_LCTRL:
				m_bKeyDown[KEYCODE_LCTL] = true;
				break;
			case SDLK_RCTRL:
				m_bKeyDown[KEYCODE_RCTL] = true;
				break;
			case SDLK_LALT:
				m_bKeyDown[KEYCODE_LALT] = true;
				break;
			case SDLK_RALT:
				m_bKeyDown[KEYCODE_RALT] = true;
				break;
			case SDLK_BACKQUOTE:
				m_bKeyDown[KEYCODE_BACKQUOTE] = true;
				break;
			case SDLK_0:
				m_bKeyDown[KEYCODE_0] = true;
				break;
			case SDLK_1:
				m_bKeyDown[KEYCODE_1] = true;
				break;
			case SDLK_2:
				m_bKeyDown[KEYCODE_2] = true;
				break;
			case SDLK_3:
				m_bKeyDown[KEYCODE_3] = true;
				break;
			case SDLK_4:
				m_bKeyDown[KEYCODE_4] = true;
				break;
			case SDLK_5:
				m_bKeyDown[KEYCODE_5] = true;
				break;
			case SDLK_6:
				m_bKeyDown[KEYCODE_6] = true;
				break;
			case SDLK_7:
				m_bKeyDown[KEYCODE_7] = true;
				break;
			case SDLK_8:
				m_bKeyDown[KEYCODE_8] = true;
				break;
			case SDLK_9:
				m_bKeyDown[KEYCODE_9] = true;
				break;
			case SDLK_a:
				m_bKeyDown[KEYCODE_A] = true;
				break;
			case SDLK_b:
				m_bKeyDown[KEYCODE_B] = true;
				break;
			case SDLK_c:
				m_bKeyDown[KEYCODE_C] = true;
				break;
			case SDLK_d:
				m_bKeyDown[KEYCODE_D] = true;
				break;
			case SDLK_e:
				m_bKeyDown[KEYCODE_E] = true;
				break;
			case SDLK_f:
				m_bKeyDown[KEYCODE_F] = true;
				break;
			case SDLK_g:
				m_bKeyDown[KEYCODE_G] = true;
				break;
			case SDLK_h:
				m_bKeyDown[KEYCODE_H] = true;
				break;
			case SDLK_i:
				m_bKeyDown[KEYCODE_I] = true;
				break;
			case SDLK_j:
				m_bKeyDown[KEYCODE_J] = true;
				break;
			case SDLK_k:
				m_bKeyDown[KEYCODE_K] = true;
				break;
			case SDLK_l:
				m_bKeyDown[KEYCODE_L] = true;
				break;
			case SDLK_m:
				m_bKeyDown[KEYCODE_M] = true;
				break;
			case SDLK_n:
				m_bKeyDown[KEYCODE_N] = true;
				break;
			case SDLK_o:
				m_bKeyDown[KEYCODE_O] = true;
				break;
			case SDLK_p:
				m_bKeyDown[KEYCODE_P] = true;
				break;
			case SDLK_q:
				m_bKeyDown[KEYCODE_Q] = true;
				break;
			case SDLK_r:
				m_bKeyDown[KEYCODE_R] = true;
				break;
			case SDLK_s:
				m_bKeyDown[KEYCODE_S] = true;
				break;
			case SDLK_t:
				m_bKeyDown[KEYCODE_T] = true;
				break;
			case SDLK_u:
				m_bKeyDown[KEYCODE_U] = true;
				break;
			case SDLK_v:
				m_bKeyDown[KEYCODE_V] = true;
				break;
			case SDLK_w:
				m_bKeyDown[KEYCODE_W] = true;
				break;
			case SDLK_x:
				m_bKeyDown[KEYCODE_X] = true;
				break;
			case SDLK_y:
				m_bKeyDown[KEYCODE_Y] = true;
				break;
			case SDLK_z:
				m_bKeyDown[KEYCODE_Z] = true;
				break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				m_bKeyDown[KEYCODE_ESCAPE] = false;
				break;
			case SDLK_DELETE:
				m_bKeyDown[KEYCODE_DELETE] = false;
				break;
			case SDLK_BACKSPACE:
				m_bKeyDown[KEYCODE_BACKSPACE] = false;
				break;
			case SDLK_LSHIFT:
				m_bKeyDown[KEYCODE_LSHIFT] = false;
				break;
			case SDLK_RSHIFT:
				m_bKeyDown[KEYCODE_RSHIFT] = false;
				break;
			case SDLK_RETURN:
				m_bKeyDown[KEYCODE_ENTER] = false;
				break;
			case SDLK_TAB:
				m_bKeyDown[KEYCODE_TAB] = false;
				break;
			case SDLK_LCTRL:
				m_bKeyDown[KEYCODE_LCTL] = false;
				break;
			case SDLK_RCTRL:
				m_bKeyDown[KEYCODE_RCTL] = false;
				break;
			case SDLK_LALT:
				m_bKeyDown[KEYCODE_LALT] = false;
				break;
			case SDLK_RALT:
				m_bKeyDown[KEYCODE_RALT] = false;
				break;
			case SDLK_BACKQUOTE:
				m_bKeyDown[KEYCODE_BACKQUOTE] = false;
				break;
			case SDLK_0:
				m_bKeyDown[KEYCODE_0] = false;
				break;
			case SDLK_1:
				m_bKeyDown[KEYCODE_1] = false;
				break;
			case SDLK_2:
				m_bKeyDown[KEYCODE_2] = false;
				break;
			case SDLK_3:
				m_bKeyDown[KEYCODE_3] = false;
				break;
			case SDLK_4:
				m_bKeyDown[KEYCODE_4] = false;
				break;
			case SDLK_5:
				m_bKeyDown[KEYCODE_5] = false;
				break;
			case SDLK_6:
				m_bKeyDown[KEYCODE_6] = false;
				break;
			case SDLK_7:
				m_bKeyDown[KEYCODE_7] = false;
				break;
			case SDLK_8:
				m_bKeyDown[KEYCODE_8] = false;
				break;
			case SDLK_9:
				m_bKeyDown[KEYCODE_9] = false;
				break;
			case SDLK_a:
				m_bKeyDown[KEYCODE_A] = false;
				break;
			case SDLK_b:
				m_bKeyDown[KEYCODE_B] = false;
				break;
			case SDLK_c:
				m_bKeyDown[KEYCODE_C] = false;
				break;
			case SDLK_d:
				m_bKeyDown[KEYCODE_D] = false;
				break;
			case SDLK_e:
				m_bKeyDown[KEYCODE_E] = false;
				break;
			case SDLK_f:
				m_bKeyDown[KEYCODE_F] = false;
				break;
			case SDLK_g:
				m_bKeyDown[KEYCODE_G] = false;
				break;
			case SDLK_h:
				m_bKeyDown[KEYCODE_H] = false;
				break;
			case SDLK_i:
				m_bKeyDown[KEYCODE_I] = false;
				break;
			case SDLK_j:
				m_bKeyDown[KEYCODE_J] = false;
				break;
			case SDLK_k:
				m_bKeyDown[KEYCODE_K] = false;
				break;
			case SDLK_l:
				m_bKeyDown[KEYCODE_L] = false;
				break;
			case SDLK_m:
				m_bKeyDown[KEYCODE_M] = false;
				break;
			case SDLK_n:
				m_bKeyDown[KEYCODE_N] = false;
				break;
			case SDLK_o:
				m_bKeyDown[KEYCODE_O] = false;
				break;
			case SDLK_p:
				m_bKeyDown[KEYCODE_P] = false;
				break;
			case SDLK_q:
				m_bKeyDown[KEYCODE_Q] = false;
				break;
			case SDLK_r:
				m_bKeyDown[KEYCODE_R] = false;
				break;
			case SDLK_s:
				m_bKeyDown[KEYCODE_S] = false;
				break;
			case SDLK_t:
				m_bKeyDown[KEYCODE_T] = false;
				break;
			case SDLK_u:
				m_bKeyDown[KEYCODE_U] = false;
				break;
			case SDLK_v:
				m_bKeyDown[KEYCODE_V] = false;
				break;
			case SDLK_w:
				m_bKeyDown[KEYCODE_W] = false;
				break;
			case SDLK_x:
				m_bKeyDown[KEYCODE_X] = false;
				break;
			case SDLK_y:
				m_bKeyDown[KEYCODE_Y] = false;
				break;
			case SDLK_z:
				m_bKeyDown[KEYCODE_Z] = false;
				break;
			}
			break;

		// Mouse buttons
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				m_bKeyDown[KEYCODE_LEFTMOUSE] = true;
				break;
			case SDL_BUTTON_RIGHT:
				m_bKeyDown[KEYCODE_RIGHTMOUSE] = true;
				break;
			case SDL_BUTTON_MIDDLE:
				m_bKeyDown[KEYCODE_MIDDLEMOUSE] = true;
				break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				m_bKeyDown[KEYCODE_LEFTMOUSE] = false;
				break;
			case SDL_BUTTON_RIGHT:
				m_bKeyDown[KEYCODE_RIGHTMOUSE] = false;
				break;
			case SDL_BUTTON_MIDDLE:
				m_bKeyDown[KEYCODE_MIDDLEMOUSE] = false;
				break;
			}
			break;

		// Mouse wheel
		case SDL_MOUSEWHEEL:
			if (event.wheel.y > 0)
			{
				m_bKeyDown[KEYCODE_MOUSEWHEELUP] = true;
				m_bPrevKeyDown[KEYCODE_MOUSEWHEELUP] = false;
			}
			else if (event.wheel.y < 0)
			{
				m_bKeyDown[KEYCODE_MOUSEWHEELDOWN] = true;
				m_bPrevKeyDown[KEYCODE_MOUSEWHEELDOWN] = false;
			}
			break;
		}
	}

	double dScale = pRenderManager->GetScreenScale();

	m_vecMouseDelta = m_vecMousePosition;
	int iMouseX, iMouseY;
	SDL_GetMouseState( &iMouseX, &iMouseY );
	m_vecMousePosition.x = (double)iMouseX;
	m_vecMousePosition.y = (double)iMouseY;
	m_vecMousePosition /= dScale;

	m_vecMouseDelta = m_vecMousePosition - m_vecMouseDelta;
	m_vecMouseDelta /= dScale;

	// Now that everything is up to date, check to see if we need to dispatch some commands
	for (int i = 0; i < KEYCODE_COUNT; i++)
	{
		if (GetKeyDown( (KeyCodes_t)i ))
			m_KeyBinds[i].DownDispatch();
		else if (GetKeyUp( (KeyCodes_t)i ))
			m_KeyBinds[i].UpDispatch();
	}
}

bool CInputManager::GetKeyDown( KeyCodes_t tKeyCode )
{
	return m_bKeyDown[tKeyCode] && !m_bPrevKeyDown[tKeyCode];
}

bool CInputManager::GetKeyUp( KeyCodes_t tKeyCode )
{
	return !m_bKeyDown[tKeyCode] && m_bPrevKeyDown[tKeyCode];
}

const Vector &CInputManager::GetMousePosition( void )
{
	return m_vecMousePosition;
}

const Vector &CInputManager::GetMouseDelta( void )
{
	return m_vecMouseDelta;
}

void CInputManager::BindKey( KeyCodes_t tKeyCode, const char *sCommand )
{
	m_KeyBinds[tKeyCode].Bind( sCommand );
}

void CInputManager::UnbindKey( KeyCodes_t tKeyCode )
{
	m_KeyBinds[tKeyCode].Unbind();
}

void CInputManager::SetTextInput( CTextInput *pTextInput )
{
	if (pTextInput)
	{
		if (m_pTextInput)
			m_pTextInput->MarkFailed();

		m_pTextInput = pTextInput;
		SDL_StartTextInput();
	}
}

void CInputManager::EnterTextInput( void )
{
	m_pTextInput->MarkComplete();
	m_pTextInput = NULL;
	SDL_StopTextInput();
}

void CInputManager::CancelTextInput( void )
{
	m_pTextInput->MarkFailed();
	m_pTextInput = NULL;
	SDL_StopTextInput();
}