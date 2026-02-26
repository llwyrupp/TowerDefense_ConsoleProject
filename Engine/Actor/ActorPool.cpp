#include "ActorPool.h"
#include "Actor/Actor.h"
#include "EngineCommon/Engine_Function.h"
using namespace System;

ActorPool::ActorPool()
{
	m_mapPool.reserve(10);
}

ActorPool::~ActorPool()
{
	for (auto& pair : m_mapPool)
	{
		while (!pair.second.empty()) {
			Safe_Delete(pair.second.front());
			pair.second.pop();
		}
	}
	m_mapPool.clear();
}

void ActorPool::Push(Actor* _pActor)
{
	if (_pActor)
	{
		int iTypeID = static_cast<int>(_pActor->GetType());
		m_mapPool[iTypeID].push(_pActor);
	}
}

Actor* ActorPool::Pop(size_t _szTypeID)
{
	unordered_map<size_t, queue<Actor*>>::iterator it = m_mapPool.find(_szTypeID);
	if (it != m_mapPool.end() && !it->second.empty())
	{
		Actor* pActor = it->second.front();
		it->second.pop();
		return pActor;
	}
	return nullptr;
}
