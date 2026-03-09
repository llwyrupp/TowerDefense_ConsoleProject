#include "Target.h"
#include "ClientCommon/ClientEnum.h"
using namespace System;

Target::Target(const Vector2& position)
	: super("T", nullptr, position, Color::eGreen, E_LAYER::E_TARGET)
{
	m_iSortingOrder = static_cast<int>(E_TYPE_SORTORDER::E_TARGET);
}

void Target::BeginPlay()
{
	super::BeginPlay();
}

void Target::Tick(float _fDeltaTime)
{
	super::Tick(_fDeltaTime);
}

void Target::Render()
{
	super::Render();
}
