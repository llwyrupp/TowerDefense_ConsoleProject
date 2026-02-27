#include "Player.h"
#include "InputMgr/InputMgr.h"
#include "Graphics/Renderer/Renderer.h"
#include "Player/PlayerCursor.h"
#include "EngineCommon/Engine_Function.h"
#include "Towers/Tower.h"
#include "Level/FieldLevel.h"

Player::Player(PlayerCursor* _cursor)
	:super("@", nullptr, Vector2::Zero, Color::eGreen, E_LAYER::E_PLAYER), m_pCursor(_cursor)
{
	m_iMoney = 1000;
	m_eCurTowerType = E_TYPE_TOWER::E_TYPE_RIFLE;

	m_iOwnTower[0] = 3;
	m_iOwnTower[1] = 3;
	m_iOwnTower[2] = 3;
	m_iTowerPrice[0] = 100;
	m_iTowerPrice[1] = 200;
	m_iTowerPrice[2] = 300;

	m_Cooldown[0].SetTargetTime(5.f);
	m_Cooldown[1].SetTargetTime(10.f);
	m_Cooldown[2].SetTargetTime(15.f);
}

Player::~Player()
{
}

void Player::BeginPlay()
{
	super::BeginPlay();

}

void Player::Tick(float _fDeltaTime)
{
	super::Tick(_fDeltaTime);

	for (int i = 0; i < 3; ++i)
	{
		m_Cooldown[i].Tick(_fDeltaTime);
	}

	if (InputMgr::Get_Instance().GetKeyDown('1'))
	{
		m_eCurTowerType = E_TYPE_TOWER::E_TYPE_RIFLE;
	}
	if (InputMgr::Get_Instance().GetKeyDown('2'))
	{
		m_eCurTowerType = E_TYPE_TOWER::E_TYPE_SHOTGUN;
	}
	if (InputMgr::Get_Instance().GetKeyDown('3'))
	{
		m_eCurTowerType = E_TYPE_TOWER::E_TYPE_MACHINEGUN;
	}

	//if (InputMgr::Get_Instance().GetMouseButtonDown(0))
	if(InputMgr::Get_Instance().GetKeyDown(VK_SPACE) && Check_EnoughMoney_TowerCooldown())
	{
		int iTower = static_cast<int>(m_eCurTowerType);
		if (dynamic_cast<FieldLevel*>(m_pLevel)->AddTower(m_eCurTowerType))
		{
			m_Cooldown[iTower].ResetTime();
			--m_iOwnTower[iTower];
			m_iMoney -= m_iTowerPrice[iTower];
			m_iMoney = m_iMoney < 0 ? 0 : m_iMoney;
		}
	}

	
}

void Player::Render()
{
	super::Render();

	//print money
	string tempStr = "Money: " + to_string(m_iMoney);

	Renderer::Get_Instance().Submit(tempStr, Vector2(101, 5), Color::eGreen);


	//print tower type
	switch (m_eCurTowerType)
	{
	case E_TYPE_TOWER::E_TYPE_RIFLE:
		tempStr = "Current Tower: Rifle, " + to_string(m_iOwnTower[0]) + " Left";
		break;
	case E_TYPE_TOWER::E_TYPE_SHOTGUN:
		tempStr = "Current Tower: Shotgun, " + to_string(m_iOwnTower[1]) + " Left";
		break;
	case E_TYPE_TOWER::E_TYPE_MACHINEGUN:
		tempStr = "Current Tower: Machinegun, " + to_string(m_iOwnTower[2]) + " Left";
		break;
	}

	Renderer::Get_Instance().Submit(tempStr, Vector2(101, 10), Color::eGreen);


	//print tower placement cooldown
	switch (m_eCurTowerType)
	{
	case E_TYPE_TOWER::E_TYPE_RIFLE:
		tempStr = "RifleTower Cooldown: " + to_string(m_Cooldown[0].GetTargetTime() - m_Cooldown[0].GetElapsedTime()) + "s";
		break;
	case E_TYPE_TOWER::E_TYPE_SHOTGUN:
		tempStr = "ShotgunTower Cooldown: " + to_string(m_Cooldown[1].GetTargetTime() - m_Cooldown[1].GetElapsedTime()) + "s";
		break;
	case E_TYPE_TOWER::E_TYPE_MACHINEGUN:
		tempStr = "MachinegunTower Cooldown: " + to_string(m_Cooldown[2].GetTargetTime() - m_Cooldown[2].GetElapsedTime()) + "s";
		break;
	}

	Renderer::Get_Instance().Submit(tempStr, Vector2(101, 11), Color::eGreen);
}

bool Player::Check_EnoughMoney_TowerCooldown()
{
	int iTower = static_cast<int>(m_eCurTowerType);
	if (iTower >= static_cast<int>(E_TYPE_TOWER::E_TYPE_MAX))
		return false;

	if (m_iMoney < 0)
	{
		return false;
	}

	if (m_Cooldown[iTower].IsTimeOut() && m_iOwnTower[iTower] > 0 && m_iMoney - m_iTowerPrice[iTower] >= 0)
	{
		return true;
	}

	return false;
}
