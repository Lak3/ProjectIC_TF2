#pragma once
#include "../../../SDK/SDK.h"

class C_SpectatorList
{
private:
	std::wstring ConvertUtf8ToWide(const std::string &str);

private:
	struct Spectator_t
	{
		std::string sName = "";
		std::string sMode = "";
	};

	std::vector<Spectator_t> Spectators;

	bool GetSpectators(C_BaseEntity *pLocal);

private:
	bool ShouldRun(C_BaseEntity *pLocal);

public:
	bool bActive = true;

	void Run();
};

extern C_SpectatorList gSpectatorList;