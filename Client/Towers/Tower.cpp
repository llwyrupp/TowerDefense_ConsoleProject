#include "Tower.h"

Tower::Tower(const E_TYPE_TOWER& _eType, const char* pPath)
	:super(nullptr, pPath, Vector2::Zero, Color::eWhite)
{
	m_tInfo.eType = _eType;
	m_iSortingOrder = 2;
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
