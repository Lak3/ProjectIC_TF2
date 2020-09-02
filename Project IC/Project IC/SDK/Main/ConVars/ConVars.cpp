#include "ConVars.h"

void ConVars_t::Init()
{
	cl_interp = gInts.CVars->FindVar("cl_interp");
	cl_interp_ratio = gInts.CVars->FindVar("cl_interp_ratio");
	cl_updaterate = gInts.CVars->FindVar("cl_updaterate");

	tf_crit_cap = gInts.CVars->FindVar("tf_weapon_criticals_bucket_cap");
	tf_crit_bottom = gInts.CVars->FindVar("tf_weapon_criticals_bucket_bottom");
	tf_crit_default = gInts.CVars->FindVar("tf_weapon_criticals_bucket_default");

	sv_gravity = gInts.CVars->FindVar("sv_gravity");
	cl_flipviewmodels = gInts.CVars->FindVar("cl_flipviewmodels");

	sv_maxunlag = gInts.CVars->FindVar("sv_maxunlag");
}

ConVars_t gConVars;