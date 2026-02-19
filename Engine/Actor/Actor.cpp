#include "Actor/Actor.h"
#include "Graphics/Renderer/Renderer.h"

BEGIN(System)

Actor::Actor(const char* pImage, const char* pPath, const Vector2& vPos, Color color)
	:m_vPosition(vPos), m_eColor(color), m_iStringHeight(1), m_iStringWidth(1)
{
	if (!pImage && !pPath) {
		std::cerr << "============YOU NEED AT LEAST ONE: CHAR or PATH============" << this->GetType();
		__debugbreak();
	}

	m_vecStr_FieldLevel.clear();

	if (pImage) {
		/*size_t szLen = strlen(pImage) + 1;
		m_pImage = new char[szLen];
		strcpy_s(m_pImage, sizeof(char) * szLen, pImage);*/
		m_strImg = pImage;
	}
	else if (pPath) {
		
		LoadString_FromFile(pPath);
	}
	
	UpdateRect();

}

Actor::~Actor()
{
	//Safe_Delete_Arr(m_pImage);
}

void Actor::BeginPlay()
{
	m_bHasBegunPlay = true;
}

void Actor::Tick(float _fDeltaTime)
{
	//TODO: Common Actor Behaviour to be added if necessary.
	UpdateRect();
}

void Actor::Render()
{
	if(m_vecStr_FieldLevel.empty())
		Renderer::Get_Instance().Submit(m_strImg, m_vPosition, m_eColor, m_iSortingOrder);
	else {
		for (int i = 0; i < m_vecStr_FieldLevel.size(); ++i) {
			Renderer::Get_Instance().Submit(m_vecStr_FieldLevel[i], Vector2(m_vPosition.m_iX, m_vPosition.m_iY), m_eColor, m_iSortingOrder);
		}
	}
}

void Actor::Destroy()
{
	m_bIsDestroyRequested = true;
	OnDestroy();
}

void Actor::OnDestroy()
{
	//logic when actor is destroyed
}

void Actor::ChangeImage(const char* newImage)
{
	m_strImg = newImage;
}

void Actor::UpdateRect()
{
	m_rtSize.left = static_cast<long>(m_vPosition.m_iX);
	m_rtSize.top = static_cast<long>(m_vPosition.m_iY);
	m_rtSize.right = static_cast<long>(m_vPosition.m_iX + m_iStringWidth);
	m_rtSize.bottom = static_cast<long>(m_vPosition.m_iY + m_iStringHeight);
}

void Actor::SetPos(const Vector2& vNewPos)
{
	//변경하려는 위치가 현 위치와 동일하면 스킵.
	if (m_vPosition == vNewPos)
		return;

	//새 위치값으로 액터 위치 갱신
	m_vPosition = vNewPos;
}

void Actor::LoadString_FromFile(const char* _pPath)//read actor's representeation in FieldLevel if its size is bigger than 1.
{
	std::ifstream file(_pPath);

	if (file.is_open())
	{
		std::string tempStr = "";
		
		while (std::getline(file, tempStr)) {
			m_vecStr_FieldLevel.emplace_back(tempStr);
		}
		m_iStringWidth = static_cast<int>(m_vecStr_FieldLevel[0].length());
		m_iStringHeight = static_cast<int>(m_vecStr_FieldLevel.size());
	}

	/*
	FILE* pFile = nullptr;
	fopen_s(&pFile, _pPath, "rt");
	if (!pFile) {
		cerr << "FAILED TO OPEN FILE"<< _pPath;
		__debugbreak();
	}
	
	char cBuffer[MAX_STRING_LEN] = {};
	
	size_t szLen = fread(cBuffer, sizeof(char), MAX_STRING_LEN, pFile);
	
	if (szLen == 0) {
		cerr << "No file at: "<< _pPath;
		__debugbreak();
	}

	char* pToken = {};
	char* pContext = {};
	if (!m_pImage)
	{
		m_pImage = new char[MAX_STRING_LEN];
		memset(m_pImage, 0, sizeof(char) * MAX_STRING_LEN);
	}

	//TODO: reserve dynamically
	vecChars.reserve(MAX_STRING_LEN);
	
	pToken = strtok_s(cBuffer, "\n", &pContext);//read the first line
	size_t szTotalLen = 0;
	while (pToken) {
		char pLine[MAX_STRING_LEN] = {};
		//개행문자로 각 행의 문자열 분리 후 저장
		sscanf_s(pToken, "%s", pLine, MAX_STRING_LEN);
		szLen = strlen(pLine) + 1;

		strcat_s(m_pImage, sizeof(char) * MAX_STRING_LEN, pLine);
		strcat_s(m_pImage, sizeof(char) * MAX_STRING_LEN, "\n");
		
		//strcpy_s(m_pImage + szTotalLen, szLen, pLine);
		//szTotalLen += szLen;

		//개행으로 문자열 분리
		pToken = strtok_s(nullptr, "\n", &pContext);

		++m_iStringHeight;
	}

	//remove the last '\n'
	size_t szTempIdx = strlen(m_pImage) - 1;
	m_pImage[szTempIdx] = '\0';

	fclose(pFile);
	*/
}

END