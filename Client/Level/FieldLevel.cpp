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
#include "Environment/Road.h"
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

using namespace System;
using namespace Util;

FieldLevel::FieldLevel()
{
	AddNewActor(m_pCursor = new PlayerCursor());
	AddNewActor(m_pPlayer = new Player(m_pCursor));
	LoadMap("../Data/Map/RoadMap.txt");

	//m_PreRoundTimer.SetTargetTime(5.f);
	m_PreRoundTimer.SetTargetTime(0.1f);
	m_SpawnEnemyTimer.SetTargetTime(2.f);
	m_PreRoundTimer.ResetTime();
	m_SpawnEnemyTimer.ResetTime();

	m_bHasRoundBegun = false;

	m_eGameState = E_TYPE_GAMESTATE::E_PREROUND;

	m_iCurBaseHP = m_iMaxBaseHP;
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
			m_eGameState = E_TYPE_GAMESTATE::E_ROUND;
		}
	}
#pragma endregion PRE_ROUND

#pragma endregion ROUND
	else if (m_bHasRoundBegun && m_eGameState == E_TYPE_GAMESTATE::E_ROUND) {

		m_SpawnEnemyTimer.Tick(_fDeltaTime);
		if (m_SpawnEnemyTimer.IsTimeOut())
		{
			int iEnemiesOnScene = m_vecLayers[static_cast<int>(E_LAYER::E_ENEMY)].size();

			if (m_iCurEnemySpawnPointIdx < static_cast<int>(m_vecEnemySpawnPoints.size()) &&
				m_iTotalSpawnedEnemies < m_iMaxEnemiesPerRound)
			{
				//spawn enemy.
				Enemy* pEnemy = SpawnActor<Enemy>(m_vecEnemySpawnPoints[m_iCurEnemySpawnPointIdx]);
				if (pEnemy)
				{
					int iRandInt = Util::RandomInt(0, 100) % static_cast<int>(E_TYPE_ENEMY::E_TYPE_MAX);
					pEnemy->SetEnemyInfo(static_cast<E_TYPE_ENEMY>(iRandInt));
				}
				++m_iTotalSpawnedEnemies;
			}

			if(m_iTotalSpawnedEnemies >= m_iMaxEnemiesPerRound && iEnemiesOnScene == 0)
			{
				m_PreRoundTimer.ResetTime();
				m_bHasRoundBegun = false;
				m_eGameState = E_TYPE_GAMESTATE::E_PREROUND;
				m_iTotalSpawnedEnemies = 0;

				++m_iCurEnemySpawnPointIdx;
				m_iCurEnemySpawnPointIdx = m_iCurEnemySpawnPointIdx >= static_cast<int>(m_vecEnemySpawnPoints.size()) ? 0 : m_iCurEnemySpawnPointIdx;
			}
			m_SpawnEnemyTimer.ResetTime();
		}
		
		//check collisions
		CheckCollision_PlayerCursor_TowerActors();
		CheckCollision_TowerBullet_Enemies();
		CheckCollision_TowerBullet_Walls();
		CheckCollision_Enemies_Target();
		CheckCollision_Enemies_TowerBoundaries();
	}
#pragma endregion ROUND
}

void FieldLevel::Render()
{
	super::Render();

	string tempStr = to_string(static_cast<int>(m_PreRoundTimer.GetTargetTime() - m_PreRoundTimer.GetElapsedTime())) + " seconds left";
	Renderer::Get_Instance().Submit(tempStr, Vector2(151, 0), Color::eWhite);

	/*tempStr = "Number of actors in m_vecActors: " + to_string(m_vecActors.size());
	Renderer::Get_Instance().Submit(tempStr, Vector2(101, 20), Color::eWhite);*/
	tempStr = "Number of TowerBullets in Layer: " + to_string(m_vecLayers[static_cast<int>(E_LAYER::E_TOWERBULLET)].size());
	Renderer::Get_Instance().Submit(tempStr, Vector2(151, 21), Color::eWhite);
	tempStr = "My Base HP: " + to_string(m_iCurBaseHP) + "/" + to_string(m_iMaxBaseHP);
	Renderer::Get_Instance().Submit(tempStr, Vector2(151, 22), Color::eGreen);
	tempStr = "Enemies Remaining: " + to_string(m_iMaxEnemiesPerRound - m_iTotalSpawnedEnemies) + "/" + to_string(m_iMaxEnemiesPerRound);
	Renderer::Get_Instance().Submit(tempStr, Vector2(151, 25), Color::eWhite);
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
			vPos.m_iX = 0, ++vPos.m_iY;//initialize column index, increase row index.
			//skip CRLF.
			continue;
		}

		E_LAYER eLayer = E_LAYER::E_NONE;
		switch (cLetter)
		{
		case 'S'://spawn points
			m_vecEnemySpawnPoints.emplace_back(vPos);
			break;
		case '#':
			eLayer = E_LAYER::E_ROAD;
			AddNewActor(new Road(vPos));
			break;
		case '.':
		case 'G':
			eLayer = E_LAYER::E_GROUND;
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
		AStarMgr::Get_Instance().SetCurNodeLayerType(vPos.m_iX, vPos.m_iY, eLayer);
		++vPos.m_iX;//increase column index
	}

	AStarMgr::Get_Instance().SetMapMaxSize(vPos.m_iY, vPos.m_iX);//HEIGHT(row) = 50, WIDTH(col) = 100

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

void FieldLevel::CheckCollision_PlayerCursor_TowerActors()
{
	if (m_pPlayer == nullptr || m_pCursor == nullptr)
		return;

	for (auto const tower : m_vecLayers[static_cast<int>(E_LAYER::E_TOWER)])
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
	for (auto const bullet : m_vecLayers[static_cast<int>(E_LAYER::E_TOWERBULLET)])
	{
		if (bullet->Get_IsDestroyRequested())
			continue;

		for (auto const enemy : m_vecLayers[static_cast<int>(E_LAYER::E_ENEMY)])
		{
			if (enemy->Get_IsDestroyRequested())
				continue;

			if (bullet->CheckIntersect(enemy))
			{
				dynamic_cast<TowerBullet*>(bullet)->OnCollisionEnter2D(enemy);

				int iEnemyHP = dynamic_cast<Enemy*>(enemy)->GetHP();
				int iBulletDmg = dynamic_cast<TowerBullet*>(bullet)->GetDamage();
				int iNewHP = iEnemyHP - iBulletDmg;
				if (iNewHP <= 0)//enemy hp hits below zero -> enemy destroyed
				{

					if (m_pPlayer)
					{
						m_pPlayer->AddMoney(dynamic_cast<Enemy*>(enemy)->GetMoney());
					}
					dynamic_cast<Enemy*>(enemy)->OnCollisionEnter2D(bullet);
					//AddNewActor(new Effect(enemy->GetPos()));
				}
				else if (iNewHP > 0)//enemy still alive
				{
					dynamic_cast<Enemy*>(enemy)->SetHP(iNewHP);
				}
			}
		}
	}
}

void FieldLevel::CheckCollision_TowerBullet_Walls()
{
	for (auto const bullet : m_vecLayers[static_cast<int>(E_LAYER::E_TOWERBULLET)])
	{
		if (bullet->Get_IsDestroyRequested())
			continue;

		for (auto const wall : m_vecLayers[static_cast<int>(E_LAYER::E_WALL)])
		{
			if (bullet->CheckIntersect(wall))
			{
				//wall stays.
				dynamic_cast<TowerBullet*>(bullet)->OnCollisionEnter2D(wall);
			}
		}
	}
}

void FieldLevel::CheckCollision_Enemies_Walls()
{
	//temporary logic.
	for (auto const enemy : m_vecLayers[static_cast<int>(E_LAYER::E_ENEMY)])
	{
		if (enemy->Get_IsDestroyRequested())
			continue;

		for (auto const wall : m_vecLayers[static_cast<int>(E_LAYER::E_WALL)])
		{
			if (enemy->CheckIntersect(wall))
			{
				dynamic_cast<Enemy*>(enemy)->OnCollisionEnter2D(wall);
			}
		}
	}
}

void FieldLevel::CheckCollision_Enemies_Target()
{
	for (auto const enemy : m_vecLayers[static_cast<int>(E_LAYER::E_ENEMY)])
	{
		if (enemy->Get_IsDestroyRequested())
			continue;

		if (enemy->CheckIntersect(m_pTarget))
		{
			dynamic_cast<Enemy*>(enemy)->OnCollisionEnter2D(m_pTarget);
			--m_iCurBaseHP;
			if (m_iCurBaseHP <= 0)
			{
				//gameover
				GameOver();
			}
		}
	}
}

void FieldLevel::CheckCollision_Enemies_TowerBoundaries()
{
	for (auto const enemy : m_vecLayers[static_cast<int>(E_LAYER::E_ENEMY)])
	{
		if (enemy->Get_IsDestroyRequested())
			continue;

		for (auto const tower : m_vecLayers[static_cast<int>(E_LAYER::E_TOWER)])
		{
			//use custom checkintersect
			if (tower->IsTypeOf<Tower>() && dynamic_cast<Tower*>(tower)->CheckIsActorInTowerBoundary(enemy))
			{
				dynamic_cast<Tower*>(tower)->OnCollisionEnter2D(enemy);
			}
		}
	}
}

