#include "Maker.h"
#include "EngineCommon/Engine_Function.h"
#include "InputMgr/InputMgr.h"

const char* m_chRoadTileBrushPath = "../Data/Tool/RoadTileBrush.txt";
const char* m_chGroundTileBrushPath = "../Data/Tool/GroundTileBrush.txt";

Maker::Maker()
	:super(nullptr, m_chGroundTileBrushPath, Vector2::Zero, Color::eGreen)
{
	m_eCurTileType = E_TILE_TYPE::E_TYPE_GROUND;
}

Maker::~Maker()
{
}

void Maker::LoadMapFile(const char* _pPath)
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, _pPath, "rt");

	//exception err
	if (!pFile) {
		cerr << "FAILED TO LOAD MAP";
		__debugbreak();
	}

	fseek(pFile, 0, SEEK_END);
	size_t szFileSize = ftell(pFile);
	rewind(pFile);
	char* pBuffer = new char[szFileSize + 1];

	size_t szLen = fread(pBuffer, sizeof(char), szFileSize, pFile);



	Safe_Delete_Arr(pBuffer);
	fclose(pFile);
}

void Maker::SaveMapFile(const char* _pPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, _pPath, "wt");

	if (!pFile)
	{
		cerr << "FAILED TO SAVE FILE.";
		__debugbreak();
	}
	//fprintf(pFile, "iHP = %d\n", m_tInfo.iHP);

	fclose(pFile);
}

void Maker::BeginPlay()
{
	super::BeginPlay();
}

void Maker::Tick(float _fDeltaTime)
{
	super::Tick(_fDeltaTime);

	if (InputMgr::Get_Instance().GetKeyDown('1'))
	{
		m_eCurTileType = E_TILE_TYPE::E_TYPE_GROUND;
	}
	else if (InputMgr::Get_Instance().GetKeyDown('2'))
	{
		m_eCurTileType = E_TILE_TYPE::E_TYPE_ROAD;
	}
	else if (InputMgr::Get_Instance().GetKeyDown('3'))
	{
		m_eCurTileType = E_TILE_TYPE::E_TYPE_ERASOR;
	}

	if (InputMgr::Get_Instance().GetMouseButtonDown(0))
	{
		//TODO:Add tile.
	}
	else if (InputMgr::Get_Instance().GetMouseButtonUp(0))
	{
		//
	}

	SetPos(InputMgr::Get_Instance().GetMousePos());
}

void Maker::Render()
{
	super::Render();
}
