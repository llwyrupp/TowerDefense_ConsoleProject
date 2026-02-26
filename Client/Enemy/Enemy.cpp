#include "Enemy.h"

using namespace System;

Enemy::Enemy()
	:super("@", nullptr, Vector2::Zero, Color::eRed, E_LAYER::E_ENEMY)
{
	m_bIsUsingActorPool = true;
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

	Vector2 vNewPos = GetPos();

	m_fX += _fDeltaTime * 10.f;
	if (m_fX >= 1.f)
	{
		++vNewPos.m_iX;
		m_fX = 0.f;
	}

	m_fY += _fDeltaTime * 10.f;
	if (m_fY >= 1.f)
	{
		++vNewPos.m_iY;
		m_fY = 0.f;
	}

	SetPos(vNewPos);
}

void Enemy::Render()
{
	super::Render();
}
