#pragma once

#ifndef __ENGINE_FUNCTION_H__
#define __ENGINE_FUNCTION_H__

#include "EngineCommon/Engine_Defines.h"
#include "EngineCommon/Engine_Macro.h"

BEGIN(System)
#pragma region GENERAL
//check if the pointer is a nullptr. if it is, return.
template<typename T>
void CheckIfNullReturn(T*& ptr) {
	if (ptr == nullptr)
		return;
}
#pragma endregion GENERAL

#pragma region MEMORY
template<typename T>
void Safe_Delete(T*& ptr) {
	if (ptr) {
		delete ptr;
		ptr = nullptr;
	}
}

template<typename T>
void Safe_Delete_Arr(T*& ptr) {
	if (ptr) {
		delete[] ptr;
		ptr = nullptr;
	}
}

// 어떤 값을 두 수 사이로 고정할 때 사용하는 함수.
template<typename T>
T Clamp(T value, T min, T max)
{
	if (value < min)
	{
		value = min;
	}
	else if (value > max)
	{
		value = max;
	}

	return value;
}


#pragma endregion MEMORY
END




#endif //__ENGINE_FUNCTION_H__