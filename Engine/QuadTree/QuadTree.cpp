#include "QuadTree.h"

BEGIN(System)
QuadTree::QuadTree(const Quadrant& _quadrant)
{
    m_Root = new Area(_quadrant);//0, 0, 150, 50
}

QuadTree::~QuadTree()
{
    Safe_Delete(m_Root);
}

void QuadTree::ResetTree()
{
    if (m_Root != nullptr)
    {
        m_Root->Reset();
    }
}

void QuadTree::InsertArea(Area* _Area)
{
    if (!m_Root)
    {
        return;
    }
    m_Root->Insert(_Area);
}

vector<Area*> QuadTree::Query(Area* _queryArea)
{
    if (!_queryArea)
        return {};

    if (!m_Root)
    {
        return {};
    }
    m_Root->ResetVisualize();

    vector<Area*> vecPossibleAreas;
    //send over my(m_root's) quadrant, and the output vector(vecPossibleAreas)
    m_Root->Query(_queryArea->GetMyQuadrant(), vecPossibleAreas);

    vector<Area*> vecIntersectingArea;
    //we then loop through all possible areas, seeking if any intersects with _queryArea
    for (Area* const area : vecPossibleAreas)
    {
        for (Area* const iter : area->GetAllAreas())
        {
            //see if the query area's boundary(quadrant) intersects with my boundary(quadrant)
            if (iter->GetMyQuadrant().CheckIntersect(_queryArea->GetMyQuadrant()))
            {
                //get the intersecting area.
                vecIntersectingArea.emplace_back(iter);
                continue;
            }
        }
    }

    //return the intersecting areas
    return vecIntersectingArea;
}

void QuadTree::Render()
{
    m_Root->Render();
}

END