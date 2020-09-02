#include "ESP.h"

#include "../../Aimbot/Aimbot.h"

std::wstring C_ESP::ConvertUtf8ToWide(const std::string& str)
{
	int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), NULL, 0);
	std::wstring wstr(count, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), &wstr[0], count);
	return wstr;
}

bool C_ESP::ShouldRun()
{
	if (!bActive)
		return false;

	if (!gInts.Engine->IsInGame() || !gInts.Engine->IsConnected() || gInts.Engine->Con_IsVisible())
		return false;

	if (gInts.EngineVGui->IsGameUIVisible())
		return false;

	return true;
}

bool C_ESP::GetDrawBounds(C_BaseEntity* pEntity, int& x, int& y, int& w, int& h)
{
	if (!gInts.GameMovement || !pEntity)
		return false;

	const matrix3x4& transform = pEntity->GetRgflCoordinateFrame();

	Vec3 mins = Vec3();
	Vec3 maxs = Vec3();

	if (pEntity->IsPlayer())
	{
		mins = gInts.GameMovement->GetPlayerMins(pEntity->IsDucking());
		maxs = gInts.GameMovement->GetPlayerMaxs(pEntity->IsDucking());
	}

	else
	{
		mins = pEntity->GetCollideableMins();
		maxs = pEntity->GetCollideableMaxs();
	}

	Vec3 points[] = {
		Vec3(mins.x, mins.y, mins.z),
		Vec3(mins.x, maxs.y, mins.z),
		Vec3(maxs.x, maxs.y, mins.z),
		Vec3(maxs.x, mins.y, mins.z),
		Vec3(maxs.x, maxs.y, maxs.z),
		Vec3(mins.x, maxs.y, maxs.z),
		Vec3(mins.x, mins.y, maxs.z),
		Vec3(maxs.x, mins.y, maxs.z)
	};

	Vec3 transformed_points[8];

	for (int i = 0; i < 8; i++)
		Math::VectorTransform(points[i], transform, transformed_points[i]);

	Vec3 flb, brt, blb, frt, frb, brb, blt, flt;

	if (   Math::W2S(transformed_points[3], flb) && Math::W2S(transformed_points[5], brt)
		&& Math::W2S(transformed_points[0], blb) && Math::W2S(transformed_points[4], frt)
		&& Math::W2S(transformed_points[2], frb) && Math::W2S(transformed_points[1], brb)
		&& Math::W2S(transformed_points[6], blt) && Math::W2S(transformed_points[7], flt)
		&& Math::W2S(transformed_points[6], blt) && Math::W2S(transformed_points[7], flt))
	{
		Vec3 arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

		float left = flb.x;
		float top = flb.y;
		float righ = flb.x;
		float bottom = flb.y;

		for (int n = 1; n < 8; n++) {
			if (left > arr[n].x)
				left = arr[n].x;

			if (top < arr[n].y)
				top = arr[n].y;

			if (righ < arr[n].x)
				righ = arr[n].x;

			if (bottom > arr[n].y)
				bottom = arr[n].y;
		}

		float x_ = left;
		float y_ = bottom;
		float w_ = (righ - left);
		float h_ = (top - bottom);

		if (pEntity->IsPlayer())
		{
			x_ += ((righ - left) / 8.0f);
			w_ -= (((righ - left) / 8.0f) * 2.0f);
		}

		x = static_cast<int>(x_);
		y = static_cast<int>(y_);
		w = static_cast<int>(w_);
		h = static_cast<int>(h_);

		if (x > gScreenSize.w || (x + w) < 0 || y > gScreenSize.h || (y + h) < 0)
			return false;

		return true;
	}

	return false;
}

Color_t C_ESP::GetDrawColor(C_BaseEntity* pLocal, C_BaseEntity* pEntity)
{
	Color_t out = gDraw.GetTeamColor(pEntity->GetTeamNum());

	//instead of the team color let's give it the friend's color, let's keep all the other overrides
	if (pLocal->IsPlayerOnSteamFriendList(pEntity))
		out = FriendColor;

	//this check is because for some reason some buildings appear to be cloaked or invulnerable??
	//Also stickies do that.
	if (pEntity->IsPlayer())
	{
		if (pEntity->IsCloaked())
			out = CloakColor;

		if (!pEntity->IsVulnerable())
			out = InvulnColor;
	}

	//target color should override all others
	if (pEntity->GetIndex() == gGlobalInfo.nCurrentTargetIdx)
		out = TargetColor;

	return out;
}

void C_ESP::Run()
{
	if (auto pLocal{ gEntCache.pLocal })
	{
		if (!ShouldRun())
			return;

		if (bPickups)
		{
			size_t FONT = (nOutline ? FONT_ESP_PICKUPS_OUTLINED : FONT_ESP_PICKUPS);

			if (bHealth)
			{
				const auto HealthPacks{ gEntCache.GetGroup(GroupType_t::HEALTHPACKS_ALL) };

				for (const auto &pHealth : HealthPacks)
				{
					int x = 0, y = 0, w = 0, h = 0;

					if (!GetDrawBounds(pHealth, x, y, w, h))
						continue;

					gDraw.StringCenterV(FONT, x + (w / 2), y + (h / 2), HealthColor, "health");
				}
			}

			if (bAmmo)
			{
				const auto AmmoPacks{ gEntCache.GetGroup(GroupType_t::AMMOPACKS_ALL) };

				for (const auto &pAmmo : AmmoPacks)
				{
					int x = 0, y = 0, w = 0, h = 0;

					if (!GetDrawBounds(pAmmo, x, y, w, h))
						continue;

					gDraw.StringCenterV(FONT, x + (w / 2), y + (h / 2), AmmoColor, "ammo");
				}
			}
		}

		if (bBuildings)
		{
			const auto Buildings{ gEntCache.GetGroup(bNoTeammateBuildings ? GroupType_t::BUILDINGS_ENEMIES : GroupType_t::BUILDINGS_ALL) };

			for (const auto& pBuilding : Buildings)
			{
				auto Building{ reinterpret_cast<C_BaseObject*>(pBuilding) };

				if (!Building->IsAlive())
					continue;

				int x = 0, y = 0, w = 0, h = 0;

				if (GetDrawBounds(Building, x, y, w, h))
				{
					Color_t DrawColor = GetDrawColor(pLocal, Building);
					Color_t HealthColor = gDraw.GetHealthColor(Building->GetHealth(), Building->GetMaxHealth());

					size_t FONT = (nOutline ? FONT_ESP_OUTLINED : FONT_ESP);
					int nTextX = ((x + w) + 3);
					int nTextOffset = 0;

					if (bBuildingName)
					{
						size_t FONT = (nOutline ? FONT_ESP_NAME_OUTLINED : FONT_ESP_NAME);

						const char* szName = "";

						if (Building->IsSentrygun())
							szName = "sentrygun";

						else if (Building->IsDispenser())
							szName = "dispenser";

						else if (Building->IsTeleporter())
							szName = "teleporter";

						else szName = "unknown";

						int offset = gDraw.Fonts.at(FONT_ESP_NAME).nTall;
						gDraw.StringCenter(FONT, (x + (w / 2)), (y - offset), DrawColor, szName);
					}

					if (bBuildingHealth)
					{
						gDraw.String(FONT, nTextX, (y + nTextOffset), HealthColor, "%d/%d", Building->GetHealth(), Building->GetMaxHealth());
						nTextOffset += gDraw.Fonts.at(FONT).nTall;
					}

					if (bBuildingLevel)
					{
						gDraw.String(FONT, nTextX, (y + nTextOffset), DrawColor, "%d/3", Building->GetLevel());
						nTextOffset += gDraw.Fonts.at(FONT).nTall;
					}

					if (bBuildingCond)
					{
						size_t FONT = (nOutline ? FONT_ESP_COND_OUTLINED : FONT_ESP_COND);

						std::string cond_str = "";

						if (Building->IsSentrygun() && Building->IsControlled())
							cond_str += "CONTROLLED "; //dispensers are also apparently controlled sometimes

						if (Building->IsDisabled())
							cond_str += "DISABLED ";

						if (!cond_str.empty())
						{
							cond_str.erase((cond_str.end() - 1), cond_str.end());

							static const int offset = 2;

							//prevent overlaping
							int nY = ((y + nTextOffset) > ((y + h) + offset) ? (y + nTextOffset) : ((y + h) + offset));

							gDraw.StringCenter(FONT, (x + (w / 2)), nY, CondColor, cond_str.c_str());
						}
					}

					if (bBuildingHealthBar)
					{
						float nHealth = static_cast<float>(Building->GetHealth());
						float nMaxHealth = static_cast<float>(Building->GetMaxHealth());

						if (nHealth > nMaxHealth)
							nHealth = nMaxHealth;

						static const int nWidth = 2;
						int nHeight = (h + (nHealth < nMaxHealth ? 2 : 1));
						int nHeight2 = (h + 1);

						float ratio = (nHealth / nMaxHealth);

						gDraw.Rect(((x - nWidth) - 2), y, nWidth, nHeight2, OutlineColor);
						gDraw.Rect(((x - nWidth) - 2), (y + nHeight - (nHeight * ratio)), nWidth, (nHeight * ratio), HealthColor);
					}

					if (nBuildingBox)
					{
						if (nBuildingBox == 1)
						{
							gDraw.OutlinedRect(x, y, w, h, DrawColor);
							if (nOutline == 2)
								gDraw.OutlinedRect((x - 1), (y - 1), (w + 2), (h + 2), OutlineColor);
						}

						else if (nBuildingBox == 2)
						{
							gDraw.CornerRect(x, y, w, h, 3, 5, DrawColor);
							if (nOutline == 2)
								gDraw.CornerRect((x - 1), (y - 1), (w + 2), (h + 2), 3, 5, OutlineColor);
						}
					}
				}
			}
		}

		if (bPlayers)
		{
			const auto Players{ gEntCache.GetGroup(GroupType_t::PLAYERS_ALL) };

			for (const auto& Player : Players)
			{
				if (!Player->IsAlive())
					continue;

				if (Player == pLocal)
					continue;

				bool bIsFriend = pLocal->IsPlayerOnSteamFriendList(Player);

				switch (nNoTeammatePlayers)
				{
					case 0: { break; }
					case 1: { if (Player->GetTeamNum() == pLocal->GetTeamNum()) { continue; } break; }
					case 2: { if (Player->GetTeamNum() == pLocal->GetTeamNum() && !bIsFriend) { continue; } break; }
				}

				switch (nNoCloakedPlayers)
				{
					case 0: { break; }
					case 1: { if (Player->IsCloaked()) { continue; } break; }
					case 2: { if (Player->IsCloaked() && Player->GetTeamNum() != pLocal->GetTeamNum()) { continue; } break; }
				}

				int x = 0, y = 0, w = 0, h = 0;

				if (GetDrawBounds(Player, x, y, w, h))
				{
					Color_t DrawColor = GetDrawColor(pLocal, Player);
					Color_t HealthColor = gDraw.GetHealthColor(Player->GetHealth(), Player->GetMaxHealth());

					size_t FONT = (nOutline ? FONT_ESP_OUTLINED : FONT_ESP);
					int nTextX = ((x + w) + 3);
					int nTextOffset = 0;

					if (bPlayerName)
					{
						size_t FONT = (nOutline ? FONT_ESP_NAME_OUTLINED : FONT_ESP_NAME);

						PlayerInfo_t PlayerInfo = {};

						if (gInts.Engine->GetPlayerInfo(Player->GetIndex(), &PlayerInfo))
						{
							auto str = ConvertUtf8ToWide(PlayerInfo.name);

							int offset = gDraw.Fonts.at(FONT).nTall;
							gDraw.StringCenterW(FONT, (x + (w / 2)), (y - offset), DrawColor, str.c_str());
						}
					}

					if (bPlayerClass)
					{
						int offset = (bPlayerName ? gDraw.Fonts.at(FONT_ESP_NAME).nTall : 0);
						static constexpr int nSize = 18;
						gDraw.Texture((x + (w / 2) - (nSize / 2)), ((y - offset) - nSize), nSize, nSize, Player->GetClassNum());

						/*gDraw.String(FONT_ESP, nTextX, (y + nTextOffset), DrawColor, Player->szGetClass());
						nTextOffset += gDraw.Fonts.at(FONT_ESP).nTall;*/
					}

					if (bPlayerHealth)
					{
						gDraw.String(FONT, nTextX, (y + nTextOffset), HealthColor, "%d/%d", Player->GetHealth(), Player->GetMaxHealth());
						nTextOffset += gDraw.Fonts.at(FONT).nTall;
					}

					if (bPlayerCond)
					{
						size_t FONT = (nOutline ? FONT_ESP_COND_OUTLINED : FONT_ESP_COND);

						std::string cond_str = "";

						if (Player->IsSlowed())
						{
							C_BaseCombatWeapon* pWeapon = Player->GetActiveWeapon();

							if (pWeapon && pWeapon->GetWeaponID() == TF_WEAPON_MINIGUN)
								cond_str += "REV ";
						}

						if (Player->IsUbered())
							cond_str += "UBER ";

						if (Player->IsBonked())
							cond_str += "BONK ";

						if (Player->IsCritBoosted())
							cond_str += "KRIT ";

						if (Player->IsCloaked())
							cond_str += "CLOAK ";

						if (Player->IsScoped())
							cond_str += "SCOPE ";

						if (Player->IsTaunting())
							cond_str += "TAUNT ";

						if (!cond_str.empty())
						{
							cond_str.erase((cond_str.end() - 1), cond_str.end());

							static const int offset = 2;

							gDraw.StringCenter(FONT, (x + (w / 2)), ((y + h) + offset), CondColor, cond_str.c_str());
						}
					}

					if (bPlayerHealthBar)
					{
						float nHealth = static_cast<float>(Player->GetHealth());
						float nMaxHealth = static_cast<float>(Player->GetMaxHealth());
						float overheal = 0.0f;

						if (nHealth > nMaxHealth) {
							overheal = fmod(nHealth, nMaxHealth);
							nHealth = nMaxHealth;
						}

						static const int nWidth = 2;
						int nHeight = (h + (nHealth < nMaxHealth ? 2 : 1));
						int nHeight2 = (h + 1);

						float ratio = (nHealth / nMaxHealth);

						gDraw.Rect(((x - nWidth) - 2), y, nWidth, nHeight2, OutlineColor);
						gDraw.Rect(((x - nWidth) - 2), (y + nHeight - (nHeight * ratio)), nWidth, (nHeight * ratio), HealthColor);
						
						if (nOutline == 2)
							gDraw.OutlinedRect(((x - nWidth) - 3), (y - 1), (nWidth + 2), (nHeight2 + 2), OutlineColor);

						if (overheal > 0.0f) {
							ratio = (overheal / nMaxHealth);
							gDraw.Rect(((x - nWidth) - 2), (y + (nHeight + 1) - (nHeight * ratio)), nWidth, (nHeight * ratio), OverhealColor);
						}
					}

					if (nPlayerBox)
					{
						if (nPlayerBox == 1)
						{
							int height = (h + 1); //don't ask me /shrug

							gDraw.OutlinedRect(x, y, w, height, DrawColor);
							if (nOutline == 2)
								gDraw.OutlinedRect((x - 1), (y - 1), (w + 2), (height + 2), OutlineColor);
						}

						else if (nPlayerBox == 2)
						{
							gDraw.CornerRect(x, y, w, h, 3, 5, DrawColor);
							if (nOutline == 2)
								gDraw.CornerRect((x - 1), (y - 1), (w + 2), (h + 2), 3, 5, OutlineColor);
						}
					}
				}
			}
		}
	}
}

//This logic considers all other than MvM's bombs as captures.
//IDK if it's better to call them "capture" or "intel", change this to whatever's best.
std::string C_ESP::szGetFlag(int nHash) {
	switch (nHash) {
		case Hash::AtomBomb: return "bomb";
		default: return "capture";
	}
}

C_ESP gESP;