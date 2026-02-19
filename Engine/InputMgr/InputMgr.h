#pragma once
#ifndef __INPUTMGR_H__
#define __INPUTMGR_H__
#include "EngineCommon/Engine_Macro.h"
#include "EngineCommon/Engine_Defines.h"

BEGIN(System)

#define MAX_SIZE_KEYSTATE 256
	class ENGINE_DLL InputMgr
	{
	friend class Engine;

	typedef struct tagKeyState {
		//if key is down now.
		bool bIsKeyDown = false;

		//if key WAS down previously.
		bool bWasKeyDown = false;
	}KEYSTATE;

	public:
		InputMgr();
		~InputMgr();
	public:
		// 이전에 입력이 안됐는데, 현재 입력이 됐으면 1번 호출.
		bool GetKeyDown(int iKeyCode);
		// 이전에 입력이 됐는데, 현재 입력이 취소됐으면 1번 호출.
		bool GetKeyUp(int iKeyCode);
		// 현재 눌려있으면 반복 호출.
		bool GetKey(int iKeyCode);
	public:
		static InputMgr& Get_Instance();
	private:
		//입력 처리
		void ProcessInput();
		//기존 입력 저장
		void SavePrevInputStates();
	private:
		KEYSTATE m_stKeyStates[MAX_SIZE_KEYSTATE] = {};
		static InputMgr* m_pInstance;
	};
END
#endif