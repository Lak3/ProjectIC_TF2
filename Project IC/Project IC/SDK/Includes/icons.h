#pragma once

#include <string>

namespace ICONS
{
	inline constexpr int WIDHT = 64;
	inline constexpr int HEIGHT = 64;
	inline constexpr int TEXTURE_AMOUNT = 30;
	inline constexpr int UNDEFINED = -1;

	inline int ID[TEXTURE_AMOUNT] =
	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
	};

	inline std::string TEXTURE[TEXTURE_AMOUNT] =
	{
		"hud/leaderboard_class_teleporter.vtf",
		"hud/leaderboard_class_scout.vtf",
		"hud/leaderboard_class_sniper.vtf",
		"hud/leaderboard_class_soldier.vtf",
		"hud/leaderboard_class_demo.vtf",
		"hud/leaderboard_class_medic.vtf",
		"hud/leaderboard_class_heavy.vtf",
		"hud/leaderboard_class_pyro.vtf",
		"hud/leaderboard_class_spy.vtf",
		"hud/leaderboard_class_engineer.vtf",
		"hud/eng_build_sentry.vtf",
		"hud/class_scoutred.vtf",
		"hud/class_sniperred.vtf",
		"hud/class_soldierred.vtf",
		"hud/class_demored.vtf",
		"hud/class_medicred.vtf",
		"hud/class_heavyred.vtf",
		"hud/class_pyrored.vtf",
		"hud/class_spyred.vtf",
		"hud/class_engired.vtf",
		"vgui/class_portraits/spy_blue_grey.vtf",
		"hud/class_scoutblue.vtf",
		"hud/class_sniperblue.vtf",
		"hud/class_soldierblue.vtf",
		"hud/class_demoblue.vtf",
		"hud/class_medicblue.vtf",
		"hud/class_heavyblue.vtf",
		"hud/class_pyroblue.vtf",
		"hud/class_spyblue.vtf",
		"hud/class_engiblue.vtf",
	};
}

/*
	"vgui/class_portraits/scout.vtf",
	"vgui/class_portraits/sniper.vtf",
	"vgui/class_portraits/soldier.vtf",
	"vgui/class_portraits/demoman.vtf",
	"vgui/class_portraits/medic.vtf",
	"vgui/class_portraits/heavy.vtf",
	"vgui/class_portraits/pyro.vtf",
	"vgui/class_portraits/spy.vtf",
	"vgui/class_portraits/engineer.vtf",
	"vgui/class_portraits/spy_blue_grey.vtf", //
	"vgui/class_portraits/scout_blue.vtf",
	"vgui/class_portraits/sniper_blue.vtf",
	"vgui/class_portraits/soldier_blue.vtf",
	"vgui/class_portraits/demoman_blue.vtf",
	"vgui/class_portraits/medic_blue.vtf",
	"vgui/class_portraits/heavy_blue.vtf",
	"vgui/class_portraits/pyro_blue.vtf",
	"vgui/class_portraits/spy_blue.vtf",
	"vgui/class_portraits/engineer_blue.vtf"
*/