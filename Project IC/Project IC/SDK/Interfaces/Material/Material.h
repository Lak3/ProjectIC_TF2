#pragma once

#include "../../Includes/Includes.h"

typedef unsigned short MaterialHandle_t;

class C_MaterialSystem
{
public:
	IMaterial* Find(char const* szMat, const char* szTextureGroup, bool bComplain = true, const char* szPrefix = NULL)
	{
		typedef IMaterial* (__thiscall* FN)(PVOID, const char*, const char*, bool, const char*);
		return GetVFunc<FN>(this, 73)(this, szMat, szTextureGroup, bComplain, szPrefix);
	}

	IMaterial* Create(char const* szName, KeyValues* pKV)
	{
		typedef IMaterial* (__thiscall* FN)(PVOID, const char*, KeyValues*);
		return GetVFunc<FN>(this, 72)(this, szName, pKV);
	}

	IMaterial* Get(MaterialHandle_t hMat)
	{
		typedef IMaterial* (__thiscall* FN)(PVOID, MaterialHandle_t);
		return GetVFunc<FN>(this, 78)(this, hMat);
	}

	MaterialHandle_t First()
	{
		typedef MaterialHandle_t(__thiscall* FN)(PVOID);
		return GetVFunc<FN>(this, 75)(this);
	}

	MaterialHandle_t Invalid()
	{
		typedef MaterialHandle_t(__thiscall* FN)(PVOID);
		return GetVFunc<FN>(this, 77)(this);
	}

	MaterialHandle_t Next(MaterialHandle_t hMat)
	{
		typedef MaterialHandle_t(__thiscall* FN)(PVOID, MaterialHandle_t);
		return GetVFunc<FN>(this, 76)(this, hMat);
	}
};

#define VMATERIALSYSTEM_INTERFACE "VMaterialSystem081"