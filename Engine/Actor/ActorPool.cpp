#include "ActorPool.h"
#include "Actor/Actor.h"
#include "EngineCommon/Engine_Function.h"

BEGIN(System)

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

void ActorPool::Push(Actor* _pActor)//retrieve actor and push into actorpool
{
	if (_pActor)
	{
		size_t szTypeID = _pActor->GetTypeID();
		m_mapPool[szTypeID].push(_pActor);
		_pActor->Set_HasBegunPlay(false);
	}
}

Actor* ActorPool::Pop(size_t _szTypeID)
//pop an actor with an actor typeid(_szTypeID) from the actorpool. 
//if the actorpool consisting of actor with typeid is empty, create a new actor and return it.
//
{
	unordered_map<size_t, queue<Actor*>>::iterator it = m_mapPool.find(_szTypeID);
	//found a queue with the corresponding typeid, and also is not empty
	if (it != m_mapPool.end() && !it->second.empty())
	{
		Actor* pActor = it->second.front();
		it->second.pop();
		return pActor;
	}
	//failed to find the matching typeid
	//implemented in Level.h
	/*else if (it == m_mapPool.end())
	{
	}*/
	return nullptr;
}

size_t ActorPool::GetPoolSize(size_t _szID) const
{
	//unordered_map<size_t, queue<Actor*>>::iterator it = m_mapPool.find(_szID);
	auto it = m_mapPool.find(_szID);
	size_t szSize = 0;
	if (it != m_mapPool.end())//found corresponding pool with the parameter key(_szID)
	{
		szSize = it->second.size();
	}
	return szSize;
}

END