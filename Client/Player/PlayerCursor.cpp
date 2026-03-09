#include "PlayerCursor.h"
#include "InputMgr/InputMgr.h"
#include "CollisionMgr/CollisionMgr.h"
#include "Graphics/Renderer/Renderer.h"
#include "ClientCommon/ClientEnum.h"
//DELEGATE
//using OnHit = bool (*)(Actor*);

using namespace System;

PlayerCursor::PlayerCursor()
	:super(nullptr, "../Data/Player/PlayerCursor.txt", Vector2::Zero, Color::eGreen, E_LAYER::E_PLAYER)
{
	m_iSortingOrder = static_cast<int>(E_TYPE_SORTORDER::E_PLAYER);
}

PlayerCursor::~PlayerCursor()
{
}

void PlayerCursor::BeginPlay()
{
	super::BeginPlay();
}

void PlayerCursor::Tick(float _fDeltaTime)
{
	super::Tick(_fDeltaTime);

	/*SetPos(Vector2(
		static_cast<int>(InputMgr::Get_Instance().GetMousePosFloat().fX), 
		static_cast<int>(InputMgr::Get_Instance().GetMousePosFloat().fY)));*/
}

void PlayerCursor::Render()
{
	super::Render();
	string tempStr = to_string(InputMgr::Get_Instance().GetMousePosFloat().fX) + ", " + to_string(InputMgr::Get_Instance().GetMousePosFloat().fY);
	Renderer::Get_Instance().Submit(tempStr, Vector2(151, 30));
}

void PlayerCursor::OnHit()
{
}
