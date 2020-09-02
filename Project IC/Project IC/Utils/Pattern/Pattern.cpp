#include "Pattern.h"

#define INRANGE(x,a,b)	(x >= a && x <= b) 
#define GetBits(x)		(INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define GetByte(x)		(GetBits(x[0]) << 4 | GetBits(x[1]))

DWORD CPattern::FindPattern(DWORD dwAddress, DWORD dwLength, PCCH szPattern)
{
	PCCH szPat = szPattern;
	DWORD dwFirstMatch = 0x0;

	for (DWORD pCur = dwAddress; pCur < dwLength; pCur++)
	{
		if (!*szPat)
			return dwFirstMatch;

		if (*(PBYTE)szPat == '\?' || *(BYTE *)pCur == GetByte(szPat))
		{
			if (!dwFirstMatch)
				dwFirstMatch = pCur;

			if (!szPat[2])
				return dwFirstMatch;

			if (*(PWORD)szPat == '\?\?' || *(PBYTE)szPat != '\?')
				szPat += 3;

			else szPat += 2;
		}

		else {
			szPat = szPattern;
			dwFirstMatch = 0;
		}
	}

	return 0x0;
}

HMODULE CPattern::GetModuleHandleSafe(PCCH szModuleName)
{
	HMODULE hmModuleHandle = 0;

	do
	{
		hmModuleHandle = GetModuleHandleA(szModuleName);
		Sleep(1);
	} while (hmModuleHandle == 0);

	return hmModuleHandle;
}

DWORD CPattern::Find(PCCH szModuleName, PCCH szPattern)
{
	HMODULE hmModule = GetModuleHandleSafe(szModuleName);
	PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)hmModule;
	PIMAGE_NT_HEADERS pNTHeaders = (PIMAGE_NT_HEADERS)(((DWORD)hmModule) + pDOSHeader->e_lfanew);

	return FindPattern(((DWORD)hmModule) + pNTHeaders->OptionalHeader.BaseOfCode, ((DWORD)hmModule) + pNTHeaders->OptionalHeader.SizeOfCode, szPattern);
}

CPattern gPattern;