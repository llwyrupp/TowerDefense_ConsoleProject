#include "Ground.h"
#include "EngineCommon/Engine_Enum.h"
#include "ClientCommon/ClientEnum.h"

using namespace System;

Ground::Ground(const Vector2& vPos)
	:super(" ", nullptr, vPos, Color::eWhite, E_LAYER::E_GROUND)
{
	m_iSortingOrder = static_cast<int>(E_TYPE_SORTORDER::E_GROUND);
}

Ground::~Ground()
{
}

void Ground::BeginPlay()
{
	super::BeginPlay();
}

void Ground::Tick(float _fDeltaTime)
{
	super::Tick(_fDeltaTime);
}

void Ground::Render()
{
	super::Render();
}
