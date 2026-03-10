#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Actor/Actor.h"
#include "Util/Timer.h"
#include "ClientCommon/ClientEnum.h"

using namespace Util;

class PlayerCursor;
class Player : public Actor
{
	RTTI_DECLARATIONS(Player, Actor)
public:
	Player(PlayerCursor* _cursor);
	virtual ~Player();
public:
	void BeginPlay() override;
	void Tick(float _fDeltaTime) override;
	void Render() override;

public:
	bool Check_TowerCooldown(int _index);
	bool Check_EnoughMoney(int _index);
	bool Check_EnoughTower(int _index);
	bool CheckCanPlaceTower(int _index);
public:
	void AddMoney(int _money) { m_iMoney += _money; }
private:
	Timer m_Cooldown[static_cast<int>(E_TYPE_TOWER::E_TYPE_MAX)];
	int m_iOwnTower[static_cast<int>(E_TYPE_TOWER::E_TYPE_MAX)];
	int m_iTowerPrice[static_cast<int>(E_TYPE_TOWER::E_TYPE_MAX)];
private:
	POINT m_MousePt = {};
	PlayerCursor* m_pCursor;
	float m_fAccX = 0.f;
	float m_fAccY = 0.f;
	const float AccX = 1.f;
	const float AccY = 1.f;
	float m_fCursorSpeed = 0.f;
private:
	int m_iMoney = 0;
	E_TYPE_TOWER m_eCurTowerType = E_TYPE_TOWER::E_TYPE_MAX;
};

#endif//!__PLAYER_H__