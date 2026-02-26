#include "Player.h"
#include "InputMgr/InputMgr.h"
#include "Graphics/Renderer/Renderer.h"
#include "Player/PlayerCursor.h"
#include "EngineCommon/Engine_Function.h"
#include "Towers/Tower.h"
#include "Level/FieldLevel.h"

using namespace System;

Player::Player(PlayerCursor* _cursor)
	:super("@", nullptr, Vector2::Zero, Color::eGreen, E_LAYER::E_PLAYER), m_pCursor(_cursor)
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

	if (InputMgr::Get_Instance().GetMouseButtonDown(0))
	{
		dynamic_cast<FieldLevel*>(m_pLevel)->AddTower();
	}
}

void Player::Render()
{
	super::Render();
	//Renderer::Get_Instance().Submit(m_strImg, Vector2::Right, m_eColor, m_iSortingOrder);
}
