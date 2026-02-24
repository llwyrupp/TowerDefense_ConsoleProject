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
}
