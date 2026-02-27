#pragma once
#ifndef __WALL_H__
#define __WALL_H__
#include "Actor/Actor.h"
#include "Math/Vector2.h"

using namespace System;

class Wall : public Actor
{
	RTTI_DECLARATIONS(Wall, Actor)
public:
	Wall(const Vector2& vPos);
	virtual ~Wall();
private:

public:
	virtual void BeginPlay() override;
	virtual void Tick(float _fDeltaTime) override;
	virtual void Render() override;

};

#endif