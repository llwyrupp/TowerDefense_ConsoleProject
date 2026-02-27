#pragma once
#ifndef __ASTARMGR_H__
#define __ASTARMGR_H__

#include "EngineCommon/Engine_Defines.h"
#include "EngineCommon/Engine_Macro.h"
#include "AStarMgr/Node.h"

BEGIN(System)

class ENGINE_DLL AStarMgr
{
public:
	AStarMgr();
	~AStarMgr();
public:
private:
	void FindPath(Node* _start, Node* _target);
	vector<Node*> ConstructPath(Node* _target);
	float CalculateHeuristic(Node* _current, Node* _target);

	//validate pos
	bool IsInRange(int _x, int _y, const vector<vector<int>>& _grid);
	inline bool IsTarget(const Node* const _node) const { return *m_Target == *_node; }
private:
	//min-priority queue
	priority_queue<Node*, vector<Node*>, greater<Node*>> m_OpenList;
	vector<Node*> m_ClosedList;
	Node* m_Start = nullptr;
	Node* m_Target = nullptr;
	vector<DIR> m_vecDir;
private:
	static AStarMgr* m_pInstance;
};

END

#endif // !__ASTARMGR_H__