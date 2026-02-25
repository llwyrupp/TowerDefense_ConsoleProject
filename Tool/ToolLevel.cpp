#include "ToolLevel.h"
#include "Maker.h"

ToolLevel::ToolLevel()
{
	AddNewActor(new Maker());
}

ToolLevel::~ToolLevel()
{
}

void ToolLevel::BeginPlay()
{
	super::BeginPlay();
}

void ToolLevel::Tick(float _fDeltaTime)
{
	super::Tick(_fDeltaTime);
}

void ToolLevel::Render()
{
	super::Render();
	ShowOption();
}

void ToolLevel::ShowOption()
{
	sprintf_s(m_strOption, 128, "Score: %d", score);
	Renderer::Get().Submit(
		scoreString,
		Vector2(0, Engine::Get().GetHeight() - 1)
	);
}
