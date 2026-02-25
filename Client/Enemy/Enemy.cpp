#include "Enemy.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::OnCollisionEnter2D(Actor* _pActor)
{
	if (!m_bIsDestroyRequested)
	{
		Destroy();
	}
}

void Enemy::BeginPlay()
{
	super::BeginPlay();
}

void Enemy::Tick(float _fDeltaTime)
{
	super::Tick(_fDeltaTime);
}

void Enemy::Render()
{
	super::Render();
}
