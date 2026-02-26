#include "Level/Level.h"
#include "Actor/Actor.h"
#include "CollisionMgr/CollisionMgr.h"
#include "EngineCommon/Engine_Function.h"
#include "Actor/ActorPool.h"

BEGIN(System)

Level::Level()
{
	m_pActorPool = new ActorPool();
}

Level::~Level()
{
	for (auto& actor : m_vecActors)
		Safe_Delete(actor);

	m_vecActors.clear();

	Safe_Delete(m_pActorPool);
}
void Level::BeginPlay()
{
	for (auto& actor : m_vecActors)
	{
		//skip any actor that called beginplay();
		if (actor->Get_HasBegunPlay())
		{
			continue;//skip
		}
		actor->BeginPlay();
	}
}
void Level::Tick(float _fDeltaTime)
{
	for (auto& actor : m_vecActors) {
		if (actor)
			actor->Tick(_fDeltaTime);
	}
}

void Level::Render()
{
	for (auto& actor : m_vecActors) {
		if (actor->IsActive())
		{
			actor->Render();
		}
		else
			continue;
	}
}

void Level::AddNewActor(Actor* pNewActor)
{
	//store actor for latter.
	m_vecAddReqActors.emplace_back(pNewActor);

	//set ownership.
	pNewActor->SetLevel(this);
}

void Level::Process_AddnDestroyActors()
{
	//delete any delete-requested actors.
	for (int i = 0; i < static_cast<int>(m_vecActors.size());)
	{
		Actor* pActor = m_vecActors[i];
		if (m_vecActors[i]->Get_IsDestroyRequested()) {
			//detelte from layer.
			int iLayer = static_cast<int>(pActor->GetLayer());
			vector<Actor*>& vecLayer = m_vecLayers[iLayer];

			vector<Actor*>::iterator it = find(vecLayer.begin(), vecLayer.end(), pActor);
			if (it != vecLayer.end())
			{
				vecLayer.erase(it);
			}

			//if requested to be del, delete
			//if the actor is using actorpool, do not delete it.
			if (pActor->Get_IsUsingActorPool())
			{
				//TODO: retrieve actor to actorpool
				m_pActorPool->Push(m_vecActors[i]);
			}
			else//if the actor's class does not use object pooling, just delete it
			{
				delete m_vecActors[i];
			}

			m_vecActors.erase(m_vecActors.begin() + i);
			continue;//skip to next iteration.
		}
		++i;
	}

	// Additional process: Moving actors from "Waiting Room" to "Active World"
	if (m_vecAddReqActors.empty())
		return;

	for (Actor* const actor : m_vecAddReqActors) {
		m_vecActors.emplace_back(actor);
		int iLayer = static_cast<int>(actor->GetLayer());
		m_vecLayers[iLayer].emplace_back(actor);
	}

	//init arr.
	m_vecAddReqActors.clear();
}

END