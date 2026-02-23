#pragma once

#ifndef __PLAYERCURSOR_H__
#define __PLAYERCURSOR_H__
#include "Actor/Actor.h"

using namespace System;
class PlayerCursor : public Actor
{
	RTTI_DECLARATIONS(PlayerCursor, Actor)
public:
	PlayerCursor();
	virtual ~PlayerCursor();

private:

	// Inherited via Actor
	void BeginPlay() override;
	void Tick(float _fDeltaTime) override;
	void Render() override;

public:
	void OnHit();
};

#endif