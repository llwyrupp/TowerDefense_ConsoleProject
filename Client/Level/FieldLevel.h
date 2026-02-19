#pragma once

#ifndef __FIELDLEVEL_H__
#define __FIELDLEVEL_H__

#include "Level/Level.h"
using namespace System;

class FieldLevel : public Level
{
	RTTI_DECLARATIONS(FieldLevel, Level)
public:
	FieldLevel();
	virtual ~FieldLevel();

public:
	void BeginPlay() override;
	void Tick(float _fDeltaTime) override;
	void Render() override;

private:

};


#endif