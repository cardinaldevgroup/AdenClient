#ifndef _GAME_EVENT_MODULE_H_
#define _GAME_EVENT_MODULE_H_

#include <functional>

// 事件基类
class GameEvent
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

	Type	emType;

protected:
	// 由于父类需要将子类从内存池中释放出来
	// 所以需要储存事件的大小
	size_t	nSize;

public:
	GameEvent()
	{
		emType = Type::UNKNOWN;
		nSize = 0;
	}

	GameEvent(Type type, size_t size)
	{
		emType = type;
		nSize = size;
	}

	friend class GameEventManager;
};

class GameEventManager
{
public:
	struct Listener
	{
		std::function<void(GameEvent*)>	funcCallback;
		uint16_t						nOrder;

		bool operator<(const Listener& compare) const {
			return nOrder < compare.nOrder;
		}
	};

	void		Notify();
	Listener*	Register(std::function<void(GameEvent*)> funcCallback, uint16_t nOrder = 0);
	void		Unregister(Listener* pListener);

protected:
	class		Impl;
	Impl*		m_pImpl;

protected:
	GameEventManager();
	virtual ~GameEventManager();
};

// 键盘事件

struct GameKeyboardEvent: GameEvent
{
	enum class Code
	{
		GKEY_RETURN = '\r',
		GKEY_ESCAPE = '\x1B',
		GKEY_BACKSPACE = '\b',
		GKEY_TAB = '\t',
		GKEY_SPACE = ' ',
		GKEY_EXCLAIM = '!',
		GKEY_QUOTEDBL = '"',
		GKEY_HASH = '#',
		GKEY_PERCENT = '%',
		GKEY_DOLLAR = '$',
		GKEY_AMPERSAND = '&',
		GKEY_QUOTE = '\'',
		GKEY_LEFTPAREN = '(',
		GKEY_RIGHTPAREN = ')',
		GKEY_ASTERISK = '*',
		GKEY_PLUS = '+',
		GKEY_COMMA = ',',
		GKEY_MINUS = '-',
		GKEY_PERIOD = '.',
		GKEY_SLASH = '/',

		GKEY_0 = '0',
		GKEY_1 = '1',
		GKEY_2 = '2',
		GKEY_3 = '3',
		GKEY_4 = '4',
		GKEY_5 = '5',
		GKEY_6 = '6',
		GKEY_7 = '7',
		GKEY_8 = '8',
		GKEY_9 = '9',

		GKEY_COLON = ':',
		GKEY_SEMICOLON = ';',
		GKEY_LESS = '<',
		GKEY_EQUALS = '=',
		GKEY_GREATER = '>',
		GKEY_QUESTION = '?',
		GKEY_AT = '@',

		GKEY_LEFTBRACKET = '[',
		GKEY_BACKSLASH = '\\',
		GKEY_RIGHTBRACKET = ']',
		GKEY_CARET = '^',
		GKEY_UNDERSCORE = '_',
		GKEY_BACKQUOTE = '`',

		GKEY_A = 'a',
		GKEY_B = 'b',
		GKEY_C = 'c',
		GKEY_D = 'd',
		GKEY_E = 'e',
		GKEY_F = 'f',
		GKEY_G = 'g',
		GKEY_H = 'h',
		GKEY_I = 'i',
		GKEY_J = 'j',
		GKEY_K = 'k',
		GKEY_L = 'l',
		GKEY_M = 'm',
		GKEY_N = 'n',
		GKEY_O = 'o',
		GKEY_P = 'p',
		GKEY_Q = 'q',
		GKEY_R = 'r',
		GKEY_S = 's',
		GKEY_T = 't',
		GKEY_U = 'u',
		GKEY_V = 'v',
		GKEY_W = 'w',
		GKEY_X = 'x',
		GKEY_Y = 'y',
		GKEY_Z = 'z',

		GKEY_CAPSLOCK = 0x40000039,

		GKEY_F1,
		GKEY_F2,
		GKEY_F3,
		GKEY_F4,
		GKEY_F5,
		GKEY_F6,
		GKEY_F7,
		GKEY_F8,
		GKEY_F9,
		GKEY_F10,
		GKEY_F11,
		GKEY_F12,

		GKEY_PRINTSCREEN,
		GKEY_SCROLLLOCK,
		GKEY_PAUSE,
		GKEY_INSERT,
		GKEY_HOME,
		GKEY_PAGEUP,
		GKEY_DELETE = '\x7F',
		GKEY_END = 0x4000004D,
		GKEY_PAGEDOWN,
		GKEY_RIGHT,
		GKEY_LEFT,
		GKEY_DOWN,
		GKEY_UP,

		GKEY_NUMLOCKCLEAR,
		GKEY_KP_DIVIDE,
		GKEY_KP_MULTIPLY,
		GKEY_KP_MINUS,
		GKEY_KP_PLUS,
		GKEY_KP_ENTER,
		GKEY_KP_1,
		GKEY_KP_2,
		GKEY_KP_3,
		GKEY_KP_4,
		GKEY_KP_5,
		GKEY_KP_6,
		GKEY_KP_7,
		GKEY_KP_8,
		GKEY_KP_9,
		GKEY_KP_0,
		GKEY_KP_PERIOD,

		GKEY_APPLICATION = 0x40000065,
		GKEY_POWER,
		GKEY_KP_EQUALS,
		GKEY_F13,
		GKEY_F14,
		GKEY_F15,
		GKEY_F16,
		GKEY_F17,
		GKEY_F18,
		GKEY_F19,
		GKEY_F20,
		GKEY_F21,
		GKEY_F22,
		GKEY_F23,
		GKEY_F24,
		GKEY_EXECUTE,
		GKEY_HELP,
		GKEY_MENU,
		GKEY_SELECT,
		GKEY_STOP,
		GKEY_AGAIN,
		GKEY_UNDO,
		GKEY_CUT,
		GKEY_COPY,
		GKEY_PASTE,
		GKEY_FIND,
		GKEY_MUTE,
		GKEY_VOLUMEUP,
		GKEY_VOLUMEDOWN,
		GKEY_KP_COMMA,
		GKEY_KP_EQUALSAS400,

		GKEY_ALTERASE = 0x40000099,
		GKEY_SYSREQ,
		GKEY_CANCEL,
		GKEY_CLEAR,
		GKEY_PRIOR,
		GKEY_RETURN2,
		GKEY_SEPARATOR,
		GKEY_OUT,
		GKEY_OPER,
		GKEY_CLEARAGAIN,
		GKEY_CRSEL,
		GKEY_EXSEL,

		GKEY_KP_00 = 0x400000B0,
		GKEY_KP_000,
		GKEY_THOUSANDSSEPARATOR,
		GKEY_DECIMALSEPARATOR,
		GKEY_CURRENCYUNIT,
		GKEY_CURRENCYSUBUNIT,
		GKEY_KP_LEFTPAREN,
		GKEY_KP_RIGHTPAREN,
		GKEY_KP_LEFTBRACE,
		GKEY_KP_RIGHTBRACE,
		GKEY_KP_TAB,
		GKEY_KP_BACKSPACE,
		GKEY_KP_A,
		GKEY_KP_B,
		GKEY_KP_C,
		GKEY_KP_D,
		GKEY_KP_E,
		GKEY_KP_F,
		GKEY_KP_XOR,
		GKEY_KP_POWER,
		GKEY_KP_PERCENT,
		GKEY_KP_LESS,
		GKEY_KP_GREATER,
		GKEY_KP_AMPERSAND,
		GKEY_KP_DBLAMPERSAND,
		GKEY_KP_VERTICALBAR,
		GKEY_KP_DBLVERTICALBAR,
		GKEY_KP_COLON,
		GKEY_KP_HASH,
		GKEY_KP_SPACE,
		GKEY_KP_AT,
		GKEY_KP_EXCLAM,
		GKEY_KP_MEMSTORE,
		GKEY_KP_MEMRECALL,
		GKEY_KP_MEMCLEAR,
		GKEY_KP_MEMADD,
		GKEY_KP_MEMSUBTRACT,
		GKEY_KP_MEMMULTIPLY,
		GKEY_KP_MEMDIVIDE,
		GKEY_KP_PLUSMINUS,
		GKEY_KP_CLEAR,
		GKEY_KP_CLEARENTRY,
		GKEY_KP_BINARY,
		GKEY_KP_OCTAL,
		GKEY_KP_DECIMAL,
		GKEY_KP_HEXADECIMAL,

		GKEY_LCTRL = 0x400000E0,
		GKEY_LSHIFT,
		GKEY_LALT,
		GKEY_LGUI,
		GKEY_RCTRL,
		GKEY_RSHIFT,
		GKEY_RALT,
		GKEY_RGUI,

		GKEY_MODE = 0x40000101,

		GKEY_AUDIONEXT,
		GKEY_AUDIOPREV,
		GKEY_AUDIOSTOP,
		GKEY_AUDIOPLAY,
		GKEY_AUDIOMUTE,
		GKEY_MEDIASELECT,
		GKEY_WWW,
		GKEY_MAIL,
		GKEY_CALCULATOR,
		GKEY_COMPUTER,
		GKEY_AC_SEARCH,
		GKEY_AC_HOME,
		GKEY_AC_BACK,
		GKEY_AC_FORWARD,
		GKEY_AC_STOP,
		GKEY_AC_REFRESH,
		GKEY_AC_BOOKMARKS,

		GKEY_BRIGHTNESSDOWN,
		GKEY_BRIGHTNESSUP,
		GKEY_DISPLAYSWITCH,
		GKEY_KBDILLUMTOGGLE,
		GKEY_KBDILLUMDOWN,
		GKEY_KBDILLUMUP,
		GKEY_EJECT,
		GKEY_SLEEP,
		GKEY_APP1,
		GKEY_APP2,

		GKEY_AUDIOREWIND,
		GKEY_AUDIOFASTFORWARD
	};
	enum class Mod
	{
		GKEY_NONE = 0x0000,
		GKEY_LSHIFT = 0x0001,
		GKEY_RSHIFT = 0x0002,
		GKEY_LCTRL = 0x0040,
		GKEY_RCTRL = 0x0080,
		GKEY_LALT = 0x0100,
		GKEY_RALT = 0x0200,
		GKEY_LGUI = 0x0400,
		GKEY_RGUI = 0x0800,
		GKEY_NUM = 0x1000,
		GKEY_CAPS = 0x2000,
		GKEY_MODE = 0x4000,
		GKEY_RESERVED = 0x8000,

		GKEY_CTRL = GKEY_LCTRL | GKEY_RCTRL,
		GKEY_SHIFT = GKEY_LSHIFT | GKEY_RSHIFT,
		GKEY_ALT = GKEY_LALT | GKEY_RALT,
		GKEY_GUI = GKEY_LGUI | GKEY_RGUI
	};

	Code	emKeyCode;
	Mod		emKeyMod;
};

class GameKeyboardEventManager : public GameEventManager
{
public:
	void PushEvent(GameKeyboardEvent* eventKeyboard);

public:
	virtual ~GameKeyboardEventManager() = default;
	GameKeyboardEventManager(const GameKeyboardEventManager&) = delete;
	GameKeyboardEventManager& operator=(const GameKeyboardEventManager&) = delete;
	static GameKeyboardEventManager& GetInstance()
	{
		static GameKeyboardEventManager instance;
		return instance;
	}
private:
	GameKeyboardEventManager() = default;
};

// 鼠标按键事件
struct GameMouseButtonEvent :GameEvent
{
	int32_t x, y;
	uint8_t nClicks;
};

class GameMouseButtonEventManager : public GameEventManager
{
public:
	void PushEvent(GameMouseButtonEvent* eventMouseButton);

public:
	virtual ~GameMouseButtonEventManager() = default;
	GameMouseButtonEventManager(const GameMouseButtonEventManager&) = delete;
	GameMouseButtonEventManager& operator=(const GameMouseButtonEventManager&) = delete;
	static GameMouseButtonEventManager& GetInstance()
	{
		static GameMouseButtonEventManager instance;
		return instance;
	}
private:
	GameMouseButtonEventManager() = default;
};

// 鼠标移动事件
struct GameMouseMotionEvent: GameEvent
{
	int32_t x, y;
	int32_t xRel, yRel;
};

class GameMouseMotionEventManager : public GameEventManager
{
public:
	void PushEvent(GameMouseMotionEvent* eventMouseMotion);

public:
	virtual ~GameMouseMotionEventManager() = default;
	GameMouseMotionEventManager(const GameMouseMotionEventManager&) = delete;
	GameMouseMotionEventManager& operator=(const GameMouseMotionEventManager&) = delete;
	static GameMouseMotionEventManager& GetInstance()
	{
		static GameMouseMotionEventManager instance;
		return instance;
	}
private:
	GameMouseMotionEventManager() = default;
};

// 鼠标滚轮事件
struct GameMouseWheelEvent : GameEvent
{

};

class GameMouseWheelEventManager : public GameEventManager
{
public:

	void PushEvent(GameMouseWheelEvent* eventMouseWheel);

public:
	virtual ~GameMouseWheelEventManager() = default;
	GameMouseWheelEventManager(const GameMouseWheelEventManager&) = delete;
	GameMouseWheelEventManager& operator=(const GameMouseWheelEventManager&) = delete;
	static GameMouseWheelEventManager& GetInstance()
	{
		static GameMouseWheelEventManager instance;
		return instance;
	}
private:
	GameMouseWheelEventManager() = default;
};

// 碰撞事件
struct GameCollisionEvent: GameEvent
{

};

class GameCollisionEventManager : public GameEventManager
{
public:
	void PushEvent(GameCollisionEvent* eventCollision);

public:
	virtual ~GameCollisionEventManager() = default;
	GameCollisionEventManager(const GameCollisionEventManager&) = delete;
	GameCollisionEventManager& operator=(const GameCollisionEventManager&) = delete;
	static GameCollisionEventManager& GetInstance()
	{
		static GameCollisionEventManager instance;
		return instance;
	}
private:
	GameCollisionEventManager() = default;
};

#endif // !_GAME_EVENT_MODULE_H_