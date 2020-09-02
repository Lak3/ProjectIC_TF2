#pragma once
#include "../../Includes/Includes.h"

class C_BaseHudChat
{
public:
	void ChatPrintf(int pIndex, const char *fmt, ...)
	{
		typedef void *(__thiscall *FN)(PVOID, int, const char *);
		GetVFunc<FN>(this, 19)(this, pIndex, fmt);
	}
};

class C_ClientModeShared
{
public:
	char szPad[28];
	C_BaseHudChat *m_pChatElement;
};