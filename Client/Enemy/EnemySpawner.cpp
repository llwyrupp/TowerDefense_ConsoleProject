#include "EnemySpawner.h"
#include "Enemy/Enemy.h"

EnemySpawner::EnemySpawner(const int _num)
{
	m_iMaxSpawnEnemyNum = _num;
}

void EnemySpawner::BeginPlay()
{
	super::BeginPlay();
}

void EnemySpawner::Tick(float _fDeltaTime)
{
	super::Tick(_fDeltaTime);
}

void EnemySpawner::Render()
{
	super::Render();
}
