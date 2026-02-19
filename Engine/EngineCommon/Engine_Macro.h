#pragma once
#ifndef __ENGINE_MACRO_H__
#define __ENGINE_MACRO_H__

#pragma warning(disable: 4251)	// 미봉책(방법 없음).
#pragma warning(disable: 4172)	// 위험성 인지한 상태로 사용.

#define BEGIN(NAMESPACE) namespace NAMESPACE {
#define END }

//#define USING(NAMESPACE) using namespace NAMESPACE;


#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)


#if ENGINE_BUILD_DLL
#define ENGINE_DLL DLLEXPORT
#else
#define ENGINE_DLL DLLIMPORT
#endif

#endif//__ENGINE_MACRO_H__