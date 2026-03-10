#include "Effect.h"
#include "Engine/Engine.h"
#include "ClientCommon/ClientEnum.h"

static const Effect::EFFECTFRAME sequence[] =
{
	Effect::EFFECTFRAME("  X  ", 0.08f, Color::eRed),
	Effect::EFFECTFRAME(" X X ", 0.08f, Color::eBlue),
	Effect::EFFECTFRAME("X X X", 0.08f, Color::eGreen),
	Effect::EFFECTFRAME(" X X ", 0.08f, Color::eCyan),
	Effect::EFFECTFRAME("  X  ", 0.08f, Color::eMagenta),
	Effect::EFFECTFRAME(" X X ", 0.08f, Color::eWhite),
	Effect::EFFECTFRAME("X X X", 0.08f, Color::eMagenta),
	Effect::EFFECTFRAME(" X X ", 0.08f, Color::eCyan),
	Effect::EFFECTFRAME("  X  ", 0.08f, Color::eGreen),
	/*Effect::EFFECTFRAME("  ¡Ù", 0.08f, Color::eRed),
	Effect::EFFECTFRAME("¡Ú  ", 0.08f, Color::eBlue),
	Effect::EFFECTFRAME("   ¡Ù", 0.08f, Color::eBlue),
	Effect::EFFECTFRAME(" ¡Ú¡Ù¡Ú", 0.08f, Color::eGreen),
	Effect::EFFECTFRAME("¡Ù¡Ú¡Ù¡Ú  ", 0.08f, Color::eWhite),
	Effect::EFFECTFRAME("  ¡Ú¡Ù¡Ú¡Ù", 0.08f, Color::eRed),
	Effect::EFFECTFRAME("¡Ù¡Ú¡Ù¡Ú¡Ù ", 0.08f, Color::eBlue),
	Effect::EFFECTFRAME(" ¡Ú¡Ù¡Ú¡Ù¡Ú", 0.08f, Color::eGreen),
	Effect::EFFECTFRAME("¡Ù¡Ú¡Ù¡Ú¡Ù¡Ú", 0.08f, Color::eWhite),*/
	//Effect::EFFECTFRAME("  X  ", 0.5f, Color::eGreen)
	/*Effect::EFFECTFRAME("¡Ù ", 0.08f, Color::eBlue),
	Effect::EFFECTFRAME("¡Ú ", 0.08f, Color::eGreen),
	Effect::EFFECTFRAME("¡Ù ", 0.08f, Color::eRed),
	Effect::EFFECTFRAME("¡Ú ", 0.08f, Color::eBlue),
	Effect::EFFECTFRAME("¡Ù ", 0.08f, Color::eGreen),
	Effect::EFFECTFRAME("¡Ú ", 0.08f, Color::eRed),
	Effect::EFFECTFRAME("¡Ù ", 0.08f, Color::eBlue),
	Effect::EFFECTFRAME("¡Ú ", 0.08f, Color::eGreen),
	Effect::EFFECTFRAME("¡Ù ", 0.08f, Color::eRed),
	Effect::EFFECTFRAME("¡Ú ", 0.08f, Color::eBlue),
	Effect::EFFECTFRAME("¡Ù ", 0.08f, Color::eGreen),
	Effect::EFFECTFRAME("¡Ú ", 0.08f, Color::eRed),*/
};

Effect::Effect(const Vector2& position)
	: super(sequence[0].frame, nullptr, position, Color::eRed)
{
	m_iSortingOrder = static_cast<int>(E_TYPE_SORTORDER::E_EFFECT);


	float effectFrameImageLength = 6.f;

	float fX = GetPos().m_fX;
	float fY = GetPos().m_fY;
	SetPos(Vector2(fX < 0 ? effectFrameImageLength + fX : fX, fY));
	SetPos(Vector2(
		fX + effectFrameImageLength > static_cast<float>(Engine::Get_Instance().GetWidth()) ?
		fX - effectFrameImageLength : fX, fY)
	);

	m_iEffSeqCnt = sizeof(sequence) / sizeof(sequence[0]);

	m_fDelayTime = sequence[0].playTime;

	m_eColor = sequence[0].color;
}

void Effect::BeginPlay()
{
	super::BeginPlay();
}

void Effect::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	m_fAccTime += deltaTime;
	if (m_fAccTime < m_fDelayTime)
	{
		return;
	}

	if (m_iCurSeqIdx == m_iEffSeqCnt - 1)
	{
		Destroy();
		return;
	}

	m_fAccTime = 0.f;

	++m_iCurSeqIdx;

	m_fDelayTime = sequence[m_iCurSeqIdx].playTime;

	ChangeImage(sequence[m_iCurSeqIdx].frame);

	m_eColor = sequence[m_iCurSeqIdx].color;
}

void Effect::Render()
{
	super::Render();
}
