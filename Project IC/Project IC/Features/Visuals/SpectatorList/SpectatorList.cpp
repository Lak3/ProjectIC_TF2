#include "SpectatorList.h"

std::wstring C_SpectatorList::ConvertUtf8ToWide(const std::string &str)
{
	int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), NULL, 0);
	std::wstring wstr(count, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), &wstr[0], count);
	return wstr;
}

bool C_SpectatorList::GetSpectators(C_BaseEntity *pLocal)
{
	Spectators.clear();

	const auto Teammates = gEntCache.GetGroup(GroupType_t::PLAYERS_TEAMMATES);

	for (const auto &Teammate : Teammates)
	{
		if (!Teammate || Teammate->IsAlive())
			continue;

		C_BaseEntity *pObservedPlayer = gInts.EntityList->GetClientEntityFromHandle(Teammate->GetObserverTarget());

		if (pObservedPlayer != pLocal)
			continue;

		std::string sMode = "";

		switch (Teammate->GetObserverMode())
		{
			case OBS_MODE_FIRSTPERSON: {
				sMode = "1st";
				break;
			}

			case OBS_MODE_THIRDPERSON: {
				sMode = "3rd";
				break;
			}

			default: continue;
		}

		PlayerInfo_t PlayerInfo = {};

		if (!gInts.Engine->GetPlayerInfo(Teammate->GetIndex(), &PlayerInfo))
			continue;

		Spectators.push_back({ PlayerInfo.name, sMode });
	}

	return !Spectators.empty();
}

bool C_SpectatorList::ShouldRun(C_BaseEntity *pLocal)
{
	if (!bActive)
		return false;

	if (!pLocal->IsAlive())
		return false;

	if (!gInts.Engine->IsInGame() || !gInts.Engine->IsConnected() || gInts.Engine->Con_IsVisible())
		return false;

	if (gInts.EngineVGui->IsGameUIVisible())
		return false;

	return true;
}

void C_SpectatorList::Run()
{
	if (const auto pLocal{ gEntCache.pLocal })
	{
		if (!ShouldRun(pLocal))
			return;

		if (!GetSpectators(pLocal))
			return;

		int nTall = gDraw.Fonts.at(FONT_ESP_NAME_OUTLINED).nTall;
		int nDrawn = 0;

		static Color_t SpectatorColor = { 0, 98, 102, 255 };

		for (const auto &Spectator : Spectators)
		{
			int nDrawX = (gScreenSize.w / 2);
			int nDrawY = (((gScreenSize.h / 2) + 50) + (nTall * nDrawn));

			//this is gay we need that var stuff in string draw func for wstrings
			std::wstring name = ConvertUtf8ToWide(Spectator.sName);
			std::wstring mode = ConvertUtf8ToWide(Spectator.sMode);
			std::wstring wstr = (std::wstring(L"[") + mode + std::wstring(L"]") + std::wstring(L" ") + name);

			gDraw.StringCenterW(FONT_ESP_NAME_OUTLINED, nDrawX, nDrawY, SpectatorColor, wstr.c_str());

			nDrawn++;
		}
	}
}

C_SpectatorList gSpectatorList;