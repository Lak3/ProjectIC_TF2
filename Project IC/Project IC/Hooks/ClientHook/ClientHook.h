#pragma once
#include "../../SDK/SDK.h"

namespace ClientHook
{
	extern VMT::Table Table;

	namespace PreEntity
	{
		const int index = 5;
		using fn = void(__thiscall *)(C_BaseClientDLL *, char const *);
		void __stdcall Hook(char const *szMapName);
	}

	namespace PostEntity
	{
		const int index = 6;
		using fn = void(__thiscall *)(C_BaseClientDLL *);
		void __stdcall Hook();
	}

	namespace ShutDown
	{
		const int index = 7;
		using fn = void(__thiscall *)(C_BaseClientDLL *);
		void __stdcall Hook();
	}

	namespace FrameStageNotify
	{
		const int index = 35;
		using fn = void(__thiscall *)(C_BaseClientDLL *, ClientFrameStage_t);
		void __stdcall Hook(ClientFrameStage_t FrameStage);
	}
}