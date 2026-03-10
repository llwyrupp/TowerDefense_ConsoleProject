#pragma once
#ifndef __SPAWNINGPOINT_H__
#define __SPAWNINGPOINT_H__
#include "Actor/Actor.h"
#include "Math/Vector2.h"

using namespace System;

class SpawningPoint : public Actor
{
	RTTI_DECLARATIONS(SpawningPoint, Actor)
public:
	SpawningPoint(const Vector2& vPos);
	virtual ~SpawningPoint();
private:

public:
	virtual void BeginPlay() override;
	virtual void Tick(float _fDeltaTime) override;
	virtual void Render() override;

};

#endif//!__SPAWNINGPOINT_H__