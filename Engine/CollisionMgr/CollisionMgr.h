#pragma once
#include "EngineCommon/Engine_Macro.h"
#include "EngineCommon/Engine_Defines.h"

BEGIN(System)

class Actor;
	//manage collisions between two actors(base class of all objects)
	class ENGINE_DLL CollisionMgr
	{
		//will use SINGLETON.

	public:
		CollisionMgr();
		~CollisionMgr();

	public:
		static CollisionMgr& Get_Instance();
		Actor* CheckCol_Player_Enemy(std::list<Actor*>& _Dst, std::list<Actor*>& _Src);
		bool CheckCol_Player_Target(std::list<Actor*>& _Dst, std::list<Actor*>& _Src);
		bool Check_Intersect(const RECT& _rtDst, const RECT& _rtSrc);
	private:
		static CollisionMgr* m_pInstance;
	};
END