#pragma once

//Main hash function
//And prehashed strings allowing for fast comparisong.
namespace Hash
{
	int String(const char* szOrg);

	//Panels
	constexpr int HudScope = 200208064;
	constexpr int FocusOverlayPanel = -861513321;

	//Health packs (Probably missing some)
	constexpr int MedKitSmall = 1941719626;
	constexpr int MedKitMedium = 642350162;
	constexpr int MedKitLarge = 1102367452;
	constexpr int MedKitSmallBday = -2020233079;
	constexpr int MedKitMediumBday = -1898223727;
	constexpr int MedKitLargeBday = -1183884197;
	constexpr int MedKitPlate = -159986486;
	constexpr int MedKitSteak = -1325824063;
	constexpr int MedKitSmallHalloween = 1667112440;
	constexpr int MedKitMediumHalloween = 170247616;
	constexpr int MedKitLargeHalloween = 827760266;
	constexpr int MedKitLargeMushRoom = 687823304;

	//Ammo packs (Probably missing some)
	constexpr int AmmoSmall = 913481717;
	constexpr int AmmoMedium = 1070237533;
	constexpr int AmmoLarge = 74129543;
	constexpr int AmmoLargeBday = 1933898982;
	constexpr int AmmoMediumBday = -2090593924;

	//Atombomb is MvM's bomb, briefcase is CTF's intel.
	//Check this when making the intel ESP, both are CCaptureFlag.
	constexpr int AtomBomb = -1938815603;
	constexpr int BriefCase = 381882864;

	bool m_bIsAmmo(int nHash);
	bool m_bIsHealth(int nHash);
}