#include "TowerBullet.h"
#include "Interface/ICollisionHandler.h"
#include "Level/Level.h"
#include "Enemy/Enemy.h"
#include "Environment/Wall.h"
#include "Graphics/Renderer/Renderer.h"

using namespace System;

TowerBullet::TowerBullet()
	:super("&", nullptr, Vector2::Zero, Color::eWhite, E_LAYER::E_TOWERBULLET)
{
	m_bIsUsingActorPool = true;

#ifdef _DEBUG
	m_fSpeed = 100.f;
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
	else if (_pActor->IsTypeOf<Wall>() && !m_bIsDestroyRequested)
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

	SetPos(Vector2(GetPos().m_iX + m_vDir.m_iX * static_cast<int>(_fDeltaTime * m_fSpeed), 
		GetPos().m_iY + m_vDir.m_iY * static_cast<int>(_fDeltaTime * m_fSpeed)));

}

void TowerBullet::Render()
{
	super::Render();

	string tempStr = "bullet pos x: " + to_string(GetPos().m_iX);
	Renderer::Get_Instance().Submit(tempStr, Vector2(101, 30), Color::eWhite);
}

