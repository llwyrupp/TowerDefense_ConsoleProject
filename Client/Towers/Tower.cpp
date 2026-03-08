#include "Tower.h"
#include "Towers/TowerBullet.h"
#include "Level/Level.h"
#include "Enemy/Enemy.h"
#include "Graphics/Renderer/Renderer.h"
#include "QuadTree/Area.h"
#include <numbers>

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
		m_fBoundary = 6.f;
		m_FireTimer.SetTargetTime(1.f);
		break;
	case E_TYPE_TOWER::E_TYPE_SHOTGUN:
		m_fBoundary = 4.f;
		m_FireTimer.SetTargetTime(2.f);
		break;
	case E_TYPE_TOWER::E_TYPE_MACHINEGUN:
		m_fBoundary = 6.f;
		m_FireTimer.SetTargetTime(0.5f);
		break;
	}

	m_eCurState = E_TOWER_STATE::E_IDLE;

	if (m_pArea)
	{
		int iRadius = static_cast<int>(m_fBoundary);
		Vector2 vPos = GetPos();
		//update width and height as the boundary
		//m_pArea->SetMyQuadrantPos(vPos.m_iX, vPos.m_iY);
		m_pArea->SetMyQuadrantWidthHeight(vPos.m_iY + iRadius * 2, vPos.m_iX + iRadius);
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
	switch (m_eCurState)
	{
	case E_TOWER_STATE::E_IDLE:

		break;
	case E_TOWER_STATE::E_ATTACK:
		
		if (m_FireTimer.IsTimeOut() && m_pTarget != nullptr)
		{
			m_FireTimer.ResetTime();
			TowerBullet* pBullet = m_pLevel->SpawnActor<TowerBullet>(GetPos());
			Vector2 vDir = m_pTarget->GetPos() - GetPos();

			if (vDir == Vector2::Zero || m_pTarget->Get_IsDestroyRequested())
			{
				m_eCurState = E_TOWER_STATE::E_IDLE;
				m_pTarget = nullptr;
				return;
			}

			pBullet->SetDir(vDir);
			switch (m_tInfo.eType)
			{
			case E_TYPE_TOWER::E_TYPE_RIFLE:
				pBullet->SetDamage(10);
				break;
			case E_TYPE_TOWER::E_TYPE_SHOTGUN:
				pBullet->SetDamage(30);
				break;
			case E_TYPE_TOWER::E_TYPE_MACHINEGUN:
				pBullet->SetDamage(50);
				break;
			}
			

			//get distance between tower and enemy
			float fDist = static_cast<float>(sqrt(vDir.m_iX * vDir.m_iX + vDir.m_iY * vDir.m_iY));

			if (fDist >= m_fBoundary)
			{
				m_eCurState = E_TOWER_STATE::E_IDLE;
				m_pTarget = nullptr;
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
	vPos.m_iX -= iBoundary * 2;
	vPos.m_iY -= iBoundary;

	//draw top
	for (int i = 0; i < iBoundary * 4; ++i)
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
	for (int i = 0; i < iBoundary * 4; ++i)
	{
		Renderer::Get_Instance().Submit(tempStr, Vector2(vPos.m_iX + i, vPos.m_iY), Color::eYellow, 1);
	}

	//go back to topmost y pos.
	vPos.m_iY -= iBoundary * 2;
	//go to rightmost x pos
	vPos.m_iX += iBoundary * 4;
	//draw right
	for (int i = 0; i < iBoundary * 2; ++i)
	{
		Renderer::Get_Instance().Submit(tempStr, Vector2(vPos.m_iX, vPos.m_iY + i), Color::eYellow, 1);
	}


	
	////draw circle to represent tower attack boundary

	//for (int i = 0; i < 12; ++i)
	//{
	//	m_vPosBoundary[i] = GetPos();
	//}

	////x = m_iX + Radius * cos(@);
	////y = m_iY + Radius * sin(@);

	//float fRadius = m_fBoundary;
	//for (int i = 0; i < 12; ++i)
	//{
	//	// index * 30.f
	//	float fAngles = i * 30.f;
	//	float fRadians = ConvertToRadians(fAngles);
	//	m_vPosBoundary[i].m_iX += static_cast<int>(fRadius * cosf(fRadians));
	//	m_vPosBoundary[i].m_iY += static_cast<int>(fRadius * sinf(fRadians));
	//}

	//for (int i = 0; i < 12; ++i)
	//{
	//	Renderer::Get_Instance().Submit(tempStr, Vector2(m_vPosBoundary[i].m_iX, m_vPosBoundary[i].m_iY), Color::eYellow, 1);
	//}
}

bool Tower::CheckIsActorInTowerBoundary(const Actor* _other)
{
	Vector2 vCenter = GetPos();
	int iRadius = static_cast<int>(m_fBoundary);

	int iMinX_This = vCenter.m_iX - (iRadius * 2), iMaxX_This = vCenter.m_iX + (iRadius * 2),//extend width range by twice
		iMinY_This = vCenter.m_iY - iRadius, iMaxY_This = vCenter.m_iY + iRadius,
		iMinX_Other = _other->GetRect().left, iMaxX_Other = _other->GetRect().right,
		iMinY_Other = _other->GetRect().top, iMaxY_Other = _other->GetRect().bottom;

	//check if two intersects.

	if (iMaxX_This <= iMinX_Other)
		return false;

	if (iMaxX_Other <= iMinX_This)
		return false;

	if (iMaxY_This <= iMinY_Other)
		return false;

	if (iMaxY_Other <= iMinY_This)
		return false;

	return true;
}

float Tower::ConvertToRadians(float _degree)
{
	return _degree * (3.14159265358979323846f / 180.f);
}

void Tower::OnCollisionEnter2D(Actor* _pActor)
{
	if (_pActor && _pActor->IsTypeOf<Enemy>() && 
		m_eCurState == E_TOWER_STATE::E_IDLE && m_pTarget == nullptr)
	{
		m_pTarget = _pActor;
		m_eCurState = E_TOWER_STATE::E_ATTACK;
	}
}
