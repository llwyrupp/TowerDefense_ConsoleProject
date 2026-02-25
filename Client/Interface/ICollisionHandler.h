#pragma once
#ifndef __ICOLLISIONHANDLER_H__
#define __ICOLLISIONHANDLER_H__
#include "Actor/Actor.h"

using namespace System;
class ICollisionHandler
{
public:
	virtual void OnCollisionEnter2D(Actor* _pActor) = 0;
};
#endif // !__ICOLLISIONHANDLER_H__