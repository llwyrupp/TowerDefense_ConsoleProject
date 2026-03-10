#pragma once
#ifndef __TOWERBULLET_H__
#define __TOWERBULLET_H__
#include "Actor/Actor.h"
#include "Interface/ICollisionHandler.h"
using namespace System;

class Tower;
class TowerBullet : public Actor, public ICollisionHandler
{
	RTTI_DECLARATIONS(TowerBullet, Actor)
public:
	TowerBullet();
	virtual ~TowerBullet();

	void OnCollisionEnter2D(Actor* _pActor) override;

public:
	void BeginPlay() override;
	void Tick(float _fDeltaTime) override;
	void Render() override;
public:
	void SetDir(const Vector2& _vPos) { m_vDir = _vPos; }
	void SetSpeed(float _speed) { m_fSpeed = _speed; }
	inline int GetDamage()const { return m_iDamage; }
	inline void SetDamage(int _damage) { m_iDamage = _damage; }
	inline void SetParentTower(Tower* _pTower) { m_pTower = _pTower; }
private:
	Vector2 m_vDir = Vector2::Zero;
	float m_fSpeed = 50.f;
	float m_fX = 0.f;
	float m_fY = 0.f;
	const float AccX = 2.f;
	const float AccY = 2.f;
	int m_iDamage = 0;
	Tower* m_pTower = nullptr;
};


#endif // !__TOWERBULLET_H__