#include <GAMEWINDOW.h>
#include <time.h>
#include <Game_Time.h>
#include <memory>
#include <Game_Device.h>
#include <Game_Scene.h>
#include <Game_Actor.h>
#include <Game_Texture.h>
#include <Game_Sprite.h>
#include <Game_3D_Debug.h>

#include "Scene_Test.h"
#include "Scene_Title.h"
#include "Scene_WorldMap.h"
#include "Scene_Slime.h"
#include "Scene_Ghost.h"
#include "Scene_Monkey.h"
#include "Shop.h"

#include <Inventory.h>

#pragma comment(lib, "GAMEENGINE.lib")
#pragma comment(lib, "GAMELOGIC.lib")
#pragma comment(lib, "GAMEBASE.lib")

TransData CBTRANSDATA;
Game_Vector CBCOLORDATA = {0.0f, 0.0f, 1.0f, 1.0f};

int __stdcall Start()
{	
	//콘솔창 띄우는것. init(ture)면 콘솔창 나옴.
	Game_3D_Debug::Init();

	Inventory::Set_Inventory_Type(Inventory_Bullet_Type::Peashot);
	Inventory::Set_Inventory_Type(Inventory_Bullet_Type::Spread);
	Inventory::Set_Inven(1, Inventory_Bullet_Type::Peashot);
	Inventory::Set_Inven(2, Inventory_Bullet_Type::Spread);
	Inventory::Peashot_Check = true;
	Inventory::Spread_Check = true;
	Inventory::Inven_Change();

	// 글로벌 로드
	{
		//월드맵 플레이어 로드
		Game_Directory Dic;
		{
			Dic.MoveParent(L"CupHead");
			Dic.Move(L"TEXTURE");
			Dic.Move(L"Object");
			Dic.Move(L"Cuphead_World");

			auto FileList = Dic.DirAllFile();

			for (auto& _File : FileList)
			{
				Game_Texture::Load(_File);
			}
			Game_Sprite::Create(L"World_idle_Down.png", 5, 1);
			Game_Sprite::Create(L"World_idle_DownSide.png", 5, 1);
			Game_Sprite::Create(L"World_idle_Side.png", 5, 1);
			Game_Sprite::Create(L"World_idle_Up.png", 5, 1);
			Game_Sprite::Create(L"World_idle_UpSide.png", 5, 1);

			Game_Sprite::Create(L"World_Down.png", 16, 1);
			Game_Sprite::Create(L"World_DownSide.png", 16, 1);
			Game_Sprite::Create(L"World_Side.png", 16, 1);
			Game_Sprite::Create(L"World_Up.png", 16, 1);
			Game_Sprite::Create(L"World_UpSide.png", 16, 1);
			Game_Sprite::Create(L"World_Win.png", 12, 1);
		}

		//플레이어 로드
		{
			Dic.MoveParent(L"CupHead");
			Dic.Move(L"TEXTURE");
			Dic.Move(L"Object");
			Dic.Move(L"Cuphead_Stage");

			auto FileList = Dic.DirAllFile();

			for (auto& _File : FileList)
			{
				Game_Texture::Load(_File);
			}
			Game_Sprite::Create(L"Stage_Idle.png", 5, 1);
			Game_Sprite::Create(L"Stage_Run.png", 5, 7);
			Game_Sprite::Create(L"Stage_Jump.png", 5, 5);
			Game_Sprite::Create(L"Stage_Parry.png", 5, 4);
			Game_Sprite::Create(L"Stage_Intro.png", 5, 6);
			Game_Sprite::Create(L"Stage_Hit.png", 4, 3);
			Game_Sprite::Create(L"Stage_Down.png", 6, 4);
			Game_Sprite::Create(L"Stage_Dash.png", 4, 2);
		}

		//플레이어 SKY 로드
		{
			Dic.MoveParent(L"CupHead");
			Dic.Move(L"TEXTURE");
			Dic.Move(L"Object");
			Dic.Move(L"Cuphead_Sky");

			auto FileList = Dic.DirAllFile();

			for (auto& _File : FileList)
			{
				Game_Texture::Load(_File);
			}
			Game_Sprite::Create(L"CupHead_Sky_Ghost.png", 5, 5);
			Game_Sprite::Create(L"CupHead_Sky_Idle_D_S_U.png", 4, 3);
			Game_Sprite::Create(L"CupHead_Sky_Intro.png", 6, 7);
			Game_Sprite::Create(L"CupHead_Sky_Mn_Idle_D_S_U.png", 4, 3);
			Game_Sprite::Create(L"CupHead_Sky_Mn_Trans_Down.png", 6, 2);
			Game_Sprite::Create(L"CupHead_Sky_Mn_Trans_Up.png", 6, 2);
			Game_Sprite::Create(L"CupHead_Sky_Parry.png", 5, 4);
			Game_Sprite::Create(L"CupHead_Sky_Super_Idle_D_S_U.png", 4, 3);
			Game_Sprite::Create(L"CupHead_Sky_Super_Intro.png", 5, 3);
			Game_Sprite::Create(L"CupHead_Sky_Super_Trans_Down.png", 5, 2);
			Game_Sprite::Create(L"CupHead_Sky_Super_Trans_Up.png", 6, 2);
			Game_Sprite::Create(L"CupHead_Sky_Trans.png", 4, 1);
			Game_Sprite::Create(L"CupHead_Sky_Trans_Down.png", 6, 2);
			Game_Sprite::Create(L"CupHead_Sky_Trans_Up.png", 6, 2);
		}
		//무기 로드
		{
			Dic.MoveParent(L"CupHead");
			Dic.Move(L"TEXTURE");
			Dic.Move(L"Weapon");

			auto FileList = Dic.DirAllFile();

			for (auto& _File : FileList)
			{
				Game_Texture::Load(_File);
			}
			//Stage
			Game_Sprite::Create(L"Peashot.png", 5, 5);
			Game_Sprite::Create(L"Spread.png", 5, 4);
			Game_Sprite::Create(L"Bouncer.png", 5, 5);
			Game_Sprite::Create(L"Charge_Large.png", 5, 6);
			Game_Sprite::Create(L"Charge_Small.png", 5, 5);

			//Sky
			Game_Sprite::Create(L"Sky_Bomb.png", 4, 2);
			Game_Sprite::Create(L"Sky_Bomb_EX.png", 5, 6);
			Game_Sprite::Create(L"Sky_PeaShot.png", 3, 3);
			Game_Sprite::Create(L"Sky_PeaShot_Hit.png", 4, 3);

			//짬
			Game_Sprite::Create(L"Bravo.png", 3, 9);

		}
		//맵 로드
		{
			Dic.MoveParent(L"CupHead");
			Dic.Move(L"TEXTURE");
			Dic.Move(L"WorldMap");

			auto FileList = Dic.DirAllFile();

			for (auto& _File : FileList)
			{
				Game_Texture::Load(_File);
			}

			Game_Sprite::Create(L"WorldMap1.png", 1, 1);
			Game_Sprite::Create(L"WorldMap1_Hit.png", 1, 1);
			Game_Sprite::Create(L"WorldMap1_Top.png", 1, 1);
			Game_Sprite::Create(L"World1_Icon.png", 3, 5);
			Game_Sprite::Create(L"World_Plat.png", 3, 1);
			Game_Sprite::Create(L"Shop_Coin.png", 5, 5);

		}
		//UI 로드
		{
			Dic.MoveParent(L"CupHead");
			Dic.Move(L"TEXTURE");
			Dic.Move(L"22");

			auto FileList = Dic.DirAllFile();

			for (auto& _File : FileList)
			{
				Game_Texture::Load(_File);
			}

			for (auto& _File : FileList)
			{
				Game_Sprite::Create(_File.FileName(), 1, 1);
			}
		}


		//노이즈 로드
		{
			Dic.MoveParent(L"CupHead");
			Dic.Move(L"TEXTURE");

			auto FileList = Dic.DirAllFile();

			for (auto& _File : FileList)
			{
				Game_Texture::Load(_File);
			}
			//Game_Sprite::Create(L"Noise.png", 8, 16);
			Game_Sprite::Create(L"Noise1.png", 8, 16);
			Game_Sprite::Create(L"Pig_Bye.png", 6, 6);
			Game_Sprite::Create(L"Pig_Idle6_Hi27.png", 6, 6);

		}

		//테스트 로드
		{
			Dic.MoveParent(L"CupHead");
			Dic.Move(L"TEXTURE");
			Dic.Move(L"Test");
			Dic.Move(L"GLOBAL");

			auto FileList = Dic.DirAllFile();

			for (auto& _File : FileList)
			{
				Game_Texture::Load(_File);
			}

			Game_Sprite::Create(L"FADE.png", 21, 1);
			Game_Sprite::Create(L"PLAYER.png", 21, 1);
			Game_Sprite::Create(L"Col.png", 1, 1);
		}
	}

	//키 생성
	if (nullptr == Game_Input::FindKey(L"SCENECHANGE"))
	{
		Game_Input::CreateKey(L"SCENECHANGE", 'L');
	}
	if (nullptr == Game_Input::FindKey(L"PLAYERLEFT"))
	{
		Game_Input::CreateKey(L"PLAYERLEFT", VK_LEFT);
		Game_Input::CreateKey(L"PLAYERRIGHT", VK_RIGHT);
		Game_Input::CreateKey(L"PLAYERUP", VK_UP);
		Game_Input::CreateKey(L"PLAYERDOWN", VK_DOWN);
		Game_Input::CreateKey(L"Dash", VK_SHIFT);
		Game_Input::CreateKey(L"JUMP", 'Z');
		Game_Input::CreateKey(L"SHOOT", 'X');
		Game_Input::CreateKey(L"EX_SHOOT", 'V');
		Game_Input::CreateKey(L"Shoot_Change", VK_TAB);
		Game_Input::CreateKey(L"DebugCheck", 'M');
		Game_Input::CreateKey(L"DebugOn", 'O');
		Game_Input::CreateKey(L"DebugOff", 'P');

		//ui
		Game_Input::CreateKey(L"1", '1');
		Game_Input::CreateKey(L"2", '2');
		Game_Input::CreateKey(L"3", '3');

		//기타
		Game_Input::CreateKey(L"GoldCheat", 'G');


	}

	//씬 생성 
	{
		Game_Scene::Create<Scene_Test>(L"Test");

		Game_Scene::Create<Scene_Title>(L"Title");
		Game_Scene::Create<Scene_WorldMap>(L"WorldMap");
		Game_Scene::Create<Scene_Slime>(L"Stage_Slime");
		Game_Scene::Create<Scene_Ghost>(L"Stage_Ghost");
		Game_Scene::Create<Scene_Monkey>(L"Stage_Monkey");
		Game_Scene::Create<Shop>(L"Shop");

		Game_Scene::Create(L"Ending");

		//Game_Scene::ChangeScene(L"Stage_Monkey");
		//Game_Scene::ChangeScene(L"WorldMap");
		Game_Scene::ChangeScene(L"Title");

	}

	return 1;
}

int __stdcall Loop()
{
	Game_Scene::Progress();
	return 1;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	new int();

	srand((unsigned int)time(NULL));

	GAMEWINDOW::Create(L"MainWindow");
	GAMEWINDOW::MAINOBJ()->Size({ 1280, 720 });
	Game_Device::Create(L"MainWindow");


	GAMEWINDOW::HWINMESSAGE(Start, Loop);
}
