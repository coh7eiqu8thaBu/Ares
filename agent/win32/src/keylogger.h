#pragma once
#include "define.h"
#include "includes.h"
#include "extern.h"

typedef struct _MYHOOKDATA
{
	int nType;
	HOOKPROC hkprc;
	HHOOK hhook;
} MYHOOKDATA;

enum LayoutCode {
	Qwerty,
	Azerty,
	Dvorak
};

char* translate(int vk, bool up);

#define SizeOfIndexKeyLayout 22 // must be recalculate manually

// Tab for translation of different layout keyboard
// First tab : Qwerty, Azerty, Dvorak
// Second tab: normal / Shift / AltGr
char TabKeyLayout[3][3][SizeOfIndexKeyLayout] = {
{/* Keyboard QWERTY */	{' ','1','2','3','4' ,'5','6','7','8','9','0','-','=',' ',' ',' ',' ',' ',' ',' ',' ',' '},
						{'"','!','@','#','$' ,'%','^','&','*','(',')','_','+',' ',' ',' ',' ',' ',' ',' ',' ',' '},
						{} },
{/* Keyboard AZERTY */	{'²','&','é','"','\'','(','-','è','_','ç','à',')','=','^','$','ù','*','<',',',';',':','!'},
						{0x0,'1','2','3','4' ,'5','6','7','8','9','0','°','+','¨','£','%','µ','>','?','.','/','§'},
						{} },
{/* Keyboard Dvorak */	{' ','1','2','3','4' ,'5','6','7','8','9','0','[',']',' ',' ',' ',' ',' ',' ',' ',' ',' '},
						{'"','!','@','#','$' ,'%','^','&','*','(',')','{','}',' ',' ',' ',' ',' ',' ',' ',' ',' '},
						{} }
};
int IndexKeyLayout[3][SizeOfIndexKeyLayout] = {
	{ 0xDE,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x30,0xDB,0xBB, 0xDD,0xBA, 0xC0,0xDC, 0xE2,0xBC,0xBE,0xBF,0xDF },
	{ 0xDE,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x30,0xDB,0xBB, 0xDD,0xBA, 0xC0,0xDC, 0xE2,0xBC,0xBE,0xBF,0xDF },
	{ 0xDE,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x30,0xDB,0xBB, 0xDD,0xBA, 0xC0,0xDC, 0xE2,0xBC,0xBE,0xBF,0xDF }
};
