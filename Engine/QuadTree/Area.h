#pragma once
#ifndef __AREA_H__
#define __AREA_H__

#include "EngineCommon/Engine_Function.h"
#include "QuadTree/Quadrant.h"

BEGIN(System)

//4 Quadrants, along with 
enum class E_AREA_INDEX {
	E_TOP_LEFT = 0,
	E_TOP_RIGHT = 1,
	E_BOTTOM_LEFT = 2,
	E_BOTTOM_RIGHT = 3,
	E_STRADDLING = 4,
	E_OUTOFBOUNDS = 5,
	E_MAX
};

//the Quad TRee consists of numerous Areas.
//each Area has four quadrant nodes as members.
//also each node has Quadrant as a member(the boundaries of this area)

class Actor;
class Area
{
public:
	Area(const Quadrant& _quadrants, int _depth = 0);
	~Area();
public:
	void Insert(Area* _node);
	void Query(const Quadrant& _quadrants, vector<Area*>& _possibleNodes);
	void Clear();
	void Reset();
	//divide into 4 quadrants
	bool SubDivide();
	//is area divided?
	bool IsDivided();
	E_AREA_INDEX TestRegion_ReturnIndex(const Quadrant& _quadrant);
	vector<E_AREA_INDEX> GetCorrespondingQuadrantsIndices(const Quadrant& _quadrant);
	void DeleteChildren();
public:
	inline const Quadrant& GetMyQuadrant() const { return m_MyQuadrant; }
	inline const vector<Area*>& GetAllAreas() const { return m_vecAllAreas; }
	inline Area* GetTopLeft() const { return m_TopLeft; }
	inline Area* GetTopRight() const { return m_TopRight; }
	inline Area* GetBottomLeft() const { return m_BottomLeft; }
	inline Area* GetBottomRight() const { return m_BottomRight; }
	inline void SetMyQuadrantPos(int _x, int _y) { m_MyQuadrant.SetPos(_x, _y); }
	inline void SetMyQuadrantWidthHeight(int _width, int _height) { m_MyQuadrant.SetWidthHeight(_width, _height); }
	inline Actor* GetActorOwner() const { return m_pActorOwner; }
	inline void SetActorOwner(Actor* _owner) { m_pActorOwner = _owner; }
private:
	int m_iDepth = 0;
	//which quadrant does it belong to?
	Quadrant m_MyQuadrant;

	//the list of nodes in current area
	vector<Area*> m_vecAllAreas;
private:
	Actor* m_pActorOwner = nullptr;

	Area* m_TopLeft = nullptr;
	Area* m_TopRight = nullptr;
	Area* m_BottomLeft = nullptr;
	Area* m_BottomRight = nullptr;
};



END
#endif // !__AREA_H__