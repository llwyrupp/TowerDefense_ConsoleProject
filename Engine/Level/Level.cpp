#include "Level/Level.h"
#include "Actor/Actor.h"
#include "CollisionMgr/CollisionMgr.h"
#include "EngineCommon/Engine_Function.h"

BEGIN(System)

Level::Level()
{

}

Level::~Level()
{
	for (auto& actor : m_vecActors)
		Safe_Delete(actor);

	m_vecActors.clear();
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

void Level::Process_AddNDestroyActors()
{
	//delete any delete-requested actors.
	for (int i = 0; i < static_cast<int>(m_vecActors.size());)
	{
		if (m_vecActors[i]->Get_IsDestroyRequested()) {
			//if requested to be del, delete
			delete m_vecActors[i];
			m_vecActors.erase(m_vecActors.begin() + i);
			continue;//skip to next iteration.
		}
		++i;
	}

	//additional process.
	if (m_vecAddReqActors.empty())
		return;

	for (Actor* const actor : m_vecAddReqActors) {
		m_vecActors.emplace_back(actor);
	}

	//init arr.
	m_vecAddReqActors.clear();
}

END