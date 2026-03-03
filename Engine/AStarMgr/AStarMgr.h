#pragma once
#ifndef __ASTARMGR_H__
#define __ASTARMGR_H__

#include "AStarMgr/Node.h"
#include "EngineCommon/Engine_Enum.h"

BEGIN(System)

struct CompareFCost
{
	bool operator()(const Node* _nodeA, const Node* _nodeB)
	{
		return _nodeA->GetFCost() > _nodeB->GetFCost();
	}
};

class Node;
class ENGINE_DLL AStarMgr
{
public:
	AStarMgr();
	~AStarMgr();
public:
	vector<POS> FindPath(Node* _start, Node* _targetNode);
	vector<POS> ConstructPath(Node* _targetNode);
	float CalculateHeuristic(Node* _current, Node* _targetNode);
	bool IsEqual_GCost (const Node*& _vecA, const Node*& _vecB);
	bool HasVisited(int _col, int _row, float _gCost);
	//validate pos
	bool IsInRange(int _col, int _row);
	//inline bool IsTarget(const Node* const _node) const { return *m_Target == *_node; }
	inline void SetMapMaxSize(int _height, int _width) { m_iMapMaxHeight = _height, m_iMapMaxWidth = _width; }
	void ClearOpenList();
	void ResetAllNodes();
	inline static AStarMgr& Get_Instance() { return *m_pInstance; }

	void SetCurNodeLayerType(int _col, int _row, E_LAYER _layer);
private:
	
	//min-priority queue
	priority_queue<Node*, vector<Node*>, CompareFCost> m_OpenList;
	vector<Node*> m_ClosedList;
	Node* m_Start = nullptr;
	vector<DIR> m_vecDir;
private:
	static AStarMgr* m_pInstance;
	unsigned int m_iMapMaxWidth = 0;
	unsigned int m_iMapMaxHeight = 0;
private:
	const int MaxHeight = 100;
	const int MaxWidth = 100;
	const unsigned int MaxDir = 8;
	const float MaxGCost = 999999.f;

	vector<vector<float>> m_vecBestGCost;
	vector<vector<E_LAYER>> m_vecLayerType;
	vector < vector<Node*>> m_vecNodes;

};

END

#endif // !__ASTARMGR_H__