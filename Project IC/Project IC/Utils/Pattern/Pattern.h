#pragma once
#include <Windows.h>

class CPattern
{
private:
	DWORD FindPattern(DWORD dwAddress, DWORD dwLength, PCCH szPattern);
	HMODULE GetModuleHandleSafe(PCCH szModuleName);
public:
	DWORD Find(PCCH szModuleName, PCCH szPattern);
};

extern CPattern gPattern;