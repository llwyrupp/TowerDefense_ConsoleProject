#pragma once
#ifndef __ENGINE_ENUM_H__
#define __ENGINE_ENUM_H__
#include "EngineCommon/Engine_Defines.h"

BEGIN(System)
enum class ENGINE_DLL Color : unsigned short {
	eBlack = 0,
	eBlue = FOREGROUND_BLUE,
	eGreen = FOREGROUND_GREEN,
	eRed = FOREGROUND_RED,
	eWhite = eBlue | eGreen | eRed,
};
END

#endif//!__ENGINE_ENUM_H__