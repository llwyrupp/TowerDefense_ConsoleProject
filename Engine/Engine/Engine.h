#pragma once
#include "EngineCommon/Engine_Defines.h"
#include "EngineCommon/Engine_Macro.h"

BEGIN(System)
	class InputMgr;
	class Renderer;
	class CollisionMgr;
	//class ObjectMgr;
	class ENGINE_DLL Engine
	{
		//grant the base class asccess to private parts
		//friend class Singleton<Engine>;

		typedef struct tagEngineSetting {
			//frame speed.
			float fFrameRate = 0.f;
			int iWidth = 0;
			int iHeight = 0;
		}ENGINESETTING;

	public:
		//CONSTRUCTOR/DESTRUCTOR
		Engine();
		virtual ~Engine();

	public:
		void Run();
		void QuitEngine();
		void SetMainLevel(class Level* _pLevel);
		void ShutDown();
		void LoadSetting();
	public:
		inline int GetWidth() const { return m_stSetting.iWidth; }
		inline int GetHeight() const { return m_stSetting.iHeight; }
	public:
		Level* Get_MainLevel() const { return m_pMainLevel; }
	public:
		static Engine& Get_Instance();
	protected:
		void BeginPlay();
		void Tick(float _fDeltaTime);
		void Render();

	protected:
		bool m_bIsQuit = false;
	protected:
		ENGINESETTING m_stSetting;
		InputMgr* m_pInput = nullptr;
		Renderer* m_pRenderer = nullptr;
		CollisionMgr* m_pCollisionMgr = nullptr;
		//ObjectMgr* m_pObjectMgr = nullptr;
		class Level* m_pMainLevel = nullptr;
		static Engine* m_pInstance;
	};
END