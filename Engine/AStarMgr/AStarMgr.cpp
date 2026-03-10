#include "AStarMgr/AStarMgr.h"
#include "AStarMgr/Node.h"
#include "EngineCommon/Engine_Function.h"
#include "EngineCommon/Engine_Defines.h"
#include "Util/Util.h"

BEGIN(System)

AStarMgr* AStarMgr::m_pInstance = nullptr;


AStarMgr::AStarMgr()
{
	m_pInstance = this;

	m_vecDir.clear();
	m_vecDir.reserve(MaxDir);
	//이동순서 정하기 (하상우좌)
	//1. 아래방향은 0, 1이고 비용이 1이다.
	m_vecDir.emplace_back(DIR(0, 1, 1.f));
	//2. 위방향은 0,-1 이고 비용이 1이다.
	m_vecDir.emplace_back(DIR(0, -1, 1.f));
	//3. 오른쪽방향은 1, 0, 비용이 1이다.
	m_vecDir.emplace_back(DIR(1, 0, 1.f));
	//4. 왼쪽방향은 -1, 0, 비용이 1이다.
	m_vecDir.emplace_back(DIR(-1, 0, 1.f));

	//DIAGONAL
	//5. 대각선방향은 1, 1, 비용이 1.414(sqrt2)이다.
	m_vecDir.emplace_back(DIR(1, 1, sqrtf(2.f)));
	// . 대각선방향은 1, -1, 비용이 1.414(sqrt2)이다.
	m_vecDir.emplace_back(DIR(1, -1, sqrtf(2.f)));
	// . 대각선방향은 -1, 1, 비용이 1.414(sqrt2)이다.
	m_vecDir.emplace_back(DIR(-1, 1, sqrtf(2.f)));
	// . 대각선방향은 -1, -1, 비용이 1.414(sqrt2)이다.
	m_vecDir.emplace_back(DIR(-1, -1, sqrtf(2.f)));

	m_vecNodes.resize(MaxHeight, vector<Node*>(MaxWidth, nullptr));

	for (int i = 0; i < MaxHeight; ++i)//row
	{
		for (int j = 0; j < MaxWidth; ++j)//col
		{
			m_vecNodes[i][j] = new Node(j, i, 0.f, nullptr);//allocate nodes before the game, manage them using vector container
		}
	}

	m_vecBestGCost.resize(MaxHeight, vector<float>(MaxWidth, MaxGCost));//allocate maxheight*maxwidth memory and initialize them with maxgcost
	m_vecLayerType.resize(MaxHeight, vector<E_LAYER>(MaxWidth, E_LAYER::E_NONE));
}

AStarMgr::~AStarMgr()
{
	for (auto& vec : m_vecNodes)
	{
		for (auto& node : vec)
		{
			Safe_Delete(node);
		}
	}
}

vector<POS> AStarMgr::FindPath(Node* _start, Node* _targetNode)
{
	if (_start == nullptr || _targetNode == nullptr)
		return {};

	ResetAllNodes();
	SetRandomWeight();

	while (!m_OpenList.empty())
	{
		m_OpenList.pop();
	}
	m_ClosedList.clear();
	
	//implement A-Star pathfinding algorithm

	m_OpenList.push(_start);

	//height: row(50), X, width: col(100), Y
	//STORE THE BEST GCOST FOR EACH COORDINATE ON THE MAP.

	while (!m_OpenList.empty())
	{
		Node* currentNode = m_OpenList.top();
		m_OpenList.pop();

		//if current node is the target node, 
		if (*currentNode == *_targetNode) {
			return ConstructPath(currentNode);
		}

		m_ClosedList.emplace_back(currentNode);

		for (int i = 0; i < m_vecDir.size(); ++i)
		{
			//get next coordinate.
			int nextRow = currentNode->GetPos().iRow + m_vecDir[i].iY;
			int nextCol = currentNode->GetPos().iCol + m_vecDir[i].iX;

			float nextGCost = currentNode->GetGCost() + m_vecDir[i].gCost + currentNode->GetWeight();//add the cost of the next direction.
			//if next direction's gcost (nextGCost) is smaller, we have found a shorter path to target.
			if (IsInRange(nextCol, nextRow) && nextGCost < m_vecBestGCost[nextRow][nextCol])//[50][100]
			{
				//so we update the new nextGCost to next coord
				m_vecBestGCost[nextRow][nextCol] = nextGCost;
				Node* pNextNode = m_vecNodes[nextRow][nextCol];
				float fHeuristicValue = CalculateHeuristic(pNextNode, _targetNode);

				pNextNode->SetGCost(nextGCost);
				pNextNode->SetHCost(fHeuristicValue);
				pNextNode->SetFCost(nextGCost + fHeuristicValue);
				pNextNode->SetParent(currentNode);
				m_OpenList.push(pNextNode);
			}
		}
	}
	return {};
}

vector<POS> AStarMgr::ConstructPath(Node* _targetNode)
{
	vector<POS> newPath;
	//backtracking via parent node.
	Node* curNode = _targetNode;
	while (curNode)
	{
		newPath.emplace_back(curNode->GetPos());
		curNode = curNode->GetParent();
	}
	//reverse the newPath vector
	reverse(newPath.begin(), newPath.end());
	return newPath;
}

float AStarMgr::CalculateHeuristic(Node* _current, Node* _targetNode)
{
	POS posDiff = *_current - *_targetNode;
	float fDist = static_cast<float>(sqrt(pow(posDiff.iCol, 2) + pow(posDiff.iRow, 2)));

	// Add a 10% random variation to the "guess"
	//float fNoise = 30.f + (Util::RandomInt(0, 100) % 10) / 100;
	return fDist;
}

bool AStarMgr::IsInRange(int _col, int _row)//width, height
{
	//exception handling
	if (m_iMapMaxWidth == 0 || m_iMapMaxHeight == 0)
		return false;

	//check x, y outofbounds
	if (_col < 0 || _col >= m_iMapMaxWidth||
		_row < 0 || _row >= m_iMapMaxHeight)
		return false;

	//check tile type
	//if next position is not ground or target
	if (m_vecLayerType[_row][_col] != E_LAYER::E_GROUND && 
		m_vecLayerType[_row][_col] != E_LAYER::E_SPAWNPOINT &&
		m_vecLayerType[_row][_col] != E_LAYER::E_TARGET )
		return false;

	return true;
}

void AStarMgr::ResetAllNodes()
{
	/*for (auto& const vec : m_vecNodes)
	{
		for (auto& const node : vec)
		{
			node->ResetNode();
		}
	}*/

	for (auto& vec : m_vecBestGCost)
	{
		for (auto& cost : vec)
		{
			cost = MaxGCost;
		}
	}
}

void AStarMgr::SetRandomWeight()
{
	for (auto& vec : m_vecNodes)
	{
		for (auto& node : vec)
		{
			node->SetWeight(Util::RandomRange(0.f, 10.f));
		}
	}
}

void AStarMgr::SetCurNodeLayerType(int _col, int _row, E_LAYER _layer)
{
	if (!m_vecLayerType.empty() && _layer != E_LAYER::E_NONE && _col < MaxWidth && _col >= 0 && _row < MaxHeight && _row >= 0)
	{
		m_vecLayerType[_row][_col] = _layer;
	}
}

//bool AStarMgr::HasVisited(int _col, int _row, float _gCost)
//{
//	auto it = find_if(m_ClosedList.begin(), m_ClosedList.end(), [&](const Node* _node)
//		{
//			return _col == _node->GetPos().iCol && _row == _node->GetPos().iRow &&
//				_gCost >= _node->GetGCost();
//		});
//
//	if (it != m_ClosedList.end())
//	{
//		return true;
//	}
//	return false;
//}

bool AStarMgr::IsEqual_GCost(const Node*& _vecA, const Node*& _vecB)
{
	return _vecA->GetGCost() == _vecB->GetGCost();
}

END

