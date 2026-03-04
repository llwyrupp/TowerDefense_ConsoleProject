#include "Tower.h"
#include "Towers/TowerBullet.h"
#include "Level/Level.h"
#include "Enemy/Enemy.h"
#include "Graphics/Renderer/Renderer.h"

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
		m_fBoundary = 10.f;
		m_FireTimer.SetTargetTime(1.f);
		break;
	case E_TYPE_TOWER::E_TYPE_SHOTGUN:
		m_fBoundary = 3.f;
		m_FireTimer.SetTargetTime(2.f);
		break;
	case E_TYPE_TOWER::E_TYPE_MACHINEGUN:
		m_fBoundary = 4.f;
		m_FireTimer.SetTargetTime(0.5f);
		break;
	}

	m_eCurState = E_TOWER_STATE::E_IDLE;
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
	
	switch (m_eCurState)
	{
	case E_TOWER_STATE::E_IDLE:

		break;
	case E_TOWER_STATE::E_ATTACK:
		m_FireTimer.Tick(_fDeltaTime);
		if (m_FireTimer.IsTimeOut() && m_vTarget != Vector2::Zero)
		{
			m_FireTimer.ResetTime();
			TowerBullet* pBullet = m_pLevel->SpawnActor<TowerBullet>(GetPos());
			pBullet->SetDir(m_vTarget - GetPos());

			//get distance between tower and enemy
			float fDist = static_cast<float>(sqrt(pow(m_vTarget.m_iX - GetPos().m_iX, 2) + pow(m_vTarget.m_iY - GetPos().m_iY, 2)));

			if (fDist >= m_fBoundary)
			{
				m_eCurState = E_TOWER_STATE::E_IDLE;
				m_vTarget = Vector2::Zero;
			}
		}
		break;
	}
	
}

void Tower::Render()
{
	super::Render();

	//draw boundary to show the tower's range.
	string tempStr = ".";

	//draw top
	int iBoundary = static_cast<int>(m_fBoundary);
	Vector2 vPos = GetPos();
	vPos.m_iX -= iBoundary;
	vPos.m_iY -= iBoundary;

	//draw top
	for (int i = 0; i < iBoundary * 2; ++i)
	{
		Renderer::Get_Instance().Submit(tempStr, Vector2(vPos.m_iX + i, vPos.m_iY), Color::eYellow, 1);
	}

	//draw left
	for (int i = 0; i < iBoundary * 2; ++i)
	{
		Renderer::Get_Instance().Submit(tempStr, Vector2(vPos.m_iX, vPos.m_iY + i), Color::eYellow, 1);
	}

	vPos.m_iY += iBoundary * 2;
	//draw bottom
	for (int i = 0; i < iBoundary * 2; ++i)
	{
		Renderer::Get_Instance().Submit(tempStr, Vector2(vPos.m_iX + i, vPos.m_iY), Color::eYellow, 1);
	}

	vPos.m_iY = GetPos().m_iY;
	vPos.m_iX += iBoundary * 2;
	//draw right
	for (int i = 0; i < iBoundary * 2; ++i)
	{
		Renderer::Get_Instance().Submit(tempStr, Vector2(vPos.m_iX, vPos.m_iY + i), Color::eYellow, 1);
	}

	//for (int i = -iBoundary; i < iBoundary; ++i)
	//{
	//	Renderer::Get_Instance().Submit(tempStr, Vector2(vPos.m_iX + i, vPos.m_iY), Color::eYellow, 1);
	//}

	////draw right;
	//for (int i = -iBoundary; i < iBoundary; ++i)
	//{
	//	Renderer::Get_Instance().Submit(tempStr, Vector2(vPos.m_iX, vPos.m_iY + i), Color::eYellow, 1);
	//}
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
	if (_pActor && _pActor->IsTypeOf<Enemy>() && 
		m_eCurState == E_TOWER_STATE::E_IDLE && m_vTarget == Vector2::Zero)
	{
		m_vTarget = _pActor->GetPos();
		m_eCurState = E_TOWER_STATE::E_ATTACK;
	}
}
