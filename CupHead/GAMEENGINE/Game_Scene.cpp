#include "Game_Scene.h"
#include "Game_Actor.h"
#include "Game_Device.h"
#include "Game_Trans.h"
#include "Game_Cam.h"
#include "Game_Renderer.h"
#include <Game_Input.h>
#include <Game_Time.h>
#include "Game_Collision.h"
#include "Game_3D_Debug.h"

Game_Ptr<Game_Scene> Game_Scene::m_CurScene = nullptr;
Game_Ptr<Game_Scene> Game_Scene::m_NextScene = nullptr;

Game_Scene::Game_Scene() {

}
Game_Scene::~Game_Scene() {

}

void Game_Scene::ChangeScene(const Game_String& _Name) 
{
	m_NextScene = Find(_Name);

	if (nullptr == m_NextScene)
	{
		AMSG(L"존재하지 않는 씬을 체인지 하려고 했습니다.");
	}

}

Game_Ptr<Game_Actor> Game_Scene::CreateActor(const Game_String& _Name) 
{
	Game_Ptr<Game_Actor> Actor = Game_UpDate_ListObj_Mgr<Game_Actor>::CreateObj<Game_Actor>(_Name);
	// 이걸 집어넣어주니까 액터는 위치가 존재하게 된다.
	// 컴포넌트 구조에서는 위치라는 것도 기능(컴포넌트)이다.
	// 씬을 알게해줘야 그걸 가지고 여러가지 일을 할수 있다.
	Actor->m_SCENE = this;
	Actor->CreateCom<Game_Trans>();
	return Actor;
}

void Game_Scene::Progress() 
{
	if (nullptr != m_NextScene)
	{
		// 최초
		if (nullptr == m_CurScene)
		{
			m_CurScene = m_NextScene;
			m_CurScene->SceneChangeStart();
			Game_Time::TimeReset();
			m_NextScene = nullptr;
		}
		else 
		{
			m_CurScene->SceneChangeEnd();
			m_CurScene = m_NextScene;
			m_CurScene->SceneChangeStart();
			Game_Time::TimeReset();
			m_NextScene = nullptr;
		}
	}

	Game_Input::Update();
	Game_Time::Update();

	m_CurScene->UpdatePrev();
	m_CurScene->Update();
	m_CurScene->UpdateNext();

	m_CurScene->RenderPrev();
	m_CurScene->Render();
	m_CurScene->RenderNext();


	m_CurScene->CollisionPrev();
	m_CurScene->Collision();
	m_CurScene->CollisionNext();

	m_CurScene->Release();
}

bool Game_Scene::ZSORT(Game_Ptr<Game_Renderer>& _Left, Game_Ptr<Game_Renderer>& _Right)
{
	Game_Vector LeftZ = _Left->WPOS();
	Game_Vector RightZ = _Right->WPOS();

	// 랜더러가 트랜스폼을 상속받은 이유
	return _Left->WPOS().Z > _Right->WPOS().Z;
}

void Game_Scene::Render()
{
	Game_Device::MAINOBJ()->RenderStart();

	for (auto& _RenderGroup : m_AllRender)
	{
		_RenderGroup.second.sort(&Game_Scene::ZSORT);

	}

	// 캠을 모두 돌린다.
	for (auto& _Cam : m_AllCam)
	{
		// 캠이 바라보는 랜더 그룹의 인덱스를 모두 탐색하고
		for (int RenderOrder : _Cam.second->m_VIEWRENDERERS)
		{
			// 그 인덱스에 해당하는 랜더 그룹을 찾는다.
			std::map<int, std::list<Game_Ptr<Game_Renderer>>>::iterator RenderGroup = m_AllRender.find(RenderOrder);

			if (m_AllRender.end() == RenderGroup)
			{
				continue;
			}

			// std::list<Game_Ptr<Game_Renderer>>
			for (auto& _Render : RenderGroup->second)
			{
				if (false == _Render->IsUpdate())
				{
					continue;
				}
				_Render->Render(_Cam.second);
			}
		}
	}

	{
		Game_3D_Debug::DebugRender();
	}

	Game_Device::MAINOBJ()->RenderEnd();

}

void Game_Scene::Collision()
{
	// 누구누구들이 충돌해야 하는가?
	// std::map<int, std::set<int>> m_ColLink;
	// 그 그룹은 이거다.
	// std::map<int, std::list<Game_Ptr<Game_Collision>>> m_AllCol;
	// std::list<Game_Ptr<Game_Collision>>

	for (auto& LeftIndex : m_ColLink)
	{
		std::map<int, std::list<Game_Ptr<Game_Collision>>>::iterator LeftGroup = m_AllCol.find(LeftIndex.first);
		if (LeftGroup == m_AllCol.end())
		{
			continue;
		}

		// 자료형 std::list<Game_Ptr<Game_Collision>>
		if (true == LeftGroup->second.empty())
		{
			continue;
		}

		for (int RightIndex : LeftIndex.second)
		{
			std::map<int, std::list<Game_Ptr<Game_Collision>>>::iterator RightGroup = m_AllCol.find(RightIndex);

			if (RightGroup == m_AllCol.end())
			{
				continue;
			}

			if (true == RightGroup->second.empty())
			{
				continue;
			}


			if (LeftIndex.first == RightIndex)
			{
				std::list<Game_Ptr<Game_Collision>>::iterator LeftStart = LeftGroup->second.begin();

				if (false == (*LeftStart)->IsUpdate())
				{
					continue;
				}

				std::list<Game_Ptr<Game_Collision>>::iterator LeftEnd = RightGroup->second.end();
				--LeftEnd;


				// 0 1 2 
				for (; LeftStart != LeftEnd; ++LeftStart)
				{
					std::list<Game_Ptr<Game_Collision>>::iterator RightStart = LeftStart;
					std::list<Game_Ptr<Game_Collision>>::iterator RightEnd = RightGroup->second.end();
					++RightStart;

					for (; RightStart != RightEnd; ++RightStart)
					{
						if (false == (*RightStart)->IsUpdate())
						{
							continue;
						}

						(*LeftStart)->ColCheck(*RightStart);
					}
				}


			}
			else
			{


				// 자기 그룹과의 충돌은 생각 안한겁니다.
				for (auto& LeftCol : LeftGroup->second)
				{

					if (false == LeftCol->IsUpdate())
					{
						continue;
					}

					for (auto& RightCol : RightGroup->second)
					{
						if (false == RightCol->IsUpdate())
						{
							continue;
						}

						LeftCol->ColCheck(RightCol);
					}
				}
			}


		}
	}
}

void Game_Scene::UpdatePrev() 			  
{
	Game_UpDate_ListObj_Mgr<Game_Scene_Com>::UpdatePrev();
	Game_UpDate_ListObj_Mgr<Game_Actor>::UpdatePrev();
}
void Game_Scene::Update() 				  
{
	Game_UpDate_ListObj_Mgr<Game_Scene_Com>::Update();
	Game_UpDate_ListObj_Mgr<Game_Actor>::Update();
}
void Game_Scene::UpdateNext() 			  
{
	Game_UpDate_ListObj_Mgr<Game_Scene_Com>::UpdateNext();
	Game_UpDate_ListObj_Mgr<Game_Actor>::UpdateNext();

}
void Game_Scene::RenderPrev() 			  
{
	Game_UpDate_ListObj_Mgr<Game_Scene_Com>::RenderPrev();
	Game_UpDate_ListObj_Mgr<Game_Actor>::RenderPrev();

	// 부모가 없는 가장 최상의 트랜스폼을 모아놓은 자료구조인것이다.
	for (auto& _TransForm : m_AllTrans)
	{
		_TransForm->TransUpdate();
	}
}
void Game_Scene::RenderNext() 			  
{
	Game_UpDate_ListObj_Mgr<Game_Scene_Com>::RenderNext();
	Game_UpDate_ListObj_Mgr<Game_Actor>::RenderNext();
}
void Game_Scene::CollisionPrev()		  
{
	Game_UpDate_ListObj_Mgr<Game_Scene_Com>::CollisionPrev();
	Game_UpDate_ListObj_Mgr<Game_Actor>::CollisionPrev();
}
void Game_Scene::CollisionNext()		  
{
	Game_UpDate_ListObj_Mgr<Game_Scene_Com>::CollisionNext();
	Game_UpDate_ListObj_Mgr<Game_Actor>::CollisionNext();
}
void Game_Scene::SceneChangeEnd() 		  
{
	Game_UpDate_ListObj_Mgr<Game_Scene_Com>::SceneChangeEnd();
	Game_UpDate_ListObj_Mgr<Game_Actor>::SceneChangeEnd();
}
void Game_Scene::SceneChangeStart() 	  
{
	Game_UpDate_ListObj_Mgr<Game_Scene_Com>::SceneChangeStart();
	Game_UpDate_ListObj_Mgr<Game_Actor>::SceneChangeStart();
}

typedef std::list<Game_UpDate_ListObj_Mgr<Game_Scene_Com>::LISTDATA>::iterator SceneComIter;
typedef std::list<Game_UpDate_ListObj_Mgr<Game_Actor>::LISTDATA>::iterator ActorIter;

void Game_Scene::Release()
{
	// SceneComRelease
	{
		SceneComIter Start = Game_UpDate_ListObj_Mgr<Game_Scene_Com>::m_ObjList.begin();
		SceneComIter End = Game_UpDate_ListObj_Mgr<Game_Scene_Com>::m_ObjList.end();

		for (; Start != End; )
		{
			if (true == Start->Obj->IsDeath())
			{
				Start = Game_UpDate_ListObj_Mgr<Game_Scene_Com>::m_ObjList.erase(Start);
			}
			else
			{
				++Start;
			}
		}
	}

	// Actor
	{
		// 가장 상위에 있는 녀석이 가장 마지막에 지워지는게 좋다.
		// 랜더러를 지운다.

		// Trans
		{
			std::list<Game_Ptr<Game_Trans>>::iterator Start = m_AllTrans.begin();
			std::list<Game_Ptr<Game_Trans>>::iterator End = m_AllTrans.end();

			for (; Start != End; )
			{
				if (true == (*Start)->IsDeath())
				{
					Start = m_AllTrans.erase(Start);
				}
				else
				{
					++Start;
				}
			}
		}

		// Col
		{
			std::map<int, std::list<Game_Ptr<Game_Collision>>>::iterator Start = m_AllCol.begin();
			std::map<int, std::list<Game_Ptr<Game_Collision>>>::iterator End = m_AllCol.end();

			for (; Start != End; ++Start)
			{
				std::list<Game_Ptr<Game_Collision>>::iterator StartCol = Start->second.begin();
				std::list<Game_Ptr<Game_Collision>>::iterator EndCol = Start->second.end();

				for (; StartCol != EndCol; )
				{
					if (true == (*StartCol)->IsDeath())
					{
						StartCol = Start->second.erase(StartCol);
					}
					else
					{
						++StartCol;
					}

				}

			}
		}

		// Cam
		{
			std::map<int, Game_Ptr<Game_Cam>>::iterator Start = m_AllCam.begin();
			std::map<int, Game_Ptr<Game_Cam>>::iterator End = m_AllCam.end();

			for (; Start != End; )
			{
				if (true == Start->second->IsDeath())
				{
					if (m_MainCam == Start->second)
					{
						m_MainCam = nullptr;
					}
					Start = m_AllCam.erase(Start);
				}
				else
				{
					++Start;
				}
			}
		}

		// Renderer
		{
			std::map<int, std::list<Game_Ptr<Game_Renderer>>>::iterator Start = m_AllRender.begin();
			std::map<int, std::list<Game_Ptr<Game_Renderer>>>::iterator End = m_AllRender.end();

			for (; Start != End; ++Start)
			{
				std::list<Game_Ptr<Game_Renderer>>::iterator StartRender = Start->second.begin();
				std::list<Game_Ptr<Game_Renderer>>::iterator EndRender = Start->second.end();

				for (; StartRender != EndRender; )
				{
					if (true == (*StartRender)->IsDeath())
					{
						StartRender = Start->second.erase(StartRender);
					}
					else
					{
						++StartRender;
					}

				}

			}
		}

		//Actor
		{
			ActorIter Start = Game_UpDate_ListObj_Mgr<Game_Actor>::m_ObjList.begin();
			ActorIter End = Game_UpDate_ListObj_Mgr<Game_Actor>::m_ObjList.end();

			for (; Start != End; )
			{
				if (true == Start->Obj->IsDeath())
				{
					Start->Obj->Release();
					Start = Game_UpDate_ListObj_Mgr<Game_Actor>::m_ObjList.erase(Start);
				}
				else
				{
					++Start;
				}
			}
		}
	}
}

void Game_Scene::PushCam(Game_Ptr<Game_Cam> _Cam) 
{
	if (m_AllCam.end() != m_AllCam.find(_Cam->Order()))
	{
		AMSG(L"카메라의 오더가 겹쳤습니다.");
	}


	if (nullptr == m_MainCam)
	{
		m_MainCam = _Cam;
	}

	m_AllCam[_Cam->Order()] = _Cam;
}

void Game_Scene::PushRender(Game_Ptr<Game_Renderer> _Render)
{
	if (m_AllRender.end() == m_AllRender.find(_Render->Order()))
	{
		m_AllRender.insert(
			std::map<int, std::list<Game_Ptr<Game_Renderer>>>::value_type(_Render->Order(), 
				std::list<Game_Ptr<Game_Renderer>>()));
	}

	m_AllRender[_Render->Order()].push_back(_Render);
}

void Game_Scene::PushCol(Game_Ptr<Game_Collision> _Col)
{
	if (m_AllCol.end() == m_AllCol.find(_Col->Order()))
	{
		m_AllCol.insert(
			std::map<int, std::list<Game_Ptr<Game_Collision>>>::value_type(_Col->Order(),
				std::list<Game_Ptr<Game_Collision>>()));
	}

	m_AllCol[_Col->Order()].push_back(_Col);
}

void Game_Scene::ActorClear()
{
	m_AllCol.clear();
	m_AllCam.clear();
	m_AllRender.clear();
	m_AllTrans.clear();
	Game_UpDate_ListObj_Mgr<Game_Actor>::m_ObjList.clear();
	m_MainCam = nullptr;
}