#include "EngineCommon/Engine_Function.h"
#include "EngineCommon/Engine_Defines.h"
#include "Math/Vector2.h"


BEGIN(System)

Vector2 Vector2::Zero(0, 0);
Vector2 Vector2::One(1, 1);
Vector2 Vector2::Up(0, 1);
Vector2 Vector2::Right(1, 0);

Vector2::Vector2(int iX, int iY)
    :m_iX(iX), m_iY(iY)
{
}

Vector2::Vector2(const Vector2& other)
    :m_iX(other.m_iX), m_iY(other.m_iY)
{
}

Vector2& Vector2::operator=(const Vector2& other)
{
    if (*this != other)
    {
        m_iX = other.m_iX;
        m_iY = other.m_iY;

        Safe_Delete(m_pString);
    }
    return *this;
}

Vector2::~Vector2()
{
    Safe_Delete(m_pString);
}

const char* Vector2::ToString()
{
    //기존 문자열 존재할 경우 제거
    Safe_Delete(m_pString);

    m_pString = new char[MAX_STRING_LEN];
    memset(m_pString, 0, sizeof(char) * MAX_STRING_LEN);
    sprintf_s(m_pString, MAX_STRING_LEN, "(%d, %d)", m_iX, m_iY);

    return m_pString;
}

Vector2 Vector2::operator+(const Vector2& other) const
{
    return Vector2(this->m_iX + other.m_iX, this->m_iY + other.m_iY);
}

Vector2 Vector2::operator-(const Vector2& other) const
{
    return Vector2(this->m_iX - other.m_iX, this->m_iY - other.m_iY);
}

bool Vector2::operator==(const Vector2& other) const
{
    return this->m_iX == other.m_iX && this->m_iY == other.m_iY;
}

bool Vector2::operator!=(const Vector2& other) const
{
    return !(*(this) == other);
}

Vector2::operator COORD() const
{
    COORD coord = {};
    coord.X = static_cast<SHORT>(m_iX);
    coord.Y = static_cast<SHORT>(m_iY);

    return coord;
}

END