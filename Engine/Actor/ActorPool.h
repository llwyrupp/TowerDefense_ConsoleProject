#pragma once
#ifndef __ACTORPOOL_H__
#define __ACTORPOOL_H__

#include "EngineCommon/Engine_Defines.h"
#include "EngineCommon/RTTI.h"

namespace System {

	class Actor;
	class ENGINE_DLL ActorPool : public RTTI
	{
		RTTI_DECLARATIONS(ActorPool, RTTI)
	public:
		ActorPool();
		~ActorPool();
	public:
		void Push(Actor* _pActor);
		Actor* Pop(size_t _szTypeID);
	private:
		unordered_map<size_t,queue<Actor*>> m_mapPool;
	};
}


#endif // !__ACTORPOOL_H__