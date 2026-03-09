#include "Wall.h"
#include "EngineCommon/Engine_Enum.h"
#include "ClientCommon/ClientEnum.h"
using namespace System;

Wall::Wall(const Vector2& vPos)
	:super("X", nullptr, vPos, Color::eWhite, E_LAYER::E_WALL)
{
	m_iSortingOrder = static_cast<int>(E_TYPE_SORTORDER::E_WALL);
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
