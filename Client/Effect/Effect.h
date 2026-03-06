#pragma once
#ifndef __EFFECT_H__
#define __EFFECT_H__

#include "Actor/Actor.h"
#include "EngineCommon/Engine_Function.h"
#include "Util/Util.h"
using namespace System;

class Effect : public Actor
{
	RTTI_DECLARATIONS(Effect, Actor)
		typedef struct tagEffectFrame
	{
		tagEffectFrame(
			const char* frame,
			float playTime = 0.05f,
			Color color = Color::eRed)
			: playTime(playTime), color(color)
		{
			size_t length = strlen(frame) + 1;
			this->frame = new char[length];
			strcpy_s(this->frame, length, frame);
		}

		~tagEffectFrame()
		{
			Safe_Delete_Arr(frame);
		}

		char* frame = nullptr;

		float playTime = 0.0f;

		Color color = Color::eWhite;
	}EFFECTFRAME;

public:

	Effect(const Vector2& position);

	virtual void BeginPlay() override;
	virtual virtual void Tick(float deltaTime) override;
	virtual void Render() override;

private:

	int m_iEffSeqCnt = 0;
	int m_iCurSeqIdx = 0;
	float m_fAccTime = 0.f;
	float m_fDelayTime = 0.f;
};


#endif // !__EFFECT_H__