#include "EngineCommon/Engine_Function.h"
#include "EngineCommon/Engine_Defines.h"
#include "Math/Vector2.h"


BEGIN(System)

Vector2 Vector2::Zero(0.f, 0.f);
Vector2 Vector2::One(1.f, 1.f);
Vector2 Vector2::Up(0.f, 1.f);
Vector2 Vector2::Right(1.f, 0.f);

Vector2::Vector2(float _fX, float _fY)
    :m_fX(_fX), m_fY(_fY)
{
}

Vector2::Vector2(const Vector2& other)
    :m_fX(other.m_fX), m_fY(other.m_fY)
{
}

Vector2& Vector2::operator=(const Vector2& other)
{
    if (*this != other)
    {
        m_fX = other.m_fX;
        m_fY = other.m_fY;

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
    sprintf_s(m_pString, MAX_STRING_LEN, "(%f, %f)", m_fX, m_fY);

    return m_pString;
}

Vector2 Vector2::operator+(const Vector2& other) const
{
    return Vector2(this->m_fX + other.m_fX, this->m_fY + other.m_fY);
}

Vector2 Vector2::operator-(const Vector2& other) const
{
    return Vector2(this->m_fX - other.m_fX, this->m_fY - other.m_fY);
}

bool Vector2::operator==(const Vector2& other) const
{
    return fabsf(this->m_fX - other.m_fX) < EPSILON  &&
        fabsf(this->m_fY - other.m_fY) < EPSILON;
}

bool Vector2::operator!=(const Vector2& other) const
{
    return !(*(this) == other);
}

Vector2::operator COORD() const
{
    COORD coord = {};
    coord.X = static_cast<SHORT>(m_fX);
    coord.Y = static_cast<SHORT>(m_fY);

    return coord;
}

void Vector2::NormalizeVector()
{
    //calc. the magnitude(length) using pythagorean theo.
    float fMagnitude = sqrtf(m_fX * m_fX + m_fY * m_fY);
    //prevent divbyzero
    if (fMagnitude > 0.f) {
        m_fX /= fMagnitude;
        m_fY /= fMagnitude;
    }
}

float Vector2::DotProduct(const Vector2& _other) const
{
    //result > 0 : the vectors point in roughly the same direction (angle < 90)
    //result = 0 : the vectors are perfectly perpendicular (angle = 90)
    //result < 0 : the vectors point away from each other (angle > 90)
    return (m_fX * _other.m_fX) + (m_fY * _other.m_fY);
}

float Vector2::CrossProduct(const Vector2& _other) const
{
    //vector that is perfectly perpendicular to both vectors
    //returns an int value that will tell us if the point is perfectly lined up with the vector
    return m_fX * _other.m_fY - m_fY * _other.m_fX;
}

END