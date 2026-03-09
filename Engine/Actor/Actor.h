#pragma once
#ifndef __ACTOR_H__
#define __ACTOR_H__


#include "EngineCommon/Engine_Defines.h"
#include "EngineCommon/Engine_Macro.h"
#include "EngineCommon/Engine_Enum.h"
#include "EngineCommon/Engine_Struct.h"
#include "Math/Vector2.h"
#include "EngineCommon/RTTI.h"

BEGIN(System)

class Level;
class Area;
class ENGINE_DLL  Actor : public RTTI
{
	RTTI_DECLARATIONS(Actor, RTTI)
public:
	Actor(const char* pImage = nullptr, const char* pPath = nullptr, const Vector2& vPos = Vector2::Zero, Color color = Color::eWhite, E_LAYER _eLayer = E_LAYER::E_NONE);
	virtual ~Actor();

public:
	//pure virtuals
	virtual void BeginPlay() = 0;
	virtual void Tick(float _fDeltaTime) = 0;
	virtual void Render() = 0;
public:
	void Destroy();
	void OnDestroy();
	void ChangeImage(const char* newImage);
	void LoadString_FromFile(const char* _pPath);
	void UpdateRect();
	bool CheckIntersect(const Actor* const _other);
	//bool CheckIntersect_ByArea(const Area* const _other);
public:
	void SetPos(const Vector2& vNewPos);
	inline Vector2 GetPos() const { return m_vPosition; }

	inline void SetLevel(Level* pLevel) { m_pLevel = pLevel; }
	inline Level* GetLevel() const { return m_pLevel; }

	inline bool Get_HasBegunPlay() const { return m_bHasBegunPlay; }
	inline void Set_HasBegunPlay(bool _bFlag) { m_bHasBegunPlay = _bFlag; }
	inline bool IsActive() const { return m_bHasBegunPlay; }
	inline bool Get_IsDestroyRequested() const { return m_bIsDestroyRequested; }
	inline void Set_IsDestroyRequested(bool _bFlag) { m_bIsDestroyRequested = _bFlag; }

	inline int Get_SortingOrder() const { return m_iSortingOrder; }
	inline void SetColor( Color const _eColor) { m_eColor = _eColor; }
	inline Color GetColor() const { return m_eColor; }
	inline void SetRect(RECT const _rect) { m_rtSize = _rect; }
	inline const RECT& GetRect() const { return m_rtSize; }
	inline void SetLayer( E_LAYER const _eLayer) { m_eLayer = _eLayer; }
	inline E_LAYER GetLayer() const { return m_eLayer; }

	inline void SetWidth(int _width) { m_iWidth = _width; }
	inline void SetHeight(int _height) { m_iHeight = _height; }

	inline bool Get_IsUsingActorPool() const { return m_bIsUsingActorPool; }
	inline void Set_IsUsingActorPool(bool _bFlag) { m_bIsUsingActorPool = _bFlag; }

	inline Area* GetArea() const { return m_pArea; }
protected:
	//set to true when enters beginplay()
	bool m_bHasBegunPlay = false;
	//is this actor active?
	bool m_bIsActive = true;

	//is this actor requested to be destroyed in current frame?
	bool m_bIsDestroyRequested = false;

	//is this actor going to be created via actorpool?
	bool m_bIsUsingActorPool = false;

	//letter to draw(image)
	//char* m_pImage = nullptr;
	std::string m_strImg = "";

	//bigger than 1x1 (in fieldlevel)
	std::vector<std::string> m_vecStr_FieldLevel;

	//is using custom image? (1x1 or NxN?)
	//bool m_bIsUsingCustomImg = false;

	//length of string
	int m_iWidth = 1;
	//height of string
	int m_iHeight = 1;

	//color of letter
	Color m_eColor = Color::eWhite;

	//the level this actor belongs to (the level which this actor will be managed by)
	Level* m_pLevel = nullptr;

	//member for quadtree
	Area* m_pArea = nullptr;

	//the bigger, the higher the priority
	int m_iSortingOrder = 0;

	E_LAYER m_eLayer = E_LAYER::E_NONE;
protected:
	RECT m_rtSize = {};

private:
	Vector2 m_vPosition;
	Vector2 m_vPrevPosition;
};

END

#endif // !__ACTOR_H__