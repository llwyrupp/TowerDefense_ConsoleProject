#include "InputMgr/InputMgr.h"
#include "EngineCommon/Engine_Function.h"
#include "Engine/Engine.h"
#include<cassert>

BEGIN(System)
InputMgr* InputMgr::m_pInstance = nullptr;

InputMgr::InputMgr()
{
	m_pInstance = this;
}

InputMgr::~InputMgr()
{
}

bool InputMgr::GetKeyDown(int iKeyCode)
{
	return m_stKeyStates[iKeyCode].bIsKeyDown && !m_stKeyStates[iKeyCode].bWasKeyDown;
}

bool InputMgr::GetKeyUp(int iKeyCode)
{
	return !m_stKeyStates[iKeyCode].bIsKeyDown && m_stKeyStates[iKeyCode].bWasKeyDown;
}

bool InputMgr::GetKey(int iKeyCode)
{
	return m_stKeyStates[iKeyCode].bIsKeyDown;
}

bool InputMgr::GetMouseButtonDown(int iBtnCode)
{
	//check if iBtnCode is 0 or 1
	assert(iBtnCode == 0 || iBtnCode == 1);

	int iKeyCode = 0;
	if (iBtnCode == 0)
		iKeyCode = VK_LBUTTON;
	else if (iBtnCode == 1)
		iKeyCode = VK_RBUTTON;

	return m_stKeyStates[iKeyCode].bIsKeyDown && !m_stKeyStates[iKeyCode].bWasKeyDown;
}

bool InputMgr::GetMouseButtonUp(int iBtnCode)
{
	//check if iBtnCode is 0 or 1
	assert(iBtnCode == 0 || iBtnCode == 1);

	int iKeyCode = 0;
	if (iBtnCode == 0)
		iKeyCode = VK_LBUTTON;
	else if (iBtnCode == 1)
		iKeyCode = VK_RBUTTON;

	return !m_stKeyStates[iKeyCode].bIsKeyDown && m_stKeyStates[iKeyCode].bWasKeyDown;
}

bool InputMgr::GetMouseButton(int iBtnCode)
{
	//check if iBtnCode is 0 or 1
	assert(iBtnCode == 0 || iBtnCode == 1);

	int iKeyCode = 0;
	if (iBtnCode == 0)
		iKeyCode = VK_LBUTTON;
	else if (iBtnCode == 1)
		iKeyCode = VK_RBUTTON;

	return m_stKeyStates[iKeyCode].bIsKeyDown;
}

void InputMgr::ProcessInput()
{

	static HANDLE hInputHandle = GetStdHandle(STD_INPUT_HANDLE);
	static bool bInit = false;

	if (!bInit)
	{
		//activate mouse event.
		DWORD dwMode = ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT;
		dwMode &= ~ENABLE_QUICK_EDIT_MODE;
		BOOL bResult = SetConsoleMode(hInputHandle, dwMode);

		if (bResult == FALSE) {
			MessageBoxA(nullptr, "InputMgr::ProcessInput(): Failed to set console mode", "Error", MB_OK);
			__debugbreak();
		}
		bInit = true;
	}

	const DWORD dwMaxRecCnt = 256;

	//array to store the incoming events.
	INPUT_RECORD inRec[dwMaxRecCnt] = {};

	//number of input events this frame has processed.
	DWORD dwEventReadCnt = 0;

	//check for any input events
	if (PeekConsoleInput(hInputHandle, inRec, dwMaxRecCnt, &dwEventReadCnt) && dwEventReadCnt > 0)
	{
		//if there was an event, read and process it.
		if (ReadConsoleInput(hInputHandle, inRec, dwMaxRecCnt, &dwEventReadCnt))
		{
			for (int i = 0; i < static_cast<int>(dwEventReadCnt); ++i)
			{
				INPUT_RECORD& rec = inRec[i];

				switch (rec.EventType)
				{
				case KEY_EVENT:
					{
						if (rec.Event.KeyEvent.bKeyDown)//when key is down
						{
							m_stKeyStates[rec.Event.KeyEvent.wVirtualKeyCode].bIsKeyDown = true;
						}
						else//if key is not down
						{
							m_stKeyStates[rec.Event.KeyEvent.wVirtualKeyCode].bIsKeyDown = false;
						}
						break;
					}
				//case MOUSE_EVENT:
				//	{
				//		//set mouse position x
				//		m_vMousePos.m_iX = rec.Event.MouseEvent.dwMousePosition.X;
				//		//adjust x so that the mouse cursor is clamped inside the window
				//		m_vMousePos.m_iX = Clamp<int>(m_vMousePos.m_iX, 0, Engine::Get_Instance().GetWidth() - 1);

				//		m_MousePos.fX = static_cast<float>(rec.Event.MouseEvent.dwMousePosition.X);
				//		m_MousePos.fX = Clamp<float>(m_MousePos.fX, 0.f, static_cast<float>(Engine::Get_Instance().GetWidth() - 1));

				//		//set mouse position x
				//		m_vMousePos.m_iY = rec.Event.MouseEvent.dwMousePosition.Y;
				//		//adjust y
				//		m_vMousePos.m_iY = Clamp<int>(m_vMousePos.m_iY, 0, Engine::Get_Instance().GetHeight() - 1);

				//		m_MousePos.fY = static_cast<float>(rec.Event.MouseEvent.dwMousePosition.Y);
				//		m_MousePos.fY = Clamp<float>(m_MousePos.fY, 0.f, static_cast<float>(Engine::Get_Instance().GetHeight() - 1));

				//		//save mouse click state.
				//		m_stKeyStates[VK_LBUTTON].bIsKeyDown = (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) != 0;
				//		m_stKeyStates[VK_RBUTTON].bIsKeyDown = (rec.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED) != 0;
				//	}
				//	break;
				}
			}
		}
	}

	//read mouse input
	/*POINT pt;
	
	if (GetCursorPos(&pt))
	{
		HWND hWnd = GetConsoleWindow();
		RECT rcWindow, rcClient;
		GetWindowRect(hWnd, &rcWindow);
		GetClientRect(hWnd, &rcClient);

		ScreenToClient(hWnd, &pt);

		CONSOLE_FONT_INFOEX cfi;
		wcscpy_s(cfi.FaceName, L"Consolas");

		HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		GetCurrentConsoleFontEx(hOutput, FALSE, &cfi);


		if (cfi.dwFontSize.X > 0 && cfi.dwFontSize.Y > 0)
		{
			m_MousePos.fX = static_cast<float>(pt.x) / static_cast<float>(abs(cfi.dwFontSize.X));
			m_MousePos.fY = static_cast<float>(pt.y) / static_cast<float>(abs(cfi.dwFontSize.Y));
		}
	}*/
}

void InputMgr::SavePrevInputStates()
{
	//update bWasKeyDown with current values for the next frame.
	for (int i = 0; i < MAX_SIZE_KEYSTATE; ++i)
		m_stKeyStates[i].bWasKeyDown = m_stKeyStates[i].bIsKeyDown;
}

InputMgr& InputMgr::Get_Instance()
{
	if (!m_pInstance) {
		cerr << "InputMgr INSTANCE is NULL";
		__debugbreak();
	}

	return *m_pInstance;
}

END