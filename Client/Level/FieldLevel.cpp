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
}

void FieldLevel::Render()
{
	super::Render();
}

bool FieldLevel::CheckCollision_PlayerCursor_TowerActors()
{
	list<Actor*> listPlayerCursor;
	list<Actor*> listTowerActors;

	for (auto& actor : m_vecActors)
	{
		if (actor->IsTypeOf<PlayerCursor>())
			listPlayerCursor.push_back(actor);
		if (actor->IsTypeOf<Tower>())
			listTowerActors.push_back(actor);
	}

	if (CollisionMgr::Get_Instance().CheckCol_ReturnBool(listPlayerCursor, listTowerActors))
	{
		return true;
	}
	
	return false;
}

