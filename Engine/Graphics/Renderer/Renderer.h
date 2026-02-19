#pragma once
#ifndef __RENDERER_H__
#define __RENDERER_H__
#include "EngineCommon/Engine_Macro.h"
#include "EngineCommon/Engine_Enum.h"
#include "EngineCommon/Engine_Defines.h"
#include "Math/Vector2.h"

BEGIN(System)
#define MAX_NUM_BUFFER 2
class ScreenBuffer;
class ENGINE_DLL Renderer
{
	typedef struct tagFrame {
		tagFrame(int iBufCnt);
		~tagFrame();

		void Clear(const Vector2& vScreenSize);

		CHAR_INFO* charInfoArr = nullptr;

		int* pSortingOrderArr = nullptr;
	}FRAME;

	typedef struct tagRenderCommand {
		/*tagRenderCommand(const char* _pText, const Vector2& _vPos, Color _eColor, int _iSortingOrder);
		~tagRenderCommand();*/

		//const char* pText = nullptr;
		std::string strText = "";

		//coordinates
		Vector2 vPosition;

		//color
		Color eColor = Color::eWhite;

		//render priority
		int iSortingOrder = 0;

	}RENDERCOM;

public:
	Renderer(const Vector2& vScreenSize);
	~Renderer();

	void Render();

	void Submit(string pText, const Vector2& vPos, Color eColor = Color::eWhite, int iSortOrder = 0);

public:
	static Renderer& Get_Instance();
private:
	//clear screen.
	void Clear();

	//use double buffering to swap the current(active) buffer
	void Present();

	ScreenBuffer* GetCurBuffer();

private:
	//screen size.
	Vector2 m_vScreenSize;

	FRAME* m_pFrame = nullptr;

	//double buffer arr.
	ScreenBuffer* m_pScreenBuffers[MAX_NUM_BUFFER] = {};

	//the index of currently active buffer.
	int m_iCurBufIdx = 0;

	//render queue(internally implemented with std::vector.
	//put all rendering commands in this vector)
	vector<RENDERCOM> m_vecRenderQueue;

private:
	static Renderer* m_pInstance;
};

END

#endif//!__RENDERER_H__