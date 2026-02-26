#pragma once
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Actor/Actor.h"
#include "Interface/ICollisionHandler.h"

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
	float m_fX = 0.f;
	float m_fY = 0.f;

};


#endif