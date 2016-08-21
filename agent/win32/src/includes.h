#pragma once
#pragma comment (lib,"wininet.lib")
#pragma warning(disable:4786)

/******************************************************************************
* System Headers
*****************************************************************************/
#include <winsock2.h>
#include <winsock.h>
#include <ws2tcpip.h>
#include <string>
#include <windows.h>
#include <winternl.h>
#include <direct.h>
#include <ctime>
#include <tlhelp32.h>
#include <tchar.h>
#include <time.h>

typedef struct KEYLOG
{
	int threadnum;
	bool state;

} KEYLOG;
