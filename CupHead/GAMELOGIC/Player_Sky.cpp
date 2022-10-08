#include "Player_Sky.h"
#include <Game_3D_Debug.h>
#include "Logic_Enum.h"

#include "Monkey_Card.h"
#include "Sky_Bomb.h"
#include "Player_UI.h"
#include "Sky_PeaShot.h"

#define Player_Sky_Speed_Lg 450
#define Player_Sky_Speed_Mn 600

#define Player_Sky_Scale_Lg { 200.f, 200.f, 1.f }
#define Player_Sky_Scale_Mn { 200.f, 200.f, 1.f }

#define Player_Sky_Collision_Scale_Default { 100.f, 100.f, 1.f }
#define Player_Sky_Collision_LPos_Default { 0.0F, 0.0F, 0.0F }

void Player_Sky::Init()
{

	m_Gravity = 30.F;
	m_Speed = Player_Sky_Speed_Lg;
	m_Cam = SCENE()->MainCam();

	m_eState = Player_Sky_State::Idle_Straight;
	m_ShootType = Player_Sky_ShootType::Bomb;

	{
		//충돌
		m_Collision = ACTOR()->CreateCom<Game_Collision>((int)COLORDER::COLORDER_PLAYER_ForHit);
		m_Collision->ColType(COLTYPE::AABB2D);
		m_Collision->LSCALE(Player_Sky_Collision_Scale_Default);
		m_Collision->LPOS(Player_Sky_Collision_LPos_Default);
		m_Collision->PushEnterFunc(this, &Player_Sky::Hit);
		m_Collision->PushStayFunc(this, &Player_Sky::Hit);

		m_Collision_1 = ACTOR()->CreateCom<Game_Collision>((int)COLORDER::COLORDER_PLAYER_ForUnknown);
		m_Collision_1->ColType(COLTYPE::AABB2D);
		m_Collision_1->LSCALE(Player_Sky_Collision_Scale_Default);
		m_Collision_1->LPOS(Player_Sky_Collision_LPos_Default);

		m_Collision_Player_Parry_ForMonster = ACTOR()->CreateCom<Game_Collision>((int)COLORDER::Player_Parry_ForMonster);
		m_Collision_Player_Parry_ForMonster->ColType(COLTYPE::AABB2D);
		m_Collision_Player_Parry_ForMonster->LSCALE(Player_Sky_Collision_Scale_Default);
		m_Collision_Player_Parry_ForMonster->LPOS(Player_Sky_Collision_LPos_Default);
		m_Collision_Player_Parry_ForMonster->PushEnterFunc(this, &Player_Sky::Parry_HitCheck);
		m_Collision_Player_Parry_ForMonster->PushStayFunc(this, &Player_Sky::Parry_HitCheck);

		m_Collision_Player_Parry = ACTOR()->CreateCom<Game_Collision>((int)COLORDER::Player_Parry);
		m_Collision_Player_Parry->ColType(COLTYPE::AABB2D);
		m_Collision_Player_Parry->LSCALE(Player_Sky_Collision_Scale_Default);
		m_Collision_Player_Parry->LPOS(Player_Sky_Collision_LPos_Default);
		m_Collision_Player_Parry->PushEnterFunc(this, &Player_Sky::Parry_Check);
		m_Collision_Player_Parry->PushStayFunc(this, &Player_Sky::Parry_Check);

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

		//ShootchangeUI
		m_ShootChange_UI_Animation = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Debug);
		m_ShootChange_UI_Animation->LSCALE(Player_Sky_Collision_Scale_Default);
		m_ShootChange_UI_Animation->WPOS({-500.f,-360.f, -2.f });
		m_ShootChange_UI_Animation->SPRITE(L"Col.png");		
		m_ShootChange_UI_Animation->Off();
	}

	Renderer_Setting();
}

void Player_Sky::Update()
{
	Debug();

	switch (m_eState)
	{
	case Player_Sky_State::Intro:
		Intro();
		break;
	case Player_Sky_State::Idle_Up:
		Idle_Up();
		break;
	case Player_Sky_State::Idle_Down:
		Idle_Down();
		break;
	case Player_Sky_State::Idle_Straight:
		Idle_Straight();
		break;
	case Player_Sky_State::Trans_Up:
		Trans_Up();
		break;
	case Player_Sky_State::Trans_Down:
		Trans_Down();
		break;
	case Player_Sky_State::Parry:
		Parry();
		break;
	case Player_Sky_State::Trans_Mn:
		Trans_Mn();
		break;
	case Player_Sky_State::Trans_Lg:
		Trans_Lg();
		break;
	case Player_Sky_State::Mn_Idle_Up:
		Mn_Idle_Up();
		break;
	case Player_Sky_State::Mn_Idle_Down:
		Mn_Idle_Down();
		break;
	case Player_Sky_State::Mn_Idle_Straight:
		Mn_Idle_Straight();
		break;
	case Player_Sky_State::Mn_Trans_Up:
		Mn_Trans_Up();
		break;
	case Player_Sky_State::Mn_Trans_Down:
		Mn_Trans_Down();
		break;
	case Player_Sky_State::Ghost:
		Ghost();
		break;
	default:
		break;
	}
	ShootChange();
	Shoot();


	//UI
	Player_UI::Set_Hp(m_Hp);

	//짬
	MonkeyCard_Back_Check();
}

void Player_Sky::Intro()
{
	Change_Ani(L"Intro");

	if (Ani_Done())
		m_eState = Player_Sky_State::Idle_Straight;
}

void Player_Sky::Idle_Up()
{
	Change_Ani(L"Idle_Up");
	Move();
	if (!Game_Input::Press(L"PLAYERUP"))
		m_eState = Player_Sky_State::Idle_Straight;

	LRAngle();
	ParryCheck();
	ShiftCheck();
}

void Player_Sky::Idle_Down()
{
	Change_Ani(L"Idle_Down");
	Move();
	if (!Game_Input::Press(L"PLAYERDOWN"))
		m_eState = Player_Sky_State::Idle_Straight;
	LRAngle();
	ParryCheck();
	ShiftCheck();
}

void Player_Sky::Idle_Straight()
{
	Change_Ani(L"Idle_Straight");

	Move();
	if (Game_Input::Down(L"PLAYERUP"))
	{
		m_eState = Player_Sky_State::Trans_Up;
	}
	else if (Game_Input::Down(L"PLAYERDOWN"))
	{
		m_eState = Player_Sky_State::Trans_Down;
	}
	LRAngle();
	ParryCheck();
	ShiftCheck();
}

void Player_Sky::Trans_Up()
{
	Change_Ani(L"Trans_Up");
	Move();

	if (Ani_Done())
		m_eState = Player_Sky_State::Idle_Up;
	ParryCheck();
	ShiftCheck();
}

void Player_Sky::Trans_Down()
{
	Change_Ani(L"Trans_Down");
	Move();

	if (Ani_Done())
		m_eState = Player_Sky_State::Idle_Down;

	ParryCheck();
	ShiftCheck();
}

void Player_Sky::Parry()
{
	Change_Ani(L"Parry");
	Move();
	if (Ani_Done())
		m_eState = Player_Sky_State::Idle_Straight;
}

void Player_Sky::Trans_Mn()
{
	Change_Ani(L"Trans");
	if (Ani_Done())
	{
		m_eState = Player_Sky_State::Mn_Idle_Straight;
	}
}

void Player_Sky::Trans_Lg()
{
	Change_Ani(L"Trans");
	if (Ani_Done())
	{
		m_eState = Player_Sky_State::Idle_Straight;
	}
}

void Player_Sky::Mn_Idle_Up()
{
	Change_Ani(L"Mn_Idle_Up");
	Move();
	if (!Game_Input::Press(L"PLAYERUP"))
		m_eState = Player_Sky_State::Mn_Idle_Straight;
	LRAngle();
	ShiftCheck();
}

void Player_Sky::Mn_Idle_Down()
{
	Change_Ani(L"Mn_Idle_Down");
	Move();
	if (!Game_Input::Press(L"PLAYERDOWN"))
		m_eState = Player_Sky_State::Mn_Idle_Straight;
	LRAngle();
	ShiftCheck();
}

void Player_Sky::Mn_Idle_Straight()
{
	Change_Ani(L"Mn_Idle_Straight");

	Move();
	if (Game_Input::Down(L"PLAYERUP"))
	{
		m_eState = Player_Sky_State::Mn_Trans_Up;
	}
	else if (Game_Input::Down(L"PLAYERDOWN"))
	{
		m_eState = Player_Sky_State::Mn_Trans_Down;
	}
	LRAngle();
	ShiftCheck();
}

void Player_Sky::Mn_Trans_Up()
{
	Change_Ani(L"Mn_Trans_Up");
	Move();

	if (Ani_Done())
		m_eState = Player_Sky_State::Mn_Idle_Up;

	ShiftCheck();
}

void Player_Sky::Mn_Trans_Down()
{
	Change_Ani(L"Mn_Trans_Down");
	Move();

	if (Ani_Done())
		m_eState = Player_Sky_State::Mn_Idle_Down;

	ShiftCheck();
}

void Player_Sky::Ghost()
{
}

//간접
bool Player_Sky::Move()
{
	bool PressCheck = true;

	if (Game_Input::Press(L"PLAYERLEFT"))
	{
		TRANS()->WMOVE(Game_Vector::LEFT * Game_Time::DeltaTime(m_Speed));
		if (TRANS()->LPOS().X < -640.f)
			TRANS()->LPOS({ -640.f, TRANS()->LPOS().Y, TRANS()->LPOS().Z });

		PressCheck = false;
	}
	if (Game_Input::Press(L"PLAYERRIGHT"))
	{
		TRANS()->WMOVE(Game_Vector::RIGHT * Game_Time::DeltaTime(m_Speed));
		if (TRANS()->LPOS().X > 640.f)
			TRANS()->LPOS({640.f, TRANS()->LPOS().Y, TRANS()->LPOS ().Z});
		PressCheck = false;
	}
	if (Game_Input::Press(L"PLAYERUP"))
	{
		TRANS()->WMOVE(Game_Vector::UP * Game_Time::DeltaTime(m_Speed));
		if (TRANS()->LPOS().Y > 360.f)
			TRANS()->LPOS({ TRANS()->LPOS().X, 360.f, TRANS()->LPOS().Z });
		PressCheck = false;
	}
	if (Game_Input::Press(L"PLAYERDOWN"))
	{
		TRANS()->WMOVE(Game_Vector::DOWN * Game_Time::DeltaTime(m_Speed));
		if (TRANS()->LPOS().Y < -360.f)
			TRANS()->LPOS({ TRANS()->LPOS().X, -360.f, TRANS()->LPOS().Z });
		PressCheck = false;
	}

	if (PressCheck)
		return true;
	return false;
}

void Player_Sky::LRAngle()
{
	if (Game_Input::Press(L"PLAYERLEFT"))
	{
		TRANS()->WROT({ 0.f, 0.f, TRANS()->WROT().Z + 0.1f });
		if(TRANS()->WROT().Z >= 10.f)
			TRANS()->WROT({ 0.f, 0.f, 10.f });
	}
	else if (Game_Input::Press(L"PLAYERRIGHT"))
	{
		TRANS()->WROT({ 0.f, 0.f, TRANS()->WROT().Z - 0.1f });
		if (TRANS()->WROT().Z <= -10.f)
			TRANS()->WROT({ 0.f, 0.f, -10.f });
	}
	else
	{
		if (TRANS()->WROT().Z < 0.f)
		{
			TRANS()->WROT({ 0.f, 0.f, TRANS()->WROT().Z + 0.15f });
			if (TRANS()->WROT().Z >= 0.f)
				TRANS()->WROT({ 0.f, 0.f, 0.f });
		}
		else if (TRANS()->WROT().Z > 0.f)
		{
			TRANS()->WROT({ 0.f, 0.f, TRANS()->WROT().Z - 0.15f });
			if (TRANS()->WROT().Z <= 0.f)
				TRANS()->WROT({ 0.f, 0.f, 0.f });
		}
	}
}

void Player_Sky::Debug()
{
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
		Game_3D_Debug::DrawDebugText(L"Player Hp : %d", m_Hp);
		Game_3D_Debug::DrawDebugText(L"Player m_ShootType : %d", m_ShootType);
	}
}

void Player_Sky::ParryCheck()
{
	if (Game_Input::Down(L"JUMP"))
	{
		m_eState = Player_Sky_State::Parry;
	}
}

void Player_Sky::ShiftCheck()
{
	if (Game_Input::Down(L"Dash"))
	{
		m_eState = Player_Sky_State::Trans_Mn;
		m_Speed = Player_Sky_Speed_Mn;
	}
	else if (Game_Input::Up(L"Dash"))
	{
		m_eState = Player_Sky_State::Trans_Lg;
		m_Speed = Player_Sky_Speed_Lg;
	}
}

void Player_Sky::Shoot()
{
	ShootTime += Game_Time::DeltaTime();
	if (ShootTime > 0.17f)
	{
		if (Game_Input::Press(L"SHOOT"))
		{
			ShootTime = 0.f;

			Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"Bullet");
			PTR->TRANS()->WSCALE({ 1.f, 1.f, 1.f });
			PTR->TRANS()->WPOS({ TRANS()->WPOS().X + 20.f,  TRANS()->WPOS().Y, BulletZ });
			BulletZ -= 0.001f;
	
			switch (m_ShootType)
			{
			case Player_Sky_ShootType::PeaShot:
				PTR->CreateCom<Sky_PeaShot>();
				break;
			case Player_Sky_ShootType::Bomb:
				PTR->CreateCom<Sky_Bomb>();
				break;
			default:
				PTR->CreateCom<Sky_Bomb>();
				break;
			}
		}
	}
}

void Player_Sky::ShootChange()
{
	if (Game_Input::Down(L"Shoot_Change"))
	{
		if (m_ShootType == Player_Sky_ShootType::PeaShot)
		{
			m_ShootType = Player_Sky_ShootType::Bomb;
			Player_UI::Set_Bullet(Bullet_Type::Sky_Bomb);
		}
		else
		{
			m_ShootType = Player_Sky_ShootType::PeaShot;
			Player_UI::Set_Bullet(Bullet_Type::Sky_Peashot);
		}
	}

	//if (m_ShootChange)
	//{
	//	m_ShootChange_UI_Animation->On();

	//	switch (m_ShootType)
	//	{
	//	case Player_Sky_ShootType::PeaShot:
	//		m_ShootChange_UI_Animation->SPRITE(L"shmup_icon_bullet.png");
	//		break;
	//	case Player_Sky_ShootType::Bomb:
	//		m_ShootChange_UI_Animation->SPRITE(L"shmup_icon_bomb.png");
	//		break;
	//	default:
	//		break;
	//	}

	//	m_ShootChange_UI_Animation->WPOS({ -450.f,m_ShootChange_UI_Animation->WPOS().Y + 1.f, -5.f });
	//	if (m_ShootChange_UI_Animation->WPOS().Y >= -200)
	//	{
	//		m_ShootChange = false;
	//		m_ShootChange_UI_Animation->WPOS({ -450.f,-100.f, -2.f });
	//		m_ShootChange_UI_Animation->Off();
	//	}

	//}

}


void Player_Sky::MonkeyCard_Back_Check()
{
	if (Card_Back_Check)
	{
		Card_Time += Game_Time::DeltaTime();
		if (Card_Time >= 2.f)
		{
			Other1->On();
			Other1->Check(false);
			Other2->On();
			Other2->Check(false);

			Card_Back_Check = false;
			Card_Time = 0.f;
		}
	}
}

//충돌
void Player_Sky::MonsterEnter(Game_Collision* _This, Game_Collision* _Other)
{
}

void Player_Sky::Hit(Game_Collision* _This, Game_Collision* _Other)
{
	m_HitTime += Game_Time::DeltaTime();
	if (m_HitTime >= 2.f)
	{
		m_HitTime = 0.f;
		m_Hp--;
	}
}

void Player_Sky::Parry_HitCheck(Game_Collision* _This, Game_Collision* _Other)
{
}

void Player_Sky::Parry_Check(Game_Collision* _This, Game_Collision* _Other)
{
	if (Game_Input::Down(L"JUMP"))
	{
		_Other->Check();
		_Other->Off();

		//ForMonkeyCard
		{
			Game_Ptr<Monkey_Card> Ptr = _Other->ACTOR()->FindComType<Monkey_Card>();
			if (Ptr != nullptr)
			{
				if (Monkey_Card::Card_1 == Monkey_CardType::End)
				{
					Monkey_Card::Card_1 = Ptr->Get_CardType();
					Other1 = _Other;
				}
				else
				{
					Monkey_Card::Card_2 = Ptr->Get_CardType();
					Other2 = _Other;

					if (Monkey_Card::Card_1 == Monkey_Card::Card_2)
					{
						Monkey::State_Check = false;
					}
					else
					{
						Card_Back_Check = true;						
						Monkey::Shake_Check = false;
					}

					Monkey_Card::Card_1 = Monkey_CardType::End;
					Monkey_Card::Card_2 = Monkey_CardType::End;
				}
			}
		}
	}
}

void Player_Sky::Renderer_Setting()
{
	m_Animation = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_ACTOR);
	m_Animation->LSCALE(Player_Sky_Scale_Lg);

	m_Animation_Type = ACTOR()->CreateCom<Game_Animation>(m_Animation);

	m_Animation_Type->CreateAni(L"Intro", L"CupHead_Sky_Intro.png", 0, 40, 0.1f, true);
	m_Animation_Type->CreateAni(L"Idle_Down", L"CupHead_Sky_Idle_D_S_U.png", 0, 3, 0.1f, true);
	m_Animation_Type->CreateAni(L"Idle_Straight", L"CupHead_Sky_Idle_D_S_U.png", 4, 7, 0.1f, true);
	m_Animation_Type->CreateAni(L"Idle_Up", L"CupHead_Sky_Idle_D_S_U.png", 8, 11, 0.1f, true);
	m_Animation_Type->CreateAni(L"Trans_Down", L"CupHead_Sky_Trans_Down.png", 0, 10, 0.07f, true);
	m_Animation_Type->CreateAni(L"Trans_Up", L"CupHead_Sky_Trans_Up.png", 0, 10, 0.07f, true);

	m_Animation_Type->CreateAni(L"Mn_Idle_Down", L"CupHead_Sky_Mn_Idle_D_S_U.png", 0, 3, 0.1f, true);
	m_Animation_Type->CreateAni(L"Mn_Idle_Straight", L"CupHead_Sky_Mn_Idle_D_S_U.png", 4, 7, 0.1f, true);
	m_Animation_Type->CreateAni(L"Mn_Idle_Up", L"CupHead_Sky_Mn_Idle_D_S_U.png", 8, 11, 0.1f, true);
	m_Animation_Type->CreateAni(L"Mn_Trans_Down", L"CupHead_Sky_Mn_Trans_Down.png", 0, 10, 0.07f, true);
	m_Animation_Type->CreateAni(L"Mn_Trans_Up", L"CupHead_Sky_Mn_Trans_Up.png", 0, 10, 0.07f, true);

	m_Animation_Type->CreateAni(L"Trans", L"CupHead_Sky_Trans.png", 0, 3, 0.05f, true);
	m_Animation_Type->CreateAni(L"Parry", L"CupHead_Sky_Parry.png", 0, 19, 0.03f, true);
	m_Animation_Type->CreateAni(L"Ghost", L"CupHead_Sky_Ghost.png", 0, 23, 0.1f, true);

	m_Animation_Type->CreateAni(L"Super_Intro", L"CupHead_Sky_Super_Intro.png", 0, 13, 0.1f, true);
	m_Animation_Type->CreateAni(L"Super_Idle_Down", L"CupHead_Sky_Super_Idle_D_S_U.png", 0, 3, 0.1f, true);
	m_Animation_Type->CreateAni(L"Super_Idle_Straight", L"CupHead_Sky_Super_Idle_D_S_U.png", 4, 7, 0.1f, true);
	m_Animation_Type->CreateAni(L"Super_Idle_Up", L"CupHead_Sky_Super_Idle_D_S_U.png", 8, 11, 0.1f, true);
	m_Animation_Type->CreateAni(L"Super_Trans_Down", L"CupHead_Sky_Super_Trans_Down.png", 0, 9, 0.1f, true);
	m_Animation_Type->CreateAni(L"Super_Trans_Up", L"CupHead_Sky_Super_Trans_Up.png", 0, 11, 0.1f, true);

	m_Animation_Type->ChangeAni(L"Intro");
}

void Player_Sky::Change_Ani(const wchar_t* _AniType)
{
	if (true)
		m_Animation_Type->ChangeAni(_AniType);
}

bool Player_Sky::Ani_Done()
{
	if (m_Animation_Type->Get_Num() >= m_Animation_Type->Get_EndNum())
	{
		return true;
	}
	return false;
}
