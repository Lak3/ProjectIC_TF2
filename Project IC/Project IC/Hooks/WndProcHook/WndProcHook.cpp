#include "WndProcHook.h"
#include "../../Features/Menu/Input/Input.h"
#include "../../Features/Menu/Menu.h"

LONG __stdcall WndProcHook::Hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_KEYDOWN: {
			gInput.m_Keyboard[wParam] = EKeyState::PRESSED;
			break;
		}

		case WM_KEYUP: {
			gInput.m_Keyboard[wParam] = EKeyState::NONE;
			break;
		}
		
		case WM_RBUTTONDOWN:
		{
			if (wParam & MK_RBUTTON)
				gInput.m_RMouse = EKeyState::PRESSED;
			
			break;
		}

		case WM_LBUTTONDOWN:
		{
			if (wParam & MK_LBUTTON)
				gInput.m_LMouse = EKeyState::PRESSED;

			break;
		}

		case WM_MBUTTONDOWN:
		{
			if (wParam & MK_MBUTTON)
				gInput.m_MMouse = EKeyState::PRESSED;

			break;
		}

		case WM_RBUTTONUP:
		{
			gInput.m_RMouse = EKeyState::NONE;
			break;
		}

		case WM_LBUTTONUP:
		{
			gInput.m_LMouse = EKeyState::NONE;
			break;
		}

		case WM_MBUTTONUP:
		{
			gInput.m_MMouse = EKeyState::NONE;
			break;
		}

		case WM_MOUSEWHEEL: {
			gInput.m_Wheel = ((int)wParam < 0 ? EScrollWheelState::UP : EScrollWheelState::DOWN);
			break;
		}
	}

	if (gMenu.m_Open && (gInts.Engine->Con_IsVisible() || gInts.EngineVGui->IsGameUIVisible()))
		return 1;

	return CallWindowProc(WndProc, hWnd, uMsg, wParam, lParam);
}

WNDPROC WndProcHook::WndProc;