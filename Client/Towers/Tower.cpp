#include "Tower.h"
#include "Towers/TowerBullet.h"
#include "Level/Level.h"
#include "Enemy/Enemy.h"
#include "Graphics/Renderer/Renderer.h"
#include "QuadTree/Area.h"
#include "EngineCommon/Engine_Defines.h"
#include "Util/Util.h"


using namespace System;
using namespace Util;

Tower::Tower(const E_TYPE_TOWER& _eType, const char* pPath)
	:super(nullptr, pPath, Vector2::Zero, Color::eMagenta, E_LAYER::E_TOWER)
{
	m_tInfo.eType = _eType;
	m_iSortingOrder = static_cast<int>(E_TYPE_SORTORDER::E_TOWER);

	//TODO: needs to vary by the tower's type.
	switch (_eType)
	{
	case E_TYPE_TOWER::E_TYPE_RIFLE:
		m_fBoundary = 7.f;
		m_FireTimer.SetTargetTime(1.f);
		break;
	case E_TYPE_TOWER::E_TYPE_SHOTGUN:
		m_fBoundary = 5.f;
		m_FireTimer.SetTargetTime(2.f);
		break;
	case E_TYPE_TOWER::E_TYPE_MACHINEGUN:
		m_fBoundary = 6.f;
		m_FireTimer.SetTargetTime(0.2f);
		break;
	}

	m_eCurState = E_TOWER_STATE::E_IDLE;

	if (m_pArea)
	{
		int iRadius = static_cast<int>(m_fBoundary);
		Vector2 vPos = GetPos();
		//update width and height as the boundary
		//m_pArea->SetMyQuadrantPos(vPos.m_fX, vPos.m_fY);
		m_pArea->SetMyQuadrantWidthHeight(static_cast<int>(vPos.m_fY) + iRadius * 2, static_cast<int>(vPos.m_fX) + iRadius);
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
		//idle logic if necessary
		break;
	case E_TOWER_STATE::E_ATTACK:
		
		if (m_FireTimer.IsTimeOut() && m_pTarget != nullptr)
		{
			m_FireTimer.ResetTime();

			TowerBullet* pBullet = nullptr;
			Vector2 vNewPos = GetPos();
			Vector2 vTargetPos = m_pTarget->GetPos();
			//set position to the center of the tower.
			vNewPos.m_fX += static_cast<float>(m_iWidth / 2);
			vNewPos.m_fY += static_cast<float>(m_iHeight / 2);
			vTargetPos.m_fX += static_cast<float>(m_pTarget->GetWidth() / 2);
			vTargetPos.m_fY += static_cast<float>(m_pTarget->GetHeight() / 2);

			//get bullet's direction vector
			Vector2 vDiff = vTargetPos - vNewPos;
			Vector2 vNormDir = vDiff;
			vNormDir.NormalizeVector();
			
			if (vDiff == Vector2::Zero || m_pTarget->Get_IsDestroyRequested())
			{
				m_eCurState = E_TOWER_STATE::E_IDLE;
				m_pTarget = nullptr;
				return;
			}

			switch (m_tInfo.eType)
			{
			case E_TYPE_TOWER::E_TYPE_RIFLE:
				{
					//normalize vector.
					pBullet = m_pLevel->SpawnActor<TowerBullet>(vNewPos);
					pBullet->SetDamage(10);
					pBullet->SetDir(vNormDir);
				}
				break;
			case E_TYPE_TOWER::E_TYPE_SHOTGUN:
				{
					//find the angle between myself(the tower) and the target
					float fRadiansCenter = atan2(vTargetPos.m_fY - vNewPos.m_fY, vTargetPos.m_fX - vNewPos.m_fX);
					float fRadianLeft = fRadiansCenter - ConvertToRadians(10.f);
					float fRadianRight = fRadiansCenter + ConvertToRadians(10.f);
					Vector2 vNewDir = Vector2::Zero;

					//second bullet (30 degrees to left)
					pBullet = m_pLevel->SpawnActor<TowerBullet>(GetPos());
					pBullet->SetDamage(15);
					vNewDir = Vector2(cosf(fRadianLeft), sinf(fRadianLeft));
					pBullet->SetDir(vNewDir);

					//third bullet (30 degrees to right)
					pBullet = m_pLevel->SpawnActor<TowerBullet>(GetPos());
					pBullet->SetDamage(15);
					vNewDir = Vector2(cosf(fRadianRight), sinf(fRadianRight));
					pBullet->SetDir(vNewDir);

					//first bullet (center)
					pBullet = m_pLevel->SpawnActor<TowerBullet>(GetPos());
					pBullet->SetDamage(15);
					vNewDir = Vector2(cosf(fRadiansCenter), sinf(fRadiansCenter));
					pBullet->SetDir(vNewDir);
				}
				break;
			case E_TYPE_TOWER::E_TYPE_MACHINEGUN:
				{
					//normalize vector.
					pBullet = m_pLevel->SpawnActor<TowerBullet>(vNewPos);
					pBullet->SetDamage(20);
					pBullet->SetDir(vNormDir);
				}
				break;
			}
			
			//OnCollisionExit2D()
			//get distance between tower and enemy
			//calculation must be done with a direction vector without normalization
			float fDist = sqrtf(vDiff.m_fX * vDiff.m_fX + vDiff.m_fY * vDiff.m_fY);

			//logic for exiting tower boundary
			if (fDist >= m_fBoundary)
			{
				//set tower state to idle for the next attack
				m_eCurState = E_TOWER_STATE::E_IDLE;
				//nullify the target for the next attack
				m_pTarget = nullptr;
				return;
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
	//move position to left-top position
	vPos.m_fX -= m_fBoundary * 2.f;
	vPos.m_fY -= m_fBoundary;

	//draw top line(left to right)
	for (int i = 0; i < iBoundary * 4; ++i)
	{
		Renderer::Get_Instance().Submit(tempStr, Vector2(vPos.m_fX + i, vPos.m_fY), Color::eYellow, 1);
	}

	//draw left(top to bottom)
	for (int i = 0; i < iBoundary * 2; ++i)
	{
		Renderer::Get_Instance().Submit(tempStr, Vector2(vPos.m_fX, vPos.m_fY + i), Color::eYellow, 1);
	}

	//move position from top-left to bottom-left
	vPos.m_fY += iBoundary * 2;
	//draw right
	for (int i = 0; i < iBoundary * 4; ++i)
	{
		Renderer::Get_Instance().Submit(tempStr, Vector2(vPos.m_fX + i, vPos.m_fY), Color::eYellow, 1);
	}

	//move position to top-right
	vPos.m_fY -= iBoundary * 2;
	vPos.m_fX += iBoundary * 4;
	//draw bottom
	for (int i = 0; i < iBoundary * 2; ++i)
	{
		Renderer::Get_Instance().Submit(tempStr, Vector2(vPos.m_fX, vPos.m_fY + i), Color::eYellow, 1);
	}


	
	////draw circle to represent tower attack boundary

	//for (int i = 0; i < 12; ++i)
	//{
	//	m_vPosBoundary[i] = GetPos();
	//}

	////x = m_fX + Radius * cos(@);
	////y = m_fY + Radius * sin(@);

	//float fRadius = m_fBoundary;
	//for (int i = 0; i < 12; ++i)
	//{
	//	// index * 30.f
	//	float fAngles = i * 30.f;
	//	float fRadians = ConvertToRadians(fAngles);
	//	m_vPosBoundary[i].m_fX += static_cast<int>(fRadius * cosf(fRadians));
	//	m_vPosBoundary[i].m_fY += static_cast<int>(fRadius * sinf(fRadians));
	//}

	//for (int i = 0; i < 12; ++i)
	//{
	//	Renderer::Get_Instance().Submit(tempStr, Vector2(m_vPosBoundary[i].m_fX, m_vPosBoundary[i].m_fY), Color::eYellow, 1);
	//}
}

bool Tower::CheckIsActorInTowerBoundary(const Actor* _other)
{
	Vector2 vCenter = GetPos();

	float fMinX_This = vCenter.m_fX - (m_fBoundary * 2.f), fMaxX_This = vCenter.m_fX + (m_fBoundary * 2.f),//extend width range by twice
		fMinY_This = vCenter.m_fY - m_fBoundary, fMaxY_This = vCenter.m_fY + m_fBoundary,
		fMinX_Other = static_cast<float>(_other->GetRect().left), fMaxX_Other = static_cast<float>(_other->GetRect().right),
		fMinY_Other = static_cast<float>(_other->GetRect().top), fMaxY_Other = static_cast<float>(_other->GetRect().bottom);

	//check if two intersects.

	if (fMaxX_This <= fMinX_Other)
		return false;

	if (fMaxX_Other <= fMinX_This)
		return false;

	if (fMaxY_This <= fMinY_Other)
		return false;

	if (fMaxY_Other <= fMinY_This)
		return false;

	return true;
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
