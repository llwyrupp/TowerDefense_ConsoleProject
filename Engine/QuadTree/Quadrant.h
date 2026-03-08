#pragma once
#ifndef __QUADRANT_H__
#define __QUADRANT_H__

#include "EngineCommon/Engine_Macro.h"

BEGIN(System)
//this class represents the rectangle area(each quadrant)
class Quadrant
{
public:
	Quadrant(int _x, int _y, int _width = 1, int _height = 1)
		:m_iOriginX(_x), m_iOriginY(_y), m_iWidth(_width), m_iHeight(_height)
	{}

public:
	inline int GetOriginX() const { return m_iOriginX; }
	inline int GetOriginY() const { return m_iOriginY; }
	inline int GetWidth() const { return m_iWidth; }
	inline int GetHeight() const { return m_iHeight; }
	inline int GetMaxX() const { return m_iOriginX + m_iWidth; }
	inline int GetMaxY() const { return m_iOriginY + m_iHeight; }

public:
	bool CheckIntersect(const Quadrant& _other) const;
private:
	//the origin point of each Quadrant
	//left-top point of a quadrant
	int m_iOriginX = 0;
	int m_iOriginY = 0;
private:
	int m_iWidth = 1;
	int m_iHeight = 1;
};


END
#endif // !__QUADRANT_H__