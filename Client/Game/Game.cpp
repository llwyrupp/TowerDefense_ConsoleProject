#include "Game.h"
#include "Player/Player.h"
#include "EngineCommon/Engine_Function.h"


using namespace System;
Game* Game::m_pInstance = nullptr;

Game::Game()
{
	m_pInstance = this;

	m_pPlayer = new Player;
	
}

Game::~Game()
{
	Safe_Delete(m_pPlayer);
}
