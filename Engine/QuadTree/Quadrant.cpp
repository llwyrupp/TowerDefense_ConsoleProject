#include "Quadrant.h"

BEGIN(System)
bool Quadrant::CheckIntersect(const Quadrant& _other) const
{
    //if my origin x is smaller than _other's 
    if (GetMaxX() >= _other.GetOriginX())
        return true;
    //if(GetMaxX() < _other.GetOriginX())
    //return false;

    if (m_iOriginX <= _other.GetMaxX())//left
        return true;
    //if(m_iOriginX > _other.GetMaxX())
    //return false;

    if (GetMaxY() >= _other.GetOriginY())
        return true;
    //if(GetMaxY() < _other.GetOriginY())
    //return false;

    if (m_iOriginY <= _other.GetMaxY())//top
        return true;
    //if(m_iOriginY > _other.GetMaxY())
    //return false;

    return false;
}
END