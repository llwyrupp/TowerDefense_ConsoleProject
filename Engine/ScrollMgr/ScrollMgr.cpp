#include "ScrollMgr.h"

BEGIN(System)

ScrollMgr* ScrollMgr::m_pInstance = nullptr;

ScrollMgr::ScrollMgr()
	:m_fScrollX(0.f), m_fScrollY(0.f)
{
	m_pInstance = this;
}

ScrollMgr::~ScrollMgr()
{
}

ScrollMgr& ScrollMgr::Get_Instance()
{
	if (!m_pInstance) {
		cerr << "ScrollMgr INSTANCE is NULL";
		__debugbreak();
	}

	return *m_pInstance;
}

END