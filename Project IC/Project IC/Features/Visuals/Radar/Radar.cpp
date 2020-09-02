#include "Radar.h"

#include "../ESP/ESP.h"
#include "../../Menu/Menu.h"

C_Radar gRadar;

constexpr int TEXTURE_SIZE = 28;
constexpr int HALF = 14;
constexpr int RECT_SIZE = 28;
constexpr int RED = 10;
constexpr int BLUE = 20;

int nRadarX = 200, nRadarY = 200;

void C_Radar::Paint()
{
	if (!ShouldRun())
		return;

	int nRadarSize = static_cast<int>(flRadarSize);

	//Background
	{
		int nCentX = nRadarX;
		int nCentY = nRadarY;
		int nSize = nRadarSize + HALF;

		gDraw.Rect(nCentX - nSize, nCentY - nSize, 2 * nSize + 2, 2 * nSize + 2, { 30, 30, 30, 128 });

		nSize++;

		gDraw.Rect(nCentX, nCentY - nSize, 1, 2 * nSize, { 207, 106, 135, 255 });
		gDraw.Rect(nCentX - nSize, nCentY, 2 * nSize, 1, { 207, 106, 135, 255 });
		gDraw.OutlinedRect(nCentX - nSize, nCentY - nSize, 2 * nSize + 2, 2 * nSize + 2, { 0, 0, 0, 255 });
	}

	if (const auto pLocal = gEntCache.pLocal)
	{
		Vec3 vLocalPos = pLocal->GetAbsOrigin();

		if (!GetPlayers(pLocal, vLocalPos))
			return;

		int nLocalTeam = pLocal->GetTeamNum();

		Vec3 vLocAngles;
		gInts.Engine->GetViewAngles(vLocAngles);

		for (const auto RadarInfo : m_vecInfo)
		{
			float flDeltaX = RadarInfo.m_vOrigin.x - vLocalPos.x;
			float flDeltaY = RadarInfo.m_vOrigin.y - vLocalPos.y;

			float flLocalY = vLocAngles.y * (PI / 180.0f);
			float flLocalCos = cos(flLocalY);
			float flLocalSin = sin(flLocalY);

			float flX = flDeltaY * (-flLocalCos) + flDeltaX * flLocalSin;
			float flY = flDeltaX * (-flLocalCos) - flDeltaY * flLocalSin;

			if (fabs(flX) > flRange || fabs(flY) > flRange)
			{
				if (flY > flX)
				{
					if (flY > -flX)
					{
						flX = flRange * flX / flY;
						flY = flRange;
					}
					else
					{
						flY = -flRange * flY / flX;
						flX = -flRange;
					}
				}
				else
				{
					if (flY > -flX)
					{
						flY = flRange * flY / flX;
						flX = flRange;
					}
					else
					{
						flX = -flRange * flX / flY;
						flY = -flRange;
					}
				}
			}

			int	nX = (nRadarX + static_cast<int>(flX / flRange * flRadarSize)) - HALF;
			int nY = (nRadarY + static_cast<int>(flY / flRange * flRadarSize)) - HALF;

			gDraw.Texture(nX, nY, TEXTURE_SIZE, TEXTURE_SIZE, RadarInfo.m_nTexture);
			//gDraw.OutlinedRect(nX, nY, RECT_SIZE, RECT_SIZE, RadarInfo.m_clrColor);

			float nHealth = static_cast<float>(RadarInfo.m_nHealth);
			float nMaxHealth = static_cast<float>(RadarInfo.m_nMaxHealth);
			float flOverHeal = 0.0f;

			if (nHealth > nMaxHealth)
			{
				flOverHeal = fmod(nHealth, nMaxHealth);
				nHealth = nMaxHealth;
			}

			static const int nWidth = 2;
			int nHeight = TEXTURE_SIZE;

			float flRatio = (nHealth / nMaxHealth);

			gDraw.Rect(((nX - nWidth) - 1), nY, nWidth, nHeight, OutlineColor);
			gDraw.Rect(((nX - nWidth) - 1), (nY + nHeight - (nHeight * flRatio)), nWidth, (nHeight * flRatio), RadarInfo.m_clrHealth);

			if (flOverHeal > 0.0f)
			{
				flRatio = (flOverHeal / nMaxHealth);
				gDraw.Rect(((nX - nWidth) - 1), (nY + (nHeight + 1) - (nHeight * flRatio)), nWidth, (nHeight * flRatio), OverhealColor);
			}
		}
	}

	if (gMenu.m_Open)
		Drag(nRadarX, nRadarY, nRadarSize + HALF, nRadarSize + HALF);
}

bool C_Radar::GetPlayers(C_BaseEntity* pLocal, Vec3 vLocal)
{
	m_vecInfo.clear();

	int nLocalTeam = pLocal->GetTeamNum();

	for (const auto pEntity : gEntCache.GetGroup(GroupType_t::PLAYERS_ALL))
	{
		if (!pEntity || !pEntity->IsAlive() || pEntity == pLocal)
			continue;

		int nEntTeam = pEntity->GetTeamNum();
		bool bIsFriend = pLocal->IsPlayerOnSteamFriendList(pEntity);

		switch (nNoTeammatePlayers)
		{
		case 0: { break; }
		case 1: { if (nEntTeam == nLocalTeam) { continue; } break; }
		case 2: { if (nEntTeam == nLocalTeam && !bIsFriend) { continue; } break; }
		}

		switch (nNoCloakedPlayers)
		{
		case 0: { break; }
		case 1: { if (pEntity->IsCloaked()) { continue; } break; }
		case 2: { if (pEntity->IsCloaked() && nEntTeam != nLocalTeam) { continue; } break; }
		}

		int nEntClass = pEntity->GetClassNum();
		int nTexture = -1;

		if (pEntity->IsPlayer() && pEntity->IsCloaked())
		{
			nTexture = 20;
		}
		else
		{
			switch (nEntTeam) {
			case 2: nTexture = (nEntClass + RED); break;
			case 3: nTexture = (nEntClass + BLUE); break;
			default: break;
			}
		}

		//Check to be sure we got it
		if (nTexture != -1)
		{
			Vec3 vEntPos = pEntity->GetAbsOrigin();
			int nEntHealth = pEntity->GetHealth();
			int nEntMaxHealth = pEntity->GetMaxHealth();

			RadarInfo_t RadarInfo =
			{
				nEntTeam,
				nEntClass,
				nEntHealth,
				nEntMaxHealth,
				nTexture,
				vLocal.DistTo(vEntPos),
				bIsFriend,
				vEntPos,
				GetDrawColor(pLocal, pEntity, bIsFriend),
				gDraw.GetHealthColor(nEntHealth, nEntMaxHealth)
			};

			m_vecInfo.push_back(RadarInfo);
		}
	}

	if (!m_vecInfo.empty())
	{
		std::sort(m_vecInfo.begin(), m_vecInfo.end(), [](const RadarInfo_t& a, const RadarInfo_t& b)
			{
				return a.m_flDistance > b.m_flDistance;
			});

		return true;
	}

	return false;
}

bool C_Radar::ShouldRun()
{
	if (!bActive)
		return false;

	if (!gInts.Engine->IsInGame() || !gInts.Engine->IsConnected() || gInts.Engine->Con_IsVisible())
		return false;

	if (gInts.EngineVGui->IsGameUIVisible())
		return false;

	return true;
}

//Straight from ESP
//Added it here incase we want some specific colors on the radar
Color_t C_Radar::GetDrawColor(C_BaseEntity* pLocal, C_BaseEntity* pEntity, bool bIsFriend)
{
	Color_t clrOut = gDraw.GetTeamColor(pEntity->GetTeamNum());

	if (bIsFriend)
		clrOut = FriendColor;

	if (pEntity->IsPlayer())
	{
		if (pEntity->IsCloaked())
			clrOut = CloakColor;

		if (!pEntity->IsVulnerable())
			clrOut = InvulnColor;
	}

	if (pEntity->GetIndex() == gGlobalInfo.nCurrentTargetIdx)
		clrOut = TargetColor;

	return clrOut;
}

void C_Radar::Drag(int& x, int& y, int w, int h)
{
	int m_nMouseX, m_nMouseY;
	gInts.Surface->GetCursorPos(m_nMouseX, m_nMouseY);

	static POINT pCorrect;
	static bool m_bDrag = false;
	static bool m_bMove = false;
	bool bHeld = (GetAsyncKeyState(VK_LBUTTON) & 0x8000);

	if ((m_nMouseX > x &&
		m_nMouseX < x + w &&
		m_nMouseY > y &&
		m_nMouseY < y + h) &&
		bHeld) {
		m_bDrag = true;

		if (!m_bMove) {
			pCorrect.x = m_nMouseX - x;
			pCorrect.y = m_nMouseY - y;
			m_bMove = true;
		}
	}

	if (m_bDrag) {
		x = m_nMouseX - pCorrect.x;
		y = m_nMouseY - pCorrect.y;
	}

	if (!bHeld) {
		m_bDrag = false;
		m_bMove = false;
	}
}