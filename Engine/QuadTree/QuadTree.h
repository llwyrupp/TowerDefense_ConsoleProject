#pragma once
#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include "EngineCommon/RTTI.h"
#include "QuadTree/Area.h"
#include "EngineCommon/Engine_Defines.h"


BEGIN(System)
class Actor;
class ENGINE_DLL QuadTree
{
public:
	QuadTree() = default;
	QuadTree(const Quadrant& _quadrant);
	~QuadTree();

	//recursive node structure
	//quadtree is just a rootnode that covers the whole map.
	//each node can be split into 4 smaller quadrants(LT, RT, LB, RB)
	//when i pass an bullet to the quadtree, it looks at the bullet's RECT.
	//it checks which quadrants the bullet overlaps with, and gives you the list
	//of actors stored in those specific quadrants.
public:
	void ResetTree();
	void InsertArea(Area* _Area);
	vector<Area*> Query(Area* _queryArea);
	void Render();
private:
	static const int m_iMaxDepth = 5;
	Area* m_Root;//the root node.
	//vector<Actor*> m_vecActors;//the actors in current quadrant.
};

END

#endif // !__QUADTREE_H__