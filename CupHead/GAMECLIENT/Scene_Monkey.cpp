#include "Scene_Monkey.h"
#include <Player_Sky.h>
#include <Player_UI.h>
#include <Logic_Enum.h>

#include <LOGIC_Value.h>
#include <Game_Header.h>
#include <Monkey.h>

void Scene_Monkey::Init()
{
	SCENE()->ColLink((int)COLORDER::Player_Parry, (int)COLORDER::Monster_Parry);

	SCENE()->ColLink((int)COLORDER::COLORDER_PLAYER_ForHit
		, (int)COLORDER::COLORDER_MONSTER_ForPlayer);

	SCENE()->ColLink((int)COLORDER::COLORDER_Bullet
		, (int)COLORDER::COLORDER_Monster_ForBullet);
}

void Scene_Monkey::Update()
{
}

void Scene_Monkey::SceneChangeStart()
{
	//이미지 로드
	{
		Game_Directory Dic;
	
		{
			Dic.MoveParent(L"CupHead");
			Dic.Move(L"TEXTURE");
			Dic.Move(L"Object");
			Dic.Move(L"Monkey");

			auto FileList = Dic.DirAllFile();

			for (auto& _File : FileList)
			{
				Game_Texture::Load(_File);
			}

			Game_Sprite::Create(L"Monkey_Attack.png", 4, 3);
			Game_Sprite::Create(L"Monkey_Death.png", 4, 3);
			Game_Sprite::Create(L"Monkey_Idle_Closed.png", 4, 3);
			Game_Sprite::Create(L"Monkey_Idle_Open.png", 4, 3);
			Game_Sprite::Create(L"Monkey_Open_to_Close.png", 4, 2);
			Game_Sprite::Create(L"Monkey_Shake.png", 3, 2);
			Game_Sprite::Create(L"Monkey_Closed_to_Open.png", 3, 2);
			Game_Sprite::Create(L"Monkey_Intro.png", 5, 5);
			Game_Sprite::Create(L"Monkey_open_idle.png", 3, 2);

			Game_Sprite::Create(L"Monkey_Claw__open(11)_Closed(22).png", 11, 3);
			Game_Sprite::Create(L"Monkey_Claw_Attack.png", 11, 1);
			Game_Sprite::Create(L"Monkey_Claw_Closed_to_Open.png", 6, 1);
			Game_Sprite::Create(L"Monkey_Claw_Death.png", 11, 1);
			Game_Sprite::Create(L"Monkey_Claw_Open_Idle.png", 5, 1);
			Game_Sprite::Create(L"Monkey_Claw_Open_to_Closed.png", 8, 1);
			Game_Sprite::Create(L"Monkey_Claw_Shake.png", 5, 1);

			Game_Sprite::Create(L"Monkey_MusicNotes.png", 5, 4);

			Game_Sprite::Create(L"Monkey_Fg.png", 1, 1);
			Game_Sprite::Create(L"Monkey_Bg.png", 1, 1);

			Game_Sprite::Create(L"Monkey_Card_Back.png", 4, 3);
			Game_Sprite::Create(L"Monkey_Card_Chips.png", 1, 1);
			Game_Sprite::Create(L"Monkey_Card_Cuphead.png", 1, 1);
			Game_Sprite::Create(L"Monkey_Card_Flowers.png", 1, 1);
			Game_Sprite::Create(L"Monkey_Card_Mugman.png", 1, 1);
			Game_Sprite::Create(L"Monkey_Card_Shield.png", 1, 1);
			Game_Sprite::Create(L"Monkey_Card_Spindle.png", 1, 1);
			Game_Sprite::Create(L"Card_Dot.png", 1, 1);
		}
	}
	//카메라 생성
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"ACTORCAMERA");
		PTR->TRANS()->WPOS({ 0.f,  0.f, -200.f });
		Game_Ptr<Game_Cam> Cam = PTR->CreateCom<Game_Cam>(0,
			(int)RENDERORDER::RENDERORDER_ACTOR,
			(int)RENDERORDER::RENDERORDER_MAP,
			(int)RENDERORDER::RENDERORDER_BACKGROUND,
			(int)RENDERORDER::RENDERORDER_Debug);
		Cam->CAMSIZE({ 1280.f, 720.f });
		Cam->MODE(Cam_Mode::ORTH);
	}

	//노이즈 생성
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"NoiseCam");
		PTR->TRANS()->WPOS({ 0.f,  0.f, -200.f });
		Game_Ptr<Game_Cam> Cam = PTR->CreateCom<Game_Cam>(2,
			(int)RENDERORDER::RENDERORDER_Noise);
		Cam->CAMSIZE({ 1024.f, 512.f });
		Cam->MODE(Cam_Mode::ORTH);
	}

	//플레이어 생성
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"Player_Sky");
		PTR->TRANS()->WSCALE({ 1.f, 1.f, 1.f });
		PTR->TRANS()->WPOS({ -200.f,  -185.f, -5.f });

		PTR->CreateCom<Player_Sky>();
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
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"Monkey");
		PTR->TRANS()->WSCALE({ 1.f, 1.f, 1.f });
		PTR->TRANS()->WPOS({ 200.f,  -185.f, -4.f });

		PTR->CreateCom<Monkey>();
	}

	//배경 생성
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor();
		PTR->TRANS()->WSCALE({ 1380.f, 820.f, 1.0f });
		PTR->TRANS()->WPOS({ 0.f, 0.f, 30.f });
		Game_Ptr<Game_Sprite_Renderer> RENDER = PTR->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_BACKGROUND);
		RENDER->SPRITE(L"Monkey_Bg.png");
	}
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor();
		PTR->TRANS()->WSCALE({ 1380.f, 820.f, 1.0f });
		PTR->TRANS()->WPOS({ 0.f, 0.f, -30.f });
		Game_Ptr<Game_Sprite_Renderer> RENDER = PTR->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_BACKGROUND);
		RENDER->SPRITE(L"Monkey_Fg.png");
	}

	//노이즈 생성
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"Noise");
		PTR->TRANS()->WSCALE({ 1024.f, 512.f, 1.0f });
		PTR->TRANS()->WPOS({ 0.f, 0.f, -200.f });
		Game_Ptr<Game_Sprite_Renderer> RENDER = PTR->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Noise);

		Game_Ptr<Game_Animation> FRAMEANI = PTR->CreateCom<Game_Animation>(RENDER);
		FRAMEANI->CreateAni(L"Noise", L"Noise1.png", 0, 125, 0.2f, true);
		FRAMEANI->ChangeAni(L"Noise");
	}


}

void Scene_Monkey::SceneChangeEnd()
{
	{
		Game_Directory Dic;

		Dic.MoveParent(L"CupHead");
		Dic.Move(L"TEXTURE");
		Dic.Move(L"Object");
		Dic.Move(L"Monkey");

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
 