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
	void FireBullet();
	inline void SetIsAttacking(bool _bFlag) { m_bIsAttacking = _bFlag; }
private:
	TOWERINFO m_tInfo = {};
	Vector2 m_vTarget = Vector2::Zero;
	Timer m_FireTimer;
private:
	bool m_bIsAttacking = false;
	float m_fBoundary = 0.f;
};


#endif