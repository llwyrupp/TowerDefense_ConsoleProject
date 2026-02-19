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

//void AllocateMem_Arr(T& _pDestArr, T& _pSrcArr)
//{
//	size_t szLen = strlen(_pSrcArr) + 1;
//	_pDestArr = new T[szLen];
//	strcpy_s(_pDestArr, sizeof(T) * szLen, _pSrcArr);
//}
#pragma endregion MEMORY
END




#endif //__ENGINE_FUNCTION_H__