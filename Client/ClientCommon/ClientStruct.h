#pragma once
#ifndef __CLIENT_STRUCT_H__
#define __CLIENT_STRUCT_H__
#include "ClientCommon/ClientEnum.h"
typedef struct tagTowerInfo {
	tagTowerInfo() {
		iHP = 0;
		eType = E_TYPE_TOWER::E_TYPE_MAX;
	}

	tagTowerInfo(int _hp, E_TYPE_TOWER _type) 
		:iHP(_hp), eType(_type)
	{

	}

	int iHP;
	E_TYPE_TOWER eType;
}TOWERINFO;

#endif