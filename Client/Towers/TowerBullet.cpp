#include "TowerBullet.h"
#include "Interface/ICollisionHandler.h"
#include "Level/Level.h"
#include "Enemy/Enemy.h"

using namespace System;

TowerBullet::TowerBullet()
	:super(".", nullptr, Vector2::Zero, Color::eBlue, E_LAYER::E_TOWERBULLET)
{
	m_bIsUsingActorPool = true;

#ifdef _DEBUG
	m_fSpeed = 10.f;
#endif

}

TowerBullet::~TowerBullet()
{

}

void TowerBullet::OnCollisionEnter2D(Actor* _pActor)
{
	if (_pActor->IsTypeOf<Enemy>() && !m_bIsDestroyRequested)
	{
		Destroy();
	}
}

void TowerBullet::BeginPlay()
{
	super::BeginPlay();
}

void TowerBullet::Tick(float _fDeltaTime)
{
	super::Tick(_fDeltaTime);

	SetPos(Vector2(GetPos().m_iX + m_vDir.m_iX * _fDeltaTime * m_fSpeed, GetPos().m_iY + m_vDir.m_iY * _fDeltaTime * m_fSpeed));

}

void TowerBullet::Render()
{
	super::Render();
}

