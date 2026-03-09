#pragma once
#ifndef __BARRICADE_H__
#define __BARRICADE_H__
#include "Actor/Actor.h"
#include "Math/Vector2.h"

using namespace System;

class Barricade : public Actor
{
	RTTI_DECLARATIONS(Barricade, Actor)
public:
	Barricade(const Vector2& vPos);
	virtual ~Barricade();
private:

public:
	virtual void BeginPlay() override;
	virtual void Tick(float _fDeltaTime) override;
	virtual void Render() override;

};

#endif//!__BARRICADE_H__