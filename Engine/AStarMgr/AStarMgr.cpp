#include "AStarMgr/AStarMgr.h"

BEGIN(System)

AStarMgr* AStarMgr::m_pInstance = nullptr;


AStarMgr::AStarMgr()
{
	m_vecDir.clear();
	m_vecDir.reserve(8);
	//이동순서 정하기 (하상우좌)
	//1. 아래방향은 0, 1이고 비용이 1이다.
	m_vecDir.emplace_back(DIR(0, 1, 1));
	//2. 위방향은 0,-1 이고 비용이 1이다.
	m_vecDir.emplace_back(DIR(0, -1, 1));
	//3. 오른쪽방향은 1, 0, 비용이 1이다.
	m_vecDir.emplace_back(DIR(1, 0, 1));
	//4. 왼쪽방향은 -1, 0, 비용이 1이다.
	m_vecDir.emplace_back(DIR(-1, 0, 1));

	//DIAGONAL
	//5. 대각선방향은 1, 1, 비용이 1.414(sqrt2)이다.
	m_vecDir.emplace_back(DIR(1, 1, 1.414));
	// . 대각선방향은 1, -1, 비용이 1.414(sqrt2)이다.
	m_vecDir.emplace_back(DIR(1, -1, 1.414));
	// . 대각선방향은 -1, 1, 비용이 1.414(sqrt2)이다.
	m_vecDir.emplace_back(DIR(-1, 1, 1.414));
	// . 대각선방향은 -1, -1, 비용이 1.414(sqrt2)이다.
	m_vecDir.emplace_back(DIR(-1, -1, 1.414));
}

AStarMgr::~AStarMgr()
{
}

void AStarMgr::FindPath(Node* _start, Node* _target)
{
	if (_start == nullptr || _target == nullptr)
		return;

	//implement A-Star pathfinding algorithm

	m_OpenList.push(_start);

	while (!m_OpenList.empty())
	{
		Node* currentNode = m_OpenList.top();
		m_OpenList.pop();

		if (currentNode == _target)
			return;

		for (size_t i = 0; i < m_vecDir.size(); ++i)
		{
			int nextX = currentNode->GetPos().iX;
			int nextY = currentNode->GetPos().iY;

			float nextGCost = currentNode->GetGCost() + 1;
			//if visitedcost.find == visitedcost.end(), meaning next node has not visited yet
			//get heuristic, push into openlist.
		}
	}
}

vector<Node*> AStarMgr::ConstructPath(Node* _target)
{
	vector<Node*> newPath;
	//backtracking via parent node.
	Node* curNode = _target;
	while (curNode)
	{
		newPath.emplace_back(curNode);
		curNode = curNode->GetParent();
	}
	//reverse the newPath vector
	reverse(newPath.begin(), newPath.end());
	return newPath;
}

float AStarMgr::CalculateHeuristic(Node* _current, Node* _target)
{
	POS posDiff = *_current - *_target;
	return static_cast<float>(sqrt( pow(posDiff.iX ,2) + pow(posDiff.iY, 2)));
}

bool AStarMgr::IsInRange(int _x, int _y, const vector<vector<int>>& _grid)
{
	//exception handling
	if (_grid.empty() || _grid[0].empty())
		return false;

	//check x, y outofbounds
	if (_x < 0 || _x >= static_cast<int>(_grid[0].size()) ||
		_y < 0 || _y >= static_cast<int>(_grid.size()))
		return false;

	return true;
}

END

