#include "Scene_Ghost.h"
#include <Player_Stage.h>
#include <LOGIC_Value.h>
#include <Game_Header.h>
#include <Ghost_B.h>
#include <Logic_Enum.h>
#include <Player_UI.h>

void Scene_Ghost::Init()
{
	SCENE()->ColLink((int)COLORDER::Player_Parry_ForMonster, (int)COLORDER::Monster_Parry);

	SCENE()->ColLink((int)COLORDER::COLORDER_Monster_ForUnknown, (int)COLORDER::Monster_Parry);

}

void Scene_Ghost::Update()
{
	Spawn_Time += Game_Time::DeltaTime();
	Stage_Time += Game_Time::DeltaTime();
	Clear_Time += Game_Time::DeltaTime();

	if (Stage_Time > 30.f) //테스트용으로 10초로 함,
	{
		//60초 버티면 승리. 이후 2.5초간 Ui띄워줌.
		ClearUi_Time += Game_Time::DeltaTime();
		if (ClearUi_Time >= 2.5f)
		{
			Game_Scene::ChangeScene(L"WorldMap");
		}
		else
		{
			//ui 출력

		}
	}

	//패배 트리거. 이후 5초간 Ui띄워줌.
	else if (EndTrigger)
	{
		Game_Scene::ChangeScene(L"Title");
	}

	//몹 스폰.
	else if (Spawn_Time > 5.f)
	{
		Spawn_Time = 0.f;

		//float x = (rand() % 1281) - 640.f;
		float x = 0;
		switch (rand()%2)
		{
		case 0:
			x = -640.f;
			break;
		case 1:
			x = 640.f;
			break;
		default:
			break;
		}
		float y = (rand() % 721) - 360.f;

		//몬스터 생성
		{
			Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"Ghost");
			PTR->TRANS()->WSCALE({ 1.f, 1.f, 1.f });
			PTR->TRANS()->WPOS({ x, y, 1.f });

			PTR->CreateCom<Ghost_B>();
		}
	}
}

void Scene_Ghost::SceneChangeStart()
{
	//이미지 로드
	{
		Game_Directory Dic;
		{
			Dic.MoveParent(L"CupHead");
			Dic.Move(L"TEXTURE");
			Dic.Move(L"Object");
			Dic.Move(L"Ghost");

			auto FileList = Dic.DirAllFile();

			for (auto& _File : FileList)
			{
				Game_Texture::Load(_File);
			}

			Game_Sprite::Create(L"Ghost_B.png", 5, 5);
			Game_Sprite::Create(L"Chalice_Shake.png", 5, 5);
			Game_Sprite::Create(L"Ghost_Bg.png", 1, 1);
			Game_Sprite::Create(L"Ghost_Fg.png", 1, 1);
		}
	}

	//카메라 생성
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"ACTORCAMERA");
		PTR->TRANS()->WPOS({ 0.f,  0.f, -50.f });
		Game_Ptr<Game_Cam> Cam = PTR->CreateCom<Game_Cam>(0,
			(int)RENDERORDER::RENDERORDER_ACTOR,
			(int)RENDERORDER::RENDERORDER_MAP,
			(int)RENDERORDER::RENDERORDER_Debug);
		Cam->CAMSIZE({ 1280.f, 720.f });
		Cam->MODE(Cam_Mode::ORTH);
	}

	//노이즈 생성
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"NoiseCam");
		PTR->TRANS()->WPOS({ 0.f,  0.f, -50.f });
		Game_Ptr<Game_Cam> Cam = PTR->CreateCom<Game_Cam>(2,
			(int)RENDERORDER::RENDERORDER_Noise);
		Cam->CAMSIZE({ 1024.f, 512.f });
		Cam->MODE(Cam_Mode::ORTH);
	}

	//플레이어 생성
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"Player_Stage");
		PTR->TRANS()->WSCALE({ 1.f, 1.f, 1.f });
		PTR->TRANS()->WPOS({ -200.f,  -185.f, -1.f });
		PTR->TRANS()->Hit_Scale({ 100.f, 150.f, 1.0f });

		PTR->CreateCom<Player_Stage>();
	}
	//플레이어ui 생성
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"Player_UI");
		PTR->TRANS()->WSCALE({ 1.f, 1.f, 1.f });
		PTR->TRANS()->WPOS({ 0.f,  0.f, -5.f });

		PTR->CreateCom<Player_UI>();
	}

#define Chalice_Collision_Scale { 0.3f, 0.3f, 1.f }
#define Chalice_Collision_LPos {0.f, 0.f, -1.f}

	//성배 생성
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"Chalice");
		Game_Ptr<Game_Sprite_Renderer> RENDER = PTR->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_MAP);
		PTR->TRANS()->WPOS({ 0.f, -35.f, 0.f });
		PTR->TRANS()->WSCALE({ 1.0f, 1.0f, 1.0f });

		Game_Ptr<Game_Animation> FRAMEANI = PTR->CreateCom<Game_Animation>(RENDER);
		FRAMEANI->TRANS()->LSCALE({ 300.0f, 300.0f, 1.0f });
		FRAMEANI->CreateAni(L"Shake", L"Chalice_Shake.png", 0, 21, 0.1f, true);
		FRAMEANI->ChangeAni(L"Shake");

		Game_Ptr<Game_Collision> m_Collision_Chalice = PTR->CreateCom<Game_Collision>((int)COLORDER::COLORDER_Monster_ForUnknown);
		m_Collision_Chalice->ColType(COLTYPE::AABB2D);
		m_Collision_Chalice->LSCALE(Chalice_Collision_Scale);
		m_Collision_Chalice->LPOS(Chalice_Collision_LPos);
		m_Collision_Chalice->PushEnterFunc(this, &Scene_Ghost::Hit);

		m_Collision_Debug_Render = PTR->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Debug);
		m_Collision_Debug_Render->LSCALE(Chalice_Collision_Scale);
		m_Collision_Debug_Render->LPOS(Chalice_Collision_LPos);
		m_Collision_Debug_Render->SPRITE(L"Col.png");
	}

	//배경 생성
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor();
		PTR->TRANS()->WSCALE({ 1380.f, 820.f, 1.0f });
		PTR->TRANS()->WPOS({ 0.f, 0.f, 30.f });
		Game_Ptr<Game_Sprite_Renderer> RENDER = PTR->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_MAP);
		RENDER->SPRITE(L"Ghost_Bg.png");
	}
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor();
		PTR->TRANS()->WSCALE({ 1380.f, 820.f, 1.0f });
		PTR->TRANS()->WPOS({ 0.f, 0.f, -30.f });
		Game_Ptr<Game_Sprite_Renderer> RENDER = PTR->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_MAP);
		RENDER->SPRITE(L"Ghost_Fg.png");
	}

	//노이즈 생성
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"Noise");
		PTR->TRANS()->WSCALE({ 1024.f, 512.f, 1.0f });
		PTR->TRANS()->WPOS({ 0.f, 0.f, -20.f });
		Game_Ptr<Game_Sprite_Renderer> RENDER = PTR->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Noise);

		Game_Ptr<Game_Animation> FRAMEANI = PTR->CreateCom<Game_Animation>(RENDER);
		FRAMEANI->CreateAni(L"Noise", L"Noise1.png", 0, 125, 0.2f, true);
		FRAMEANI->ChangeAni(L"Noise");
	}

}

void Scene_Ghost::SceneChangeEnd()
{
	{
		Game_Directory Dic;

		Dic.MoveParent(L"CupHead");
		Dic.Move(L"TEXTURE");
		Dic.Move(L"Object");
		Dic.Move(L"Ghost");

		auto FileList = Dic.DirAllFile();

		for (auto& _File : FileList)
		{
			Game_Texture::Delete(_File.FileName());
		}

		for (auto& _File : FileList)
		{
			Game_Sprite::Delete(_File.FileName());
		}
	}

	SCENE()->ActorClear();
}
