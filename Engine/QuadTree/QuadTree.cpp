#include "QuadTree.h"
using namespace System;

QuadTree::QuadTree(const Quadrant& _quadrant)
{
    m_Root = new QTNode(_quadrant);//0, 0, 150, 50
}

QuadTree::~QuadTree()
{
    Safe_Delete(m_Root);
}

void QuadTree::InsertNode(QTNode* _qtNode)
{
    if (!m_Root)
    {
        return;
    }
    m_Root->Insert(_qtNode);
}

vector<QTNode*> QuadTree::Query(QTNode* _queryNode)
{
    if (!_queryNode)
        return {};

    vector<QTNode*> vecPossibleNodes;
    //send over my(m_root's) quadrant, and the output vector(vecPossibleNodes)
    m_Root->Query(_queryNode->GetMyQuadrant(), vecPossibleNodes);

    vector<QTNode*> vecIntersects;
    //we then loop through all possible nodes, seeking if any intersects with _querynode
    for (QTNode* const node : vecPossibleNodes)
    {
        for (QTNode* const point : node->GetAllNodes())
        {
            //see if the query node's boundary(quadrant) intersects with my boundary(quadrant)
            if (point->GetMyQuadrant().CheckIntersect(_queryNode->GetMyQuadrant()))
            {
                //get the intersecting nodes.
                vecIntersects.emplace_back(point);
                continue;
            }
        }
    }

    //return the intersecting nodes
    return vecIntersects;
}
