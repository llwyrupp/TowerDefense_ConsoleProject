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

enum class ENGINE_DLL E_LAYER {
	E_NONE = -1,
	E_PLAYER = 0,
	E_ENEMY = 1,
	E_TOWER = 2,
	E_TOWERBULLET = 3,
	E_GROUND = 4,
	E_ROAD = 5,
	E_ETCETERA = 6,
	E_MAX
};
END

#endif//!__ENGINE_ENUM_H__