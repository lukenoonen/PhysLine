#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "Global.h"

class CTextInput;

enum KeyCodes_t
{
	KEYCODE_LEFTMOUSE = 0,
	KEYCODE_RIGHTMOUSE,
	KEYCODE_MIDDLEMOUSE,
	KEYCODE_MOUSEWHEELUP,
	KEYCODE_MOUSEWHEELDOWN,
	KEYCODE_ESCAPE,
	KEYCODE_DELETE,
	KEYCODE_BACKSPACE,
	KEYCODE_LSHIFT,
	KEYCODE_RSHIFT,
	KEYCODE_ENTER,
	KEYCODE_TAB,
	KEYCODE_LCTL,
	KEYCODE_RCTL,
	KEYCODE_LALT,
	KEYCODE_RALT,
	KEYCODE_BACKQUOTE,
	KEYCODE_0,
	KEYCODE_1,
	KEYCODE_2,
	KEYCODE_3,
	KEYCODE_4,
	KEYCODE_5,
	KEYCODE_6,
	KEYCODE_7,
	KEYCODE_8,
	KEYCODE_9,
	KEYCODE_A,
	KEYCODE_B,
	KEYCODE_C,
	KEYCODE_D,
	KEYCODE_E,
	KEYCODE_F,
	KEYCODE_G,
	KEYCODE_H,
	KEYCODE_I,
	KEYCODE_J,
	KEYCODE_K,
	KEYCODE_L,
	KEYCODE_M,
	KEYCODE_N,
	KEYCODE_O,
	KEYCODE_P,
	KEYCODE_Q,
	KEYCODE_R,
	KEYCODE_S,
	KEYCODE_T,
	KEYCODE_U,
	KEYCODE_V,
	KEYCODE_W,
	KEYCODE_X,
	KEYCODE_Y,
	KEYCODE_Z,
	KEYCODE_COUNT,
	KEYCODE_INVALID = KEYCODE_COUNT,
};

static const char *sKeycodeNames[] =
{
	"m0",
	"m1",
	"m2",
	"mwup",
	"mwdn",
	"esc",
	"del",
	"back",
	"lsft",
	"rsft",
	"entr",
	"tab",
	"lctl",
	"rctl",
	"lalt",
	"ralt",
	"`",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"a",
	"b",
	"c",
	"d",
	"e",
	"f",
	"g",
	"h",
	"i",
	"j",
	"k",
	"l",
	"m",
	"n",
	"o",
	"p",
	"q",
	"r",
	"s",
	"t",
	"u",
	"v",
	"w",
	"x",
	"y",
	"z"
};

const char *UTIL_KeyCodeToStr( KeyCodes_t tKeyCode );
KeyCodes_t UTIL_KeyStrToCode( const char *sKeyCode );

class CKeyBind
{
public:
	DECLARE_CLASS_NOBASE( CInputManager );

	CKeyBind();
	~CKeyBind();

	void Bind( const char *sCommand );
	void Unbind( void );

	void DownDispatch( void );
	void UpDispatch( void );

private:
	char *m_sCommand;
};

class CInputManager
{
public:
	DECLARE_CLASS_NOBASE( CInputManager );

	CInputManager();

	void OnLoop( void );

	const Vector &GetMousePosition( void );
	const Vector &GetMouseDelta( void );

	void BindKey( KeyCodes_t tKeyCode, const char *sCommand );
	void UnbindKey( KeyCodes_t tKeyCode );

	void SetTextInput( CTextInput *pTextInput );
	void EnterTextInput( void );
	void CancelTextInput( void );

private:
	bool GetKeyDown( KeyCodes_t tKeyCode );
	bool GetKeyUp( KeyCodes_t tKeyCode );

private:
	bool m_bKeyDown[KEYCODE_COUNT];
	bool m_bPrevKeyDown[KEYCODE_COUNT];

	CKeyBind m_KeyBinds[KEYCODE_COUNT];

	Vector m_vecMousePosition;
	Vector m_vecMouseDelta;

	CTextInput *m_pTextInput;
};

#endif // INPUTMANAGER_H