#include "Player_Stage.h"
#include "Game_FreeCam.h"
#include <Game_3D_Debug.h>
#include "Peashot.h" 
#include "Game_Header.h"
#include "Logic_Enum.h"
#include "Spread.h"
#include "Inventory.h"
#include "Bouncer.h"
#include "Monkey_Card.h"
#include "Monkey.h"
#include "Player_UI.h"
#include "Charge.h"

#define Player_Collision_Scale_Default { 100.f, 100.f, 1.f }
#define Player_Collision_LPos_Default { 0.0F, +50.0F, 0.0F }
#define Player_Collision_LPos_Y 50.f
#define Player_Collision_Scale_Y 100.f

void Player_Stage::Init()
{

	m_Gravity = 50.F;
	m_Speed = 300.0F;
	m_Cam = SCENE()->MainCam();

	m_eState = Player_Stage_State::Intro;

	Inventory::Inven_Change();

	{
		//충돌
		m_Collision = ACTOR()->CreateCom<Game_Collision>((int)COLORDER::COLORDER_PLAYER_ForHit);
		m_Collision->ColType(COLTYPE::AABB2D);
		m_Collision->LSCALE(Player_Collision_Scale_Default);
		m_Collision->LPOS(Player_Collision_LPos_Default);
		m_Collision->PushEnterFunc(this, &Player_Stage::Hit);
		m_Collision->PushStayFunc(this, &Player_Stage::Hit);

		m_Collision_1 = ACTOR()->CreateCom<Game_Collision>((int)COLORDER::COLORDER_PLAYER_ForUnknown);
		m_Collision_1->ColType(COLTYPE::AABB2D);
		m_Collision_1->LSCALE(Player_Collision_Scale_Default);
		m_Collision_1->LPOS(Player_Collision_LPos_Default);

		m_Collision_Player_Parry_ForMonster = ACTOR()->CreateCom<Game_Collision>((int)COLORDER::Player_Parry_ForMonster);
		m_Collision_Player_Parry_ForMonster->ColType(COLTYPE::AABB2D);
		m_Collision_Player_Parry_ForMonster->LSCALE(Player_Collision_Scale_Default);
		m_Collision_Player_Parry_ForMonster->LPOS(Player_Collision_LPos_Default);
		m_Collision_Player_Parry_ForMonster->PushEnterFunc(this, &Player_Stage::Parry_HitCheck);
		m_Collision_Player_Parry_ForMonster->PushStayFunc(this, &Player_Stage::Parry_HitCheck);

		m_Collision_Player_Parry = ACTOR()->CreateCom<Game_Collision>((int)COLORDER::Player_Parry);
		m_Collision_Player_Parry->ColType(COLTYPE::AABB2D);
		m_Collision_Player_Parry->LSCALE(Player_Collision_Scale_Default);
		m_Collision_Player_Parry->LPOS(Player_Collision_LPos_Default);
		m_Collision_Player_Parry->PushEnterFunc(this, &Player_Stage::Parry_Check);
		m_Collision_Player_Parry->PushStayFunc(this, &Player_Stage::Parry_Check);

		//충돌 범위 테스트용 출력
		m_Collision_Debug_Render = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Debug);
		m_Collision_Debug_Render->LSCALE(m_Collision->LSCALE());
		m_Collision_Debug_Render->LPOS({ m_Collision->LPOS().X,m_Collision->LPOS().Y, -2.f });
		m_Collision_Debug_Render->SPRITE(L"Col.png");

		m_Collision_Player_Parry_ForMonster_Debug_Render = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Debug);
		m_Collision_Player_Parry_ForMonster_Debug_Render->LSCALE(m_Collision_Player_Parry_ForMonster->LSCALE());
		m_Collision_Player_Parry_ForMonster_Debug_Render->LPOS({ m_Collision_Player_Parry_ForMonster->LPOS().X,m_Collision_Player_Parry_ForMonster->LPOS().Y, -2.f });
		m_Collision_Player_Parry_ForMonster_Debug_Render->SPRITE(L"Col.png");

		m_Collision_Player_Parry_Debug_Render = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Debug);
		m_Collision_Player_Parry_Debug_Render->LSCALE(m_Collision_Player_Parry->LSCALE());
		m_Collision_Player_Parry_Debug_Render->LPOS({ m_Collision_Player_Parry->LPOS().X,m_Collision_Player_Parry->LPOS().Y, -2.f });
		m_Collision_Player_Parry_Debug_Render->SPRITE(L"Col.png");
	}


	{
		m_Animation = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_ACTOR);
		m_Animation->LSCALE({ 400.f, 400.f, 1.0f });

		m_Animation_Type = ACTOR()->CreateCom<Game_Animation>(m_Animation);

		m_Animation_Type->CreateAni(L"Idle", L"Stage_Idle.png", 0, 4, 0.15f, true);
		m_Animation_Type->CreateAni(L"Run", L"Stage_Run.png", 0, 15, 0.1f, true);
		m_Animation_Type->CreateAni(L"Run_Shoot", L"Stage_Run.png", 16, 31, 0.1f, true);

		m_Animation_Type->CreateAni(L"Jump", L"Stage_Jump.png", 0, 7, 0.08f, true);
		m_Animation_Type->CreateAni(L"Intro", L"Stage_Intro.png", 0, 27, 0.1f, true);
		m_Animation_Type->CreateAni(L"Hit", L"Stage_Hit.png", 0, 5, 0.1f, false);
		m_Animation_Type->CreateAni(L"Hit_Sky", L"Stage_Hit.png", 6, 11, 0.1f, false);
		m_Animation_Type->CreateAni(L"Trans_To_Down", L"Stage_Down.png", 0, 7, 0.1f, true);
		m_Animation_Type->CreateAni(L"Down_Idle", L"Stage_Down.png", 8, 12, 0.1f, true);
		m_Animation_Type->CreateAni(L"Down_Shot", L"Stage_Down.png", 13, 15, 0.1f, true);

		m_Animation_Type->CreateAni(L"Dash", L"Stage_Dash.png", 0, 5, 0.15f, true);
		m_Animation_Type->CreateAni(L"Dash_Start", L"Stage_Dash.png", 6, 7, 0.15f, true);

		//m_Animation_Type->CreateAni(L"Down_Trans", L"Stage_Down.png", 8, 15, 0.1f, true);

		m_Animation_Type->CreateAni(L"Parry", L"Stage_Parry.png", 0, 7, 0.08f, true);
		m_Animation_Type->CreateAni(L"Parry_Hit", L"Stage_Parry.png", 8, 15, 0.08f, false);

		m_Animation_Type->ChangeAni(L"Intro");
	}

	{
		Charge_Ani = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_ACTOR);
		Charge_Ani->LSCALE({ 400.f, 400.f, 1.0f });

		Charge_Ani_Type = ACTOR()->CreateCom<Game_Animation>(Charge_Ani);

		Charge_Ani_Type->CreateAni(L"L", L"Charge_Large.png", 15, 20, 0.1f, true);
		Charge_Ani_Type->CreateAni(L"S", L"Charge_Small.png", 0, 5, 0.1f, true);

		Charge_Ani_Type->ChangeAni(L"S");
		Charge_Ani->Off();
		Charge_Ani_Type->Off();

	}


	//ㅆㄷㄴㅅ
	Inventory::Set_Inventory_Type(Inventory_Bullet_Type::Charge);
	Inventory::Charge_Check = true;

}

void Player_Stage::Update()
{
	Debug();

	YCheck();
	ShootChange();
	Shoot();
	m_HitTime += Game_Time::DeltaTime();

	switch (m_eState)
	{
	case Player_Stage_State::Intro:
		Intro();
		break;
	case Player_Stage_State::Idle:
		Idle();
		Move();
		JumpCheck();
		break;
	case Player_Stage_State::Walk:
		Move();
		JumpCheck();
		break;
	case Player_Stage_State::Jump:
		Move();
		JumpCheck();
		Jump();
		break;
	case Player_Stage_State::Down:
		Move();
		JumpCheck();
		Down();
		break;
	case Player_Stage_State::Dash:
		Dash();
		break;
	default:
		break;
	}

	if (m_eState != Player_Stage_State::Down && m_eState != Player_Stage_State::Dash)
	{
		m_Collision->Set_LScaleY(Player_Collision_Scale_Y);
		m_Collision_1->Set_LScaleY(Player_Collision_Scale_Y);
		m_Collision_Player_Parry_ForMonster->Set_LScaleY(Player_Collision_Scale_Y);
		m_Collision_Player_Parry->Set_LScaleY(Player_Collision_Scale_Y);
		m_Collision->Set_LPosY(Player_Collision_LPos_Y);
		m_Collision_1->Set_LPosY(Player_Collision_LPos_Y);
		m_Collision_Player_Parry_ForMonster->Set_LPosY(Player_Collision_LPos_Y);
		m_Collision_Player_Parry->Set_LPosY(Player_Collision_LPos_Y);

	}

	if (m_Animation_Type->Get_Ani() == L"Hit" && m_Animation_Type->Get_Num() == 5
		|| m_Animation_Type->Get_Ani() == L"Hit_Sky" && m_Animation_Type->Get_Num() == 11)
	{
		m_Hit = false;
	}

	//대쉬중이 아니라면 대쉬로 바꿔줌.
	if (Game_Input::Down(L"Dash") && m_eState != Player_Stage_State::Intro)
	{
		if(m_eState != Player_Stage_State::Dash)
			m_eState = Player_Stage_State::Dash;
	}
	//점프하다가 대쉬해서 끊겼을때
	else if (m_eState != Player_Stage_State::Jump && m_JumpCheck1)
	{
		Jump();
	}

	if(m_eState != Player_Stage_State::Dash)
		Gravity();


	//UI
	Player_UI::Set_Hp(m_Hp);

	Synchro_Collistion();
}


void Player_Stage::Intro()
{
	m_Animation_Type->ChangeAni(L"Intro");

	//인트로 체크
	if (m_Animation_Type->Get_Ani() == L"Intro")
	{
		if (m_Animation_Type->Get_Num() == 26)
		{
			m_eState = Player_Stage_State::Idle;
			m_Animation_Type->ChangeAni(L"Idle");
		}
	}
}

void Player_Stage::Idle()
{
	Change_Ani(L"Idle");

	//이동하려 하면 상태변경
	if (Game_Input::Press(L"PLAYERLEFT") || Game_Input::Press(L"PLAYERRIGHT"))
	{
		m_eState = Player_Stage_State::Walk;
	}
	if (Game_Input::Press(L"PLAYERDOWN"))
	{
		m_eState = Player_Stage_State::Down;
	}
}

void Player_Stage::Move()
{
	if (m_eState == Player_Stage_State::Walk)
	{
		Change_Ani(L"Run");
	}

	//좌우 이동
	if (Game_Input::Press(L"PLAYERLEFT"))
	{
		TRANS()->WMOVE(Game_Vector::LEFT * Game_Time::DeltaTime(m_Speed));
		TRANS()->Reverse_L();
	}
	else if (Game_Input::Press(L"PLAYERRIGHT"))
	{
		TRANS()->WMOVE(Game_Vector::RIGHT * Game_Time::DeltaTime(m_Speed));
		TRANS()->Reverse_R();
	}
	else if (Game_Input::Press(L"PLAYERDOWN") && m_eState != Player_Stage_State::Jump)
	{
		m_eState = Player_Stage_State::Down;
	}
	else if(m_eState != Player_Stage_State::Jump)
		m_eState = Player_Stage_State::Idle;

}

void Player_Stage::Jump()
{
	TRANS()->WMOVE(JUMPVEC * Game_Time::DeltaTime());
	JUMPVEC -= GRAVITYVEC * Game_Time::DeltaTime();

	m_JumpCheck1 = true;
	if (TRANS()->LPOS().Y < Floor + (Player_Collision_Scale_Y / 2) - Player_Collision_LPos_Y)
	{
		m_eState = Player_Stage_State::Idle;
	}

	if (m_Animation_Type->Get_Ani() == L"Parry" || m_Animation_Type->Get_Ani() == L"Parry_Hit")
		if (m_Animation_Type->Get_Num() >= m_Animation_Type->Get_EndNum())
			Change_Ani(L"Jump");
}

void Player_Stage::JumpCheck()
{
	if (Game_Input::Down(L"JUMP"))
	{
		if (m_JumpCheck2)
		{
			return;
		}
		else if (m_JumpCheck1)
		{
			m_JumpCheck2 = true;
			Change_Ani(L"Parry");
		}
		else
		{
			Change_Ani(L"Jump");
			m_JumpCheck1 = true;
			GRAVITYVEC = { 0,450.0F,0 };
			JUMPVEC = { 0,500.0F,0 };
		}

		m_eState = Player_Stage_State::Jump;
	}
}

void Player_Stage::Shoot()
{
	//총알 생성
	Atk_CoolTime += Game_Time::DeltaTime();
	if (Game_Input::Up(L"SHOOT") && Inventory::Get_Inven() == Inventory_Bullet_Type::Charge) //charge 용
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"Bullet");
		PTR->TRANS()->WSCALE({ 1.f, 1.f, 1.f });
		//총알 위치/방향 설정
		{
			if (Game_Input::Press(L"PLAYERUP"))
			{
				if (Game_Input::Press(L"PLAYERLEFT"))
				{
					PTR->TRANS()->WPOS({ TRANS()->WPOS().X - 50.f,  TRANS()->WPOS().Y + 70.f, Weapon_Z });
					PTR->TRANS()->WROT({ 0.0f, 0.f, 135.0f });
				}
				else if (Game_Input::Press(L"PLAYERRIGHT"))
				{
					PTR->TRANS()->WPOS({ TRANS()->WPOS().X + 50.f,  TRANS()->WPOS().Y + 70.f, Weapon_Z });
					PTR->TRANS()->WROT({ 0.0f, 0.f, 45.0f });
				}
				else
				{
					PTR->TRANS()->WPOS({ TRANS()->WPOS().X,  TRANS()->WPOS().Y + 70.f, Weapon_Z });
					PTR->TRANS()->WROT({ 0.0f, 0.f, 90.0f });
				}
			}
			else if (Game_Input::Press(L"PLAYERDOWN"))
			{
				if (Game_Input::Press(L"PLAYERLEFT"))
				{
					PTR->TRANS()->WPOS({ TRANS()->WPOS().X - 50,  TRANS()->WPOS().Y + 25.f, Weapon_Z });
					PTR->TRANS()->WROT({ 0.0f, 0.f, 180.0f });
				}
				else if (Game_Input::Press(L"PLAYERRIGHT"))
				{
					PTR->TRANS()->WPOS({ TRANS()->WPOS().X + 50,  TRANS()->WPOS().Y + 25.f, Weapon_Z });
					PTR->TRANS()->WROT({ 0.0f, 0.f, 0.0f });
				}
				else
				{
					PTR->TRANS()->WPOS({ TRANS()->WPOS().X,  TRANS()->WPOS().Y - 20.f, Weapon_Z });
					PTR->TRANS()->WROT({ 0.0f, 0.f, 270.0f });
				}
			}
			else if (Game_Input::Press(L"PLAYERLEFT"))
			{
				PTR->TRANS()->WPOS({ TRANS()->WPOS().X - 50,  TRANS()->WPOS().Y + 50.f, Weapon_Z });
				PTR->TRANS()->WROT({ 0.0f, 0.f, 180.0f });
			}
			else if (Game_Input::Press(L"PLAYERRIGHT"))
			{
				PTR->TRANS()->WPOS({ TRANS()->WPOS().X + 50,  TRANS()->WPOS().Y + 50.f, Weapon_Z });
				PTR->TRANS()->WROT({ 0.0f, 0.f, 0.0f });
			}
			else
			{
				if (TRANS()->WSCALE().X < 0)
				{
					PTR->TRANS()->WPOS({ TRANS()->WPOS().X - 50,  TRANS()->WPOS().Y + 50.f, Weapon_Z });
					PTR->TRANS()->WROT({ 0.0f, 0.f, 180.0f });
				}
				else if (TRANS()->WSCALE().X > 0)
				{
					PTR->TRANS()->WPOS({ TRANS()->WPOS().X + 50,  TRANS()->WPOS().Y + 50.f, Weapon_Z });
					PTR->TRANS()->WROT({ 0.0f, 0.f, 0.0f });
				}
			}
		}
		Game_Ptr<Charge> Charge_Shot = PTR->CreateCom<Charge>();
		if (Charge_Time >= 1.f)
			Charge_Shot->Set_Type(true);
		else
			Charge_Shot->Set_Type(false);

		Charge_Time = 0.f;
		Weapon_Z -= 0.01f;
		Charge_Ani->Off();
		Charge_Ani_Type->Off();
	}
	if (Game_Input::Press(L"SHOOT") && Atk_CoolTime >= Inventory::Get_CoolTime())
	{
		if (m_eState == Player_Stage_State::Walk)
		{
			m_Animation_Type->ChangeAni(L"Run_Shoot");
		}

		if (Inventory::Get_Inven() == Inventory_Bullet_Type::Charge) //charge 용
		{
			Charge_Time += Game_Time::DeltaTime();

			Charge_Ani->On();
			Charge_Ani_Type->On();
			if(Charge_Time < 1.f)
				Charge_Ani_Type->ChangeAni(L"S");
			else
				Charge_Ani_Type->ChangeAni(L"L");

			if (TRANS()->LSCALE().X >= 0)
			{
				Charge_Ani->WPOS({ TRANS()->WPOS().X + 50,  TRANS()->WPOS().Y + 50.f, Weapon_Z });
			}
			else
				Charge_Ani->WPOS({ TRANS()->WPOS().X - 50,  TRANS()->WPOS().Y + 50.f, Weapon_Z });
			if (Game_Input::Press(L"PLAYERUP"))
			{
				Charge_Ani->WPOS({ TRANS()->WPOS().X - 50,  TRANS()->WPOS().Y + 70.f, Weapon_Z });
				if(m_eState == Player_Stage_State::Idle)
					Charge_Ani->WPOS({ TRANS()->WPOS().X,  TRANS()->WPOS().Y + 70.f, Weapon_Z });
			}
			

			return;
		}
		else
		{
			Charge_Ani->Off();
			Charge_Ani_Type->Off();
		}
		Atk_CoolTime = 0.f;
		{
			Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"Bullet");
			PTR->TRANS()->WSCALE({ 1.f, 1.f, 1.f });
			
			//총알 위치/방향 설정
			{
				if (Game_Input::Press(L"PLAYERUP"))
				{
					if (Game_Input::Press(L"PLAYERLEFT"))
					{
						PTR->TRANS()->WPOS({ TRANS()->WPOS().X -50.f,  TRANS()->WPOS().Y + 70.f, Weapon_Z });
						PTR->TRANS()->WROT({ 0.0f, 0.f, 135.0f });
					}
					else if (Game_Input::Press(L"PLAYERRIGHT"))
					{
						PTR->TRANS()->WPOS({ TRANS()->WPOS().X + 50.f,  TRANS()->WPOS().Y + 70.f, Weapon_Z });
						PTR->TRANS()->WROT({ 0.0f, 0.f, 45.0f });
					}
					else
					{
						PTR->TRANS()->WPOS({ TRANS()->WPOS().X,  TRANS()->WPOS().Y + 70.f, Weapon_Z });
						PTR->TRANS()->WROT({ 0.0f, 0.f, 90.0f });
					}
				}
				else if (Game_Input::Press(L"PLAYERDOWN"))
				{
					if (Game_Input::Press(L"PLAYERLEFT"))
					{
						PTR->TRANS()->WPOS({ TRANS()->WPOS().X - 50,  TRANS()->WPOS().Y + 25.f, Weapon_Z });
						PTR->TRANS()->WROT({ 0.0f, 0.f, 180.0f });
					}
					else if (Game_Input::Press(L"PLAYERRIGHT"))
					{
						PTR->TRANS()->WPOS({ TRANS()->WPOS().X + 50,  TRANS()->WPOS().Y + 25.f, Weapon_Z });
						PTR->TRANS()->WROT({ 0.0f, 0.f, 0.0f });
					}
					else
					{
						PTR->TRANS()->WPOS({ TRANS()->WPOS().X,  TRANS()->WPOS().Y - 20.f, Weapon_Z });
						PTR->TRANS()->WROT({ 0.0f, 0.f, 270.0f });
					}
				}
				else if (Game_Input::Press(L"PLAYERLEFT"))
				{
					PTR->TRANS()->WPOS({ TRANS()->WPOS().X - 50,  TRANS()->WPOS().Y + 50.f, Weapon_Z });
					PTR->TRANS()->WROT({ 0.0f, 0.f, 180.0f });						  
				}																	  
				else if (Game_Input::Press(L"PLAYERRIGHT"))							  
				{																	  
					PTR->TRANS()->WPOS({ TRANS()->WPOS().X + 50,  TRANS()->WPOS().Y + 50.f, Weapon_Z });
					PTR->TRANS()->WROT({ 0.0f, 0.f, 0.0f });
				}
				else
				{
					if (TRANS()->WSCALE().X < 0)
					{
						PTR->TRANS()->WPOS({ TRANS()->WPOS().X - 50,  TRANS()->WPOS().Y + 50.f, Weapon_Z });
						PTR->TRANS()->WROT({ 0.0f, 0.f, 180.0f });
					}
					else if (TRANS()->WSCALE().X > 0)
					{
						PTR->TRANS()->WPOS({ TRANS()->WPOS().X + 50,  TRANS()->WPOS().Y + 50.f, Weapon_Z });
						PTR->TRANS()->WROT({ 0.0f, 0.f, 0.0f });
					}
				}
			}

			switch (Inventory::Get_Inven())
			{
			case Inventory_Bullet_Type::Peashot:
				PTR->CreateCom<Bouncer>();
				Weapon_Z -= 0.01f;
				break;
			case Inventory_Bullet_Type::Spread:
				PTR->CreateCom<Spread>();
				Weapon_Z -= 0.01f;
				{
					Game_Ptr<Game_Actor> Plus1 = SCENE()->CreateActor(L"Bullet");
					Plus1->TRANS()->WSCALE({ 1.f, 1.f, 1.f });
					Plus1->TRANS()->WPOS(PTR->TRANS()->WPOS());
					Plus1->TRANS()->WROT({ 0.f, 0.f, PTR->TRANS()->WROT().Z + 15.f });
					Plus1->CreateCom<Spread>();

					Game_Ptr<Game_Actor> Plus2 = SCENE()->CreateActor(L"Bullet");
					Plus2->TRANS()->WSCALE({ 1.f, 1.f, 1.f });
					Plus2->TRANS()->WPOS(PTR->TRANS()->WPOS());
					Plus2->TRANS()->WROT({ 0.f, 0.f, PTR->TRANS()->WROT().Z + 30.f });
					Plus2->CreateCom<Spread>();

					Game_Ptr<Game_Actor> Plus3 = SCENE()->CreateActor(L"Bullet");
					Plus3->TRANS()->WSCALE({ 1.f, 1.f, 1.f });
					Plus3->TRANS()->WPOS(PTR->TRANS()->WPOS());
					Plus3->TRANS()->WROT({ 0.f, 0.f, PTR->TRANS()->WROT().Z - 15.f });
					Plus3->CreateCom<Spread>();

					Game_Ptr<Game_Actor> Plus4 = SCENE()->CreateActor(L"Bullet");
					Plus4->TRANS()->WSCALE({ 1.f, 1.f, 1.f });
					Plus4->TRANS()->WPOS(PTR->TRANS()->WPOS());
					Plus4->TRANS()->WROT({ 0.f, 0.f, PTR->TRANS()->WROT().Z - 30.f });
					Plus4->CreateCom<Spread>();
				}
				break;
			case Inventory_Bullet_Type::Bouncer:
				PTR->CreateCom<Bouncer>();
				Weapon_Z -= 0.01f;
				break;
			case Inventory_Bullet_Type::End:
				break;
			default:
				PTR->CreateCom<Peashot>();
				break;
			}

		}
	}
}

void Player_Stage::ShootChange()
{
	if (Game_Input::Down(L"Shoot_Change"))
	{
		//if (m_ShootType == m_Inven[0])
		//{
		//	m_ShootType = m_Inven[1];
		//}
		//else
		//{
		//	m_ShootType = m_Inven[0];
		//}

		Inventory::Inven_Change();
	}
	//switch (m_ShootType)
	//{
	//case Player_Inventory::Peashot:
	//	Player_UI::Set_Bullet(Bullet_Type::Peashot);
	//	break;
	//case Player_Inventory::Charge:
	//	Player_UI::Set_Bullet(Bullet_Type::Charge);
	//	break;
	//case Player_Inventory::Spread:
	//	Player_UI::Set_Bullet(Bullet_Type::Spread);
	//	break;
	//default:
	//	break;
	//}

	switch (Inventory::Get_Inven())
	{
	case Inventory_Bullet_Type::Peashot:
		Player_UI::Set_Bullet(Bullet_Type::Peashot);
		break;
	case Inventory_Bullet_Type::Spread:
		Player_UI::Set_Bullet(Bullet_Type::Spread);
		break;
	case Inventory_Bullet_Type::Charge:
		Player_UI::Set_Bullet(Bullet_Type::Charge);
		break;
	default:
		break;
	}
}

void Player_Stage::Down()
{
	m_Collision->Set_LScaleY(Player_Collision_Scale_Y / 2);
	m_Collision_1->Set_LScaleY(Player_Collision_Scale_Y / 2);
	m_Collision_Player_Parry_ForMonster->Set_LScaleY(Player_Collision_Scale_Y / 2);
	m_Collision_Player_Parry->Set_LScaleY(Player_Collision_Scale_Y / 2);
	m_Collision->Set_LPosY(0.f);
	m_Collision_1->Set_LPosY(-0.f);
	m_Collision_Player_Parry_ForMonster->Set_LPosY(-0.f);
	m_Collision_Player_Parry->Set_LPosY(-0.f);

	if (!DownCheck)
	{
		m_Animation_Type->ChangeAni(L"Trans_To_Down");
		if (m_Animation_Type->Get_Num() >= 7)
			DownCheck = true;
	}
	else if (DownCheck && Game_Input::Down(L"SHOOT") || DownShotCheck)
	{
		m_Animation_Type->ChangeAni(L"Down_Shot");
		DownShotCheck = true;
		if (m_Animation_Type->Get_Num() >= 15)
			DownShotCheck = false;
	}
	else
		m_Animation_Type->ChangeAni(L"Down_Idle");

	if (!Game_Input::Press(L"PLAYERDOWN")) //누르고있지 않다면 원상복귀
	{
		m_eState = Player_Stage_State::Idle;
		DownCheck = false;
	}
}

void Player_Stage::Dash()
{
	if (m_Animation_Type->Get_Num() >= m_Animation_Type->Get_EndNum() && !DashStartAni_Check)
	{
		DashStartAni_Check = true;
		Change_Ani(L"Dash");
	}
	else if (!DashStartAni_Check)
	{
		if (m_Animation_Type->Get_Ani() != L"Dash_Start" && !m_Hit)
		{
			TRANS()->Set_LPosY(TRANS()->LPOS().Y + 50.f);
			m_Collision->Set_LPosY(m_Collision->LPOS().Y - 50.f);
			m_Collision_Player_Parry_ForMonster->Set_LPosY(m_Collision_Player_Parry_ForMonster->LPOS().Y - 50.f);
			m_Collision_Player_Parry->Set_LPosY(m_Collision_Player_Parry->LPOS().Y - 50.f);

			m_Collision_Debug_Render->Set_LPosY(m_Collision->LPOS().Y);
			m_Collision_Player_Parry_ForMonster_Debug_Render->Set_LPosY(m_Collision_Player_Parry_ForMonster->LPOS().Y);
			m_Collision_Player_Parry_Debug_Render->Set_LPosY(m_Collision_Player_Parry->LPOS().Y);
		}
		Change_Ani(L"Dash_Start");
	}
	else
		Change_Ani(L"Dash");


	DashTime += Game_Time::DeltaTime();

	if (TRANS()->WSCALE().X > 0)
	{
		TRANS()->WMOVE(Game_Vector::RIGHT * Game_Time::DeltaTime(m_Speed * 2.5f));
	}
	else
		TRANS()->WMOVE(Game_Vector::LEFT * Game_Time::DeltaTime(m_Speed * 2.5f));

	if (DashTime >= 0.3f)
	{
		DashTime = 0.f;

		TRANS()->Set_LPosY(TRANS()->LPOS().Y - 50.f);
		m_Collision->Set_LPosY(m_Collision->LPOS().Y + 50.f);
		m_Collision_Player_Parry_ForMonster->Set_LPosY(m_Collision_Player_Parry_ForMonster->LPOS().Y + 50.f);
		m_Collision_Player_Parry->Set_LPosY(m_Collision_Player_Parry->LPOS().Y + 50.f);
		m_Collision_Debug_Render->Set_LPosY(m_Collision->LPOS().Y);
		m_Collision_Player_Parry_ForMonster_Debug_Render->Set_LPosY(m_Collision_Player_Parry_ForMonster->LPOS().Y);
		m_Collision_Player_Parry_Debug_Render->Set_LPosY(m_Collision_Player_Parry->LPOS().Y);

		m_eState = Player_Stage_State::Idle;
		DashStartAni_Check = false;
	}
}

void Player_Stage::Debug()
{
	//디버그
	{
		if (Game_Input::Down(L"DebugCheck"))
		{
			if (m_DebugCheck)
				m_DebugCheck = false;
			else
				m_DebugCheck = true;
		}

		if (Game_Input::Down(L"DebugOn"))
		{
			m_DebugCheck = true;
			m_Collision_Debug_Render->On();
			m_Collision_Player_Parry_Debug_Render->On();
			m_Collision_Player_Parry_ForMonster_Debug_Render->On();
		}
		else if (Game_Input::Down(L"DebugOff"))
		{
			m_DebugCheck = false;
			m_Collision_Debug_Render->Off();
			m_Collision_Player_Parry_Debug_Render->Off();
			m_Collision_Player_Parry_ForMonster_Debug_Render->Off();
		}

		if (m_DebugCheck)
		{
			Game_3D_Debug::DrawDebugText(L"Player Local Pos : %f, %f", TRANS()->LPOS().X, TRANS()->LPOS().Y);
			Game_3D_Debug::DrawDebugText(L"Col Local Pos : %f, %f", m_Collision->LPOS().X, m_Collision->LPOS().Y);
			Game_3D_Debug::DrawDebugText(L"Player Hp : %d", m_Hp);
			Game_3D_Debug::DrawDebugText(L"Player 바닥체크 : %f", TRANS()->LPOS().Y - m_Collision->LPOS().Y + (m_Collision->LSCALE().Y/2));
			Game_3D_Debug::DrawDebugText(L"총알 Z값 : %f", Weapon_Z);
			Game_3D_Debug::DrawDebugText(L"Player WScale : %f , L : %f", TRANS()->WSCALE().X, TRANS()->LSCALE().X);

		}
	}

	//COL 동기화
	m_Collision_Debug_Render->LSCALE(m_Collision->LSCALE());
	m_Collision_Debug_Render->LPOS({ m_Collision->LPOS().X,m_Collision->LPOS().Y, -2.f });
	m_Collision_Player_Parry_ForMonster_Debug_Render->LSCALE(m_Collision_Player_Parry_ForMonster->LSCALE());
	m_Collision_Player_Parry_ForMonster_Debug_Render->LPOS({ m_Collision_Player_Parry_ForMonster->LPOS().X,m_Collision_Player_Parry_ForMonster->LPOS().Y, -2.f });
	m_Collision_Player_Parry_Debug_Render->LSCALE(m_Collision_Player_Parry->LSCALE());
	m_Collision_Player_Parry_Debug_Render->LPOS({ m_Collision_Player_Parry->LPOS().X,m_Collision_Player_Parry->LPOS().Y, -2.f });
}

void Player_Stage::Gravity()
{
	TRANS()->WMOVE(Game_Vector::DOWN * Game_Time::DeltaTime(m_Gravity));

}

void Player_Stage::YCheck()
{

	if (TRANS()->WPOS().Y < Floor + (Player_Collision_Scale_Y / 2) - Player_Collision_LPos_Y)
	{
		m_JumpCheck1 = false;
		m_JumpCheck2 = false;
		TRANS()->Set_LPosY(Floor + (Player_Collision_Scale_Y / 2) - Player_Collision_LPos_Y);
		//TRANS()->Set_Y();
	}
}


void Player_Stage::MonsterEnter(Game_Collision* _This, Game_Collision* _Other)
{
	//_Other->ACTOR()->Death();
}

void Player_Stage::Hit(Game_Collision* _This, Game_Collision* _Other)
{
	if (m_HitTime <= 3.0f)
		return;

	if(TRANS()->LPOS().Y == -185.f)
		Change_Ani(L"Hit");
	else
		Change_Ani(L"Hit_Sky");

	m_Hit = true;
	--m_Hp;
	m_HitTime = 0.f;
}

void Player_Stage::Parry_HitCheck(Game_Collision* _This, Game_Collision* _Other)
{
	if (m_Animation_Type->Get_Ani() == L"Parry")
	{
		GRAVITYVEC = { 0,450.0F,0 };
		JUMPVEC = { 0,500.0F,0 };
		m_Parry = true;
		Change_Ani(L"Parry_Hit");
		_Other->ACTOR()->Death();
	}
	else
	{
		Hit(_This, _Other);
	}
}

void Player_Stage::Parry_Check(Game_Collision* _This, Game_Collision* _Other)
{
	if (m_Animation_Type->Get_Ani() == L"Parry" || m_Animation_Type->Get_Ani() == L"Parry_Hit")
	{
		GRAVITYVEC = { 0,450.0F,0 };
		JUMPVEC = { 0,500.0F,0 };
		m_Parry = true;
		Change_Ani(L"Parry_Hit");

		_Other->Check();
		_Other->Off();
		
	}
}

void Player_Stage::Change_Ani(const wchar_t* _AniType)
{
	if (!m_Hit && 
		m_Animation_Type->Get_Ani() != L"Intro"
		
		)
		m_Animation_Type->ChangeAni(_AniType);

}