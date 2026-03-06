#pragma once
#ifndef __TOWER_H__
#define __TOWER_H__
#include "ClientCommon/ClientEnum.h"
#include "ClientCommon/ClientStruct.h"
#include "Interface/ICollisionHandler.h"
#include "Actor/Actor.h"
#include "Util/Timer.h"

using namespace System;
using namespace Util;

enum class E_TOWER_STATE {
	E_IDLE = 0,
	E_ATTACK = 1,
	E_MAX
};

class Tower : public Actor , public ICollisionHandler
{
	RTTI_DECLARATIONS(Tower, Actor)
public:
	Tower(const E_TYPE_TOWER& _eType, const char* pPath);
	virtual ~Tower();

public:
	void BeginPlay() override;
	void Tick(float _fDeltaTime) override;
	void Render() override;
	// Inherited via ICollisionHandler
	void OnCollisionEnter2D(Actor* _pActor) override;
public:
	bool CheckIsActorInTowerBoundary(const Actor* _other);
	float ConvertToRadians(float _degree);
private:
	TOWERINFO m_tInfo = {};
	//Vector2 m_vTarget = Vector2::Zero;
	Actor* m_pTarget = nullptr;
	Vector2 m_vPosBoundary[12] = {};
	Timer m_FireTimer;
private:
	float m_fBoundary = 0.f;
	E_TOWER_STATE m_eCurState = E_TOWER_STATE::E_IDLE;
};


#endif