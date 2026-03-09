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
	m_iSortingOrder = 5;
	m_bIsUsingActorPool = true;
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

		int iMyX = static_cast<int>(GetPos().m_fX);
		int iMyY = static_cast<int>(GetPos().m_fY);
		int iTargetX = static_cast<int>(targetPos.m_fX);
		int iTargetY = static_cast<int>(targetPos.m_fY);

		Node* currentNode = new Node(iMyX, iMyY, 0.f, nullptr);
		Node* targetNode = new Node(iTargetX, iTargetY, 0.f, nullptr);
		vector<POS> vecPath = AStarMgr::Get_Instance().FindPath(currentNode, targetNode);
		reverse(vecPath.begin(), vecPath.end());
		for (int i = 0; i < vecPath.size(); ++i)
		{
			//convert node to vector2 and push into actor
			Vector2 vPos(static_cast<float>(vecPath[i].iCol), static_cast<float>(vecPath[i].iRow));
			m_stackPath.push(vPos);
		}

		//set initial values
		ProcessNextPos();
		

		Safe_Delete(currentNode);
		Safe_Delete(targetNode);
	}
}

void Enemy::Tick(float _fDeltaTime)
{
	super::Tick(_fDeltaTime);
	
	Vector2 vNewPos = GetPos();

	//if past 
	if (CheckIfPastDest())
	{
		vNewPos = m_vNextPos;
		m_bIsMoving = false;
		SetPos(vNewPos);
	}

	
	if (!m_bIsMoving)
	{
		ProcessNextPos();
	}

	if (m_bIsMoving)
	{
		m_fX += m_vNextDir.m_fX * _fDeltaTime * m_fSpeed;

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

		m_fY += m_vNextDir.m_fY * _fDeltaTime * m_fSpeed;

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
}

void Enemy::Render()
{
	super::Render();
}

void Enemy::ProcessNextPos()
{
	if (!m_stackPath.empty())
	{
		m_vNextPos = m_stackPath.top();
		m_stackPath.pop();
		m_vNextDir = m_vNextPos - GetPos();//the direction vector to next dest. THIS MAY NOT WORK ALL THE TIME
		m_vNextDir.NormalizeVector();
		m_bIsMoving = true;
	}
}

void Enemy::SetEnemyInfo(E_TYPE_ENEMY _type)
{
	m_eType = _type;

	switch (_type)
	{
	case E_TYPE_ENEMY::E_TYPE_SOLDIER:
		m_strImg = "A";
		m_fSpeed = 40.f;
		m_iHP = 20;
		m_iMoney = 10;
		break;
	case E_TYPE_ENEMY::E_TYPE_TANKER:
		m_strImg = "$";
		m_fSpeed = 30.f;
		m_iHP = 70;
		m_iMoney = 50;
		break;
	case E_TYPE_ENEMY::E_TYPE_ASSASSIN:
		m_strImg = "&";
		m_fSpeed = 50.f;
		m_iHP = 40;
		m_iMoney = 100;
		break;
	}
}

bool Enemy::CheckIfPastDest()
{
	Vector2 curPos = GetPos();

	Vector2 vTargetToEnemy = curPos - m_vNextPos;

	//vTargetToEnemy will point the same direction if enemy has passed m_vNextPos.
	//if we dot product nextDir vector and vTargetToEnemy vector, the result can tell whether this enemy has passed next pos or not.
	//return of positive value means the two vectors are pointing the same direction.
	//before reaching nextPos, the two vectors will point the opposite direction(more or less), 
	// hence returning negative value when dot-producted on the two vectors.
	if (m_vNextDir.DotProduct(vTargetToEnemy) >= 0.f)
		return true;

	return false;
}
