#include "FieldLevel.h"
#include "Player/Player.h"

FieldLevel::FieldLevel()
{
	AddNewActor(new Player());
}

FieldLevel::~FieldLevel()
{
}

void FieldLevel::BeginPlay()
{
	super::BeginPlay();

	
}

void FieldLevel::Tick(float _fDeltaTime)
{
	super::Tick(_fDeltaTime);
}

void FieldLevel::Render()
{
	super::Render();
}

