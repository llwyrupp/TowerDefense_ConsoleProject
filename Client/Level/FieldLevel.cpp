#include "FieldLevel.h"
#include "Player/Player.h"
#include "InputMgr/InputMgr.h"
#include "Engine/Engine.h"
#include "Player/PlayerCursor.h"
#include "CollisionMgr/CollisionMgr.h"
#include "Towers/Tower.h"
#include "Ground/Ground.h"
#include "Road/Road.h"
#include "Target/Target.h"
#include "EngineCommon/Engine_Function.h"
#include "Game/Game.h"
#include "Actor/Actor.h"
#include "Towers/TowerBullet.h"
#include "Enemy/Enemy.h"

using namespace System;

FieldLevel::FieldLevel()
{
	AddNewActor(m_pCursor = new PlayerCursor());
	AddNewActor(m_pPlayer = new Player(m_pCursor));
	LoadMap("../Data/Map/RoadMap.txt");
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

void FieldLevel::LoadMap(const char* _pPath)
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, _pPath, "rt");

	//exception err
	if (!pFile) {
		cerr << "FAILED TO LOAD MAP";
		__debugbreak();
	}

	fseek(pFile, 0, SEEK_END);
	size_t szFileSize = ftell(pFile);
	rewind(pFile);
	char* pBuffer = new char[szFileSize + 1];

	size_t szLen = fread(pBuffer, sizeof(char), szFileSize, pFile);

	int iIndex = 0;
	Vector2 vPos;
	int iEnemyCnt = 0;
	//read the file char by char.
	while (iIndex < szFileSize) {
		char cLetter = pBuffer[iIndex++];

		if (cLetter == '\n') {
			//update pos.
			vPos.m_iX = 0, ++vPos.m_iY;
			//skip CRLF.
			continue;
		}

		switch (cLetter)
		{
		case '@':
			AddNewActor(new Road(vPos));
			break;
		case '.':
			AddNewActor(new Ground(vPos));
			break;
		case 'T':
			AddNewActor(new Target(vPos));
			AddNewActor(new Ground(vPos));
			break;
		}

		++vPos.m_iX;
	}

	Safe_Delete_Arr(pBuffer);
	fclose(pFile);
}

void FieldLevel::AddTower()
{
	Tower* pTower = nullptr;
	AddNewActor(pTower = new Tower(E_TYPE_TOWER::E_TYPE_RIFLE, "../Data/Tower/TowerRifle.txt"));

	if (pTower && m_pCursor)
		pTower->SetPos(m_pCursor->GetPos());
}

void FieldLevel::CheckCollision_PlayerCursor_TowerActors()
{
	list<Actor*> listTowerActors;

	for (auto const actor : m_vecActors)
	{
		if(actor->IsTypeOf<Tower>())
			listTowerActors.push_back(actor);

	}
	if (m_pPlayer == nullptr || m_pCursor == nullptr)
		return;

	for (auto const tower : listTowerActors)
	{
		if (m_pCursor->CheckIntersect(tower))
		{
			m_bCanPlaceTower = false;
			m_pCursor->SetColor(Color::eRed);
			return;
		}
	}
	m_bCanPlaceTower = true;
	m_pCursor->SetColor(Color::eGreen);
}

void FieldLevel::CheckCollision_TowerBullet_Enemies()
{
	list<Actor*> listTowerBulletActors;
	list<Actor*> listEnemyActors;

	for (auto const actor : m_vecActors)
	{
		if (actor->IsTypeOf<TowerBullet>())
			listTowerBulletActors.push_back(actor);

		if (actor->IsTypeOf<Enemy>())
			listEnemyActors.push_back(actor);
	}

	for (auto const bullet : listTowerBulletActors)
	{
		for (auto const enemy : listEnemyActors)
		{
			if (bullet->CheckIntersect(enemy))
			{
				dynamic_cast<TowerBullet*>(bullet)->OnCollisionEnter2D(enemy);
				dynamic_cast<Enemy*>(enemy)->OnCollisionEnter2D(bullet);
			}
		}
	}
}

