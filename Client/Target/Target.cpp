#include "Target.h"

using namespace System;

Target::Target(const Vector2& position)
	: super("T", nullptr, position, Color::eGreen, E_LAYER::E_ETCETERA)
{
	m_iSortingOrder = 3;
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
