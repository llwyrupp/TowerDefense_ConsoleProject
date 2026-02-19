#include "Player.h"
#include "InputMgr/InputMgr.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::BeginPlay()
{
	super::BeginPlay();
}

void Player::Tick(float _fDeltaTime)
{
	super::Tick(_fDeltaTime);

	if (InputMgr::Get_Instance().GetKeyDown(VK_LBUTTON))
	{
		//get current position on console window

		cout << GetMousePos().x << ' ' << GetMousePos().y << '\n';
	}
}

void Player::Render()
{
	super::Render();
}

POINT& Player::GetMousePos()
{
	// 1. Get the handle to the console window and the output (for font info)
	HWND hWnd = GetConsoleWindow();
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	//get mouse pos
	POINT pt = {};
	GetCursorPos(&pt);

	//convert screen pixels to coords relative to the console window size
	ScreenToClient(hWnd, &pt);

	//get cur console font size(width/height of one char)
	CONSOLE_FONT_INFO cInfo;
	GetCurrentConsoleFont(hOut, FALSE, &cInfo);

	//div window pixels by font size to get the console cell( col/row)
	int iConsoleX = pt.x / cInfo.dwFontSize.X;
	int iConsoleY = pt.y / cInfo.dwFontSize.Y;

	POINT curPt = { iConsoleX, iConsoleY };

	return curPt;
}

