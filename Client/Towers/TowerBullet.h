#pragma once
#ifndef __TOWERBULLET_H__
#define __TOWERBULLET_H__
#include "Actor/Actor.h"
#include "Interface/ICollisionHandler.h"
using namespace System;

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

};


#endif // !__TOWERBULLET_H__