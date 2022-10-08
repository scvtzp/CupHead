#include "Scene_Title.h"
#include <HGAMEPLAYER.h>
#include <Logic_Enum.h>

void Scene_Title::Init()
{
	{
		Game_Directory Dic;

		Dic.MoveParent(L"CupHead");
		Dic.Move(L"TEXTURE");
		Dic.Move(L"title");

		auto FileList = Dic.DirAllFile();

		for (auto& _File : FileList)
		{
			Game_Texture::Load(_File);
		}

		Game_Sprite::Create(L"css_sprites.png", 8, 1);
		Game_Sprite::Create(L"title_screen_background.png", 1, 1);
	}

	//카메라 생성
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"ACTORCAMERA");
		PTR->TRANS()->WPOS({ 0, 0, -2.0F });
		Game_Ptr<Game_Cam> Cam = PTR->CreateCom<Game_Cam>(0,
			(int)RENDERORDER::RENDERORDER_ACTOR,
			(int)RENDERORDER::RENDERORDER_MAP);
		Cam->CAMSIZE({ 1280.f, 720.f });
		Cam->MODE(Cam_Mode::ORTH);
	}

	//오브젝트 생성
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"FIRSOBJECT");
		{
			Game_Ptr<Game_Sprite_Renderer> RENDER = PTR->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_MAP);
			RENDER->LPOS({ 0, -0.0f, 0 });
			PTR->TRANS()->WSCALE({ 1280.0f, 720.0f, 1.0f });

			Game_Ptr<Game_Animation> FRAMEANI = PTR->CreateCom<Game_Animation>(RENDER);
			FRAMEANI->CreateAni(L"IDLE", L"css_sprites.png", 0, 7, 0.1f, true);
			FRAMEANI->ChangeAni(L"IDLE");
		}
	}

	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor();
		PTR->TRANS()->WSCALE({ 1280.0f, 720.0f, 1.0f });

		PTR->TRANS()->WPOS({ 0, 0, 30.0F });

		Game_Ptr<Game_Sprite_Renderer> RENDER = PTR->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_MAP);
		RENDER->SPRITE(L"title_screen_background.png");
	}
}

void Scene_Title::Update()
{
	if (true == Game_Input::Down(L"SCENECHANGE"))
	{
		Game_Scene::ChangeScene(L"WorldMap");
	}
}
