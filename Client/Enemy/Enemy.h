#pragma once
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Actor/Actor.h"
#include "Interface/ICollisionHandler.h"
#include "EngineCommon/Engine_Defines.h"
#include "ClientCommon/ClientEnum.h"

using namespace System;

class Enemy : public Actor, public ICollisionHandler
{
	RTTI_DECLARATIONS(Enemy, Actor)
public:
	Enemy();
	virtual ~Enemy();

	void OnCollisionEnter2D(Actor* _pActor) override;

public:
	void BeginPlay() override;
	void Tick(float _fDeltaTime) override;
	void Render() override;
public:
	inline int GetMoney() const { return m_iMoney; }
	inline int GetHP() const { return m_iHP; }
	inline void SetHP(int _hp) { m_iHP = _hp; }
	inline int GetDamage()const { return m_iDamage; }
	void ProcessNextPos();
	void SetEnemyInfo(E_TYPE_ENEMY _type);
	bool CheckIfPastDest();
private:
	int m_iDamage = 0;
	int m_iHP = 0;
	int m_iMoney = 0;
	float m_fSpeed = 40.f;
	float m_fX = 0.f;
	float m_fY = 0.f;
	const float AccX = 5.f;
	const float AccY = 5.f;
	std::stack<Vector2> m_stackPath;
private:
	bool m_bIsMoving = false;
	Vector2 m_vNextPos = Vector2::Zero;
	Vector2 m_vNextDir = Vector2::Zero;
	E_TYPE_ENEMY m_eType = E_TYPE_ENEMY::E_TYPE_NONE;
};


#endif