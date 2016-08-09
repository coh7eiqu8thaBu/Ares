#include "includes.h"
#include "define.h"
#include "ntp.h"

void noav()
{
	unsigned int ExitCode = 0;

	ExitCode += check_time_warp();



	if (ExitCode != 0)
	{
		exit(ExitCode);
	}
}

int check_time_warp(void)
{
	SYSTEMTIME st, lt, sft;
	LARGE_INTEGER ntst;
	FILETIME ft;
	time_t ntp_time, sys_time;
	int diff_time;
	struct tm  sys_tm_time;
	unsigned int ExitCode = 0;

	GetSystemTime(&st);
	GetLocalTime(&lt);
	NtQuerySystemTime(&ntst);
	GetSystemTimeAsFileTime(&ft);
	FileTimeToSystemTime(&ft, &sft);

	if (&st == NULL || &lt == NULL || &ft == NULL)
	{
		ExitCode += EXIT_CODE_TIME_WARP_NULL; // probably BitDefender or an another AV SandBox
	}

	if (st.wYear <= 2015)
	{
		ExitCode += EXIT_CODE_TIME_WARP_2015; // Incoherent Date
	}

	/* Check if the time is too far from the NTP time
	sometime sandbox try to do timewarp ...
	*/

	// convert system time in UTC to a tm struct
	sys_tm_time.tm_year = st.wYear - 1900;
	sys_tm_time.tm_mon = st.wMonth - 1;
	sys_tm_time.tm_mday = st.wDay;
	sys_tm_time.tm_hour = st.wHour;
	sys_tm_time.tm_min = st.wMinute;
	sys_tm_time.tm_sec = st.wSecond;
	// convert to EPOC
	sys_time = mktime(&sys_tm_time);
	// Get UTC from NTP server
	ntp_time = Get_NTP_Time();
	// Calculate the diff
	diff_time = sys_time - ntp_time;
	// Check the diff, if it's too different, we are in a time warp
	if ((diff_time <= -UTC_TIME_DIFF) && (diff_time >= UTC_TIME_DIFF))
	{
		ExitCode += EXIT_CODE_TIME_WARP_NTPD;
	}
	return ExitCode;
}

int dns_lookup(const char *host, sockaddr_in *out)
{
	struct addrinfo *result;
	int ret = getaddrinfo(host, "ntp", NULL, &result);
	for (struct addrinfo *p = result; p; p = p->ai_next)
	{
		if (p->ai_family != AF_INET)
			continue;

		memcpy(out, p->ai_addr, sizeof(*out));
	}
	freeaddrinfo(result);
}

time_t Get_NTP_Time()
{
	WSADATA wsaData;
	DWORD ret = WSAStartup(MAKEWORD(2, 0), &wsaData);

	char *host = POOL_NTP_HOST;  /* Don't distribute stuff pointing here, it's not polite. */
								 //char *host = "time.nist.gov"; 
								 /* This one's probably ok, but can get grumpy about request rates during debugging. */

	NTPMessage msg;
	/* Important, if you don't set the version/mode, the server will ignore you. */
	msg.clear();
	msg.version = 3;
	msg.mode = 3 /* client */;

	NTPMessage response;
	response.clear();

	int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockaddr_in srv_addr;
	memset(&srv_addr, 0, sizeof(srv_addr));
	dns_lookup(host, &srv_addr); /* Helper function defined below. */

	msg.sendto(sock, &srv_addr);
	response.recv(sock);

	WSACleanup();

	return response.tx.to_time_t();
}

void noreverse()
{

}