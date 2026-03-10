#include "SpawningPoint.h"
#include "EngineCommon/Engine_Enum.h"
#include "ClientCommon/ClientEnum.h"

using namespace System;
SpawningPoint::SpawningPoint(const Vector2& vPos)
	:super("S", nullptr, vPos, Color::eRed, E_LAYER::E_SPAWNPOINT)
{
	m_iSortingOrder = static_cast<int>(E_TYPE_SORTORDER::E_SPAWNINGPOINT);
}

SpawningPoint::~SpawningPoint()
{
}

void SpawningPoint::BeginPlay()
{
	super::BeginPlay();
}

void SpawningPoint::Tick(float _fDeltaTime)
{
	super::Tick(_fDeltaTime);
}

void SpawningPoint::Render()
{
	super::Render();
}
