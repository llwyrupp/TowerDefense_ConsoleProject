#pragma once
#ifndef __ASTARMGR_H__
#define __ASTARMGR_H__

#include "AStarMgr/Node.h"

BEGIN(System)

class Node;
class ENGINE_DLL AStarMgr
{
public:
	AStarMgr();
	~AStarMgr();
public:
	vector<Node*> FindPath(Node* _start, Node* _targetNode);
	vector<Node*> ConstructPath(Node* _targetNode);
	float CalculateHeuristic(Node* _current, Node* _targetNode);
	bool IsEqual_GCost (const Node*& _vecA, const Node*& _vecB);
	bool HasVisited(int _x, int _y, float _gCost);
	//validate pos
	bool IsInRange(int _y, int _x);
	inline bool IsTarget(const Node* const _node) const { return *m_Target == *_node; }
	inline void SetMapMaxSize(int _height, int _width) { m_iMapMaxHeight = _height, m_iMapMaxWidth = _width; }

	inline static AStarMgr& Get_Instance() { return *m_pInstance; }
private:
	//min-priority queue
	priority_queue<Node*, vector<Node*>, greater<Node*>> m_OpenList;
	vector<Node*> m_ClosedList;
	Node* m_Start = nullptr;
	Node* m_Target = nullptr;
	vector<DIR> m_vecDir;
private:
	static AStarMgr* m_pInstance;
	unsigned int m_iMapMaxWidth = 0;
	unsigned int m_iMapMaxHeight = 0;
};

END

#endif // !__ASTARMGR_H__