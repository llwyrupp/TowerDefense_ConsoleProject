#include "ScreenBuffer.h"
#include "Graphics/Renderer/Renderer.h"
#include <iostream>

//TODO: Analyze this cpp

BEGIN(System)
ScreenBuffer::ScreenBuffer(const Vector2& vScreenSize)
	:m_vScreenSize(vScreenSize)
{
	// Console Output 생성.
	m_hBuffer = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		nullptr,
		CONSOLE_TEXTMODE_BUFFER,
		nullptr
	);

	// 예외 처리.
	if (m_hBuffer == INVALID_HANDLE_VALUE)
	{
		MessageBoxA(
			nullptr,
			"ScreenBuffer - Failed to create buffer.",
			"Buffer creation error",
			MB_OK
		);
		__debugbreak();
	}

	// 콘솔 창 크기 지정.
	SMALL_RECT rect;
	rect.Left = 0;
	rect.Top = 0;
	rect.Right = static_cast<short>(m_vScreenSize.m_iX - 1);
	rect.Bottom = static_cast<short>(m_vScreenSize.m_iY - 1);

	if (!SetConsoleWindowInfo(m_hBuffer, true, &rect))
	{
		//DWORD errorCode = GetLastError();
		std::cerr<< "Failed to set console window info.\n";
		__debugbreak();
	}

	// 버퍼 크기 설정.
	if (!SetConsoleScreenBufferSize(m_hBuffer, m_vScreenSize))
	{
		std::cerr << "Failed to set console buffer size.\n";
		__debugbreak();
	}

	// 커서 끄기.
	CONSOLE_CURSOR_INFO cInfo;
	GetConsoleCursorInfo(m_hBuffer, &cInfo);

	// 끄도록 설정.
	cInfo.bVisible = false;
	SetConsoleCursorInfo(m_hBuffer, &cInfo);
}

ScreenBuffer::~ScreenBuffer()
{
	// 버퍼 해제.
	if (m_hBuffer)
	{
		CloseHandle(m_hBuffer);
	}
}

void ScreenBuffer::Clear_Buffer()
{
	// 실제로 화면을 지우고 난 뒤에 
		// 몇 글자를 썼는지 반환 받는데 사용.
	DWORD dwWrittenCount = 0;

	// 콘솔 버퍼에 있는 화면 지우기.
	// 그래픽스 -> 지우기 -> 한 색상(또는 값)으로 덮어쓰기.
	FillConsoleOutputCharacterA(
		m_hBuffer,
		' ',
		m_vScreenSize.m_iX * m_vScreenSize.m_iY,
		Vector2::Zero,
		&dwWrittenCount
	);
}

void ScreenBuffer::Draw_Char(CHAR_INFO* chInfo)
{
	SMALL_RECT stWriteRegion = {};
	stWriteRegion.Left = 0;
	stWriteRegion.Top = 0;
	stWriteRegion.Right = static_cast<short>(m_vScreenSize.m_iX - 1);
	stWriteRegion.Bottom = static_cast<short>(m_vScreenSize.m_iY - 1);

	// 버퍼에 전달 받은 글자 배열 설정.
	WriteConsoleOutputA(
		m_hBuffer,
		chInfo,
		m_vScreenSize,
		Vector2::Zero,
		&stWriteRegion
	);
}
END