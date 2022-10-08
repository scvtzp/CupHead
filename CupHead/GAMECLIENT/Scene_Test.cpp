#include "Scene_Test.h"
#include <HGAMEPLAYER.h>
#include <Logic_Enum.h>
#include <Fade_Com.h>
#include <Game_Collision.h>

Scene_Test::Scene_Test()
{
}
Scene_Test::~Scene_Test()
{
}

void Scene_Test::Init()
{

	if (nullptr == Game_Input::FindKey(L"FADEOUTON"))
	{
		Game_Input::CreateKey(L"FADEOUTON", '0');
	}

	{
		Game_Directory Dic;

		Dic.MoveParent(L"CupHead");
		Dic.Move(L"TEXTURE");
		Dic.Move(L"Test");
		Dic.Move(L"STAGE");

		auto FileList = Dic.DirAllFile();

		for (auto& _File : FileList)
		{
			Game_Texture::Load(_File);
		}

		for (auto& _File : FileList)
		{
			Game_Sprite::Create(_File.FileName(), 1, 1);
		}

		Game_Sprite::Create(L"COLTEST.png", 1, 1);
		Game_Sprite::Create(L"COLTESTS.png", 1, 1);
		Game_Sprite::Create(L"backHill2Col.png", L"StageAni", 2, 2);
	}

	//SCENE()->ColLink((int)COLORDER::COLORDER_PLAYER
	//	, (int)COLORDER::COLORDER_GOLD);

	// 이건 이건 뭘 만든건가요?
	// 존재를 만든거야
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"ACTORCAMERA");
		PTR->TRANS()->WPOS({ 0, 0, -10.0F });
		Game_Ptr<Game_Cam> Cam = PTR->CreateCom<Game_Cam>(0,
			(int)RENDERORDER::RENDERORDER_ACTOR,
			(int)RENDERORDER::RENDERORDER_MAP);
		Cam->CAMSIZE({ 12.8f , 7.2f });
		Cam->MODE(Cam_Mode::ORTH);
	}

	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"UICAMERA");
		PTR->TRANS()->WPOS({ 0, 0, -10.0F });
		Game_Ptr<Game_Cam> Cam = PTR->CreateCom<Game_Cam>(1, (int)RENDERORDER::RENDERORDER_UI);
		Cam->CAMSIZE({ 1280.0f, 720.0f });
		Cam->MODE(Cam_Mode::ORTH);
	}

	// FSM에 점프 함수는 구현되어 있어야한다.
	// 키를 누르면 점프로 이행되는 기능과

	//플레이어
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"FIRSTOBJECT");
		PTR->TRANS()->LSCALE({ 2.0F, 2.0F, 2.0F });
		PTR->TRANS()->LPOS({ 0.0F, 0, -2.0F });
		PLAYER = PTR;
		{
			PLAYERRENDER = PTR->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_ACTOR);
			PLAYERRENDER->DebugOn();
			PLAYERRENDER->LPOS({ -1.0F, 0.0F, 0.0F });
			PLAYERRENDER->SPRITE(L"COLTEST.png");
		}

		{
			Game_Ptr<Game_Collision> ACTORCOL = PTR->CreateCom<Game_Collision>((int)COLORDER::COLORDER_PLAYER_ForUnknown);
			ACTORCOL->LPOS({ 0.0F, 0.0F, 0.0F });
		}

		{
			Game_Ptr<Game_Sprite_Renderer> RENDER = PTR->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_ACTOR);
			RENDER->LPOS({ 0.0F, 0.0F, 0.0F });
			RENDER->SPRITE(L"COLTEST.png");
		}

		PTR->CreateCom<HGAMEPLAYER>();
	}
	//몬스터
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor();
		MONSTER = PTR;
		PTR->TRANS()->WPOS({ 5.0F, 0, -5.0F });
		PTR->TRANS()->WROT({ 0.0F, 0, 45.0F });
		Game_Ptr<Game_Sprite_Renderer> RENDER = PTR->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_ACTOR);
		RENDER->SPRITE(L"COLTESTS.png");

		{
			Game_Ptr<Game_Collision> ACTORCOL = PTR->CreateCom<Game_Collision>((int)COLORDER::COLORDER_PLAYER_ForUnknown);
			ACTORCOL->LPOS({ -1.0F, 0.0F, 0.0F });
		}
	}

	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor();
		PTR->TRANS()->WSCALE({ 675.0f * 0.01f, 144.0f * 0.01f, 1.0f });
		PTR->TRANS()->WPOS({ 0, 0, 20.0F });

		Game_Ptr<Game_Sprite_Renderer> RENDER = PTR->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_MAP);
		RENDER->SPRITE(L"backHill2Col.png");
	}

	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor();
		PTR->TRANS()->WSCALE({ 1280.0f, 720.0f, 1.0f });
		PTR->TRANS()->WPOS({ 0, 0, -9.9F });

		Game_Ptr<Game_Sprite_Renderer> RENDER = PTR->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_UI);
		RENDER->SPRITE(L"FADE.png");

		m_FADE = PTR->CreateCom<Fade_Com>();
	}


	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor();
		PTR->TRANS()->WSCALE({ 448.0F, 96.0F, 1.0f });
		PTR->TRANS()->WPOS({ -400.0, 200, -5.0F });
		Game_Ptr<Game_Sprite_Renderer> RENDER = PTR->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_UI);
		RENDER->SPRITE(L"HPBAR.png");
	}

	m_FADE->FadeOutStart();

}
void Scene_Test::Update()
{
	if (true == Game_Input::Down(L"FADEOUTON"))
	{
	}

	if (true == PLAYERRENDER->Col(COLTYPE::OBB2D, MONSTER->TRANS(), COLTYPE::OBB2D))
	{
		PLAYERRENDER->Color(Game_Vector::RED);
	}
	else
	{
		PLAYERRENDER->Color(Game_Vector::BLACK);
	}

	if (true == Game_Input::Down(L"SCENECHANGE"))
	{
		Game_Scene::ChangeScene(L"Stage_Slime");
	}

}