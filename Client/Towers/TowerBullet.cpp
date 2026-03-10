#include "TowerBullet.h"
#include "Interface/ICollisionHandler.h"
#include "Level/Level.h"
#include "Enemy/Enemy.h"
#include "Environment/Wall.h"
#include "Graphics/Renderer/Renderer.h"

using namespace System;

TowerBullet::TowerBullet()
	:super("@", nullptr, Vector2::Zero, Color::eWhite, E_LAYER::E_TOWERBULLET)
{
	m_bIsUsingActorPool = true;
	m_iSortingOrder = static_cast<int>(E_TYPE_SORTORDER::E_TOWERBULLET);

	m_fSpeed = 100.f;
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

	//initialize re-spawned bullets, if necessary
	

}

void TowerBullet::Tick(float _fDeltaTime)
{
	super::Tick(_fDeltaTime);

	Vector2 vNewPos = GetPos();
	m_fX += m_vDir.m_fX * _fDeltaTime * m_fSpeed;

	if (m_fX >= AccX)
	{
		m_fX = 0.f;
		vNewPos.m_fX += 1.f;
	}
	else if (m_fX <= -AccX)
	{
		m_fX = 0.f;
		vNewPos.m_fX -= 1.f;
	}

	m_fY += m_vDir.m_fY * _fDeltaTime * m_fSpeed;

	if (m_fY >= AccY)
	{
		m_fY = 0.f;
		vNewPos.m_fY += 1.f;
	}
	else if (m_fY <= -AccY)
	{
		m_fY = 0.f;
		vNewPos.m_fY -= 1.f;
	}

	SetPos(vNewPos);

}

void TowerBullet::Render()
{
	super::Render();

	/*string tempStr = "bullet pos x: " + to_string(GetPos().m_fX);
	Renderer::Get_Instance().Submit(tempStr, Vector2(151, 30), Color::eWhite);*/
}

