#include "FieldLevel.h"
#include "Level/Level.h"
#include "Player/Player.h"
#include "InputMgr/InputMgr.h"
#include "Engine/Engine.h"
#include "Player/PlayerCursor.h"
#include "CollisionMgr/CollisionMgr.h"
#include "Towers/Tower.h"
#include "Environment/Ground.h"
#include "Environment/Wall.h"
#include "Environment/Barricade.h"
#include "Environment/SpawningPoint.h"
#include "Target/Target.h"
#include "EngineCommon/Engine_Function.h"
#include "Game/Game.h"
#include "Actor/Actor.h"
#include "Towers/TowerBullet.h"
#include "Enemy/Enemy.h"
#include "Enemy/EnemySpawner.h"
#include "Util/Timer.h"
#include "Graphics/Renderer/Renderer.h"
#include "AStarMgr/AStarMgr.h"
#include "Effect/Effect.h"
#include "QuadTree/QuadTree.h"
#include "QuadTree/Quadrant.h"

using namespace System;
using namespace Util;

FieldLevel::FieldLevel()
{
	AddNewActor(m_pCursor = new PlayerCursor());
	AddNewActor(m_pPlayer = new Player(m_pCursor));
	LoadMap("../Data/Map/RoadMap.txt");

	m_PreRoundTimer.SetTargetTime(5.f);
	m_SpawnEnemyTimer.SetTargetTime(2.f);
	m_SpawnBossTimer.SetTargetTime(10.f);

	m_PreRoundTimer.ResetTime();
	m_SpawnEnemyTimer.ResetTime();
	m_SpawnBossTimer.ResetTime();

	m_bHasRoundBegun = false;

	m_eGameState = E_TYPE_GAMESTATE::E_PREROUND;

	m_iCurBaseHP = m_iMaxBaseHP;

	m_iWaveCount = 1;//start from round 1

	//TODO: set mapsize from the values read in loadmap
	m_QuadTree = new System::QuadTree(Quadrant(0, 0, m_iMapWidth, m_iMapHeight));
}

FieldLevel::~FieldLevel()
{
	Safe_Delete(m_QuadTree);
}

void FieldLevel::BeginPlay()
{
	super::BeginPlay();
}

void FieldLevel::Tick(float _fDeltaTime)
{
	super::Tick(_fDeltaTime);

#pragma region INPUT
	if (InputMgr::Get_Instance().GetKeyDown('Q'))
	{
		Engine::Get_Instance().QuitEngine();
	}
#pragma endregion INPUT

#pragma region PRE_ROUND
	if (!m_bHasRoundBegun && m_eGameState == E_TYPE_GAMESTATE::E_PREROUND)
	{
		m_PreRoundTimer.Tick(_fDeltaTime);

		if (m_PreRoundTimer.IsTimeOut())
		{
			m_bHasRoundBegun = true;
			m_PreRoundTimer.ResetTime();

			if (m_iWaveCount % static_cast<int>(m_vecEnemySpawnPoints.size()) == 0)//every 3 rounds, bossround starts.
			{
				m_eGameState = E_TYPE_GAMESTATE::E_BOSSROUND;
			}
			else {
				m_eGameState = E_TYPE_GAMESTATE::E_ROUND;
			}

		}
	}
#pragma endregion PRE_ROUND

#pragma region ROUND
	else if (m_bHasRoundBegun) 
	{
		if (m_eGameState == E_TYPE_GAMESTATE::E_ROUND)
		{
			m_SpawnEnemyTimer.Tick(_fDeltaTime);
			if (m_SpawnEnemyTimer.IsTimeOut())
			{
				size_t iEnemiesOnScene = m_vecLayers[static_cast<int>(E_LAYER::E_ENEMY)].size();

				if (m_iCurEnemySpawnPointIdx < static_cast<int>(m_vecEnemySpawnPoints.size()) &&
					m_iTotalSpawnedEnemies < m_iMaxEnemiesPerRound)
				{
					//spawn enemy.
					Enemy* pEnemy = SpawnActor<Enemy>(m_vecEnemySpawnPoints[m_iCurEnemySpawnPointIdx]);
					if (pEnemy)
					{
						//generate random type of enemies, except boss monster
						int iRandInt = Util::RandomInt(0, 100) % (static_cast<int>(E_TYPE_ENEMY::E_TYPE_ASSASSIN) + 1);
						pEnemy->SetEnemyInfo(static_cast<E_TYPE_ENEMY>(iRandInt));
					}
					++m_iTotalSpawnedEnemies;
				}

				if(m_iTotalSpawnedEnemies >= m_iMaxEnemiesPerRound && iEnemiesOnScene == 0)
				{
					m_PreRoundTimer.ResetTime();
					m_bHasRoundBegun = false;
					++m_iWaveCount;
					m_iTotalSpawnedEnemies = 0;
					++m_iCurEnemySpawnPointIdx;
					m_iCurEnemySpawnPointIdx = m_iCurEnemySpawnPointIdx >= static_cast<int>(m_vecEnemySpawnPoints.size()) ? 0 : m_iCurEnemySpawnPointIdx;
					m_eGameState = E_TYPE_GAMESTATE::E_PREROUND;
				}
				m_SpawnEnemyTimer.ResetTime();
			}
		}
#pragma endregion ROUND
#pragma region BOSSROUND
		else if (m_eGameState == E_TYPE_GAMESTATE::E_BOSSROUND)
		{
			m_SpawnBossTimer.Tick(_fDeltaTime);
			if (m_SpawnBossTimer.IsTimeOut())
			{
				size_t iEnemiesOnScene = m_vecLayers[static_cast<int>(E_LAYER::E_ENEMY)].size();

				if (m_iCurEnemySpawnPointIdx < static_cast<int>(m_vecEnemySpawnPoints.size()) &&
					m_iTotalSpawnedEnemies < m_iMaxBossPerRound)
				{
					//spawn enemy.
					Enemy* pEnemy = SpawnActor<Enemy>(m_vecEnemySpawnPoints[0]);
					if (pEnemy)
					{
						pEnemy->SetEnemyInfo(E_TYPE_ENEMY::E_TYPE_BOSS);
					}
					++m_iTotalSpawnedEnemies;
				}

				if (m_iTotalSpawnedEnemies >= m_iMaxBossPerRound && iEnemiesOnScene == 0)
				{
					m_PreRoundTimer.ResetTime();
					m_bHasRoundBegun = false;
					++m_iWaveCount;
					m_iTotalSpawnedEnemies = 0;
					m_iCurEnemySpawnPointIdx = 0;
					m_eGameState = E_TYPE_GAMESTATE::E_PREROUND;
				}
				m_SpawnBossTimer.ResetTime();
			}
		}
#pragma endregion BOSSROUND
		
		//Todo: clear quadtree, insert every actors on scene that can be collided with.
		//filter: when checking collisions for a bullet, don't loop thru all enemies.
		//1.ask quadtree(query), get vector<Actor*> of only 2 or 3 enemies in the same quadrant as the current bullet.
		//perform actual checkintersect only on those 2~3 enemies.


		//check collisions
		CheckCollision_PlayerCursor_Ground_Barricade_TowerActors();
		CheckCollision_TowerBullet_Enemies();
		CheckCollision_TowerBullet_Walls();
		CheckCollision_Enemies_Target();
		CheckCollision_Enemies_TowerBoundaries();
	}
}

void FieldLevel::Render()
{
	super::Render();

	string tempStr = to_string(static_cast<int>(m_PreRoundTimer.GetTargetTime() - m_PreRoundTimer.GetElapsedTime())) + " seconds left";
	Renderer::Get_Instance().Submit(tempStr, Vector2(151, 0), Color::eWhite);

	/*tempStr = "Number of actors in m_vecActors: " + to_string(m_vecActors.size());
	Renderer::Get_Instance().Submit(tempStr, Vector2(101, 20), Color::eWhite);*/
	size_t iTypeID = TowerBullet::GetType();
	tempStr = "TowerBullets in ActorPool: " + 
		to_string(m_pActorPool->GetPoolSize(iTypeID));
	Renderer::Get_Instance().Submit(tempStr, Vector2(151, 21), Color::eWhite);

	//TODO: replace with hp gauge
	tempStr = "My Base HP: " + to_string(m_iCurBaseHP) + "/" + to_string(m_iMaxBaseHP);
	Renderer::Get_Instance().Submit(tempStr, Vector2(151, 22), Color::eGreen);
	
	
	tempStr = "Enemies Remaining: " + to_string((m_eGameState == E_TYPE_GAMESTATE::E_ROUND ? m_iMaxEnemiesPerRound : m_iMaxBossPerRound) - m_iTotalSpawnedEnemies) + "/" +
		to_string(m_eGameState == E_TYPE_GAMESTATE::E_ROUND ? m_iMaxEnemiesPerRound : m_iMaxBossPerRound);

	Renderer::Get_Instance().Submit(tempStr, Vector2(151, 25), Color::eWhite);

	tempStr = "Enemies on map: " + to_string(m_vecLayers[static_cast<int>(E_LAYER::E_ENEMY)].size());
	Renderer::Get_Instance().Submit(tempStr, Vector2(151, 26), Color::eWhite);

	m_QuadTree->Render();
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
	Vector2 vPos = Vector2::Zero;
	int iEnemyCnt = 0;
	//read the file char by char.
	while (iIndex < szFileSize) {
		char cLetter = pBuffer[iIndex++];

		if (cLetter == '\n') {
			//update pos.
			vPos.m_fX = 0.f, ++vPos.m_fY;//initialize column index, increase row index.
			//skip CRLF.
			continue;
		}

		E_LAYER eLayer = E_LAYER::E_NONE;
		switch (cLetter)
		{
		case 'S'://spawn points
			eLayer = E_LAYER::E_SPAWNPOINT;
			m_vecEnemySpawnPoints.emplace_back(vPos);
			AddNewActor(new SpawningPoint(vPos));
			break;
		case '#':
			eLayer = E_LAYER::E_BARRICADE;
			AddNewActor(new Barricade(vPos));
			break;
		case '.':
			eLayer = E_LAYER::E_GROUND;
			AddNewActor(new Ground(vPos));
			break;
		case 'T':
			eLayer = E_LAYER::E_TARGET;
			AddNewActor(m_pTarget = new Target(vPos));
			break;
		case 'X':
			eLayer = E_LAYER::E_WALL;
			AddNewActor(new Wall(vPos));
			break;
		}
		AStarMgr::Get_Instance().SetCurNodeLayerType(static_cast<int>(vPos.m_fX), static_cast<int>(vPos.m_fY), eLayer);
		++vPos.m_fX;//increase column index
	}

	AStarMgr::Get_Instance().SetMapMaxSize(static_cast<int>(vPos.m_fY), static_cast<int>(vPos.m_fX));//HEIGHT(row) = 50, WIDTH(col) = 100
	m_iMapWidth = static_cast<int>(vPos.m_fX);
	m_iMapHeight = static_cast<int>(vPos.m_fY);

	Safe_Delete_Arr(pBuffer);
	fclose(pFile);
}

bool FieldLevel::AddTower(E_TYPE_TOWER _eType)
{
	if (!m_bCanPlaceTower)
		return false;

	Tower* pTower = nullptr;
	const char* pPath = "";

	switch (_eType)
	{
	case E_TYPE_TOWER::E_TYPE_RIFLE:
		pPath = "../Data/Tower/TowerRifle.txt";
		break;
	case E_TYPE_TOWER::E_TYPE_SHOTGUN:
		pPath = "../Data/Tower/TowerShotgun.txt";
		break;
	case E_TYPE_TOWER::E_TYPE_MACHINEGUN:
		pPath = "../Data/Tower/TowerMachinegun.txt";
		break;
	}

	//tower objects will not use pooling.
	AddNewActor(pTower = new Tower(_eType, pPath));

	if (pTower && m_pCursor)
		pTower->SetPos(m_pCursor->GetPos());

	return true;
}

void FieldLevel::StartRound()
{
	//Generate Enemies.

}

void FieldLevel::GameOver()
{
	Renderer::Get_Instance().Submit("GAME OVER", Vector2(50, 50), Color::eRed, 100);
	Renderer::Get_Instance().ImmidiateRender();
	Sleep(5000);
	Game::Get_Instance().QuitEngine();
}

void FieldLevel::CheckCollision_PlayerCursor_Ground_Barricade_TowerActors()
{
	//
	if (m_pPlayer == nullptr || m_pCursor == nullptr)
		return;

	m_QuadTree->ResetTree();

	for (auto const barricade : m_vecLayers[static_cast<int>(E_LAYER::E_BARRICADE)])
	{
		m_QuadTree->InsertArea(barricade->GetArea());
	}

	for (auto const ground : m_vecLayers[static_cast<int>(E_LAYER::E_GROUND)])
	{
		m_QuadTree->InsertArea(ground->GetArea());
	}

	for (auto const tower : m_vecLayers[static_cast<int>(E_LAYER::E_TOWER)])
	{
		m_QuadTree->InsertArea(tower->GetArea());
	}


	auto vecIntersects = m_QuadTree->Query(m_pCursor->GetArea());

	for (auto const area : vecIntersects)
	{
		Actor* pActor = area->GetActorOwner();
		if (pActor == nullptr)
			continue;

		if (m_pCursor->CheckIntersect(pActor))
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
	/*
	insert the more numerous group(targets) into the tree,
	and querying with the fewer or active group(seekers)

	Targets: Enemies, Bullets, numerous entities
	Seekers: Towers, Target Base, Walls

	 1. Bounding Box Size (The "Pruning" Factor)
	The rule: Insert the larger objects into the tree; Query with the smaller objects. (as in SIZE)
	* Why: A small query (like a bullet) will only traverse a narrow path down the tree, visiting
     very few nodes. A large query (like a giant AOE explosion or a large boss enemy) will
     overlap many quadrants, forcing the tree to return many "possible" matches, which increases
     the work for the final collision check.

	2. The "Active" vs. "Passive" Logic
	The rule: Query with the object that "owns" the collision logic.
	 * Why: Usually, a bullet's job is to check if it hit an enemy, deal damage, and then destroy
     itself. If you query with the bullet, the resulting code is cleaner:

	3. Spatial Distribution
	The rule: Insert the group that is more "statically" distributed across the map.
	 * Why: If enemies are spread out across the whole map, the QuadTree's spatial partitioning
     works perfectly. If all bullets are fired from a single point (like a machine gun), they
     will all be querying the same branch of the tree, which is efficient for the CPU cache.

	 The QuadTree's primary goal is to reduce the number of queries, 
	 so when the "Few vs. Many" rule is broken, always query with the "Few."
	*/

	m_QuadTree->ResetTree();

	for (auto const enemy : m_vecLayers[static_cast<int>(E_LAYER::E_ENEMY)])
	{
		if (enemy->Get_IsDestroyRequested())
			continue;

		m_QuadTree->InsertArea(enemy->GetArea());
	}

	for (auto const bullet : m_vecLayers[static_cast<int>(E_LAYER::E_TOWERBULLET)])
	{
		if (bullet->Get_IsDestroyRequested())
			continue;

		vector<Area*> vecIntersects = m_QuadTree->Query(bullet->GetArea());

		for (auto& enemy : vecIntersects)
		{
			Actor* pEnemy = enemy->GetActorOwner();

			if (pEnemy == nullptr)
				continue;

			if (pEnemy->Get_IsDestroyRequested())
				continue;

			//if normal CheckIntersect did not work, check again using cross and dot product
			if (bullet->CheckIntersect(pEnemy) || bullet->CheckIntersect_CrossDot(pEnemy))
			{
				dynamic_cast<TowerBullet*>(bullet)->OnCollisionEnter2D(pEnemy);

				int iEnemyHP = dynamic_cast<Enemy*>(pEnemy)->GetHP();
				int iBulletDmg = dynamic_cast<TowerBullet*>(bullet)->GetDamage();
				int iNewHP = iEnemyHP - iBulletDmg;
				if (iNewHP <= 0)//enemy hp hits below zero -> enemy destroyed
				{

					if (m_pPlayer)
					{
						m_pPlayer->AddMoney(dynamic_cast<Enemy*>(pEnemy)->GetMoney());
					}
					dynamic_cast<Enemy*>(pEnemy)->OnCollisionEnter2D(bullet);
					AddNewActor(new Effect(pEnemy->GetPos()));
				}
				else if (iNewHP > 0)//enemy still alive
				{
					dynamic_cast<Enemy*>(pEnemy)->SetHP(iNewHP);
				}
			}
			
		}
	}

	//OLD
	//for (auto const bullet : m_vecLayers[static_cast<int>(E_LAYER::E_TOWERBULLET)])
	//{
	//	if (bullet->Get_IsDestroyRequested())
	//		continue;

	//	for (auto const enemy : m_vecLayers[static_cast<int>(E_LAYER::E_ENEMY)])
	//	{
	//		if (enemy->Get_IsDestroyRequested())
	//			continue;

	//		if (bullet->CheckIntersect(enemy))
	//		{
	//			dynamic_cast<TowerBullet*>(bullet)->OnCollisionEnter2D(enemy);

	//			int iEnemyHP = dynamic_cast<Enemy*>(enemy)->GetHP();
	//			int iBulletDmg = dynamic_cast<TowerBullet*>(bullet)->GetDamage();
	//			int iNewHP = iEnemyHP - iBulletDmg;
	//			if (iNewHP <= 0)//enemy hp hits below zero -> enemy destroyed
	//			{

	//				if (m_pPlayer)
	//				{
	//					m_pPlayer->AddMoney(dynamic_cast<Enemy*>(enemy)->GetMoney());
	//				}
	//				dynamic_cast<Enemy*>(enemy)->OnCollisionEnter2D(bullet);
	//				//AddNewActor(new Effect(enemy->GetPos()));
	//			}
	//			else if (iNewHP > 0)//enemy still alive
	//			{
	//				dynamic_cast<Enemy*>(enemy)->SetHP(iNewHP);
	//			}
	//		}
	//	}
	//}
}

void FieldLevel::CheckCollision_TowerBullet_Walls()
{
	m_QuadTree->ResetTree();

	for (auto const wall : m_vecLayers[static_cast<int>(E_LAYER::E_WALL)])
	{
		if (wall->Get_IsDestroyRequested())
			continue;

		m_QuadTree->InsertArea(wall->GetArea());
	}

	for (auto const bullet : m_vecLayers[static_cast<int>(E_LAYER::E_TOWERBULLET)])
	{
		vector<Area*> vecIntersect = m_QuadTree->Query(bullet->GetArea());

		for (auto const area : vecIntersect)
		{
			Actor* pWall = area->GetActorOwner();
			if (bullet->CheckIntersect(pWall))
			{
				if(bullet->IsTypeOf<TowerBullet>())
					dynamic_cast<TowerBullet*>(bullet)->OnCollisionEnter2D(pWall);
			}
		}
	}

	//OLD
	//for (auto const bullet : m_vecLayers[static_cast<int>(E_LAYER::E_TOWERBULLET)])
	//{
	//	if (bullet->Get_IsDestroyRequested())
	//		continue;

	//	for (auto const wall : m_vecLayers[static_cast<int>(E_LAYER::E_WALL)])
	//	{
	//		if (bullet->CheckIntersect(wall))
	//		{
	//			//wall stays.
	//			dynamic_cast<TowerBullet*>(bullet)->OnCollisionEnter2D(wall);
	//		}
	//	}
	//}
}

void FieldLevel::CheckCollision_Enemies_Target()
{
	//QQUADTREE DEBUG
	m_QuadTree->ResetTree();

	for (auto const enemy : m_vecLayers[static_cast<int>(E_LAYER::E_ENEMY)])
	{
		if (enemy->Get_IsDestroyRequested())
			continue;

		m_QuadTree->InsertArea(enemy->GetArea());
	}

	vector<Area*> vecIntersect = m_QuadTree->Query(m_pTarget->GetArea());

	for (auto const area : vecIntersect)
	{
		Actor* pEnemy = area->GetActorOwner();
		if (pEnemy == nullptr)
			continue;
		if (m_pTarget->CheckIntersect(pEnemy))
		{
			dynamic_cast<Enemy*>(pEnemy)->OnCollisionEnter2D(m_pTarget);
			m_iCurBaseHP -= dynamic_cast<Enemy*>(pEnemy)->GetDamage();
			if (m_iCurBaseHP <= 0)
			{
				m_iCurBaseHP = 0;
				GameOver();
			}
		}
	}

	//OLD
	//for (auto const enemy : m_vecLayers[static_cast<int>(E_LAYER::E_ENEMY)])
	//{
	//	if (enemy->Get_IsDestroyRequested())
	//		continue;

	//	if (enemy->CheckIntersect(m_pTarget))
	//	{
	//		dynamic_cast<Enemy*>(enemy)->OnCollisionEnter2D(m_pTarget);
	//		--m_iCurBaseHP;
	//		if (m_iCurBaseHP <= 0)
	//		{
	//			//gameover
	//			GameOver();
	//		}
	//	}
	//}
}

void FieldLevel::CheckCollision_Enemies_TowerBoundaries()
{
	//QQUADTREE DEBUG
	m_QuadTree->ResetTree();

	for (auto const enemy : m_vecLayers[static_cast<int>(E_LAYER::E_ENEMY)])
	{
		if (enemy->Get_IsDestroyRequested())
			continue;

		//push all enemy actors into the tree
		m_QuadTree->InsertArea(enemy->GetArea());
	}

	//towers are relatively fewer.
	for (auto const tower : m_vecLayers[static_cast<int>(E_LAYER::E_TOWER)])
	{
		vector<Area*> vecIntersectingAreas = m_QuadTree->Query(tower->GetArea());

		for (auto const area : vecIntersectingAreas)
		{
			Actor* pEnemy = area->GetActorOwner();
			if (pEnemy == nullptr)
				continue;

			if (tower->IsTypeOf<Tower>() && dynamic_cast<Tower*>(tower)->CheckIsActorInTowerBoundary(pEnemy))
			{
				dynamic_cast<Tower*>(tower)->OnCollisionEnter2D(pEnemy);
			}
		}
	}

	//QUADTREE
	//for (auto const tower : m_vecLayers[static_cast<int>(E_LAYER::E_TOWER)])
	//{
	//	if (tower->Get_IsDestroyRequested())
	//		continue;

	//	//push all enemy actors into the tree
	//	m_QuadTree->InsertArea(tower->GetArea());
	//}

	//for (auto const enemy : m_vecLayers[static_cast<int>(E_LAYER::E_ENEMY)])
	//{
	//	if (enemy->Get_IsDestroyRequested())
	//		continue;

	//	vector<Area*> vecIntersectingAreas = m_QuadTree->Query(enemy->GetArea());

	//	for (auto const area : vecIntersectingAreas)
	//	{
	//		Actor* pTower = area->GetActorOwner();

	//		if (pTower == nullptr)
	//			continue;

	//		//if (pTower->CheckIntersect(enemy))
	//		if(pTower->IsTypeOf<Tower>() && dynamic_cast<Tower*>(pTower)->CheckIsActorInTowerBoundary(enemy))
	//		{
	//			dynamic_cast<Tower*>(pTower)->OnCollisionEnter2D(enemy);
	//		}
	//	}
	//}

	//ORIGIN
	//for (auto const enemy : m_vecLayers[static_cast<int>(E_LAYER::E_ENEMY)])
	//{
	//	if (enemy->Get_IsDestroyRequested())
	//		continue;

	//	for (auto const tower : m_vecLayers[static_cast<int>(E_LAYER::E_TOWER)])
	//	{
	//		//use custom checkintersect
	//		if (tower->IsTypeOf<Tower>() && dynamic_cast<Tower*>(tower)->CheckIsActorInTowerBoundary(enemy))
	//		{
	//			dynamic_cast<Tower*>(tower)->OnCollisionEnter2D(enemy);
	//		}
	//	}
	//}
}

