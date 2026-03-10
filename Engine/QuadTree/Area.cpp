#include "Area.h"
#include "EngineCommon/Engine_Defines.h"

BEGIN(System)
Area::Area(const Quadrant& _quadrants, int _depth)
	:m_MyQuadrant(_quadrants), m_iDepth(_depth)
{
	//m_vecAllAreas.reserve(100);
}

Area::~Area()
{
	Clear();
	m_pActorOwner = nullptr;//never delete owner actor. it is managed in the Level class
}

void Area::Insert(Area* _node)
{
	//recursive Insertion.

	//BASE CASE:
	if (!_node)
		return;

	//insert node.
	//check the overlapping areas.(testregion)
	E_AREA_INDEX eIndex = TestRegion_ReturnIndex(_node->GetMyQuadrant());

	//if the result is straddling, push it into possible points.
	if (eIndex == E_AREA_INDEX::E_STRADDLING)
	{
		m_vecAllAreas.emplace_back(_node);
		return;
	}
	else if (eIndex != E_AREA_INDEX::E_OUTOFBOUNDS)
	{
		//if the result is not straddling on more than 2 regions,
		// subdivide it into the 4 quadrants.(until maxdepth)
		//when the result (from testregion) belongs to only one quadrant

		if (SubDivide())//create 4 child quadrant nodes
		{
			switch (eIndex)
			{
			case E_AREA_INDEX::E_TOP_LEFT:
				m_TopLeft->Insert(_node);
				break;
			case E_AREA_INDEX::E_TOP_RIGHT:
				m_TopRight->Insert(_node);
				break;
			case E_AREA_INDEX::E_BOTTOM_LEFT:
				m_BottomLeft->Insert(_node);
				break;
			case E_AREA_INDEX::E_BOTTOM_RIGHT:
				m_BottomRight->Insert(_node);
				break;
			}

		}
		else {
			//if failed to subdivide, we have reached the end
			//push the node into current quadrant
			m_vecAllAreas.emplace_back(_node);
		}
	}

	//if it is not possible to subdivide,(if reached max depth)
	// //just push it back.

}

void Area::Query(const Quadrant& _quadrants, vector<Area*>& _possibleNodes)
{
	//add current node(this node) to possible nodes.
	_possibleNodes.emplace_back(this);

	//check if divided.
	if (!IsDivided())
		return;

	//get the straddling quadrants, and check all four quadrants
	vector<E_AREA_INDEX> vecIndices = GetCorrespondingQuadrantsIndices(_quadrants);

	//for each quadrants, process query.
	//Query is Recursive. it will continue until we reach max depth
	for (const auto& index : vecIndices)
	{
		if (index == E_AREA_INDEX::E_TOP_LEFT)
		{
			m_TopLeft->Query(_quadrants, _possibleNodes);
		}
		else if (index == E_AREA_INDEX::E_TOP_RIGHT)
		{
			m_TopRight->Query(_quadrants, _possibleNodes);
		}
		else if (index == E_AREA_INDEX::E_BOTTOM_LEFT)
		{
			m_BottomLeft->Query(_quadrants, _possibleNodes);
		}
		else if (index == E_AREA_INDEX::E_BOTTOM_RIGHT)
		{
			m_BottomRight->Query(_quadrants, _possibleNodes);
		}
	}

}
//called when releases memory(end of program)
void Area::Clear() {
	//all the actors are managed by Levels
	//so never delete them here.

	//just clear container
	m_vecAllAreas.clear();

	if (IsDivided())//if current area is divided, meaning there are children areas to be deleted,
	{
		if(m_TopLeft)
			m_TopLeft->Clear();
		if(m_TopRight)
			m_TopRight->Clear();
		if(m_BottomLeft)
			m_BottomLeft->Clear();
		if(m_BottomRight)
			m_BottomRight->Clear();

		DeleteChildren();
	}
}
//called during game loop(reset the whole tree)
void Area::Reset()
{
	//don't delete
	m_vecAllAreas.clear();

	if (IsDivided())//if current area is divided, meaning there are children areas to be deleted,
	{
		if (m_TopLeft)
			m_TopLeft->Clear();
		if (m_TopRight)
			m_TopRight->Clear();
		if (m_BottomLeft)
			m_BottomLeft->Clear();
		if (m_BottomRight)
			m_BottomRight->Clear();
	}
}

bool Area::SubDivide()
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
		m_TopLeft = new Area(Quadrant(iOriginX, iOriginY, iHalfWidth, iHalfHeight), m_iDepth + 1);

		//second quadrant: 0~25, 75~150, depth+1
		m_TopRight = new Area(Quadrant(iOriginX + iHalfWidth, iOriginY, iHalfWidth, iHalfHeight), m_iDepth + 1);

		//third quadrant: 25~50, 0~75, depth+1
		m_BottomLeft = new Area(Quadrant(iOriginX, iOriginY + iHalfHeight, iHalfWidth, iHalfHeight), m_iDepth + 1);

		//fourth quadrant: 25~50, 75~150, depth+1
		m_BottomRight = new Area(Quadrant(iOriginX + iHalfWidth, iOriginY + iHalfHeight, iHalfWidth, iHalfHeight), m_iDepth + 1);
	}
	else {
		return false;
	}

	return true;
}

bool Area::IsDivided()
{
	//if current region is divided, all nodes will not be nullptrs.
	//so we only have to check one of the child nodes.
	return m_TopLeft != nullptr;
}

E_AREA_INDEX Area::TestRegion_ReturnIndex(const Quadrant& _quadrant)
{
	//return which index the parameter _quadrant belongs to.
	vector<E_AREA_INDEX> vecIndices = GetCorrespondingQuadrantsIndices(_quadrant);

	if (vecIndices.empty())
		return E_AREA_INDEX::E_OUTOFBOUNDS;

	else if(vecIndices.size() == 1) {
		return *vecIndices.begin();
	}
	//else if(vecIndices.size() > 1)//if there is more than one index
	return E_AREA_INDEX::E_STRADDLING;
}

vector<E_AREA_INDEX> Area::GetCorrespondingQuadrantsIndices(const Quadrant& _quadrant)
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
	bool bRight = iCenterX <= _quadrant.GetMaxX() && _quadrant.GetOriginX() < m_MyQuadrant.GetMaxX();
	bool bTop = _quadrant.GetOriginY() < iCenterY && iOriginY <= _quadrant.GetMaxY();
	bool bBottom = iCenterY <= _quadrant.GetMaxY() && _quadrant.GetOriginY() < m_MyQuadrant.GetMaxY();

	vector<E_AREA_INDEX> vecIndices;
	//we want to know where the parameter(_quadrant) belongs to, 
	// and return them as a vector with matching indices
	if (bTop && bLeft)
	{
		vecIndices.emplace_back(E_AREA_INDEX::E_TOP_LEFT);
	}
	if (bTop && bRight)
	{
		vecIndices.emplace_back(E_AREA_INDEX::E_TOP_RIGHT);
	}
	if (bBottom && bLeft)
	{
		vecIndices.emplace_back(E_AREA_INDEX::E_BOTTOM_LEFT);
	}
	if (bBottom && bRight)
	{
		vecIndices.emplace_back(E_AREA_INDEX::E_BOTTOM_RIGHT);
	}

	return vecIndices;
}

void Area::DeleteChildren()
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

END