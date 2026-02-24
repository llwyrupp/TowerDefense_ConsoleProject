#include "Player.h"
#include "InputMgr/InputMgr.h"
#include "Graphics/Renderer/Renderer.h"
#include "Player/PlayerCursor.h"
#include "EngineCommon/Engine_Function.h"
#include "Towers/Tower.h"
#include "Level/FieldLevel.h"

Player::Player(PlayerCursor* _cursor)
	:super("@", nullptr, Vector2::Zero, Color::eGreen), m_pCursor(_cursor), m_bCanPlaceTower(false)
{
	
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

	if (InputMgr::Get_Instance().GetMouseButtonDown(0) && m_bCanPlaceTower)
	{
		Tower* pTower = nullptr;
		m_pLevel->AddNewActor(pTower = new Tower(E_TYPE_TOWER::E_TYPE_RIFLE, "../Data/Tower/TowerRifle.txt"));

		if (pTower && m_pCursor)
			pTower->SetPos(m_pCursor->GetPos());
	}
	//SetPos(Vector2(InputMgr::Get_Instance().GetMousePos().m_iX, InputMgr::Get_Instance().GetMousePos().m_iY));
}

void Player::Render()
{
	super::Render();
	//Renderer::Get_Instance().Submit(m_strImg, Vector2::Right, m_eColor, m_iSortingOrder);
}
