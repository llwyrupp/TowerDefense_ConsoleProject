#pragma once
#ifndef __TOOLLEVEL_H__
#define __TOOLLEVEL_H__

#include "Level/Level.h"

using namespace System;

class ToolLevel : public Level
{
	RTTI_DECLARATIONS(ToolLevel, Level)
public:
	ToolLevel();
	virtual ~ToolLevel();
public:
	// Inherited via Level
	void BeginPlay() override;
	void Tick(float _fDeltaTime) override;
	void Render() override;
public:
	void ShowOption();
private:
	string m_strOption;
};


#endif // !__TOOLLEVEL_H__