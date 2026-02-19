#include "Renderer.h"
#include "Graphics/ScreenBuffer/ScreenBuffer.h"
#include "EngineCommon/Engine_Function.h"
#include<iostream>
BEGIN(System)

Renderer* Renderer::m_pInstance = nullptr;

#pragma region FRAME
Renderer::tagFrame::tagFrame(int iBufCnt)
{
	//¹è¿­ »ý¼º ¹× ÃÊ±âÈ­(RAII)
	charInfoArr = new CHAR_INFO[iBufCnt];//¸Þ¸ð¸® µ¿ÀûÇÒ´ç
	memset(charInfoArr, 0, sizeof(CHAR_INFO) * iBufCnt);


	pSortingOrderArr = new int[iBufCnt];//¸Þ¸ð¸® µ¿ÀûÇÒ´ç
	memset(pSortingOrderArr, 0, sizeof(int) * iBufCnt);
}

Renderer::tagFrame::~tagFrame()
{
	Safe_Delete_Arr(charInfoArr);
	Safe_Delete_Arr(pSortingOrderArr);
}

void Renderer::tagFrame::Clear(const Vector2& vScreenSize)
{
	//2Â÷¿ø ¹è¿­·Î ´Ù·ç´Â 1Â÷¿ø ¹è¿­À» µ¹¸é¼­ ¸ðµÎ blank(' ')·Î ¼¼ÆÃ
	const int iWidth = vScreenSize.m_iX;
	const int iHeight = vScreenSize.m_iY;

	for (int iRow = 0; iRow < iHeight; ++iRow) {
		for (int iCol = 0; iCol < iWidth; ++iCol) {
			//get arr index;
			//index = (current row idx * width) + current column idx
			int iIndex = (iRow * iWidth) + iCol;

			//set string and attributes
			CHAR_INFO& info = charInfoArr[iIndex];
			info.Char.AsciiChar = ' ';
			info.Attributes = 0;

			//init sort order
			pSortingOrderArr[iIndex] = -1;
		}
	}
}
#pragma endregion FRAME

#pragma region RENDERER
Renderer::Renderer(const Vector2& vScreenSize)
	:m_vScreenSize(vScreenSize)
{
	m_pInstance = this;

	const int iBufCnt = vScreenSize.m_iX * vScreenSize.m_iY;

	m_pFrame = new FRAME(iBufCnt);

	//init frame.
	m_pFrame->Clear(vScreenSize);

	//RAII
	m_pScreenBuffers[0] = new ScreenBuffer(vScreenSize);
	m_pScreenBuffers[0]->Clear_Buffer();

	m_pScreenBuffers[1] = new ScreenBuffer(vScreenSize);
	m_pScreenBuffers[1]->Clear_Buffer();

	//set active buffer
	Present();
}


Renderer::~Renderer()
{
	Safe_Delete(m_pFrame);
	for (auto& buf : m_pScreenBuffers) {
		Safe_Delete(buf);
	}
}

void Renderer::Render()
{
	//clear screen.
	Clear();

	//all actors on the scene has finished "Submit"ting to the Renderer.
	//Now, iterate the renderQueue and add up the FRAMEs.

	for (const RENDERCOM& com : m_vecRenderQueue) {
		//if no text, skip
		if (com.strText.empty())
			continue;

		//if the y coordinate is out of screen, skip.
		if (com.vPosition.m_iY < 0 || com.vPosition.m_iY >= m_vScreenSize.m_iY)
			continue;

		//check validity of text
		const int iLen = static_cast<int>(com.strText.length());
		if (iLen <= 0)
			continue;


		//print X
		const int iStartX = com.vPosition.m_iX;
		const int iEndX = com.vPosition.m_iX + iLen - 1;
		if (iEndX < 0 || iStartX >= m_vScreenSize.m_iX)
			continue;

		const int iVisibleStartX = iStartX < 0 ? 0 : iStartX;
		const int iVisibleEndX = iEndX >= m_vScreenSize.m_iX ? m_vScreenSize.m_iX - 1 : iEndX;

		//render the visible parts only.
		for (int i = iVisibleStartX; i <= iVisibleEndX; ++i) {
			//srcidx: char index inside the string.
			const int iSrcIdx = i - iStartX;

			//2d array index.
			const int iFrameIdx = (com.vPosition.m_iY * m_vScreenSize.m_iX) + i;

			//compare sort priority
			if (m_pFrame->pSortingOrderArr[iFrameIdx] > com.iSortingOrder)
				continue;

			m_pFrame->charInfoArr[iFrameIdx].Char.AsciiChar = com.strText[iSrcIdx];
			m_pFrame->charInfoArr[iFrameIdx].Attributes = (WORD)com.eColor;

			m_pFrame->pSortingOrderArr[iFrameIdx] = com.iSortingOrder;
		}

		////print Y
		//const int iStartY = com.vPosition.m_iY;
		//const int iEndY = com.vPosition.m_iY + iLen - 1;
		//if (iEndY < 0 || iStartY >= m_vScreenSize.m_iX)
		//	continue;

		//const int iVisibleStartY = iStartY < 0 ? 0 : iStartY;
		//const int iVisibleEndY = iEndY >= m_vScreenSize.m_iY ? m_vScreenSize.m_iY - 1 : iEndY;

		//for (int i = iVisibleStartY; i <= iVisibleEndY; ++i) {
		//	const int iSrcIdx = i - iStartY;

		//	const int iFrameIdx = (com.vPosition.m_iY * m_vScreenSize.m_iY) + i;

		//	if (m_pFrame->pSortingOrderArr[iFrameIdx] > com.iSortingOrder)
		//		continue;

		//	m_pFrame->charInfoArr[iFrameIdx].Char.AsciiChar = com.strText[iSrcIdx];
		//	m_pFrame->charInfoArr[iFrameIdx].Attributes = (WORD)com.eColor;

		//	m_pFrame->pSortingOrderArr[iFrameIdx] = com.iSortingOrder;
		//}
	}

	//draw the letters.
	GetCurBuffer()->Draw_Char(m_pFrame->charInfoArr);

	//swap buffer.
	Present();

	//empty renderqueue
	m_vecRenderQueue.clear();
}

void Renderer::Submit(std::string pText, const Vector2& vPos, Color eColor, int iSortOrder)
{
	//create a render data and push it into renderqueue.
	RENDERCOM renderCom = {};
	renderCom.strText = pText;
	renderCom.vPosition = vPos;
	renderCom.eColor = eColor;
	renderCom.iSortingOrder = iSortOrder;

	m_vecRenderQueue.emplace_back(renderCom);
}

Renderer& Renderer::Get_Instance() {
	if (!m_pInstance) {
		std::cerr << "RENDERER INSTANCE is NULL";
		__debugbreak();
	}

	return *m_pInstance;
}

void Renderer::Clear()
{
	//clearing screen.
	//1. erase frame(2d arr data.)
	m_pFrame->Clear(m_vScreenSize);

	//2. erase console buffer.
	GetCurBuffer()->Clear_Buffer();
}

void Renderer::Present()
{
	//swap buffer
	SetConsoleActiveScreenBuffer(GetCurBuffer()->Get_Buffer());

	//swap index
	m_iCurBufIdx = 1 - m_iCurBufIdx;
}

ScreenBuffer* Renderer::GetCurBuffer()
{
	if (m_iCurBufIdx >= MAX_NUM_BUFFER || m_iCurBufIdx < 0) {
		std::cerr << "BUFFER OUTOFINDEX\n";
		__debugbreak();
		return nullptr;
	}

	return m_pScreenBuffers[m_iCurBufIdx];
}


#pragma endregion RENDERER


//#pragma region RENDERCOMMAND
//Renderer::tagRenderCommand::tagRenderCommand(const char* _pText, const Vector2& _vPos, Color _eColor, int _iSortingOrder)
//	:vPosition(_vPos), eColor(_eColor), iSortingOrder(_iSortingOrder)
//{
//	//deep copy.
//	size_t szLen = strlen(_pText) + 1;
//	pText = new char[szLen];
//	strcpy_s(pText, sizeof(char) * szLen, _pText);
//}
//
//Renderer::tagRenderCommand::~tagRenderCommand()
//{
//
//}
//#pragma endregion RENDERCOMMAND


END