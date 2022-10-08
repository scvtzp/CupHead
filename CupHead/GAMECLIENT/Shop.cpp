#include "Shop.h"
#include <Logic_Enum.h>
#include <Game_Header.h>
#include <Inventory.h>
#include <Player_Map.h>

#define Shop_Size {100.f, 150.f, 1.f}
#define Shop_Size_Up {120.f, 175.f, 1.f}

bool Shop::SoldCheck[7] = {true, true, true, true, true, true };

void Shop::Init()
{
}

void Shop::Update()
{
	if (Game_Input::Down(L"GoldCheat"))
	{
		if(Player_Map::Gold < 9)
			Player_Map::Gold +=1;
	}
	else if (Player_Map::Gold <= 0)
	{
		Player_Map::Gold = 1;
	}


	//퇴장
	if (true == Game_Input::Down(L"SCENECHANGE"))
	{
		EndTriger = true;
	}
	if (EndTriger)
	{
		Pig_Ani->ChangeAni(L"Bye");
		if (Pig_Ani->Get_Num() >= Pig_Ani->Get_EndNum())
		{
			EndTriger = false;
			Game_Scene::ChangeScene(L"WorldMap");
		}
	}
	//입장
	if (Pig_Ani->Get_Ani() == L"Hi")
	{
		if (Pig_Ani->Get_Num() >= Pig_Ani->Get_EndNum())
			Pig_Ani->ChangeAni(L"Idle");
	}

	//아이콘
	if(SoldCheck[0])
		Hp1_Renderer->SPRITE(L"shop_charm_hp-one.png");
	else
		Hp1_Renderer->SPRITE(L"shop_charm_hp-one_dim.png");

	if(SoldCheck[1])
		Bounce_Renderer->SPRITE(L"shop_weapon_bounce.png");
	else
		Bounce_Renderer->SPRITE(L"shop_weapon_bounce_dim.png");

	if(SoldCheck[2])
		Coffee_Renderer->SPRITE(L"shop_charm_coffee.png");
	else
		Coffee_Renderer->SPRITE(L"shop_charm_coffee_dim.png");

	if (SoldCheck[3])
		Hp2_Renderer->SPRITE(L"shop_item_hp2.png");
	else
		Hp2_Renderer->SPRITE(L"shop_item_hp2_dim.png");

	if (SoldCheck[4])
		Cube_Renderer->SPRITE(L"shop_item_sugarcube.png");
	else
		Cube_Renderer->SPRITE(L"shop_item_sugarcube_dim.png");

	if (SoldCheck[5])
		Charge_Renderer->SPRITE(L"shop_weapon_charge.png");
	else
		Charge_Renderer->SPRITE(L"shop_weapon_charge_dim.png");

	//이동
	if (Game_Input::Down(L"PLAYERRIGHT"))
	{
		TargetNum++;

		for (int i = TargetNum; i <= 6; i++)
		{
			if (i == 6)
				i = 0;
			if (!SoldCheck[TargetNum])
				TargetNum++;
			else
				break;
		}

		if (TargetNum > 5)
			TargetNum = 0;
	}
	else if (Game_Input::Down(L"PLAYERLEFT"))
	{
		TargetNum--;
		for (int i = TargetNum; i >= -1; i--)
		{
			if (i == -1)
				i = 5;
			if (!SoldCheck[TargetNum])
				TargetNum--;
			else
				break;
		}

		if (TargetNum < 0)
			TargetNum = 5;
	}

	Hp1_Renderer->LSCALE(Shop_Size);
	Bounce_Renderer->LSCALE(Shop_Size);
	Coffee_Renderer->LSCALE(Shop_Size);
	Hp2_Renderer->LSCALE(Shop_Size);
	Cube_Renderer->LSCALE({ 130.f, 130.f, 1.f });
	Charge_Renderer->LSCALE(Shop_Size);


	switch (TargetNum)
	{
	case 0:
		Hp1_Renderer->LSCALE(Shop_Size_Up);
		break;
	case 1:
		Bounce_Renderer->LSCALE(Shop_Size_Up);
		break;
	case 2:
		Coffee_Renderer->LSCALE(Shop_Size_Up);
		break;
	case 3:
		Hp2_Renderer->LSCALE(Shop_Size_Up);
		break;
	case 4:
		Cube_Renderer->LSCALE({ 160.f, 160.f, 1.f });
		break;
	case 5:
		Charge_Renderer->LSCALE(Shop_Size_Up);
		break;
	default:
		break;
	}

	//돈
	switch (Player_Map::Gold)
	{
	case 1:
		Money->SPRITE(L"shopkeep_numbers_1.png");
		break;
	case 2:
		Money->SPRITE(L"shopkeep_numbers_2.png");
		break;
	case 3:
		Money->SPRITE(L"shopkeep_numbers_3.png");
		break;
	case 4:
		Money->SPRITE(L"shopkeep_numbers_4.png");
		break;
	case 5:
		Money->SPRITE(L"shopkeep_numbers_5.png");
		break;
	case 6:
		Money->SPRITE(L"shopkeep_numbers_6.png");
		break;
	case 7:
		Money->SPRITE(L"shopkeep_numbers_7.png");
		break;
	case 8:
		Money->SPRITE(L"shopkeep_numbers_8.png");
		break;
	case 9:
		Money->SPRITE(L"shopkeep_numbers_9.png");
		break;
	default:
		Money->SPRITE(L"Col.png");
		break;
	}

	//구매
	if (Game_Input::Down(L"JUMP"))
	{
		if (Player_Map::Gold >= 3)
			Player_Map::Gold -= 3;
		else
			return;
		switch (TargetNum)
		{
		case 0:
			Inventory::Set_Inventory_Ab_Type(Inventory_Ability_Type::Hp1);
			break;
		case 1:
			Inventory::Set_Inventory_Type(Inventory_Bullet_Type::Bouncer);
			Inventory::Bouncer_Check = true;
			break;
		case 2:
			Inventory::Set_Inventory_Ab_Type(Inventory_Ability_Type::Coffee);
			break;
		case 3:
			Inventory::Set_Inventory_Ab_Type(Inventory_Ability_Type::Hp2);
			break;
		case 4:
			Inventory::Set_Inventory_Ab_Type(Inventory_Ability_Type::Cube);
			break;
		case 5:
			Inventory::Set_Inventory_Type(Inventory_Bullet_Type::Charge);
			Inventory::Charge_Check = true;
			break;
		default:
			break;
		}

		SoldCheck[TargetNum] = false;
	}



}

void Shop::SceneChangeStart()
{
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
	//배경 생성
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor();
		PTR->TRANS()->WSCALE({ 1280.f, 720.f, 1.0f });
		PTR->TRANS()->WPOS({ 0.f, 0.f, 30.f });
		Game_Ptr<Game_Sprite_Renderer> RENDER = PTR->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_MAP);
		RENDER->SPRITE(L"Shop_Bg.png");
	}
	//돈
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor();
		PTR->TRANS()->WSCALE({ 61.f, 59.f, 1.0f });
		PTR->TRANS()->WPOS({ 550.f, 270.f, 0.f });
		Money = PTR->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_MAP);
		Money->SPRITE(L"Col.png"); //up에서 실시간 변경.
	}

	//코인
	{
		Coin_Actor = SCENE()->CreateActor();
		Coin_Actor->TRANS()->WSCALE({ 128.f, 128.f, 1.0f });
		Coin_Actor->TRANS()->WPOS({ 590.f, 270.f, 0.f });
		Coin_Renderer = Coin_Actor->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_MAP);
		Coin_Ani = Coin_Actor->CreateCom<Game_Animation>(Coin_Renderer);
		Coin_Ani->CreateAni(L"Idle", L"Shop_Coin.png", 0, 22, 0.11f, true);
		Coin_Ani->ChangeAni(L"Idle");
	}

	//돼지
	{
		Pig_Actor = SCENE()->CreateActor(L"Pig");
		Pig_Actor->TRANS()->WSCALE({ 800.f, 800.f, 1.0f });
		Pig_Actor->TRANS()->WPOS({ 0.f, 100.f, -10.f });
		Pig = Pig_Actor->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Noise);
		Pig_Ani = Pig_Actor->CreateCom<Game_Animation>(Pig);
		Pig_Ani->CreateAni(L"Hi", L"Pig_Idle6_Hi27.png", 6, 32, 0.08f, false);
		Pig_Ani->CreateAni(L"Bye", L"Pig_Bye.png", 0, 33, 0.08f, false);
		Pig_Ani->CreateAni(L"Idle", L"Pig_Idle6_Hi27.png", 0, 5, 0.1f, true);
		Pig_Ani->ChangeAni(L"Hi");
	}


	//상품
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor();
		Hp1_Renderer = PTR->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_MAP);
		Hp1_Renderer->LSCALE(Shop_Size);
		Hp1_Renderer->LPOS({ -500.f, 0.f, -2.f });
		Hp1_Renderer->SPRITE(L"shop_charm_hp-one.png");
	}
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor();
		Bounce_Renderer = PTR->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_MAP);
		Bounce_Renderer->LSCALE(Shop_Size);
		Bounce_Renderer->LPOS({ -370.f, 0.f, -2.f });
		Bounce_Renderer->SPRITE(L"shop_weapon_bounce.png");
	}
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor();
		Coffee_Renderer = PTR->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_MAP);
		Coffee_Renderer->LSCALE(Shop_Size);
		Coffee_Renderer->LPOS({ -240.f, 0.f, -2.f });
		Coffee_Renderer->SPRITE(L"shop_charm_coffee.png");
	}
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor();
		Hp2_Renderer = PTR->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_MAP);
		Hp2_Renderer->LSCALE(Shop_Size);
		Hp2_Renderer->LPOS({ 240.f, 0.f, -2.f });
		Hp2_Renderer->SPRITE(L"shop_item_hp2.png");
	}
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor();
		Cube_Renderer = PTR->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_MAP);
		Cube_Renderer->LSCALE({130.f, 130.f, 1.f});
		Cube_Renderer->LPOS({ 370.f, 0.f, -2.f });
		Cube_Renderer->SPRITE(L"shop_item_sugarcube.png");
	}
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor();
		Charge_Renderer = PTR->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_MAP);
		Charge_Renderer->LSCALE(Shop_Size);
		Charge_Renderer->LPOS({ 500.f, 0.f, -2.f });
		Charge_Renderer->SPRITE(L"shop_weapon_charge.png");
	}
}

void Shop::SceneChangeEnd()
{
	SCENE()->ActorClear();
}
