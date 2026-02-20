#pragma once
#ifndef __GAME_H__
#define __GAME_H__

#include "Engine/Engine.h"

using namespace System;

class Game : public Engine
{
public:
	Game();
	~Game();
public:
	inline static Game& Get_Instance() { return *m_pInstance; }
private:
	static Game* m_pInstance;
	class Player* m_pPlayer = nullptr;

private:
	vector<Level*> m_vecLevels;
};


#endif