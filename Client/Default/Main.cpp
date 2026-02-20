#include "EngineCommon/Engine_Defines.h"
#include "Game/Game.h"

using namespace System;

int main(void) {

	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hIn, &mode);

	// ENABLE_MOUSE_INPUT: Allows the console to track mouse position
	// ENABLE_EXTENDED_FLAGS: Required to toggle QuickEdit
	// ~ENABLE_QUICK_EDIT_MODE: Prevents the mouse from "freezing" the game to select text
	SetConsoleMode(hIn, (mode | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS) & ~ENABLE_QUICK_EDIT_MODE);

	Game game;
	game.Run();
	
	return 0;
}