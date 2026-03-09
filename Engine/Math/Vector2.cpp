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

void Vector2::NormalizeVector()
{
    //calc. the magnitude(length) using pythagorean theo.
    float fMagnitude = static_cast<float>(sqrt(m_iX * m_iX + m_iY * m_iY));
    //prevent divbyzero
    if (fMagnitude > 0.f) {
        m_iX /= fMagnitude;
        m_iY /= fMagnitude;
    }
}

int Vector2::DotProduct(const Vector2& _other) const
{
    //result > 0 : the vectors point in roughly the same direction (angle < 90)
    //result = 0 : the vectors are perfectly perpendicular (angle = 90)
    //result < 0 : the vectors point away from each other (angle > 90)
    return (m_iX * _other.m_iX) + (m_iY * _other.m_iY);
}

int Vector2::CrossProduct(const Vector2& _other) const
{
    //vector that is perfectly perpendicular to both vectors
    //returns an int value that will tell us if the point is perfectly lined up with the vector
    return m_iX * _other.m_iY - m_iY * _other.m_iX;
}

END