#pragma once
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Actor/Actor.h"
#include "Interface/ICollisionHandler.h"
#include "EngineCommon/Engine_Defines.h"

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

private:
	float m_fSpeed = 0.f;
	float m_fX = 0.f;
	float m_fY = 0.f;
	const float AccX = 5.f;
	const float AccY = 5.f;
	std::stack<Vector2> m_stackPath;
private:
	bool m_bIsMoving = false;
	Vector2 m_vNextPos = Vector2::Zero;
	Vector2 m_vNextDir = Vector2::Zero;
};


#endif