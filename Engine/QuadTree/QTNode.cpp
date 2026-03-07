#include "QTNode.h"
#include "EngineCommon/Engine_Defines.h"
using namespace System;

QTNode::QTNode(const Quadrant& _quadrants, int _depth)
	:m_MyQuadrant(_quadrants), m_iDepth(_depth)
{

}

QTNode::~QTNode()
{
	Clear();
}

void QTNode::Insert(QTNode* _node)
{
	//recursive Insertion.

	//BASE CASE:
	if (!_node)
		return;

	//insert node.
	//check the overlapping areas.(testregion)
	E_NODE_INDEX eIndex = TestRegion_ReturnIndex(_node->GetMyQuadrant());

	//if the result is straddling, push it into possible points.
	if (eIndex == E_NODE_INDEX::E_STRADDLING)
	{
		m_vecAllNodes.emplace_back(_node);
		return;
	}
	else if (eIndex != E_NODE_INDEX::E_OUTOFBOUNDS)
	{
		//if the result is not straddling on more than 2 regions,
		// subdivide it into the 4 quadrants.(until maxdepth)
		//when the result (from testregion) belongs to only one quadrant

		if (SubDivide())//create 4 child quadrant nodes
		{
			switch (eIndex)
			{
			case E_NODE_INDEX::E_TOP_LEFT:
				m_TopLeft->Insert(_node);
				break;
			case E_NODE_INDEX::E_TOP_RIGHT:
				m_TopRight->Insert(_node);
				break;
			case E_NODE_INDEX::E_BOTTOM_LEFT:
				m_BottomLeft->Insert(_node);
				break;
			case E_NODE_INDEX::E_BOTTOM_RIGHT:
				m_BottomRight->Insert(_node);
				break;
			}

		}
		else {
			//if failed to subdivide, we have reached the end
			//push the node into current quadrant
			m_vecAllNodes.emplace_back(_node);
		}
	}

	//if it is not possible to subdivide,(if reached max depth)
	// //just push it back.

}

void QTNode::Query(const Quadrant& _quadrants, vector<QTNode*>& _possibleNodes)
{
	//add current node(this node) to possible nodes.
	_possibleNodes.emplace_back(this);

	//check if divided.
	if (!IsDivided())
		return;

	//get the straddling quadrants, and check all four quadrants
	vector<E_NODE_INDEX> vecIndices = GetCorrespondingQuadrantsIndices(_quadrants);

	//for each quadrants, process query.
	//Query is Recursive. it will continue until we reach max depth
	for (const auto& index : vecIndices)
	{
		if (index == E_NODE_INDEX::E_TOP_LEFT)
		{
			m_TopLeft->Query(_quadrants, _possibleNodes);
		}
		else if (index == E_NODE_INDEX::E_TOP_RIGHT)
		{
			m_TopRight->Query(_quadrants, _possibleNodes);
		}
		else if (index == E_NODE_INDEX::E_BOTTOM_LEFT)
		{
			m_BottomLeft->Query(_quadrants, _possibleNodes);
		}
		else if (index == E_NODE_INDEX::E_BOTTOM_RIGHT)
		{
			m_BottomRight->Query(_quadrants, _possibleNodes);
		}
	}

}

bool QTNode::SubDivide()
{
	//try dividing current quadrant into four regions.

	//first, check if we hit the maximum depth
	if (m_iDepth == MAX_QUADTREE_DEPTH)
		return false;

	//check if we can divide.
	//if it is before division, we can divide right away
	if (!IsDivided())
	{
		int iOriginX = m_MyQuadrant.GetOriginX();
		int iOriginY = m_MyQuadrant.GetOriginY();
		int iHalfWidth = m_MyQuadrant.GetWidth() / 2;
		int iHalfHeight = m_MyQuadrant.GetHeight() / 2;

		//for each 4 quadrants, create child nodes.

		//ex. map height = 50, map width = 150
		//first quadrant: 0~25, 0~75, depth+1
		m_TopLeft = new QTNode(Quadrant(iOriginX, iOriginY, iHalfWidth, iHalfHeight), m_iDepth + 1);

		//second quadrant: 0~25, 75~150, depth+1
		m_TopRight = new QTNode(Quadrant(iOriginX + iHalfWidth, iOriginY, iHalfWidth, iHalfHeight), m_iDepth + 1);

		//third quadrant: 25~50, 0~75, depth+1
		m_BottomLeft = new QTNode(Quadrant(iOriginX, iOriginY + iHalfHeight, iHalfWidth, iHalfHeight), m_iDepth + 1);

		//fourth quadrant: 25~50, 75~150, depth+1
		m_BottomRight = new QTNode(Quadrant(iOriginX + iHalfWidth, iOriginY + iHalfHeight, iHalfWidth, iHalfHeight), m_iDepth + 1);
	}
	else {
		return false;
	}

	return true;
}

bool QTNode::IsDivided()
{
	//if current region is divided, all nodes will not be nullptrs.
	//so we only have to check one of the child nodes.
	return m_TopLeft != nullptr;
}

E_NODE_INDEX QTNode::TestRegion_ReturnIndex(const Quadrant& _quadrant)
{
	//return which index the parameter _quadrant belongs to.
	vector<E_NODE_INDEX> vecIndices = GetCorrespondingQuadrantsIndices(_quadrant);

	if (!vecIndices.empty())
		return E_NODE_INDEX::E_OUTOFBOUNDS;

	else if(vecIndices.size() == 1) {
		return *vecIndices.begin();
	}
	//else if(vecIndices.size() > 1)
	return E_NODE_INDEX::E_STRADDLING;
}

vector<E_NODE_INDEX> QTNode::GetCorrespondingQuadrantsIndices(const Quadrant& _quadrant)
{
	//return the straddling quadrants.

	int iOriginX = m_MyQuadrant.GetOriginX();
	int iOriginY = m_MyQuadrant.GetOriginY();
	int iHalfWidth = m_MyQuadrant.GetWidth() / 2;
	int iHalfHeight = m_MyQuadrant.GetHeight() / 2;
	int iCenterX = iOriginX + iHalfWidth;
	int iCenterY = iOriginY + iHalfHeight;

	//ex) height = 50, width = 150, origin (0,0), center: (25, 75)
	//ex) parameter _quadrant's bounds: origin(20, 100), width = 1, height = 1
	//check if the parameter quadrant staddles with the left area.
	
	//left: is the parameter's origin x smaller than my quadrant's center x?
	//bLeft = 100 < 25 && 0 <= 101  X
	//bRight = 75 <= 100 && 101 < 150 V
	//bTop = 20 < 25 && 21 >= 0 V
	//bBottom = 25 <= 20 && 20 < 50 X
	bool bLeft = _quadrant.GetOriginX() < iCenterX && iOriginX <= _quadrant.GetMaxX();
	bool bRight = iCenterX <= _quadrant.GetOriginX() && _quadrant.GetOriginX() < m_MyQuadrant.GetMaxX();
	bool bTop = _quadrant.GetOriginY() < iCenterY && _quadrant.GetMaxY() >= iOriginY;
	bool bBottom = iCenterY <= _quadrant.GetOriginY() && _quadrant.GetOriginY() < m_MyQuadrant.GetMaxY();

	vector<E_NODE_INDEX> vecIndices;
	//we want to know where the parameter(_quadrant) belongs to, 
	// and return them as a vector with matching indices
	if (bTop && bLeft)
	{
		vecIndices.emplace_back(E_NODE_INDEX::E_TOP_LEFT);
	}
	else if (bTop && bRight)
	{
		vecIndices.emplace_back(E_NODE_INDEX::E_TOP_RIGHT);
	}
	else if (bBottom && bLeft)
	{
		vecIndices.emplace_back(E_NODE_INDEX::E_BOTTOM_LEFT);
	}
	else if (bBottom && bRight)
	{
		vecIndices.emplace_back(E_NODE_INDEX::E_BOTTOM_RIGHT);
	}

	return vecIndices;
}

void QTNode::ClearChildren()
{
	//if divided, delete all the 4 quadrant nodes.
	if (IsDivided())
	{
		Safe_Delete(m_TopLeft);
		Safe_Delete(m_TopRight);
		Safe_Delete(m_BottomLeft);
		Safe_Delete(m_BottomRight);
	}
}
