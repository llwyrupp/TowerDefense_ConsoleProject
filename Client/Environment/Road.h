#pragma once
#ifndef __ROAD_H__
#define __ROAD_H__
#include "Actor/Actor.h"
#include "Math/Vector2.h"

using namespace System;

class Road : public Actor
{
	RTTI_DECLARATIONS(Road, Actor)
public:
	Road(const Vector2& vPos);
	virtual ~Road();
private:

public:
	virtual void BeginPlay() override;
	virtual void Tick(float _fDeltaTime) override;
	virtual void Render() override;

};

#endif