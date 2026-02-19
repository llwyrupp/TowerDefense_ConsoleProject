#pragma once
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Actor/Actor.h"

using namespace System;

class Enemy : public Actor
{
	RTTI_DECLARATIONS(Enemy, Actor)
public:
	Enemy();
	virtual ~Enemy();
public:
	void BeginPlay() override;
	void Tick(float _fDeltaTime) override;
	void Render() override;

private:


};


#endif