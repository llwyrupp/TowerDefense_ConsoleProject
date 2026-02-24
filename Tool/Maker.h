#pragma once
#ifndef __MAKER_H__
#define __MAKER_H__
#include "Actor/Actor.h"
using namespace System;

enum class E_TILE_TYPE {
	E_TYPE_NONE = -1,
	E_TYPE_GROUND = 0,
	E_TYPE_ROAD = 1,
	E_TYPE_ERASOR = 2,
	E_TYPE_MAX
};

class Maker : public Actor
{
	RTTI_DECLARATIONS(Maker, Actor)
public:
	Maker();
	virtual ~Maker();

public:
	void LoadMapFile(const char* _pPath);
	void SaveMapFile(const char* _pPath);
public:


	// Inherited via Actor
	void BeginPlay() override;

	void Tick(float _fDeltaTime) override;

	void Render() override;
public:
	

public:
	RECT m_tDragArea = {};
	E_TILE_TYPE m_eCurTileType = E_TILE_TYPE::E_TYPE_NONE;

	
};


#endif // !__MAKER_H__