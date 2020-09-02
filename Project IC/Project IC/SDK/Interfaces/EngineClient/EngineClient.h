#pragma once
#include "../../Includes/Includes.h"

class C_NetChannel;

class C_EngineClient
{
public:
	void GetScreenSize(int &width_out, int &height_out)
	{
		typedef void(__thiscall *FN)(PVOID, int &, int &);
		return GetVFunc<FN>(this, 5)(this, width_out, height_out);
	}

	void ServerCmd(const char *szCmdString, bool bReliable)
	{
		typedef void(__thiscall *FN)(PVOID, const char *, bool);
		return GetVFunc<FN>(this, 6)(this, szCmdString, bReliable);
	}

	bool GetPlayerInfo(int nEntityIndex, PlayerInfo_t *pPlayerInfo)
	{
		typedef bool(__thiscall *FN)(PVOID, int, PlayerInfo_t *);
		return GetVFunc<FN>(this, 8)(this, nEntityIndex, pPlayerInfo);
	}

	bool Con_IsVisible()
	{
		typedef bool(__thiscall *FN)(PVOID);
		return GetVFunc<FN>(this, 11)(this);
	}

	int GetLocalPlayer()
	{
		typedef int(__thiscall *FN)(PVOID);
		return GetVFunc<FN>(this, 12)(this);
	}

	float Time()
	{
		typedef float(__thiscall *FN)(PVOID);
		return GetVFunc<FN>(this, 14)(this);
	}

	void GetViewAngles(Vec3 &va)
	{
		typedef void(__thiscall *FN)(PVOID, Vec3 &va);
		return GetVFunc<FN>(this, 19)(this, va);
	}

	Vec3 GetViewAngles()
	{
		Vec3 out;
		GetViewAngles(out);
		return out;
	}

	void SetViewAngles(Vec3 &va)
	{
		typedef void(__thiscall *FN)(PVOID, Vec3 &va);
		return GetVFunc<FN>(this, 20)(this, va);
	}

	int GetMaxClients()
	{
		typedef int(__thiscall *FN)(PVOID);
		return GetVFunc<FN>(this, 21)(this);
	}

	bool IsInGame()
	{
		typedef bool(__thiscall *FN)(PVOID);
		return GetVFunc<FN>(this, 26)(this);
	}

	bool IsConnected()
	{
		typedef bool(__thiscall *FN)(PVOID);
		return GetVFunc<FN>(this, 27)(this);
	}

	bool IsDrawingLoadingImage()
	{
		typedef bool(__thiscall *FN)(PVOID);
		return GetVFunc<FN>(this, 28)(this);
	}

	const matrix3x4 &WorldToScreenMatrix()
	{
		typedef const matrix3x4 &(__thiscall *FN)(PVOID);
		return GetVFunc<FN>(this, 36)(this);
	}

	bool IsTakingScreenshot()
	{
		typedef bool(__thiscall *FN)(PVOID);
		return GetVFunc<FN>(this, 85)(this);
	}

	bool IsHLTV()
	{
		typedef bool(__thiscall *FN)(PVOID);
		return GetVFunc<FN>(this, 86)(this);
	}

	C_NetChannel* GetNetChannelInfo()
	{
		typedef C_NetChannel* (__thiscall *FN)(PVOID);
		return GetVFunc<FN>(this, 72)(this);
	}

	void ClientCmd_Unrestricted(const char *szCommandString)
	{
		typedef void(__thiscall *FN)(PVOID, const char *);
		return GetVFunc<FN>(this, 106)(this, szCommandString);
	}
};

#define VENGINE_CLIENT_INTERFACE_VERSION_13	"VEngineClient013"