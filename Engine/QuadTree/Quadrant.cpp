#include "Quadrant.h"

BEGIN(System)
bool Quadrant::CheckIntersect(const Quadrant& _other) const
{
    //if my origin x is smaller than _other's 
    if (GetMaxX() >= _other.GetOriginX())
        return true;

    if (m_iOriginX <= _other.GetMaxX())
        return true;

    if (GetMaxY() >= _other.GetOriginY())
        return true;

    if (m_iOriginY <= _other.GetMaxY())
        return true;

    return false;
}
END