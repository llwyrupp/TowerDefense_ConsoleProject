#pragma once
#ifndef __SCREENBUFFER_H__
#define __SCREENBUFFER_H__

#include "EngineCommon/Engine_Macro.h"
#include "EngineCommon/Engine_Defines.h"
#include "Math/Vector2.h"

BEGIN(System)
	class ScreenBuffer
	{
	public:
		ScreenBuffer(const Vector2& vScreenSize);
		~ScreenBuffer();

		//erase console buffer
		void Clear_Buffer();

		//draw 2-dimensional char arr on console.
		void Draw_Char(CHAR_INFO* chInfo);

		//버퍼 반환함수
		inline HANDLE Get_Buffer() const { return m_hBuffer; }


	private:
		//콘솔 출력핸들
		HANDLE m_hBuffer = nullptr;

		//screen size
		Vector2 m_vScreenSize;
	};
END
#endif