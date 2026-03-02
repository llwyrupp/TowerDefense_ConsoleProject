#pragma once
#ifndef __NODE_H__
#define __NODE_H__

#include "EngineCommon/Engine_Defines.h"
#include "EngineCommon/Engine_Struct.h"
#include "EngineCommon/Engine_Macro.h"

BEGIN(System)
class Node {
public:
	Node(int _x, int _y, Node* _parent)
		:m_Parent(_parent)
	{
		m_Pos.iX = _x;
		m_Pos.iY = _y;
	}
public:
	
public:
	POS operator-(const Node& _other)
	{
		return POS(m_Pos.iX - _other.m_Pos.iX, m_Pos.iY - _other.m_Pos.iY);
	}
	bool operator==(const Node& _other)
	{
		return m_Pos.iX == _other.m_Pos.iX && m_Pos.iY == _other.m_Pos.iY;
	}
	bool operator>(const Node& _other)
	{
		return GetFCost() > _other.GetFCost();
	}
	bool operator<(const Node& _other)
	{
		return GetFCost() < _other.GetFCost();
	}
public:
	Node* GetParent() const { return m_Parent; }
	POS GetPos() const { return m_Pos; }
	inline float GetGCost() const { return m_GCost; }
	inline void SetGCost(float _cost) { m_GCost = _cost; }
	inline float GetHCost() const { return m_HCost; }
	inline void SetHCost(float _cost) { m_HCost = _cost; }
	inline float GetFCost() const { return m_FCost; }
	inline void SetFCost(float _cost) { m_FCost = _cost; }
private:
	Node* m_Parent = nullptr;
	POS m_Pos = {};
	float m_FCost = 0.f;//The total estimated cost of the cheapest path from the start node to the goal node, going through the current node
	float m_HCost = 0.f;//Heuristic: The estimated cost of moving from the current node n to the final destination
	float m_GCost = 0.f;//The actual cost (e.g., distance or time) of the path from the starting node to the current node
};
END
#endif // !__NODE_H__
