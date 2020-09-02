#pragma once
#include "../../Interfaces/Interfaces.h"

struct ScreenSize_t
{
	int w = 0, h = 0;
	void Update();
};

extern ScreenSize_t gScreenSize;

struct Font_t
{
	DWORD dwFont = 0x0;
	const char *szName = 0;
	int nTall = 0;
	int nWeight = 0;
	int nFlags = 0;
};

struct Draw_t
{
	std::vector<Font_t> Fonts;

	void InitFonts(const std::vector<Font_t> &fonts);
	void ReloadFonts();
	void String(const size_t &font_idx, int x, int y, const Color_t &clr, const char *str, ...);
	void StringW(const size_t &font_idx, int x, int y, const Color_t &clr, const wchar_t *str);
	void StringCenter(const size_t &font_idx, int x, int y, const Color_t &clr, const char *str, ...);
	void StringCenterV(const size_t &font_idx, int x, int y, const Color_t &clr, const char *str, ...);
	void StringCenterVOnly(const size_t &font_idx, int x, int y, const Color_t &clr, const char *str, ...);
	void StringCenterW(const size_t &font_idx, int x, int y, const Color_t &clr, const wchar_t *str);
	void Line(int x, int y, int x1, int y1, const Color_t &clr);
	void Rect(int x, int y, int w, int h, const Color_t &clr);
	void OutlinedRect(int x, int y, int w, int h, const Color_t &clr);
	void GradientRect(int x, int y, int x1, int y1, const Color_t &top_clr, const Color_t &bottom_clr);
	void OutlinedCircle(int x, int y, float radius, int segments, const Color_t &clr);
	void CornerRect(int x, int y, int w, int h, int _x, int _y, const Color_t &color);
	void Texture(int x, int y, int w, int h, int nIndex);


	Color_t GetTeamColor(int nTeamNum);
	Color_t GetHealthColor(int nHP, int nMAXHP);
};

enum Fonts_t
{
	FONT_ESP,
	FONT_ESP_OUTLINED,

	FONT_ESP_NAME,
	FONT_ESP_NAME_OUTLINED,

	FONT_ESP_COND,
	FONT_ESP_COND_OUTLINED,

	FONT_ESP_PICKUPS,
	FONT_ESP_PICKUPS_OUTLINED,

	FONT_DEBUG,
	FONT_MENU,
	FONT_MENU_NAME
};

extern Draw_t gDraw;

#pragma warning (disable : 6385)
#pragma warning (disable : 26451)
#pragma warning (disable : 4305)

#include "../GlobalInfo/GlobalInfo.h"

namespace Math
{
	inline bool W2S(const Vec3 &vOrigin, Vec3 &vScreen)
	{	
		const matrix3x4 &worldToScreen = gGlobalInfo.WorldToProjection.As3x4();
		
		float w = worldToScreen[3][0] * vOrigin[0] + worldToScreen[3][1] * vOrigin[1] + worldToScreen[3][2] * vOrigin[2] + worldToScreen[3][3];
		vScreen.z = 0;
		
		if (w > 0.001)
		{
			float fl1DBw = 1 / w;
			vScreen.x = (gScreenSize.w / 2) + (0.5 * ((worldToScreen[0][0] * vOrigin[0] + worldToScreen[0][1] * vOrigin[1] + worldToScreen[0][2] * vOrigin[2] + worldToScreen[0][3]) * fl1DBw) * gScreenSize.w + 0.5);
			vScreen.y = (gScreenSize.h / 2) - (0.5 * ((worldToScreen[1][0] * vOrigin[0] + worldToScreen[1][1] * vOrigin[1] + worldToScreen[1][2] * vOrigin[2] + worldToScreen[1][3]) * fl1DBw) * gScreenSize.h + 0.5);
			return true;
		}

		return false;
	}
}