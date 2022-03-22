#ifndef _GAME_EVENT_MODULE_H_
#define _GAME_EVENT_MODULE_H_

#include <functional>

class GameEventBase
{
public:
	enum class Type
	{
		UNKNOWN = 0,

		QUIT = 0x100,

		KEY_DOWN = 0x300,
		KEY_UP,

		MOUSE_MOTION = 0x400,
		MOUSE_BUTTON_DOWN,
		MOUSE_BUTTON_UP,
		MOUSE_WHEEL,

		COLLISION = 0x3000,

		USER_EVENT = 0x8000
	};

	struct BaseEvent
	{
		Type	emType;
		size_t	nSize;

		BaseEvent(Type type, size_t size)
		{
			emType = type;
			nSize = size;
		}
	};

	struct Listener
	{
		std::function<void(BaseEvent*)>	funcCallback;
		uint16_t						nOrder;

		bool operator<(const Listener& compare) const {
			return nOrder < compare.nOrder;
		}
	};

	void		Notify();
	Listener*	Register(std::function<void(BaseEvent*)> funcCallback, uint16_t nOrder);
	void		Unregister(Listener* pListener);

protected:
	class		Impl;
	Impl*		m_pImpl;

protected:
	GameEventBase();
	virtual ~GameEventBase();
};

// 键盘事件
class GameKeyboard : public GameEventBase
{
public:
	enum class Code
	{
		KEY_RETURN = '\r',
		KEY_ESCAPE = '\x1B',
		KEY_BACKSPACE = '\b',
		KEY_TAB = '\t',
		KEY_SPACE = ' ',
		KEY_EXCLAIM = '!',
		KEY_QUOTEDBL = '"',
		KEY_HASH = '#',
		KEY_PERCENT = '%',
		KEY_DOLLAR = '$',
		KEY_AMPERSAND = '&',
		KEY_QUOTE = '\'',
		KEY_LEFTPAREN = '(',
		KEY_RIGHTPAREN = ')',
		KEY_ASTERISK = '*',
		KEY_PLUS = '+',
		KEY_COMMA = ',',
		KEY_MINUS = '-',
		KEY_PERIOD = '.',
		KEY_SLASH = '/',

		KEY_0 = '0',
		KEY_1 = '1',
		KEY_2 = '2',
		KEY_3 = '3',
		KEY_4 = '4',
		KEY_5 = '5',
		KEY_6 = '6',
		KEY_7 = '7',
		KEY_8 = '8',
		KEY_9 = '9',

		KEY_COLON = ':',
		KEY_SEMICOLON = ';',
		KEY_LESS = '<',
		KEY_EQUALS = '=',
		KEY_GREATER = '>',
		KEY_QUESTION = '?',
		KEY_AT = '@',

		KEY_LEFTBRACKET = '[',
		KEY_BACKSLASH = '\\',
		KEY_RIGHTBRACKET = ']',
		KEY_CARET = '^',
		KEY_UNDERSCORE = '_',
		KEY_BACKQUOTE = '`',

		KEY_A = 'a',
		KEY_B = 'b',
		KEY_C = 'c',
		KEY_D = 'd',
		KEY_E = 'e',
		KEY_F = 'f',
		KEY_G = 'g',
		KEY_H = 'h',
		KEY_I = 'i',
		KEY_J = 'j',
		KEY_K = 'k',
		KEY_L = 'l',
		KEY_M = 'm',
		KEY_N = 'n',
		KEY_O = 'o',
		KEY_P = 'p',
		KEY_Q = 'q',
		KEY_R = 'r',
		KEY_S = 's',
		KEY_T = 't',
		KEY_U = 'u',
		KEY_V = 'v',
		KEY_W = 'w',
		KEY_X = 'x',
		KEY_Y = 'y',
		KEY_Z = 'z',

		KEY_CAPSLOCK = 0x40000039,

		KEY_F1,
		KEY_F2,
		KEY_F3,
		KEY_F4,
		KEY_F5,
		KEY_F6,
		KEY_F7,
		KEY_F8,
		KEY_F9,
		KEY_F10,
		KEY_F11,
		KEY_F12,

		KEY_PRINTSCREEN,
		KEY_SCROLLLOCK,
		KEY_PAUSE,
		KEY_INSERT,
		KEY_HOME,
		KEY_PAGEUP,
		KEY_DELETE = '\x7F',
		KEY_END = 0x4000004D,
		KEY_PAGEDOWN,
		KEY_RIGHT,
		KEY_LEFT,
		KEY_DOWN,
		KEY_UP,

		KEY_NUMLOCKCLEAR,
		KEY_KP_DIVIDE,
		KEY_KP_MULTIPLY,
		KEY_KP_MINUS,
		KEY_KP_PLUS,
		KEY_KP_ENTER,
		KEY_KP_1,
		KEY_KP_2,
		KEY_KP_3,
		KEY_KP_4,
		KEY_KP_5,
		KEY_KP_6,
		KEY_KP_7,
		KEY_KP_8,
		KEY_KP_9,
		KEY_KP_0,
		KEY_KP_PERIOD,

		KEY_APPLICATION = 0x40000065,
		KEY_POWER,
		KEY_KP_EQUALS,
		KEY_F13,
		KEY_F14,
		KEY_F15,
		KEY_F16,
		KEY_F17,
		KEY_F18,
		KEY_F19,
		KEY_F20,
		KEY_F21,
		KEY_F22,
		KEY_F23,
		KEY_F24,
		KEY_EXECUTE,
		KEY_HELP,
		KEY_MENU,
		KEY_SELECT,
		KEY_STOP,
		KEY_AGAIN,
		KEY_UNDO,
		KEY_CUT,
		KEY_COPY,
		KEY_PASTE,
		KEY_FIND,
		KEY_MUTE,
		KEY_VOLUMEUP,
		KEY_VOLUMEDOWN,
		KEY_KP_COMMA,
		KEY_KP_EQUALSAS400,

		KEY_ALTERASE = 0x40000099,
		KEY_SYSREQ,
		KEY_CANCEL,
		KEY_CLEAR,
		KEY_PRIOR,
		KEY_RETURN2,
		KEY_SEPARATOR,
		KEY_OUT,
		KEY_OPER,
		KEY_CLEARAGAIN,
		KEY_CRSEL,
		KEY_EXSEL,

		KEY_KP_00 = 0x400000B0,
		KEY_KP_000,
		KEY_THOUSANDSSEPARATOR,
		KEY_DECIMALSEPARATOR,
		KEY_CURRENCYUNIT,
		KEY_CURRENCYSUBUNIT,
		KEY_KP_LEFTPAREN,
		KEY_KP_RIGHTPAREN,
		KEY_KP_LEFTBRACE,
		KEY_KP_RIGHTBRACE,
		KEY_KP_TAB,
		KEY_KP_BACKSPACE,
		KEY_KP_A,
		KEY_KP_B,
		KEY_KP_C,
		KEY_KP_D,
		KEY_KP_E,
		KEY_KP_F,
		KEY_KP_XOR,
		KEY_KP_POWER,
		KEY_KP_PERCENT,
		KEY_KP_LESS,
		KEY_KP_GREATER,
		KEY_KP_AMPERSAND,
		KEY_KP_DBLAMPERSAND,
		KEY_KP_VERTICALBAR,
		KEY_KP_DBLVERTICALBAR,
		KEY_KP_COLON,
		KEY_KP_HASH,
		KEY_KP_SPACE,
		KEY_KP_AT,
		KEY_KP_EXCLAM,
		KEY_KP_MEMSTORE,
		KEY_KP_MEMRECALL,
		KEY_KP_MEMCLEAR,
		KEY_KP_MEMADD,
		KEY_KP_MEMSUBTRACT,
		KEY_KP_MEMMULTIPLY,
		KEY_KP_MEMDIVIDE,
		KEY_KP_PLUSMINUS,
		KEY_KP_CLEAR,
		KEY_KP_CLEARENTRY,
		KEY_KP_BINARY,
		KEY_KP_OCTAL,
		KEY_KP_DECIMAL,
		KEY_KP_HEXADECIMAL,

		KEY_LCTRL = 0x400000E0,
		KEY_LSHIFT,
		KEY_LALT,
		KEY_LGUI,
		KEY_RCTRL,
		KEY_RSHIFT,
		KEY_RALT,
		KEY_RGUI,

		KEY_MODE = 0x40000101,

		KEY_AUDIONEXT,
		KEY_AUDIOPREV,
		KEY_AUDIOSTOP,
		KEY_AUDIOPLAY,
		KEY_AUDIOMUTE,
		KEY_MEDIASELECT,
		KEY_WWW,
		KEY_MAIL,
		KEY_CALCULATOR,
		KEY_COMPUTER,
		KEY_AC_SEARCH,
		KEY_AC_HOME,
		KEY_AC_BACK,
		KEY_AC_FORWARD,
		KEY_AC_STOP,
		KEY_AC_REFRESH,
		KEY_AC_BOOKMARKS,

		KEY_BRIGHTNESSDOWN,
		KEY_BRIGHTNESSUP,
		KEY_DISPLAYSWITCH,
		KEY_KBDILLUMTOGGLE,
		KEY_KBDILLUMDOWN,
		KEY_KBDILLUMUP,
		KEY_EJECT,
		KEY_SLEEP,
		KEY_APP1,
		KEY_APP2,

		KEY_AUDIOREWIND,
		KEY_AUDIOFASTFORWARD
	};

	enum class Mod
	{
		KEY_NONE = 0x0000,
		KEY_LSHIFT = 0x0001,
		KEY_RSHIFT = 0x0002,
		KEY_LCTRL = 0x0040,
		KEY_RCTRL = 0x0080,
		KEY_LALT = 0x0100,
		KEY_RALT = 0x0200,
		KEY_LGUI = 0x0400,
		KEY_RGUI = 0x0800,
		KEY_NUM = 0x1000,
		KEY_CAPS = 0x2000,
		KEY_MODE = 0x4000,
		KEY_RESERVED = 0x8000,

		KEY_CTRL = KEY_LCTRL | KEY_RCTRL,
		KEY_SHIFT = KEY_LSHIFT | KEY_RSHIFT,
		KEY_ALT = KEY_LALT | KEY_RALT,
		KEY_GUI = KEY_LGUI | KEY_RGUI
	};

	struct Event: BaseEvent
	{
		Code	emKeyCode;
		Mod		emKeyMod;
	};

	void PushEvent(const Event& eventKeyboard);

public:
	virtual ~GameKeyboard() = default;
	GameKeyboard(const GameKeyboard&) = delete;
	GameKeyboard& operator=(const GameKeyboard&) = delete;
	static GameKeyboard& GetInstance()
	{
		static GameKeyboard instance;
		return instance;
	}
private:
	GameKeyboard() = default;
};

// 鼠标按键事件
class GameMouseButton : public GameEventBase
{
public:
	struct Event
	{
		Type emType;
	};

	void PushEvent(Event* pEvent);

public:
	virtual ~GameMouseButton() = default;
	GameMouseButton(const GameMouseButton&) = delete;
	GameMouseButton& operator=(const GameMouseButton&) = delete;
	static GameMouseButton& GetInstance()
	{
		static GameMouseButton instance;
		return instance;
	}
private:
	GameMouseButton() = default;
};

// 鼠标移动事件
class GameMouseMotion : public GameEventBase
{
public:
	struct Event
	{
		Type emType;
	};

	void PushEvent(Event* pEvent);

public:
	virtual ~GameMouseMotion() = default;
	GameMouseMotion(const GameMouseMotion&) = delete;
	GameMouseMotion& operator=(const GameMouseMotion&) = delete;
	static GameMouseMotion& GetInstance()
	{
		static GameMouseMotion instance;
		return instance;
	}
private:
	GameMouseMotion() = default;
};

// 鼠标滚轮事件
class GameMouseWheel : public GameEventBase
{
public:
	struct Event
	{
		Type emType;
	};

	void PushEvent(Event* pEvent);

public:
	virtual ~GameMouseWheel() = default;
	GameMouseWheel(const GameMouseWheel&) = delete;
	GameMouseWheel& operator=(const GameMouseWheel&) = delete;
	static GameMouseWheel& GetInstance()
	{
		static GameMouseWheel instance;
		return instance;
	}
private:
	GameMouseWheel() = default;
};

// 碰撞事件
class GameCollision : public GameEventBase
{
public:
	struct Event
	{
		Type emType;
	};

	void PushEvent(Event* pEvent);

public:
	virtual ~GameCollision() = default;
	GameCollision(const GameCollision&) = delete;
	GameCollision& operator=(const GameCollision&) = delete;
	static GameCollision& GetInstance()
	{
		static GameCollision instance;
		return instance;
	}
private:
	GameCollision() = default;
};

#endif // !_GAME_EVENT_MODULE_H_