#include "Road.h"
#include "EngineCommon/Engine_Enum.h"

using namespace System;
Road::Road(const Vector2& vPos)
	:super("#", nullptr, vPos, Color::eWhite, E_LAYER::E_ROAD)
{
	m_iSortingOrder = 1;
}

Road::~Road()
{
}

void Road::BeginPlay()
{
	super::BeginPlay();
}

void Road::Tick(float _fDeltaTime)
{
	super::Tick(_fDeltaTime);
}

void Road::Render()
{
	super::Render();
}
