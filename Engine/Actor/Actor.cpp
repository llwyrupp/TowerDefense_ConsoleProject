#include "Actor/Actor.h"
#include "Graphics/Renderer/Renderer.h"
#include "QuadTree/Area.h"

BEGIN(System)

Actor::Actor(const char* pImage, const char* pPath, const Vector2& vPos, Color color, E_LAYER _eLayer)
	:m_vPosition(vPos), m_eColor(color), m_eLayer(_eLayer), m_iHeight(1), m_iWidth(1)
{
	if (!pImage && !pPath) {
		std::cerr << "============YOU NEED AT LEAST ONE: CHAR or PATH============" << this->GetTypeID();
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

	m_pArea = new Area(Quadrant(static_cast<int>(vPos.m_fX), static_cast<int>(vPos.m_fY), m_iWidth, m_iHeight));
	m_pArea->SetActorOwner(this);
}

Actor::~Actor()
{
	//Safe_Delete_Arr(m_pImage);
	Safe_Delete(m_pArea);
}

void Actor::BeginPlay()
{
	m_bHasBegunPlay = true;
}

void Actor::Tick(float _fDeltaTime)
{
	//TODO: Common Actor Behaviour to be added if necessary.
	UpdateRect();
	m_vPrevPosition = m_vPosition;
}

void Actor::Render()
{
	if (m_vecStr_FieldLevel.empty())
		Renderer::Get_Instance().Submit(m_strImg, m_vPosition, m_eColor, m_iSortingOrder);
	else {//assume that all objects' height and width are even numbers.
		size_t szIndex = 0;
		Vector2 newPos = GetPos();
		//when rendering, use the console window coordinate system(X for width(col) and Y for height(row))
		newPos.m_fX -= m_iWidth / 2;
		newPos.m_fY -= m_iHeight / 2;
		for (int iY = 0; iY < m_iHeight; ++iY)
		{
			if (szIndex >= m_vecStr_FieldLevel.size())
				break;
			++newPos.m_fY;
			//object with multiple strings will be drawn with the position as the center.
			Renderer::Get_Instance().Submit(m_vecStr_FieldLevel[szIndex++], newPos, m_eColor, m_iSortingOrder);
		}
	}
}

void Actor::Destroy()
{
	m_bIsDestroyRequested = true;
}

void Actor::ChangeImage(const char* newImage)
{
	m_strImg = newImage;
}

void Actor::UpdateRect()
{
	//OLD
	/*m_rtSize.left = static_cast<long>(m_vPosition.m_fX);
	m_rtSize.top = static_cast<long>(m_vPosition.m_fY);
	m_rtSize.right = static_cast<long>(m_vPosition.m_fX + m_iWidth);
	m_rtSize.bottom = static_cast<long>(m_vPosition.m_fY + m_iHeight);*/

	m_rtSize.left = static_cast<long>(m_vPosition.m_fX);
	m_rtSize.top = static_cast<long>(m_vPosition.m_fY);
	m_rtSize.right = static_cast<long>(m_vPosition.m_fX + m_iWidth);
	m_rtSize.bottom = static_cast<long>(m_vPosition.m_fY + m_iHeight);

	//update area info for Quadtree
	if(m_pArea)
		m_pArea->SetMyQuadrantPos(static_cast<int>(m_vPosition.m_fX), static_cast<int>(m_vPosition.m_fY));
}

bool Actor::CheckIntersect(const Actor* const _other)
{
	RECT rtOther = _other->GetRect();

	//OLD
	int iMinX_This = this->m_rtSize.left, iMaxX_This = this->m_rtSize.right,
		iMinY_This = this->m_rtSize.top, iMaxY_This = this->m_rtSize.bottom,
		iMinX_Other = rtOther.left, iMaxX_Other = rtOther.right,
		iMinY_Other = rtOther.top, iMaxY_Other = rtOther.bottom;

	//check if two intersects.

	if (iMaxX_This <= iMinX_Other)
		return false;

	if (iMaxX_Other <= iMinX_This)
		return false;

	if (iMaxY_This <= iMinY_Other)
		return false;

	if (iMaxY_Other <= iMinY_This)
		return false;

	return true;
}

bool Actor::CheckIntersect_CrossDot(const Actor* const _other)
{
	//if collision skipped in a frame, check with previous position
	//we use dot product to see if _other is in front of us or behind us.
	//we use cross product to see if _other is on our left or on the right.
	//cross product result > 0.f means _other is on the left of my path.
	//result < 0.f means _other is on the right of my path.
	//if result == 0.f, _other is exactly on the same line as my path.
	Vector2 vPath = m_vPosition - m_vPrevPosition;
	Vector2 vDirPrevToOther = _other->GetPos() - m_vPrevPosition;//AP
	Vector2 vDirCurToOther = _other->GetPos() - m_vPosition;//BP
	float fCrossProduct = vPath.CrossProduct(vDirPrevToOther);//AB cross AP
	float fDot1 = vPath.DotProduct(vDirPrevToOther);//AB dot AP
	float fDot2 = vPath.DotProduct(vDirCurToOther);//AB dot BP
	float fDot3 = vDirPrevToOther.DotProduct(vDirCurToOther);//AP dot BP

	if (fabsf(fCrossProduct) < EPSILON) //float cannot be exactly 0.0f. use epsilon instead.
	{
		//two vectors point same direction || two vectors point opposite dir
		/*if (fDot1 > 0.f || fDot2 < 0.f || fDot3 > 0.f)
		{
			return true;
		}*/
		if (fabsf(fDot3) < EPSILON)
			return true;
	}
	//else if (vDir1.DotProduct(vDir2) == 0.f)//two vectors perfectly perpendicular
	//{

	//}

	return false;
}

//bool Actor::CheckIntersect_ByArea(const Area* const _other)
//{
//	int iMinX_This = this->m_rtSize.left, iMaxX_This = this->m_rtSize.right,
//		iMinY_This = this->m_rtSize.top, iMaxY_This = this->m_rtSize.bottom,
//		iMinX_Other = _other., iMaxX_Other = _other->GetRect().right,
//		iMinY_Other = _other->GetRect().top, iMaxY_Other = _other->GetRect().bottom;
//
//	//check if two intersects.
//
//	if (iMaxX_This <= iMinX_Other)
//		return false;
//
//	if (iMaxX_Other <= iMinX_This)
//		return false;
//
//	if (iMaxY_This <= iMinY_Other)
//		return false;
//
//	if (iMaxY_Other <= iMinY_This)
//		return false;
//
//	return true;
//}

void Actor::SetPos(const Vector2& vNewPos)
{
	//if my position is the same as the new pos, skip setpos
	if (m_vPosition == vNewPos)
		return;

	//update the actor's pos as the new pos
	m_vPosition = vNewPos;
	if (m_pArea)
		m_pArea->SetMyQuadrantPos(static_cast<int>(vNewPos.m_fX), static_cast<int>(vNewPos.m_fY));
}

void Actor::LoadString_FromFile(const char* _pPath)//read actor's representeation in FieldLevel if its size is bigger than 1.
{
	m_vecStr_FieldLevel.clear();

	std::ifstream file(_pPath);

	if (file.is_open())
	{
		std::string tempStr = "";

		while (std::getline(file, tempStr)) {
			m_vecStr_FieldLevel.emplace_back(tempStr);
		}
		m_iWidth = static_cast<int>(m_vecStr_FieldLevel[0].length());
		m_iHeight = static_cast<int>(m_vecStr_FieldLevel.size());
	}
}

END