#include "Tower.h"
#include "Towers/TowerBullet.h"
#include "Level/Level.h"
#include "Enemy/Enemy.h"

using namespace System;

Tower::Tower(const E_TYPE_TOWER& _eType, const char* pPath)
	:super(nullptr, pPath, Vector2::Zero, Color::eWhite, E_LAYER::E_TOWER)
{
	m_tInfo.eType = _eType;
	m_iSortingOrder = 2;

	//TODO: needs to vary by the tower's type.
	switch (_eType)
	{
	case E_TYPE_TOWER::E_TYPE_RIFLE:
		m_fBoundary = 50.f;
		m_FireTimer.SetTargetTime(1.f);
		break;
	case E_TYPE_TOWER::E_TYPE_SHOTGUN:
		m_fBoundary = 30.f;
		m_FireTimer.SetTargetTime(2.f);
		break;
	case E_TYPE_TOWER::E_TYPE_MACHINEGUN:
		m_fBoundary = 40.f;
		m_FireTimer.SetTargetTime(0.5f);
		break;
	}
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
	
	m_FireTimer.Tick(_fDeltaTime);

	if (m_FireTimer.IsTimeOut())
	{
		m_FireTimer.ResetTime();
		m_pLevel->SpawnActor<TowerBullet>(GetPos());
	}

	//get distance between tower and enemy
	float fDist = sqrtf( powf(m_vTarget.m_iX - GetPos().m_iX, 2.f) + powf(m_vTarget.m_iY - GetPos().m_iY, 2.f));

	//if (fDist);
}

void Tower::Render()
{
	super::Render();
}

void Tower::FireBullet()
{
	//get bullet from spawning pool
	if (m_pLevel)
	{
		TowerBullet* pBullet = m_pLevel->SpawnActor<TowerBullet>(this->GetPos());
		if (pBullet)
		{
			pBullet->SetDir(m_vTarget - this->GetPos());
		}
	}
}

void Tower::OnCollisionEnter2D(Actor* _pActor)
{
	if (_pActor && _pActor->IsTypeOf<Enemy>())
	{
		m_vTarget = _pActor->GetPos();
		m_bIsAttacking = true;
	}
}
