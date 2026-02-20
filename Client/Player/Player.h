#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Actor/Actor.h"
using namespace System;

class Player : public Actor
{
	RTTI_DECLARATIONS(Player, Actor)
public:
	Player();
	virtual ~Player();
public:
	void BeginPlay() override;
	void Tick(float _fDeltaTime) override;
	void Render() override;

public:
private:
	POINT m_MousePt = {};
};

#endif//!__PLAYER_H__