#pragma once
#ifndef __LEVEL_H__
#define __LEVEL_H__
#include "EngineCommon/Engine_Macro.h"
#include "EngineCommon/Engine_Defines.h"
#include "EngineCommon/RTTI.h"

BEGIN(System)

class Actor;
class ENGINE_DLL Level : public RTTI
{
	RTTI_DECLARATIONS(Level, RTTI)
public:
	Level();
	virtual ~Level();

public:
	//pure virtuals
	virtual void BeginPlay() = 0;
	virtual void Tick(float _fDeltaTime) = 0;
	virtual void Render() = 0;

public:
	void AddNewActor(Actor* pNewActor);
	void Process_AddNDestroyActors();

	inline const std::vector<Actor*>& GetActors() const { return m_vecActors; }
protected:
	std::vector<Actor*> m_vecActors;
	std::vector<Actor*> m_vecAddReqActors;
};

END
#endif