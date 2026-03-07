#pragma once
#ifndef __QTNODE_H__
#define __QTNODE_H__

#include "EngineCommon/Engine_Function.h"
#include "QuadTree/Quadrant.h"

BEGIN(System)

//4 Quadrants, along with 
enum class E_NODE_INDEX {
	E_TOP_LEFT = 0,
	E_TOP_RIGHT = 1,
	E_BOTTOM_LEFT = 2,
	E_BOTTOM_RIGHT = 3,
	E_STRADDLING = 4,
	E_OUTOFBOUNDS = 5,
	E_MAX
};

//the Quad TRee consists of numerous QTNodes.
//each Node has four quadrant nodes as members.
//also each node has Quadrant as a member(the boundaries of this node)
//
class QTNode
{
public:
	QTNode(const Quadrant& _quadrants, int _depth = 0);
	~QTNode();
public:
	void Insert(QTNode* _node);
	void Query(const Quadrant& _quadrants, vector<QTNode*>& _possibleNodes);
	void Clear();
	//divide into 4 quadrants
	bool SubDivide();
	//is area divided?
	bool IsDivided();
	E_NODE_INDEX TestRegion_ReturnIndex(const Quadrant& _quadrant);
	vector<E_NODE_INDEX> GetCorrespondingQuadrantsIndices(const Quadrant& _quadrant);
	void ClearChildren();
public:
	inline const Quadrant& GetMyQuadrant() const { return m_MyQuadrant; }
	inline const vector<QTNode*>& GetAllNodes() const { return m_vecAllNodes; }
	inline QTNode* GetTopLeft() const { return m_TopLeft; }
	inline QTNode* GetTopRight() const { return m_TopRight; }
	inline QTNode* GetBottomLeft() const { return m_BottomLeft; }
	inline QTNode* GetBottomRight() const { return m_BottomRight; }
private:
	int m_iDepth = 0;
	//which quadrant does it belong to?
	Quadrant m_MyQuadrant;

	//the list of nodes in current area
	vector<QTNode*> m_vecAllNodes;
private:
	QTNode* m_TopLeft = nullptr;
	QTNode* m_TopRight = nullptr;
	QTNode* m_BottomLeft = nullptr;
	QTNode* m_BottomRight = nullptr;
};


#endif // !__QTNODE_H__

END