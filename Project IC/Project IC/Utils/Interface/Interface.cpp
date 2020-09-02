#include "Interface.h"

PVOID CInterface::Get(PCCH szModule, PCCH szObject)
{
	HMODULE Module = GetModuleHandleA(szModule);

	if (!Module)
		return nullptr;

	DWORD CreateInterface	= (DWORD)GetProcAddress(Module, "CreateInterface");
	DWORD ShortJump			= (DWORD)CreateInterface + 5;
	DWORD Jump				= (((DWORD)CreateInterface + 5) + *(DWORD *)ShortJump) + 4;
	Interface_t *List		= **(Interface_t ***)(Jump + 6);

	while (List)
	{
		if (List && (strstr(List->szInterfaceName, szObject) && (strlen(List->szInterfaceName) - strlen(szObject)) < 5))
			return List->Interface();

		List = List->NextInterface;
	}

	return nullptr;
}

CInterface gInterface;