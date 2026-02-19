#pragma once
#include "EngineCommon/Engine_Defines.h"
#include "EngineCommon/Engine_Macro.h"
#include "EngineCommon/Engine_Enum.h"
#include "EngineCommon/Engine_Struct.h"
#include "Math/Vector2.h"
#include "EngineCommon/RTTI.h"

namespace System {

	class Level;
	class ENGINE_DLL  Actor : public RTTI
	{
		RTTI_DECLARATIONS(Actor, RTTI)

	public:
		Actor(const char* pImage = nullptr, const char* pPath = nullptr, const Vector2& vPos = Vector2::Zero, Color color = Color::eWhite);
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
		void SetPos(const Vector2& vNewPos);
		inline Vector2 GetPos() const { return m_vPosition; }

		inline void SetLevel(Level* pLevel) { m_pLevel = pLevel; }
		inline Level* GetLevel() const { return m_pLevel; }

		inline bool Get_HasBegunPlay() const { return m_bHasBegunPlay; }
		inline bool IsActive() const { return m_bHasBegunPlay; }
		inline bool Get_IsDestroyRequested() const { return m_bIsDestroyRequested; }
		inline void Set_IsDestroyRequested(bool _bFlag) { m_bIsDestroyRequested = _bFlag; }

		inline int Get_SortingOrder() const { return m_iSortingOrder; }

		inline const RECT& GetRect() const { return m_rtSize; }
	protected:
		//beginplay에 들어가면 세팅되는 플래그값
		bool m_bHasBegunPlay = false;

		//활성화상태?
		bool m_bIsActive = true;

		//현재 프레임에 삭제요청 받았는지 확인하는 용도
		bool m_bIsDestroyRequested = false;

		//letter to draw(image)
		//char* m_pImage = nullptr;
		std::string m_strImg = "";

		//bigger than 1x1 (in fieldlevel)
		std::vector<std::string> m_vecStr_FieldLevel;

		//is using custom image? (1x1 or NxN?)
		//bool m_bIsUsingCustomImg = false;

		//length of string
		int m_iStringWidth = 1;
		//height of string
		int m_iStringHeight = 1;

		//color of letter
		Color m_eColor = Color::eWhite;

		//종속된 레벨
		Level* m_pLevel = nullptr;

		//the bigger, the higher the priority
		int m_iSortingOrder = 0;

	protected:
		RECT m_rtSize = {};

	private:
		Vector2 m_vPosition;
	};

}