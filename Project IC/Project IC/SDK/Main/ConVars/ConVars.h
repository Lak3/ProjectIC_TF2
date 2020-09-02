#pragma once
#include "../BaseEntity/BaseEntity.h"

struct ConVars_t
{
	ConVar* cl_interp = nullptr;
	ConVar* cl_interp_ratio = nullptr;
	ConVar* cl_updaterate = nullptr;
	ConVar* tf_crit_cap = nullptr;
	ConVar* tf_crit_bottom = nullptr;
	ConVar* tf_crit_default = nullptr;

	ConVar* sv_gravity = nullptr;
	ConVar* cl_flipviewmodels = nullptr;
	ConVar* sv_maxunlag = nullptr;

	void Init();
};

extern ConVars_t gConVars;