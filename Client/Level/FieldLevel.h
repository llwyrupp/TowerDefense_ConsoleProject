#pragma once

#ifndef __FIELDLEVEL_H__
#define __FIELDLEVEL_H__

#include "Level/Level.h"
#include "Interface/ICollisionHandler.h"
using namespace System;

class Player;
class PlayerCursor;
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
	void CheckCollision_PlayerCursor_TowerActors();
	void CheckCollision_TowerBullet_Enemies();

private:
	Player* m_pPlayer = nullptr;
	PlayerCursor* m_pCursor = nullptr;
	bool m_bCanPlaceTower = false;
};


#endif