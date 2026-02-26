#pragma once
#ifndef __ENEMYSPAWNER_H__
#define __ENEMYSPAWNER_H__

#include"Actor/Actor.h"
using namespace System;

const int cMaxSpawnEnemyNum = 30;

class Enemy;
class EnemySpawner : public Actor
{
	RTTI_DECLARATIONS(EnemySpawner, Actor)
public:
	EnemySpawner(const int _num);

public:
	// Inherited via Actor
	void BeginPlay() override;
	void Tick(float _fDeltaTime) override;
	void Render() override;
private:
	//make a spawning pool(object pooling)
	int m_iMaxSpawnEnemyNum = 0;
};


#endif // !__ENEMYSPAWNER_H__