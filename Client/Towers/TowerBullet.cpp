#include "TowerBullet.h"
#include "Interface/ICollisionHandler.h"
#include "Level/Level.h"
#include "Enemy/Enemy.h"

using namespace System;

TowerBullet::TowerBullet()
	:super(".", nullptr, Vector2::Zero, Color::eBlue)
{
}

TowerBullet::~TowerBullet()
{

}

void TowerBullet::OnCollisionEnter2D(Actor* _pActor)
{
	if (_pActor->IsTypeOf<Enemy>() && !m_bIsDestroyRequested)
	{
		Destroy();
	}
}

void TowerBullet::BeginPlay()
{
	super::BeginPlay();
}

void TowerBullet::Tick(float _fDeltaTime)
{
	super::Tick(_fDeltaTime);

	static ICollisionHandler* InterfaceCollisionHandler = nullptr;
	
	if (!InterfaceCollisionHandler && m_pLevel)
	{
		InterfaceCollisionHandler = dynamic_cast<ICollisionHandler*>(m_pLevel);
		if (InterfaceCollisionHandler == nullptr)
		{
			__debugbreak();
		}
	}

}

void TowerBullet::Render()
{
	super::Render();
}

