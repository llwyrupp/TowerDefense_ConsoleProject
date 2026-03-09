#include "Barricade.h"
#include "EngineCommon/Engine_Enum.h"
#include "ClientCommon/ClientEnum.h"

using namespace System;
Barricade::Barricade(const Vector2& vPos)
	:super("#", nullptr, vPos, Color::eBlue, E_LAYER::E_BARRICADE)
{
	m_iSortingOrder = static_cast<int>(E_TYPE_SORTORDER::E_BARRICADE);
}

Barricade::~Barricade()
{
}

void Barricade::BeginPlay()
{
	super::BeginPlay();
}

void Barricade::Tick(float _fDeltaTime)
{
	super::Tick(_fDeltaTime);
}

void Barricade::Render()
{
	super::Render();
}
