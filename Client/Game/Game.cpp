#include "Game.h"
#include "Player/Player.h"
#include "EngineCommon/Engine_Function.h"
#include "ClientCommon/ClientEnum.h"
#include "Level/FieldLevel.h"

using namespace System;
Game* Game::m_pInstance = nullptr;

Game::Game()
{
	m_pInstance = this;

	m_vecLevels.clear();
	m_vecLevels.reserve((int)E_TYPE_LEVEL::E_TYPE_MAX);

	m_vecLevels.emplace_back(new FieldLevel());

	m_pMainLevel = *m_vecLevels.begin();
}

Game::~Game()
{
	Safe_Delete(m_pPlayer);
	for (auto& level : m_vecLevels)
	{
		if(level != m_pMainLevel)//m_pMainLevel del in ~Engine()
			Safe_Delete(level);
	}
}
