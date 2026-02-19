#pragma once

#include "EngineCommon/Engine_Defines.h"
#include "EngineCommon/Engine_Macro.h"

BEGIN(System)
class ScrollMgr
{
public:
	ScrollMgr();
	~ScrollMgr();

public:
	//SETTER
	void Add_ScrollX(const float _fScrollX) { m_fScrollX += _fScrollX; }
	void Add_ScrollY(const float _fScrollY) { m_fScrollY += _fScrollY; }

	//GETTER
	inline float Get_ScrollX() const { return m_fScrollX; }
	inline float Get_ScrollY() const { return m_fScrollY; }

	static ScrollMgr& Get_Instance();
private:
	float m_fScrollX = 0.f;
	float m_fScrollY = 0.f;

private:
	static ScrollMgr* m_pInstance;
};


END