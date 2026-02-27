#include "Wall.h"
#include "EngineCommon/Engine_Enum.h"

using namespace System;

Wall::Wall(const Vector2& vPos)
	:super("X", nullptr, vPos, Color::eWhite, E_LAYER::E_WALL)
{
	m_iSortingOrder = 0;
}

Wall::~Wall()
{
}

void Wall::BeginPlay()
{
	super::BeginPlay();
}

void Wall::Tick(float _fDeltaTime)
{
	super::Tick(_fDeltaTime);
}

void Wall::Render()
{
	super::Render();
}
