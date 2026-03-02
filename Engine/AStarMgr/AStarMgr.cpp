#include "AStarMgr/AStarMgr.h"
#include "AStarMgr/Node.h"
#include "EngineCommon/Engine_Function.h"

BEGIN(System)

AStarMgr* AStarMgr::m_pInstance = nullptr;

const unsigned int MaxDir = 8;
const float MaxGCost = 999999.f;

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

	////DIAGONAL
	////5. 대각선방향은 1, 1, 비용이 1.414(sqrt2)이다.
	//m_vecDir.emplace_back(DIR(1, 1, 1.414f));
	//// . 대각선방향은 1, -1, 비용이 1.414(sqrt2)이다.
	//m_vecDir.emplace_back(DIR(1, -1, 1.414f));
	//// . 대각선방향은 -1, 1, 비용이 1.414(sqrt2)이다.
	//m_vecDir.emplace_back(DIR(-1, 1, 1.414f));
	//// . 대각선방향은 -1, -1, 비용이 1.414(sqrt2)이다.
	//m_vecDir.emplace_back(DIR(-1, -1, 1.414f));


}

AStarMgr::~AStarMgr()
{
}

vector<Node*> AStarMgr::FindPath(Node* _start, Node* _targetNode)
{
	if (_start == nullptr || _targetNode == nullptr)
		return {};

	vector<Node*> allNodes;

	m_ClosedList.clear();

	//implement A-Star pathfinding algorithm

	m_OpenList.push(_start);

	//height: row(50), X, width: col(100), Y
	vector < vector<float>> vecBestGCost(m_iMapMaxHeight, vector<float>(m_iMapMaxWidth, MaxGCost));

	while (!m_OpenList.empty())
	{
		Node* currentNode = m_OpenList.top();
		m_OpenList.pop();

		//if current node is the target node, 
		if (*currentNode == *_targetNode) {

			for (auto& const node : allNodes)
			{
				//Safe_Delete(node);
			}
			return ConstructPath(currentNode);
		}

		m_ClosedList.emplace_back(currentNode);

		for (int i = 0; i < m_vecDir.size(); ++i)
		{
			int nextX = currentNode->GetPos().iX + m_vecDir[i].iX;
			int nextY = currentNode->GetPos().iY + m_vecDir[i].iY;

			float nextGCost = currentNode->GetGCost() + m_vecDir[i].gCost;//add the cost of the next direction.
			//if next direction's gcost is smaller, we have found a shorter path to target.
			if (IsInRange(nextY, nextX) && nextGCost < vecBestGCost[nextY][nextX])//[50][100]
			{
				//update the new gcost to next coord
				vecBestGCost[nextY][nextX] = nextGCost;
				Node* pNextNode = new Node(nextX, nextY, currentNode);
				float fHeuristicValue = CalculateHeuristic(pNextNode, _targetNode);
				pNextNode->SetGCost(nextGCost);
				pNextNode->SetHCost(fHeuristicValue);
				pNextNode->SetFCost(nextGCost + fHeuristicValue);
				m_OpenList.push(pNextNode);

				allNodes.emplace_back(pNextNode);
			}
		}
	}
	return {};
}

vector<Node*> AStarMgr::ConstructPath(Node* _targetNode)
{
	vector<Node*> newPath;
	//backtracking via parent node.
	Node* curNode = _targetNode;
	while (curNode)
	{
		newPath.emplace_back(curNode);
		curNode = curNode->GetParent();
	}
	//reverse the newPath vector
	reverse(newPath.begin(), newPath.end());
	return newPath;
}

float AStarMgr::CalculateHeuristic(Node* _current, Node* _targetNode)
{
	POS posDiff = *_current - *_targetNode;
	return static_cast<float>(sqrt(pow(posDiff.iX, 2) + pow(posDiff.iY, 2)));
}

bool AStarMgr::IsInRange(int _y, int _x)//height, width
{
	//exception handling
	if (m_iMapMaxWidth == 0 || m_iMapMaxHeight == 0)
		return false;

	//check x, y outofbounds
	if (_x <= 0 || _x >= m_iMapMaxWidth||
		_y <= 0 || _y >= m_iMapMaxHeight)
		return false;

	return true;
}

bool AStarMgr::HasVisited(int _x, int _y, float _gCost)
{
	auto it = find_if(m_ClosedList.begin(), m_ClosedList.end(), [&](const Node* _node)
		{
			return _x == _node->GetPos().iX && _y == _node->GetPos().iY &&
				_gCost >= _node->GetGCost();
		});

	if (it != m_ClosedList.end())
	{
		return true;
	}
	return false;
}

bool AStarMgr::IsEqual_GCost(const Node*& _vecA, const Node*& _vecB)
{
	return _vecA->GetGCost() == _vecB->GetGCost();
}

END

