#pragma once
#ifndef __GROUND_H__
#define __GROUND_H__
#include "Actor/Actor.h"
#include "Math/Vector2.h"

using namespace System;

class Ground : public Actor
{
	RTTI_DECLARATIONS(Ground, Actor)
public:
	Ground(const Vector2& vPos);
	virtual ~Ground();
private:

public:
	virtual void BeginPlay() override;
	virtual void Tick(float _fDeltaTime) override;
	virtual void Render() override;

};

#endif