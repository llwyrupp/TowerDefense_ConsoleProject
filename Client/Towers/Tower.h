#pragma once
#ifndef __TOWER_H__
#define __TOWER_H__
#include "ClientCommon/ClientEnum.h"
#include "ClientCommon/ClientStruct.h"
#include "Actor/Actor.h"

using namespace System;
class Tower : public Actor
{
	RTTI_DECLARATIONS(Tower, Actor)
public:
	Tower(const E_TYPE_TOWER& _eType);
	virtual ~Tower();

public:
	void BeginPlay() override;
	void Tick(float _fDeltaTime) override;
	void Render() override;

private:
	TOWERINFO m_tInfo = {};
};


#endif