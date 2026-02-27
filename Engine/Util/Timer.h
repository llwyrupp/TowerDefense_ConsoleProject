#pragma once
#ifndef __TIMER_H__
#define __TIMER_H__
#include "EngineCommon/Engine_Defines.h"
#include "EngineCommon/Engine_Macro.h"

using namespace System;

BEGIN(Util)
class Timer 
{
public:
	Timer() 
		:m_fElapsedTime(0.f), m_fTargetTime(0.f)
	{

	}
	Timer(float _elapsedTime, float _targetTime)
		:m_fElapsedTime(_elapsedTime), m_fTargetTime(_targetTime)
	{
	}
public:
	inline void Tick(float _fDeltaTime) { m_fElapsedTime += _fDeltaTime; }
	inline void ResetTime() { m_fElapsedTime = 0.f; }
	inline bool IsTimeOut() { 
		m_bIsActive = false;
		return  m_fElapsedTime >= m_fTargetTime; }
	inline void SetTargetTime(float _time) { m_fTargetTime = _time; }
	inline float GetElapsedTime() const { return m_fElapsedTime; }
	inline float GetTargetTime() const { return m_fTargetTime; }
	inline bool GetIsActive() const { return m_bIsActive; }
	inline void SetIsActive(bool _bFlag){ m_bIsActive = _bFlag; }
private:
	float m_fTargetTime = 0.f;
	float m_fElapsedTime = 0.f;
	bool m_bIsActive = false;
};
END

#endif // !__TIMER_H__