#include "MapTool.h"
#include "ToolLevel.h"
using namespace System;
MapTool* MapTool::m_pInstance = nullptr;

MapTool::MapTool()
{
	m_pInstance = this;
	m_pMainLevel = new ToolLevel();
}

MapTool::~MapTool()
{
}
