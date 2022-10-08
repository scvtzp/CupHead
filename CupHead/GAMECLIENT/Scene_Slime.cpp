#include "Scene_Slime.h"
#include <Player_Stage.h>
#include <Slime.h>
#include <LOGIC_Value.h>
#include <Game_Header.h>
#include <Logic_Enum.h>
#include <Player_UI.h>
void Scene_Slime::Init()
{
	SCENE()->ColLink((int)COLORDER::COLORDER_PLAYER_ForHit
		, (int)COLORDER::COLORDER_MONSTER_ForPlayer);

	SCENE()->ColLink((int)COLORDER::COLORDER_Bullet
		, (int)COLORDER::COLORDER_Monster_ForBullet);

	SCENE()->ColLink((int)COLORDER::COLORDER_PLAYER_ForUnknown, (int)COLORDER::COLORDER_Monster_ForUnknown);

}

void Scene_Slime::Update()
{



	if (true == Game_Input::Down(L"SCENECHANGE"))
	{
		Game_Scene::ChangeScene(L"Title");
	}
}

void Scene_Slime::SceneChangeStart()
{
	//이미지 로드
	{
		Game_Directory Dic;

		//배경
		{
			Dic.MoveParent(L"CupHead");
			Dic.Move(L"TEXTURE");
			Dic.Move(L"Object");
			Dic.Move(L"Slime");

			auto FileList = Dic.DirAllFile();

			for (auto& _File : FileList)
			{
				Game_Texture::Load(_File);
			}

			Game_Sprite::Create(L"Slime_Bg.png", 1, 1);
		}
		{
			Dic.MoveParent(L"CupHead");
			Dic.Move(L"TEXTURE");
			Dic.Move(L"Object");
			Dic.Move(L"Slime");
			Dic.Move(L"Mn");

			auto FileList = Dic.DirAllFile();

			for (auto& _File : FileList)
			{
				Game_Texture::Load(_File);
			}

			Game_Sprite::Create(L"Mn_Slime_intro.png", 5, 6);
			Game_Sprite::Create(L"Mn_Slime_Jump.png", 5, 2);
			Game_Sprite::Create(L"Mn_Slime_Punch.png", 5, 4);
			Game_Sprite::Create(L"Mn_Slime_Morph.png", 7, 7);
			Game_Sprite::Create(L"Mn_Slime_Air.png", 4, 3);
		}
		{
			Dic.MoveParent(L"CupHead");
			Dic.Move(L"TEXTURE");
			Dic.Move(L"Object");
			Dic.Move(L"Slime");
			Dic.Move(L"Lg");

			auto FileList = Dic.DirAllFile();

			for (auto& _File : FileList)
			{
				Game_Texture::Load(_File);
			}

			Game_Sprite::Create(L"Lg_Slime_Punch.png", 5, 4);
			Game_Sprite::Create(L"Lg_Slime_Jump.png", 4, 2);
			Game_Sprite::Create(L"Lg_Slime_Death.png", 5, 4);
			Game_Sprite::Create(L"Lg_Slime_Air.png", 5, 4);
			Game_Sprite::Create(L"Lg_Slime_Explode.png", 4, 4);
			Game_Sprite::Create(L"Lg_Slime_Falling.png", 2, 2);
		}
		{
			Dic.MoveParent(L"CupHead");
			Dic.Move(L"TEXTURE");
			Dic.Move(L"Object");
			Dic.Move(L"Slime");
			Dic.Move(L"To");

			auto FileList = Dic.DirAllFile();

			for (auto& _File : FileList)
			{
				Game_Texture::Load(_File);
			}

			Game_Sprite::Create(L"To_Slime_Death.png", 3, 2);
			Game_Sprite::Create(L"To_Slime_Dust.png", 4, 5);
			Game_Sprite::Create(L"To_Slime_Move_L.png", 3, 3);
			Game_Sprite::Create(L"To_Slime_Move_R.png", 3, 3);
			Game_Sprite::Create(L"To_Slime_Slam.png", 2, 2);
			Game_Sprite::Create(L"To_Slime_Smash.png", 4, 4);
			Game_Sprite::Create(L"To_Slime_Trans.png", 2, 2);
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
		PTR->TRANS()->WSCALE({ 1.f, 1.f, 1.0f });
		PTR->TRANS()->WPOS({ -200.f,  -300.f, -1.f });
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
	//몬스터 생성
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"Slime");
		PTR->TRANS()->WSCALE({ 1.f, 1.f, 1.0f });
		PTR->TRANS()->WPOS({ 300.f,  -235.f, 1.f });

		PTR->CreateCom<Slime>();
	}
	
	//배경 생성
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor();
		PTR->TRANS()->WSCALE({ 1280.f, 720.f, 1.0f });
		PTR->TRANS()->WPOS({ 0.f, 0.f, 30.f });
		Game_Ptr<Game_Sprite_Renderer> RENDER = PTR->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_MAP);
		RENDER->SPRITE(L"Slime_Bg.png");
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

void Scene_Slime::SceneChangeEnd()
{
	{
		// 플레이어가 무슨 택스처를 가지고 있다.

		Game_Directory Dic;
		{
			Dic.MoveParent(L"CupHead");
			Dic.Move(L"TEXTURE");
			Dic.Move(L"Object");
			Dic.Move(L"Slime");

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
		{
			Dic.MoveParent(L"CupHead");
			Dic.Move(L"TEXTURE");
			Dic.Move(L"Object");
			Dic.Move(L"Slime");
			Dic.Move(L"Mn");

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
		{
			Dic.MoveParent(L"CupHead");
			Dic.Move(L"TEXTURE");
			Dic.Move(L"Object");
			Dic.Move(L"Slime");
			Dic.Move(L"Lg");

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
		{
			Dic.MoveParent(L"CupHead");
			Dic.Move(L"TEXTURE");
			Dic.Move(L"Object");
			Dic.Move(L"Slime");
			Dic.Move(L"To");

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
	}

	SCENE()->ActorClear();
}
