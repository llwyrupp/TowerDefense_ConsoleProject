#include "PlayerCursor.h"
#include "InputMgr/InputMgr.h"
#include "CollisionMgr/CollisionMgr.h"

//DELEGATE
//using OnHit = bool (*)(Actor*);

PlayerCursor::PlayerCursor()
	:super(nullptr, "../Data/Player/PlayerCursor.txt", Vector2::Zero, Color::eGreen)
{
	m_iSortingOrder = 3;
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
	SetPos(Vector2(InputMgr::Get_Instance().GetMousePos().m_iX, InputMgr::Get_Instance().GetMousePos().m_iY));
}

void PlayerCursor::Render()
{
	super::Render();
}

void PlayerCursor::OnHit()
{
}
