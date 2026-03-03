#pragma once
#ifndef __ENGINE_STRUCT_H__
#define __ENGINE_STRUCT_H__

typedef struct tagPosition
{
	tagPosition() {
		iCol = 0;
		iRow = 0;
	}
	tagPosition(int _col, int _row)
		:iCol(_col), iRow(_row)
	{

	}
	int iCol;
	int iRow;
}POS;

typedef struct tagDirection
{
	tagDirection() {
		iX = 0;
		iY = 0;
		gCost = 0.f;
	}
	tagDirection(int _x, int _y, float _cost)
		:iX(_x), iY(_y), gCost(_cost)
	{
		//fCost = sqrt()
	}
	int iX;
	int iY;
	float gCost;
}DIR;

#endif//!__ENGINE_STRUCT_H__