#include "Tower.h"

Tower::Tower(const E_TYPE_TOWER& _eType)
{
	m_tInfo.eType = _eType;
}

Tower::~Tower()
{
}

void Tower::BeginPlay()
{
	super::BeginPlay();
}

void Tower::Tick(float _fDeltaTime)
{
	super::Tick(_fDeltaTime);
}

void Tower::Render()
{
	super::Render();
}
