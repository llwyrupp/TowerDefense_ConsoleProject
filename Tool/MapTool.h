#pragma once
#ifndef __MAPTOOL_H__
#define __MAPTOOL_H__

#include "Engine/Engine.h"

using namespace System;

class MapTool : public Engine
{
public:
	MapTool();
	~MapTool();
public:
	inline static MapTool& Get_Instance() { return *m_pInstance; }
private:
	static MapTool* m_pInstance;
};


#endif // !__MAPTOOL_H__