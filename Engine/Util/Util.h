#pragma once
#ifndef __UTIL_H__
#define __UTIL_H__

#include "EngineCommon/Engine_Defines.h"
#include "EngineCommon/Engine_Enum.h"
#include "Math/Vector2.h"

using namespace System;

BEGIN(Util)
// 콘솔 커서 위치 이동(설정)하는 함수.
inline void SetConsolePos(const Vector2& vPos) { 
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),static_cast<COORD>(vPos));
}
// 콘솔 텍스트 설정 함수.
inline void SetConsoleTextColor(Color color) { 
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<unsigned short>(color)); 
}

// 커서 끄기.
inline void TurnOffCursor(){ 
	CONSOLE_CURSOR_INFO info = {};	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&info);
	info.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&info);
}

// 커서 켜기.
inline void TurnOnCursor()
{
	// 커서 끄기.
	CONSOLE_CURSOR_INFO info = {};
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&info);

	info.bVisible = true;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&info);
}

#pragma region RANDOM
inline void SetRandomSeed() {
	srand(static_cast<unsigned int>(time(nullptr)));
}

inline int RandomInt(int iMin, int iMax) {
	int iDiff = iMax - iMin + 1;
	// div by 32768
	return ((iDiff * rand()) / (RAND_MAX + 1)) + iMin;
}

inline float RandomRange(float fMin, float fMax) {
	float fRandom = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float fDiff = fMax - fMin;
	return (fRandom * fDiff) + fMin;
}
#pragma endregion RANDOM

END

#endif//!__UTIL_H__