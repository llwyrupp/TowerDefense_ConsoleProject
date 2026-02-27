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
	bool Check_EnoughMoney_TowerCooldown();
private:
	Timer m_Cooldown[3];
	unsigned int m_iOwnTower[3];
	unsigned int m_iTowerPrice[3];
private:
	POINT m_MousePt = {};
	PlayerCursor* m_pCursor;
private:
	int m_iMoney = 0;
	E_TYPE_TOWER m_eCurTowerType = E_TYPE_TOWER::E_TYPE_MAX;
};

#endif//!__PLAYER_H__