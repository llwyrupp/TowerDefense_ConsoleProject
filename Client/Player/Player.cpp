#include "Player.h"
#include "InputMgr/InputMgr.h"
#include "Graphics/Renderer/Renderer.h"

Player::Player()
	:super("@", nullptr, Vector2::Zero, Color::eGreen)
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

	if (InputMgr::Get_Instance().GetMouseButton(0))
	{
		SetPos(Vector2(InputMgr::Get_Instance().GetMousePos().m_iX, InputMgr::Get_Instance().GetMousePos().m_iY));
	}
}

void Player::Render()
{
	super::Render();
	Renderer::Get_Instance().Submit(m_strImg, Vector2::Right, m_eColor, m_iSortingOrder);
}