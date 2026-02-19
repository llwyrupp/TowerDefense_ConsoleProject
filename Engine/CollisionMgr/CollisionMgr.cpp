#include "CollisionMgr.h"
#include "Actor/Actor.h"

BEGIN(System)

CollisionMgr* CollisionMgr::m_pInstance = nullptr;

CollisionMgr::CollisionMgr()
{
	m_pInstance = this;
}

CollisionMgr::~CollisionMgr()
{
}

CollisionMgr& CollisionMgr::Get_Instance()
{
	if (!m_pInstance) {
		std::cerr << "CollisionMgr INSTANCE is NULL";
		__debugbreak();
	}

	return *m_pInstance;
}

Actor* CollisionMgr::CheckCol_Player_Enemy(std::list<Actor*>& _Dst, std::list<Actor*>& _Src)//dst: player, src: enemy
{
	for (auto& dst : _Dst) {
		for (auto& src : _Src) {
			if (Check_Intersect(dst->GetRect(), src->GetRect())) 
			{
				return src;
			}
		}
	}
	return nullptr;
}

bool CollisionMgr::Check_Intersect(const RECT& _rtDst, const RECT& _rtSrc)
{
	int iMinX_Dst = _rtDst.left, iMaxX_Dst = _rtDst.right,
		iMinY_Dst = _rtDst.top, iMaxY_Dst = _rtDst.bottom,
		iMinX_Src = _rtSrc.left, iMaxX_Src = _rtSrc.right,
		iMinY_Src = _rtSrc.top, iMaxY_Src = _rtSrc.bottom;

	//check if two intersects.

	if (iMaxX_Dst <= iMinX_Src)
		return false;

	if (iMaxX_Src <= iMinX_Dst)
		return false;

	if (iMaxY_Dst <= iMinY_Src)
		return false;

	if (iMaxY_Src <= iMinY_Dst)
		return false;

	return true;
}

bool CollisionMgr::CheckCol_Player_Target(std::list<Actor*>& _Dst, std::list<Actor*>& _Src)
{
	for (auto& dst : _Dst) {
		for (auto& src : _Src) {
			if (Check_Intersect(dst->GetRect(), src->GetRect()))
			{
				return true;
			}
		}
	}
	return false;
}

END