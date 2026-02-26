#pragma once

#ifndef __FIELDLEVEL_H__
#define __FIELDLEVEL_H__

#include "Level/Level.h"
#include "Interface/ICollisionHandler.h"
#include "Util/Timer.h"

using namespace System;
using namespace Util;

class Player;
class PlayerCursor;
class TowerBullet;
class Enemy;
class FieldLevel : public Level
{
	RTTI_DECLARATIONS(FieldLevel, Level)
public:
	FieldLevel();
	virtual ~FieldLevel();
public:
	void BeginPlay() override;
	void Tick(float _fDeltaTime) override;
	void Render() override;
public:
	void LoadMap(const char* _pPath);
	void AddTower();
public:
	void StartRound();
public:
	void CheckCollision_PlayerCursor_TowerActors();
	void CheckCollision_TowerBullet_Enemies();

private:
	Player* m_pPlayer = nullptr;
	PlayerCursor* m_pCursor = nullptr;
private:
	Timer m_PreRoundTimer;
	Timer m_RoundTimer;
private:
	vector<TowerBullet*> m_vecTowerBullet;
	vector<Enemy*> m_vecEnemy;
private:
	bool m_bCanPlaceTower = false;
	bool m_bHasRoundBegun = false;
private:
};


#endif