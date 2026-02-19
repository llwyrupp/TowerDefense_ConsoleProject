#include "InputMgr/InputMgr.h"

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

void InputMgr::ProcessInput()
{
	for(int i = 0; i < MAX_SIZE_KEYSTATE; ++i)
		m_stKeyStates[i].bIsKeyDown = ((GetAsyncKeyState(i) & 0x8000) > 0 ? true : false);
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