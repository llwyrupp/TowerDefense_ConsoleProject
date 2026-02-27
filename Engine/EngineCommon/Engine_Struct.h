#pragma once
#ifndef __ENGINE_STRUCT_H__
#define __ENGINE_STRUCT_H__

typedef struct tagPosition
{
	tagPosition() {
		iX = 0;
		iY = 0;
	}
	tagPosition(int _x, int _y)
		:iX(_x), iY(_y)
	{

	}
	int iX;
	int iY;
}POS;

typedef struct tagDirection
{
	tagDirection() {
		iX = 0;
		iY = 0;
		fCost = 0.f;
	}
	tagDirection(int _x, int _y, float _cost)
		:iX(_x), iY(_y), fCost(_cost)
	{
		//fCost = sqrt()
	}
	int iX;
	int iY;
	float fCost;
}DIR;

#endif//!__ENGINE_STRUCT_H__