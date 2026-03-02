#pragma once
#ifndef __TARGET_H__
#define __TARGET_H__

#include "Actor/Actor.h"

using namespace System;

class Target : public Actor
{
	RTTI_DECLARATIONS(Target, Actor)

public:
	Target() {}
	Target(const Vector2& position);
public:
	void BeginPlay() override;
	void Tick(float _fDeltaTime) override;
	void Render() override;

};


#endif