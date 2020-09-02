#include "DrawUtils.h"

#include "../../../SDK/Includes/icons.h"

void ScreenSize_t::Update() {
	gInts.Engine->GetScreenSize(this->w, this->h);
}

void Draw_t::InitFonts(const std::vector<Font_t>& fonts)
{
	for (const auto& Font : fonts)
		Fonts.push_back(Font);

	if (!Fonts.empty())
		ReloadFonts();
}

void Draw_t::ReloadFonts()
{
	for (auto& v : Fonts)
	{
		v.dwFont = gInts.Surface->CreateFont();
		gInts.Surface->SetFontGlyphSet(v.dwFont, v.szName, v.nTall, v.nWeight, 0, 0, v.nFlags);
	}
}

void Draw_t::String(const size_t& font_idx, int x, int y, const Color_t& clr, const char* str, ...)
{
	if (str == 0)
		return;

	va_list va_alist;
	char cbuffer[1024] = { '\0' };
	wchar_t wstr[1024] = { '\0' };

	va_start(va_alist, str);
	vsprintf_s(cbuffer, str, va_alist);
	va_end(va_alist);

	wsprintfW(wstr, L"%S", cbuffer);

	gInts.Surface->SetTextPos(x, y);
	gInts.Surface->SetTextFont(Fonts.at(font_idx).dwFont);
	gInts.Surface->SetTextColor(clr.r, clr.g, clr.b, clr.a);
	gInts.Surface->PrintText(wstr, wcslen(wstr));
}

void Draw_t::StringW(const size_t &font_idx, int x, int y, const Color_t &clr, const wchar_t *str)
{
	if (str == 0)
		return;

	gInts.Surface->SetTextPos(x, y);
	gInts.Surface->SetTextFont(Fonts.at(font_idx).dwFont);
	gInts.Surface->SetTextColor(clr.r, clr.g, clr.b, clr.a);
	gInts.Surface->PrintText(str, wcslen(str));
}

void Draw_t::StringCenter(const size_t& font_idx, int x, int y, const Color_t& clr, const char* str, ...)
{
	if (str == 0)
		return;

	va_list va_alist;
	char cbuffer[1024] = { '\0' };
	wchar_t wstr[1024] = { '\0' };

	va_start(va_alist, str);
	vsprintf_s(cbuffer, str, va_alist);
	va_end(va_alist);

	wsprintfW(wstr, L"%S", cbuffer);

	int w = 0, h = 0;
	gInts.Surface->GetTextSize(Fonts.at(font_idx).dwFont, wstr, w, h);
	x -= (w / 2);

	gInts.Surface->SetTextPos(x, y);
	gInts.Surface->SetTextFont(Fonts.at(font_idx).dwFont);
	gInts.Surface->SetTextColor(clr.r, clr.g, clr.b, clr.a);
	gInts.Surface->PrintText(wstr, wcslen(wstr));
}

void Draw_t::StringCenterV(const size_t &font_idx, int x, int y, const Color_t &clr, const char *str, ...)
{
	if (str == 0)
		return;

	va_list va_alist;
	char cbuffer[1024] = { '\0' };
	wchar_t wstr[1024] = { '\0' };

	va_start(va_alist, str);
	vsprintf_s(cbuffer, str, va_alist);
	va_end(va_alist);

	wsprintfW(wstr, L"%S", cbuffer);

	int w = 0, h = 0;
	gInts.Surface->GetTextSize(Fonts.at(font_idx).dwFont, wstr, w, h);
	x -= (w / 2);
	y -= (h / 2);

	gInts.Surface->SetTextPos(x, y);
	gInts.Surface->SetTextFont(Fonts.at(font_idx).dwFont);
	gInts.Surface->SetTextColor(clr.r, clr.g, clr.b, clr.a);
	gInts.Surface->PrintText(wstr, wcslen(wstr));
}

void Draw_t::StringCenterVOnly(const size_t &font_idx, int x, int y, const Color_t &clr, const char *str, ...)
{
	if (str == 0)
		return;

	va_list va_alist;
	char cbuffer[1024] = { '\0' };
	wchar_t wstr[1024] = { '\0' };

	va_start(va_alist, str);
	vsprintf_s(cbuffer, str, va_alist);
	va_end(va_alist);

	wsprintfW(wstr, L"%S", cbuffer);

	int w = 0, h = 0;
	gInts.Surface->GetTextSize(Fonts.at(font_idx).dwFont, wstr, w, h);
	y -= (h / 2);

	gInts.Surface->SetTextPos(x, y);
	gInts.Surface->SetTextFont(Fonts.at(font_idx).dwFont);
	gInts.Surface->SetTextColor(clr.r, clr.g, clr.b, clr.a);
	gInts.Surface->PrintText(wstr, wcslen(wstr));
}

void Draw_t::StringCenterW(const size_t &font_idx, int x, int y, const Color_t &clr, const wchar_t *str)
{
	if (str == 0)
		return;

	int w = 0, h = 0;
	gInts.Surface->GetTextSize(Fonts.at(font_idx).dwFont, str, w, h);
	x -= (w / 2);
	
	gInts.Surface->SetTextPos(x, y);
	gInts.Surface->SetTextFont(Fonts.at(font_idx).dwFont);
	gInts.Surface->SetTextColor(clr.r, clr.g, clr.b, clr.a);
	gInts.Surface->PrintText(str, wcslen(str));
}

void Draw_t::Line(int x, int y, int x1, int y1, const Color_t& clr)
{
	gInts.Surface->SetDrawColor(clr.r, clr.g, clr.b, clr.a);
	gInts.Surface->DrawLine(x, y, x1, y1);
}

void Draw_t::Rect(int x, int y, int w, int h, const Color_t& clr)
{
	gInts.Surface->SetDrawColor(clr.r, clr.g, clr.b, clr.a);
	gInts.Surface->DrawFilledRect(x, y, x + w, y + h);
}

void Draw_t::OutlinedRect(int x, int y, int w, int h, const Color_t& clr)
{
	gInts.Surface->SetDrawColor(clr.r, clr.g, clr.b, clr.a);
	gInts.Surface->DrawOutlinedRect(x, y, x + w, y + h);
}

void Draw_t::GradientRect(int x, int y, int x1, int y1, const Color_t& top_clr, const Color_t& bottom_clr)
{
	gInts.Surface->SetDrawColor(top_clr.r, top_clr.g, top_clr.b, top_clr.a);
	gInts.Surface->DrawFilledRectFade(x, y, x + x1, y + y1, 255, 255, false);
	gInts.Surface->SetDrawColor(bottom_clr.r, bottom_clr.g, bottom_clr.b, bottom_clr.a);
	gInts.Surface->DrawFilledRectFade(x, y, x + x1, y + y1, 0, 255, false);
}

void Draw_t::OutlinedCircle(int x, int y, float radius, int segments, const Color_t& clr)
{
	float Step = PI * 2.0 / segments;

	for (float a = 0; a < (PI * 2.0); a += Step) 
	{
		float x1 = radius * cos(a) + x;
		float y1 = radius * sin(a) + y;
		float x2 = radius * cos(a + Step) + x;
		float y2 = radius * sin(a + Step) + y;
		Line(x1, y1, x2, y2, clr);
	}
}

void Draw_t::CornerRect(int x, int y, int w, int h, int _x, int _y, const Color_t& color)
{
	Line(x, y, x + (w / _x), y, color);
	Line(x, y, x, y + (h / _y), color);

	Line(x + w, y, x + w - (w / _x), y, color);
	Line(x + w, y, x + w, y + (h / _y), color);

	Line(x, y + h, x + (w / _x), y + h, color);
	Line(x, y + h, x, y + h - (h / _y), color);

	Line(x + w, y + h, x + w - (w / _x), y + h, color);
	Line(x + w, y + h, x + w, y + h - (h / _y), color);
}

void Draw_t::Texture(int x, int y, int w, int h, int nIndex)
{
	static int nTexture = 0;
	
	if (ICONS::ID[nIndex] != ICONS::UNDEFINED)
	{
		nTexture = ICONS::ID[nIndex];
	}
	else 
	{
		nTexture = gInts.Surface->CreateNewTextureID();
		gInts.Surface->DrawSetTextureFile(nTexture, ICONS::TEXTURE[nIndex].c_str(), true, true);
		ICONS::ID[nIndex] = nTexture;
	}

	gInts.Surface->SetDrawColor(255, 255, 255, 255);
	gInts.Surface->DrawSetTexture(nTexture);
	gInts.Surface->DrawTexturedRect(x, y, w, h);
}

Color_t Draw_t::GetTeamColor(int nTeamNum)
{
	switch (nTeamNum)
	{
		case 2: return { 207, 106, 135, 255 };
		case 3: return { 88, 133, 162, 255 };
		default: return { 255, 255, 255, 255 };
	}
}

Color_t Draw_t::GetHealthColor(int nHP, int nMAXHP)
{
	nHP = std::max(0, std::min(nHP, nMAXHP));

	int r = std::min((510 * (nMAXHP - nHP)) / nMAXHP, 200);
	int g = std::min((510 * nHP) / nMAXHP, 200);

	return { static_cast<byte>(r), static_cast<byte>(g), 0, 255 };
}

Draw_t gDraw;
ScreenSize_t gScreenSize;