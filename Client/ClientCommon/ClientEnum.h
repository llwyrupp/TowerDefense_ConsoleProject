#pragma once
#ifndef __CLIENT_ENUM_H__
#define __CLIENT_ENUM_H__

enum class E_TYPE_TOWER : unsigned int {
	E_TYPE_RIFLE = 0,
	E_TYPE_SHOTGUN = 1,
	E_TYPE_MACHINEGUN = 2,
	//E_TYPE_LASER = 3,
	//E_TYPE_ENEMYCHASER = 4,
	E_TYPE_MAX
};

enum class E_TYPE_ENEMY{
	E_TYPE_NONE = -1,
	E_TYPE_SOLDIER = 0,//mid hp, mid speed
	E_TYPE_TANKER = 1,//high hp, low speed
	E_TYPE_ASSASSIN = 2,//low hp, high speed
	E_TYPE_MAX
};

enum class E_TYPE_LEVEL {
	E_TYPE_NONE = -1,
	E_TYPE_MENU = 0,
	E_TYPE_FIELD = 1,
	E_TYPE_UPGRADE = 2,
	E_TYPE_MAX
};

enum class E_TYPE_GAMESTATE {
	E_NONE = -1,
	E_PREROUND = 0,
	E_ROUND = 1,
	E_MAX
};
#endif