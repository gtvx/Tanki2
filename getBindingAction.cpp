#include "getBindingAction.h"
#include <QDebug>

#define KEY_RESERVED		0
#define KEY_ESC			1
#define KEY_1			2
#define KEY_2			3
#define KEY_3			4
#define KEY_4			5
#define KEY_5			6
#define KEY_6			7
#define KEY_7			8
#define KEY_8			9
#define KEY_9			10
#define KEY_0			11
#define KEY_MINUS		12
#define KEY_EQUAL		13
#define KEY_BACKSPACE		14
#define KEY_TAB			15
#define KEY_Q			16
#define KEY_W			17
#define KEY_E			18
#define KEY_R			19
#define KEY_T			20
#define KEY_Y			21
#define KEY_U			22
#define KEY_I			23
#define KEY_O			24
#define KEY_P			25
#define KEY_LEFTBRACE		26
#define KEY_RIGHTBRACE		27
#define KEY_ENTER		28
#define KEY_LEFTCTRL		29
#define KEY_A			30
#define KEY_S			31
#define KEY_D			32
#define KEY_F			33
#define KEY_G			34
#define KEY_H			35
#define KEY_J			36
#define KEY_K			37
#define KEY_L			38
#define KEY_SEMICOLON		39
#define KEY_APOSTROPHE		40
#define KEY_GRAVE		41
#define KEY_LEFTSHIFT		42
#define KEY_BACKSLASH		43
#define KEY_Z			44
#define KEY_X			45
#define KEY_C			46
#define KEY_V			47
#define KEY_B			48
#define KEY_N			49
#define KEY_M			50
#define KEY_COMMA		51
#define KEY_DOT			52
#define KEY_SLASH		53
#define KEY_RIGHTSHIFT		54
#define KEY_KPASTERISK		55
#define KEY_LEFTALT		56
#define KEY_SPACE		57
#define KEY_CAPSLOCK		58
#define KEY_F1			59
#define KEY_F2			60
#define KEY_F3			61
#define KEY_F4			62
#define KEY_F5			63
#define KEY_F6			64
#define KEY_F7			65
#define KEY_F8			66
#define KEY_F9			67
#define KEY_F10			68
#define KEY_NUMLOCK		69
#define KEY_SCROLLLOCK		70
#define KEY_KP7			71
#define KEY_KP8			72
#define KEY_KP9			73
#define KEY_KPMINUS		74
#define KEY_KP4			75
#define KEY_KP5			76
#define KEY_KP6			77
#define KEY_KPPLUS		78
#define KEY_KP1			79
#define KEY_KP2			80
#define KEY_KP3			81
#define KEY_KP0			82
#define KEY_KPDOT		83

#define KEY_ZENKAKUHANKAKU	85
#define KEY_102ND		86
#define KEY_F11			87
#define KEY_F12			88
#define KEY_RO			89
#define KEY_KATAKANA		90
#define KEY_HIRAGANA		91
#define KEY_HENKAN		92
#define KEY_KATAKANAHIRAGANA	93
#define KEY_MUHENKAN		94
#define KEY_KPJPCOMMA		95
#define KEY_KPENTER		96
#define KEY_RIGHTCTRL		97
#define KEY_KPSLASH		98
#define KEY_SYSRQ		99
#define KEY_RIGHTALT		100
#define KEY_LINEFEED		101
#define KEY_HOME		102
#define KEY_UP			103
#define KEY_PAGEUP		104
#define KEY_LEFT		105
#define KEY_RIGHT		106
#define KEY_END			107
#define KEY_DOWN		108
#define KEY_PAGEDOWN		109
#define KEY_INSERT		110
#define KEY_DELETE		111
#define KEY_MACRO		112
#define KEY_MUTE		113
#define KEY_VOLUMEDOWN		114
#define KEY_VOLUMEUP		115
#define KEY_POWER		116	/* SC System Power Down */
#define KEY_KPEQUAL		117
#define KEY_KPPLUSMINUS		118
#define KEY_PAUSE		119
#define KEY_SCALE		120	/* AL Compiz Scale (Expose) */

#define KEY_KPCOMMA		121
#define KEY_HANGEUL		122
#define KEY_HANGUEL		KEY_HANGEUL
#define KEY_HANJA		123
#define KEY_YEN			124
#define KEY_LEFTMETA		125
#define KEY_RIGHTMETA		126
#define KEY_COMPOSE		127

#define KEY_STOP		128	/* AC Stop */
#define KEY_AGAIN		129
#define KEY_PROPS		130	/* AC Properties */
#define KEY_UNDO		131	/* AC Undo */
#define KEY_FRONT		132
#define KEY_COPY		133	/* AC Copy */
#define KEY_OPEN		134	/* AC Open */
#define KEY_PASTE		135	/* AC Paste */
#define KEY_FIND		136	/* AC Search */
#define KEY_CUT			137	/* AC Cut */
#define KEY_HELP		138	/* AL Integrated Help Center */
#define KEY_MENU		139	/* Menu (show menu) */
#define KEY_CALC		140	/* AL Calculator */
#define KEY_SETUP		141
#define KEY_SLEEP		142	/* SC System Sleep */
#define KEY_WAKEUP		143	/* System Wake Up */
#define KEY_FILE		144	/* AL Local Machine Browser */
#define KEY_SENDFILE		145
#define KEY_DELETEFILE		146
#define KEY_XFER		147
#define KEY_PROG1		148
#define KEY_PROG2		149
#define KEY_WWW			150	/* AL Internet Browser */



namespace
{

	class Action
	{
		GameActionEnum e;

	public:
		Action(GameActionEnum e)
		{
			this->e = e;
		}

		GameActionEnum getGameActionEnum()
		{
			return e;
		}
	};


	class Key
	{
		const char* _name;
		int _key;

		Action *_action;

	public:

		Key(int key, const char*name)
		{
			this->_key = key;
			this->_name = name;
			this->_action = nullptr;
		}

		int key() const
		{
			return _key;
		}

		const char* name() const
		{
			return _name;
		}

		void setAction(Action *action)
		{
			this->_action = action;
		}

		Action* action() const
		{
			return _action;
		}
	};



	Key _keys[] =
	{
		{KEY_RESERVED, "RESERVED"},
		{KEY_ESC, "ESC"},
		{KEY_1, "1"},
		{KEY_2, "2"},
		{KEY_3, "3"},
		{KEY_4, "4"},
		{KEY_5, "5"},
		{KEY_6, "6"},
		{KEY_7, "7"},
		{KEY_8, "8"},
		{KEY_9, "9"},
		{KEY_0, "0"},
		{KEY_MINUS, "MINUS"},
		{KEY_EQUAL, "EQUAL"},
		{KEY_BACKSPACE, "BACKSPACE"},
		{KEY_TAB, "TAB"},
		{KEY_Q, "Q"},
		{KEY_W, "W"},
		{KEY_E, "E"},
		{KEY_R, "R"},
		{KEY_T, "T"},
		{KEY_Y, "Y"},
		{KEY_U, "U"},
		{KEY_I, "I"},
		{KEY_O, "O"},
		{KEY_P, "P"},
		{KEY_LEFTBRACE, "LEFTBRACE"},
		{KEY_RIGHTBRACE, "RIGHTBRACE"},
		{KEY_ENTER, "ENTER"},
		{KEY_LEFTCTRL, "LEFTCTRL"},
		{KEY_A, "A"},
		{KEY_S, "S"},
		{KEY_D, "D"},
		{KEY_F, "F"},
		{KEY_G, "G"},
		{KEY_H, "H"},
		{KEY_J, "J"},
		{KEY_K, "K"},
		{KEY_L, "L"},
		{KEY_SEMICOLON, "SEMICOLON"},
		{KEY_APOSTROPHE, "APOSTROPHE"},
		{KEY_GRAVE, "GRAVE"},
		{KEY_LEFTSHIFT, "LEFTSHIFT"},
		{KEY_BACKSLASH, "BACKSLASH"},
		{KEY_Z, "Z"},
		{KEY_X, "X"},
		{KEY_C, "C"},
		{KEY_V, "V"},
		{KEY_B, "B"},
		{KEY_N, "N"},
		{KEY_M, "M"},
		{KEY_COMMA, "COMMA"},
		{KEY_DOT, "DOT"},
		{KEY_SLASH, "SLASH"},
		{KEY_RIGHTSHIFT, "RIGHTSHIFT"},
		{KEY_KPASTERISK, "KPASTERISK"},
		{KEY_LEFTALT, "LEFTALT"},
		{KEY_SPACE, "SPACE"},
		{KEY_CAPSLOCK, "CAPSLOCK"},
		{KEY_F1, "F1"},
		{KEY_F2, "F2"},
		{KEY_F3, "F3"},
		{KEY_F4, "F4"},
		{KEY_F5, "F5"},
		{KEY_F6, "F6"},
		{KEY_F7, "F7"},
		{KEY_F8, "F8"},
		{KEY_F9, "F9"},
		{KEY_F10, "F10"},
		{KEY_NUMLOCK, "NUMLOCK"},
		{KEY_SCROLLLOCK, "SCROLLLOCK"},
		{KEY_KP7, "KP7"},
		{KEY_KP8, "KP8"},
		{KEY_KP9, "KP9"},
		{KEY_KPMINUS, "KPMINUS"},
		{KEY_KP4, "KP4"},
		{KEY_KP5, "KP5"},
		{KEY_KP6, "KP6"},
		{KEY_KPPLUS, "KPPLUS"},
		{KEY_KP1, "KP1"},
		{KEY_KP2, "KP2"},
		{KEY_KP3, "KP3"},
		{KEY_KP0, "KP0"},
		{KEY_KPDOT, "KPDOT"},
		{0, "NONE"},
		{KEY_ZENKAKUHANKAKU, "ZENKAKUHANKAKU"},
		{KEY_102ND, "102ND"},
		{KEY_F11, "F11"},
		{KEY_F12, "F12"},
		{KEY_RO, "RO"},
		{KEY_KATAKANA, "KATAKANA"},
		{KEY_HIRAGANA, "HIRAGANA"},
		{KEY_HENKAN, "HENKAN"},
		{KEY_KATAKANAHIRAGANA, "KATAKANAHIRAGANA"},
		{KEY_MUHENKAN, "MUHENKAN"},
		{KEY_KPJPCOMMA, "KPJPCOMMA"},
		{KEY_KPENTER, "KPENTER"},
		{KEY_RIGHTCTRL, "RIGHTCTRL"},
		{KEY_KPSLASH, "KPSLASH"},
		{KEY_SYSRQ, "SYSRQ"},
		{KEY_RIGHTALT, "RIGHTALT"},
		{KEY_LINEFEED, "LINEFEED"},
		{KEY_HOME, "HOME"},
		{KEY_UP, "UP"},
		{KEY_PAGEUP, "PAGEUP"},
		{KEY_LEFT, "LEFT"},
		{KEY_RIGHT, "RIGHT"},
		{KEY_END, "END"},
		{KEY_DOWN, "DOWN"},
		{KEY_PAGEDOWN, "PAGEDOWN"},
		{KEY_INSERT, "INSERT"},
		{KEY_DELETE, "DELETE"},
		{KEY_MACRO, "MACRO"},
		{KEY_MUTE, "MUTE"},
		{KEY_VOLUMEDOWN, "VOLUMEDOWN"},
		{KEY_VOLUMEUP, "VOLUMEUP"},
		{KEY_POWER, "POWER"},
		{KEY_KPEQUAL, "KPEQUAL"},
		{KEY_KPPLUSMINUS, "KPPLUSMINUS"},
		{KEY_PAUSE, "PAUSE"},
		{KEY_SCALE, "SCALE"},
		{KEY_KPCOMMA, "KPCOMMA"},
		{KEY_HANGEUL, "HANGEUL"}, //{KEY_HANGUEL, "HANGUEL"},
		{KEY_HANJA, "HANJA"},
		{KEY_YEN, "YEN"},
		{KEY_LEFTMETA, "LEFTMETA"},
		{KEY_RIGHTMETA, "RIGHTMETA"},
		{KEY_COMPOSE, "COMPOSE"},
		{KEY_STOP, "STOP"},
		{KEY_AGAIN, "AGAIN"},
		{KEY_PROPS, "PROPS"},
		{KEY_UNDO, "UNDO"},
		{KEY_FRONT, "FRONT"},
		{KEY_COPY, "COPY"},
		{KEY_OPEN, "OPEN"},
		{KEY_PASTE, "PASTE"},
		{KEY_FIND, "FIND"},
		{KEY_CUT, "CUT"},
		{KEY_HELP, "HELP"},
		{KEY_MENU, "MENU"},
		{KEY_CALC, "CALC"},
		{KEY_SETUP, "SETUP"},
		{KEY_SLEEP, "SLEEP"},
		{KEY_WAKEUP, "WAKEUP"},
		{KEY_FILE, "FILE"},
		{KEY_SENDFILE, "SENDFILE"},
		{KEY_DELETEFILE, "DELETEFILE"},
		{KEY_XFER, "XFER"},
		{KEY_PROG1, "PROG1"},
		{KEY_PROG2, "PROG2"},
		{KEY_WWW, "WWW"},
	};


	class Manager
	{

		void set(int id, Action *action)
		{
			 Key *key = &_keys[id];

			 //qDebug() << "set" << id << key->name();

			 key->setAction(action);
		}

	public:
		Manager()
		{
			set(KEY_LEFT, new Action(GameActionEnum::CHASSIS_LEFT_MOVEMENT));
			set(KEY_RIGHT, new Action(GameActionEnum::CHASSIS_RIGHT_MOVEMENT));
			set(KEY_UP, new Action(GameActionEnum::CHASSIS_FORWARD_MOVEMENT));
			set(KEY_DOWN, new Action(GameActionEnum::CHASSIS_BACKWARD_MOVEMENT));
			set(KEY_A, new Action(GameActionEnum::CHASSIS_LEFT_MOVEMENT));
			set(KEY_D, new Action(GameActionEnum::CHASSIS_RIGHT_MOVEMENT));
			set(KEY_E, new Action(GameActionEnum::FOLLOW_CAMERA_DOWN));
			set(KEY_Q, new Action(GameActionEnum::FOLLOW_CAMERA_UP));
			set(KEY_S, new Action(GameActionEnum::CHASSIS_BACKWARD_MOVEMENT));
			set(KEY_W, new Action(GameActionEnum::CHASSIS_FORWARD_MOVEMENT));
			set(KEY_Z, new Action(GameActionEnum::ROTATE_TURRET_LEFT));
			set(KEY_X, new Action(GameActionEnum::ROTATE_TURRET_RIGHT));
			set(KEY_C, new Action(GameActionEnum::CENTER_TURRET));
			set(KEY_COMMA, new Action(GameActionEnum::ROTATE_TURRET_LEFT));
			set(KEY_DOT, new Action(GameActionEnum::ROTATE_TURRET_RIGHT));
			set(KEY_SLASH, new Action(GameActionEnum::CENTER_TURRET));
			set(KEY_SPACE, new Action(GameActionEnum::SHOT));
			set(KEY_PAGEDOWN, new Action(GameActionEnum::FOLLOW_CAMERA_DOWN));
			set(KEY_PAGEUP, new Action(GameActionEnum::FOLLOW_CAMERA_UP));
			set(KEY_KP3, new Action(GameActionEnum::FOLLOW_CAMERA_DOWN));
			set(KEY_KP9, new Action(GameActionEnum::FOLLOW_CAMERA_UP));
			set(KEY_KP8, new Action(GameActionEnum::CHASSIS_FORWARD_MOVEMENT));
			set(KEY_KP2, new Action(GameActionEnum::CHASSIS_BACKWARD_MOVEMENT));
			set(KEY_KP4, new Action(GameActionEnum::CHASSIS_LEFT_MOVEMENT));
			set(KEY_KP6, new Action(GameActionEnum::CHASSIS_RIGHT_MOVEMENT));
			set(KEY_KP5, new Action(GameActionEnum::SHOT));
		}

	} manager;
}


GameActionEnum getBindingAction(KeyCode keycode)
{
#ifdef __linux__
	if (keycode < 8)
		return GameActionEnum::_NULL;

	keycode -= 8;
#endif

	if (keycode >= (sizeof(_keys) / sizeof(_keys[0])))
	{
		return GameActionEnum::_NULL;
	}

	const Key *key = &_keys[keycode];

	if (key == nullptr)
	{
		return GameActionEnum::_NULL;
	}

	//qDebug() << key->name() << (keycode + 8) << key->key() << (keycode == key->key());

	Action *action = key->action();

	if (action == nullptr)
		return GameActionEnum::_NULL;

	return action->getGameActionEnum();
}
