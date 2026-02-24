#include "FieldLevel.h"
#include "Player/Player.h"
#include "InputMgr/InputMgr.h"
#include "Engine/Engine.h"
#include "Player/PlayerCursor.h"
#include "CollisionMgr/CollisionMgr.h"
#include "Towers/Tower.h"

FieldLevel::FieldLevel()
{
	PlayerCursor* pCursor = nullptr;
	AddNewActor(pCursor = new PlayerCursor());
	AddNewActor(new Player(pCursor));

}

FieldLevel::~FieldLevel()
{
}

void FieldLevel::BeginPlay()
{
	super::BeginPlay();

	
}

void FieldLevel::Tick(float _fDeltaTime)
{
	super::Tick(_fDeltaTime);

	if (InputMgr::Get_Instance().GetKeyDown('Q'))
	{
		Engine::Get_Instance().QuitEngine();
	}

	//check collisions
	CheckCollision_PlayerCursor_TowerActors();
}

void FieldLevel::Render()
{
	super::Render();
}

void FieldLevel::CheckCollision_PlayerCursor_TowerActors()
{
	Player* pPlayer = nullptr;
	PlayerCursor* pCursor = nullptr;
	list<Actor*> listTowerActors;

	for (auto const actor : m_vecActors)
	{
		if (actor->IsTypeOf<Player>())
			pPlayer = dynamic_cast<Player*>(actor);

		else if (actor->IsTypeOf<PlayerCursor>())
			pCursor = dynamic_cast<PlayerCursor*>(actor);

		else if(actor->IsTypeOf<Tower>())
			listTowerActors.push_back(actor);

	}
	if (pPlayer == nullptr || pCursor == nullptr)
		return;

	for (auto const tower : listTowerActors)
	{
		if (pCursor->CheckIntersect(tower))
		{
			pPlayer->SetCanPlaceTower(false);
			pCursor->SetColor(Color::eRed);
			return;
		}
	}
	pPlayer->SetCanPlaceTower(true);
	pCursor->SetColor(Color::eGreen);
}

