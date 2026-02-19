#pragma once
#include "EngineCommon/Engine_Defines.h"
#include "EngineCommon/Engine_Macro.h"

BEGIN(System)

class ENGINE_DLL Vector2
{
public:
	Vector2() = default;
	Vector2(int iX, int iY);
	Vector2(const Vector2& other);
	Vector2& operator=(const Vector2& other);
	~Vector2();

	const char* ToString();

	//operator overloading
	Vector2 operator+(const Vector2& other) const;
	Vector2 operator-(const Vector2& other) const;

	bool operator==(const Vector2& other) const;
	bool operator!=(const Vector2& other) const;

	operator COORD() const;//short vars.

	//default vectors.
	static Vector2 Zero;
	static Vector2 One;
	static Vector2 Up;
	static Vector2 Right;

public:
	int m_iX = 0;
	int m_iY = 0;
private:

	char* m_pString = nullptr;
};

END