#include "Enemy.h"
#include "AStarMgr/AStarMgr.h"
#include "Level/FieldLevel.h"
#include "Target/Target.h"
#include "Math/Vector2.h"
#include "EngineCommon/Engine_Function.h"

using namespace System;



Enemy::Enemy()
	:super("@", nullptr, Vector2::Zero, Color::eRed, E_LAYER::E_ENEMY)
{
	m_bIsUsingActorPool = true;

	m_fSpeed = 50.f;
}

Enemy::~Enemy()
{
}

void Enemy::OnCollisionEnter2D(Actor* _pActor)
{
	if (!m_bIsDestroyRequested)
	{
		Destroy();
	}
}

void Enemy::BeginPlay()
{
	super::BeginPlay();

	if (m_pLevel)
	{
		Vector2 targetPos = dynamic_cast<FieldLevel*>(m_pLevel)->GetTarget()->GetPos();

		Node* currentNode = new Node(GetPos().m_iX, GetPos().m_iY,0.f, nullptr);
		Node* targetNode = new Node(targetPos.m_iX, targetPos.m_iY, 0.f, nullptr);
		vector<POS> vecPath = AStarMgr::Get_Instance().FindPath(currentNode, targetNode);
		reverse(vecPath.begin(), vecPath.end());
		for (int i = 0; i < vecPath.size(); ++i)
		{
			//convert node to vector2 and push into actor
			Vector2 vPos(vecPath[i].iCol, vecPath[i].iRow);
			m_stackPath.push(vPos);
		}

		Safe_Delete(currentNode);
		Safe_Delete(targetNode);
	}
}

void Enemy::Tick(float _fDeltaTime)
{
	super::Tick(_fDeltaTime);
	Vector2 vNewPos = GetPos();

	//the position check may not always work.
	if (vNewPos == m_vNextPos)
	{
		m_bIsMoving = false;
		m_vNextDir = Vector2::Zero;
	}

	
	if (!m_bIsMoving && m_vNextDir == Vector2::Zero && !m_stackPath.empty())
	{
		m_vNextPos = m_stackPath.top();
		m_stackPath.pop();
		m_vNextDir = m_vNextPos - GetPos();//the direction vector to next dest.
		m_bIsMoving = true;
	}

	m_fX += static_cast<float>(m_vNextDir.m_iX) * _fDeltaTime * m_fSpeed;
	if (m_fX >= AccX)
	{
		vNewPos.m_iX += 1;
		m_fX = 0.f;
	}
	else if (m_fX <= -AccX)
	{
		vNewPos.m_iX -= 1;
		m_fX = 0.f;
	}

	m_fY += static_cast<float>(m_vNextDir.m_iY) * _fDeltaTime * m_fSpeed;
	if (m_fY >= AccY)
	{
		vNewPos.m_iY += 1;
		m_fY = 0.f;
	}
	else if (m_fY <= -AccY)
	{
		vNewPos.m_iY -= 1;
		m_fY = 0.f;
	}

	SetPos(vNewPos);
}

void Enemy::Render()
{
	super::Render();
}
