#pragma once
#ifndef __NODE_H__
#define __NODE_H__

#include "EngineCommon/Engine_Defines.h"
#include "EngineCommon/Engine_Struct.h"
#include "EngineCommon/Engine_Macro.h"

BEGIN(System)
class Node {
public:
	Node(int _col, int _row, float _weight, Node* _parent)
		:m_fWeight(_weight), m_Parent(_parent)
	{
		m_Pos.iCol = _col;
		m_Pos.iRow = _row;
	}
public:
	
public:
	POS operator-(const Node& _other)
	{
		return POS(m_Pos.iCol - _other.m_Pos.iCol, m_Pos.iRow - _other.m_Pos.iRow);
	}
	bool operator==(const Node& _other)
	{
		return m_Pos.iCol == _other.m_Pos.iCol && m_Pos.iRow == _other.m_Pos.iRow;
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
	inline POS GetPos() const { return m_Pos; }
	inline float GetGCost() const { return m_GCost; }
	inline void SetGCost(float _cost) { m_GCost = _cost; }
	inline float GetHCost() const { return m_HCost; }
	inline void SetHCost(float _cost) { m_HCost = _cost; }
	inline float GetFCost() const { return m_FCost; }
	inline void SetFCost(float _cost) { m_FCost = _cost; }

	inline Node* GetParent() const { return m_Parent; }
	inline void SetParent(Node* _parent) { m_Parent = _parent; }
	inline float GetWeight() const { return m_fWeight; }
	inline void SetWeight(float _cost) { m_fWeight = _cost; }
	void ResetNode() {
		m_Parent = nullptr;
		m_Pos = { 0, 0 };
		m_FCost = 0.f;
		m_HCost = 0.f;
		m_GCost = 0.f;
	}

private:
	Node* m_Parent = nullptr;
	POS m_Pos = {};
	float m_FCost = 0.f;//The total estimated cost of the cheapest path from the start node to the goal node, going through the current node
	float m_HCost = 0.f;//Heuristic: The estimated cost of moving from the current node n to the final destination
	float m_GCost = 0.f;//The actual cost (e.g., distance or time) of the path from the starting node to the current node

	float m_fWeight = 0.f;
};
END
#endif // !__NODE_H__
