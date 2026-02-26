#pragma once
#ifndef __LEVEL_H__
#define __LEVEL_H__
#include "EngineCommon/Engine_Macro.h"
#include "EngineCommon/Engine_Defines.h"
#include "EngineCommon/Engine_Enum.h"
#include "EngineCommon/RTTI.h"
#include "Actor/Actor.h"
#include "Actor/ActorPool.h"
using namespace std;

BEGIN(System)

class Actor;
class ActorPool;
class ENGINE_DLL Level : public RTTI
{
	RTTI_DECLARATIONS(Level, RTTI)
public:
	Level();
	virtual ~Level();
public:
	template<typename T>
	T* SpawnActor(const Vector2& _vPos) {
		// 1. Try to pull an actor of type T from the pool using its RTTI ID
		Actor* pPooledActor = m_pActorPool->Pop(T::TypeIdClass());
		T* pActor = nullptr;
		// 2. We found one! Reuse it.
		if (pPooledActor != nullptr)
		{
			pActor = pPooledActor->As<T>();
			pActor->Set_IsDestroyRequested(false);//init status
		}
		// 3. Pool was empty, create a brand new one
		else if(pActor == nullptr)
		{
			pActor = new T();//if there is none in the queue, make a new one
			pActor->Set_IsUsingActorPool(true);
			pActor->Set_IsDestroyRequested(false);
		}
		// 4. Set common data and register it to the active level
		pActor->SetPos(_vPos);
		AddNewActor(pActor);

		return pActor;
	}
public:
	//pure virtuals
	virtual void BeginPlay() = 0;
	virtual void Tick(float _fDeltaTime) = 0;
	virtual void Render() = 0;

public:
	void AddNewActor(Actor* pNewActor);
	void Process_AddnDestroyActors();

	inline const std::vector<Actor*>& GetActors() const { return m_vecActors; }
protected:
	vector<Actor*>	m_vecActors;
	vector<Actor*>	m_vecAddReqActors;
	vector<Actor*>	m_vecLayers[static_cast<int>(E_LAYER::E_MAX)];

	ActorPool* m_pActorPool = nullptr;
};

END
#endif