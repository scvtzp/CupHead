#include "Slime.h"
#include "Game_FreeCam.h"
#include "LOGIC_Value.h"
#include "Game_Header.h"
#include "Logic_Enum.h"

#define Slime_Size_D2 {300.f, 300.f, 1.f}
#define Slime_Size_Default {600.f, 600.f, 1.f}
#define Slime_Size_X2 {1200.f, 1200.f, 1.f}

#define Slime_Collision_Scale_Default {150.f, 150.f, 1.f}
#define Slime_Collision_LPos_Default { -70.0F, +50.0F, -1.0F }

#define Lg_Slime_Collision_Scale_Default {250.f, 250.f, 1.f}
#define Lg_Slime_Collision_LPos_Default { -20.0F, +100.0F, 0.0F }

#define To_Slime_Collision_LPos_Default { 0.0F, -50.0F, 0.0F }


void Slime::Init()
{
	m_Hp = 150;

	m_Gravity = 0.5F;
	m_Speed = 200.0F;
	m_Cam = SCENE()->MainCam();

	GRAVITYVEC = { 0,450.0F,0 };
	JUMPVEC = { 0,500.0F,0 };

	m_eState = Slime_State::Mn_Intro;


	{
		m_Collision = ACTOR()->CreateCom<Game_Collision>((int)COLORDER::COLORDER_MONSTER_ForPlayer);
		m_Collision->ColType(COLTYPE::AABB2D);
		m_Collision->LSCALE(Slime_Collision_Scale_Default);
		m_Collision->LPOS(Slime_Collision_LPos_Default);

		m_Collision_1 = ACTOR()->CreateCom<Game_Collision>((int)COLORDER::COLORDER_Monster_ForBullet);
		m_Collision_1->ColType(COLTYPE::AABB2D);
		m_Collision_1->LSCALE(Slime_Collision_Scale_Default);
		m_Collision_1->LPOS(Slime_Collision_LPos_Default);
		m_Collision_1->PushEnterFunc(this, &Slime::Hit);

		m_Collision_ForTo = ACTOR()->CreateCom<Game_Collision>((int)COLORDER::COLORDER_Monster_ForUnknown);
		m_Collision_ForTo->ColType(COLTYPE::AABB2D);
		m_Collision_ForTo->LSCALE({300.f, 100.f, 1.f});
		m_Collision_ForTo->LPOS(To_Slime_Collision_LPos_Default);
		m_Collision_ForTo->PushEnterFunc(this, &Slime::To_AtkCheck);
		m_Collision_ForTo->Off();

		//충돌 범위 테스트용 출력
		m_Collision_Debug_Render = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Debug);
		m_Collision_Debug_Render->LSCALE(Slime_Collision_Scale_Default);
		m_Collision_Debug_Render->LPOS(Slime_Collision_LPos_Default);
		m_Collision_Debug_Render->SPRITE(L"Col.png");

		m_Collision_1_Debug_Render = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Debug);
		m_Collision_1_Debug_Render->LSCALE(Slime_Collision_Scale_Default);
		m_Collision_1_Debug_Render->LPOS(Slime_Collision_LPos_Default);
		m_Collision_1_Debug_Render->SPRITE(L"Col.png");

		//m_Collision_1_Debug = SCENE()->CreateActor(L"Test");
		//m_Collision_1_Debug->TRANS()->WSCALE(m_Collision->WSCALE());
		//m_Collision_1_Debug->TRANS()->WPOS({ m_Collision->WPOS().X,m_Collision->WPOS().Y, -2.f });
		//m_Collision_1_Debug_Render = m_Collision_1_Debug->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Debug);
		//m_Collision_1_Debug_Render->SPRITE(L"Col.png");
	}

	//이펙트
	{
		m_Effect_Animation = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_MAP);
		m_Effect_Animation->LSCALE(Slime_Size_X2);
		m_Effect_Animation->Off();

		m_Effect_Animation_Type = ACTOR()->CreateCom<Game_Animation>(m_Effect_Animation);
		m_Effect_Animation_Type->CreateAni(L"Lg_Explode", L"Lg_Slime_Explode.png", 0, 14, 0.1f, false);
		m_Effect_Animation_Type->ChangeAni(L"Lg_Explode");
		m_Effect_Animation_Type->Off();
	}
	Renderer_Setting();

	Set_Y_Floor;
}

void Slime::Update()
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

		if (m_DebugCheck)
		{
			//m_Collision_Debug_Render->On();
		}
		else
		{
			//m_Collision_Debug_Render->Off();
		}
	}
	switch (m_eState)
	{
	case Slime_State::Mn_Intro:
		Mn_Intro();
		break;
	case Slime_State::Mn_Jump:
		Move();
		Jump();
		PunchCheck();
		break;
	case Slime_State::Mn_Atk:
		Punch();
		break;
	case Slime_State::Mn_Trans:
		Mn_Trans();
		break;
	////////////

	case Slime_State::Lg_Jump:
		Move();
		Jump();
		PunchCheck();
		break;
	case Slime_State::Lg_Atk:
		Lg_Punch();
		break;
	case Slime_State::Lg_Trans:
		Lg_Trans();
		break;

	/////////////
	case Slime_State::To_Move:
		To_AtkCheck();
		To_Move();
		break;
	case Slime_State::To_Atk:
		To_Atk();
		break;
	case Slime_State::To_Dead:
		To_Death();
		break;

	default:
		break;
	}
	DeathCheck();


	if (m_Phase == Slime_Phase::Tomb)
	{
		m_Collision_ForTo->LPOS(To_Slime_Collision_LPos_Default);

		m_Collision_Debug_Render->LSCALE(m_Collision->LSCALE());
		m_Collision_Debug_Render->LPOS({ m_Collision->LPOS().X,m_Collision->LPOS().Y, -2.f });

		m_Collision_1_Debug_Render->LSCALE(m_Collision_1->LSCALE());
		m_Collision_1_Debug_Render->LPOS({ m_Collision_1->LPOS().X,m_Collision_1->LPOS().Y, -2.f });
	}
	else
	{
		m_Collision_1->LSCALE(m_Collision->LSCALE());
		m_Collision_1->LPOS(m_Collision->LPOS());

		m_Collision_Debug_Render->LSCALE(m_Collision->LSCALE());
		m_Collision_Debug_Render->LPOS({ m_Collision->LPOS().X,m_Collision->LPOS().Y, -2.f });

		m_Collision_1_Debug_Render->LSCALE(m_Collision_1->LSCALE());
		m_Collision_1_Debug_Render->LPOS({ m_Collision_1->LPOS().X,m_Collision_1->LPOS().Y, -2.f });
	}
}

void Slime::Hit(Game_Collision* _This, Game_Collision* _Other)
{
	m_Hp--;
}

void Slime::Renderer_Setting()
{
		m_Animation = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_ACTOR);
		m_Animation->LSCALE(Slime_Size_Default);

		m_Animation_Type = ACTOR()->CreateCom<Game_Animation>(m_Animation);

		m_Animation_Type->CreateAni(L"Mn_Intro", L"Mn_Slime_intro.png", 0, 26, 0.1f, false);
		m_Animation_Type->CreateAni(L"Mn_Jump", L"Mn_Slime_Jump.png", 0, 8, 0.1f, true);
		m_Animation_Type->CreateAni(L"Mn_Punch", L"Mn_Slime_Punch.png", 0, 15, 0.1f, false);
		m_Animation_Type->CreateAni(L"Morph", L"Mn_Slime_Morph.png", 0, 47, 0.1f, false);
		m_Animation_Type->CreateAni(L"Mn_Air_Up", L"Mn_Slime_Air.png", 6, 8, 0.1f, true);
		m_Animation_Type->CreateAni(L"Mn_Air_Down", L"Mn_Slime_Air.png", 0, 2, 0.1f, true);
		m_Animation_Type->CreateAni(L"Mn_Air_UpTrans", L"Mn_Slime_Air.png", 9, 11, 0.1f, true);
		m_Animation_Type->CreateAni(L"Mn_Air_DownTrans", L"Mn_Slime_Air.png", 3, 5, 0.1f, true);

		m_Animation_Type->CreateAni(L"Lg_Air_Up", L"Lg_Slime_Air.png", 8, 11, 0.1f, true);
		m_Animation_Type->CreateAni(L"Lg_Air_Down", L"Lg_Slime_Air.png", 0, 4, 0.1f, true);
		m_Animation_Type->CreateAni(L"Lg_Air_UpTrans", L"Lg_Slime_Air.png", 12, 14, 0.1f, true);
		m_Animation_Type->CreateAni(L"Lg_Air_DownTrans", L"Lg_Slime_Air.png", 5, 7, 0.1f, true);
		m_Animation_Type->CreateAni(L"Lg_Air_UptoDownTrans", L"Lg_Slime_Air.png", 15, 19, 0.1f, true);

		m_Animation_Type->CreateAni(L"Lg_Death", L"Lg_Slime_Death.png", 0, 19, 0.1f, true);
		m_Animation_Type->CreateAni(L"Lg_Jump", L"Lg_Slime_Jump.png", 0, 7, 0.1f, true);
		m_Animation_Type->CreateAni(L"Lg_Punch", L"Lg_Slime_Punch.png", 0, 18, 0.2f, true);
		m_Animation_Type->CreateAni(L"Lg_Explode", L"Lg_Slime_Explode.png", 0, 14, 0.1f, true);

		m_Animation_Type->CreateAni(L"To_Dust", L"To_Slime_Dust.png", 0, 16, 0.1f, true);
		m_Animation_Type->CreateAni(L"To_Death", L"To_Slime_Death.png", 0, 5, 0.1f, true);
		m_Animation_Type->CreateAni(L"To_Move_R", L"To_Slime_Move_R.png", 0, 3, 0.1f, true);
		m_Animation_Type->CreateAni(L"To_Move_L", L"To_Slime_Move_L.png", 0, 3, 0.1f, true);
		m_Animation_Type->CreateAni(L"To_Move_Turn_R", L"To_Slime_Move_R.png", 4, 6, 0.1f, false);
		m_Animation_Type->CreateAni(L"To_Move_Turn_L", L"To_Slime_Move_L.png", 4, 6, 0.1f, false);
		m_Animation_Type->CreateAni(L"To_Slam", L"To_Slime_Slam.png", 0, 2, 0.1f, true);
		m_Animation_Type->CreateAni(L"To_Smash", L"To_Slime_Smash.png", 0, 14, 0.1f, true);
		m_Animation_Type->CreateAni(L"To_Trans", L"To_Slime_Trans.png", 0, 3, 0.1f, false);

		m_Animation_Type->ChangeAni(L"Mn_Intro");
}


void Slime::DeathCheck()
{
	if (m_Hp <= 0)
	{
		switch (m_Phase)
		{
		case Slime_Phase::Mini:
			m_Phase = Slime_Phase::Big;
			m_eState = Slime_State::Mn_Trans;
			m_Hp = 200;
			TimeCount_Death = 0.f;
			break;
		case Slime_Phase::Big:
			m_Phase = Slime_Phase::Tomb;
			m_eState = Slime_State::Lg_Trans;
			m_bCheck = true;
			m_Hp = 300;
			break;
		case Slime_Phase::Tomb:
			m_Hp = 1000000.f;
			TimeCount_Death = 0.f;
			m_eState = Slime_State::To_Dead;
			break;
		default:
			break;
		}
	}
}

void Slime::Move()
{
	if (m_Animation_Type->Get_Ani() == L"Mn_Jump" || m_Animation_Type->Get_Ani() == L"Lg_Jump")
		return;

	if (m_bLeft)
	{
		TRANS()->WMOVE(Game_Vector::LEFT * Game_Time::DeltaTime(m_Speed));
		if(m_Phase == Slime_Phase::Tomb)
			m_Animation_Type->ChangeAni(L"To_Move_L");
	}

	else
	{
		TRANS()->WMOVE(Game_Vector::RIGHT * Game_Time::DeltaTime(m_Speed));
		if (m_Phase == Slime_Phase::Tomb)
			m_Animation_Type->ChangeAni(L"To_Move_R");
	}

	if (TRANS()->WPOS().X < -675.f + (m_Collision->LSCALE().X / 2))
	{
		m_bLeft = false;
		m_bCheck = true;
		TRANS()->LSCALE(TRANS()->LSCALE() * Game_Vector { -1.f, 1.f, 1.f });
	}
	else if (TRANS()->WPOS().X > 675.f - (m_Collision->LSCALE().X / 2))
	{
		m_bLeft = true;
		m_bCheck = true;
		TRANS()->LSCALE(TRANS()->LSCALE() * Game_Vector { -1.f, 1.f, 1.f });
	}
}

void Slime::Jump()
{
	if (m_Phase == Slime_Phase::Mini)
	{
		if (Animation_Check(L"Mn_Jump", 8) || m_JumpCheck)
		{
			m_JumpCheck = true;

			if (JUMPVEC.Y <= 0)
			{
				if (m_bCheck)
					m_Animation_Type->ChangeAni(L"Mn_Air_DownTrans");
				else
					m_Animation_Type->ChangeAni(L"Mn_Air_Down");
			}
			else if(JUMPVEC.Y > 0)
			{
				if (m_bCheck)
					m_Animation_Type->ChangeAni(L"Mn_Air_UpTrans");
				else
					m_Animation_Type->ChangeAni(L"Mn_Air_Up");
			}
		}
		else if(!m_JumpCheck)
		{
			m_Animation_Type->ChangeAni(L"Mn_Jump");
		}

		if (Animation_Check(L"Mn_Air_DownTrans", 5) || Animation_Check(L"Mn_Air_UpTrans", 11))
		{
			m_bCheck = false;
		}
	}
	else if (m_Phase == Slime_Phase::Big)
	{
		if (Animation_Check(L"Lg_Jump", 7) || m_JumpCheck)
		{
			m_JumpCheck = true;

			if (JUMPVEC.Y <= 0)
			{
				if (m_bCheck)
					m_Animation_Type->ChangeAni(L"Lg_Air_DownTrans");
				else
					m_Animation_Type->ChangeAni(L"Lg_Air_Down");
			}
			else if (JUMPVEC.Y > 0)
			{
				if (m_bCheck)
					m_Animation_Type->ChangeAni(L"Lg_Air_UpTrans");
				else
					m_Animation_Type->ChangeAni(L"Lg_Air_Up");
			}
		}
		else if (!m_JumpCheck)
		{
			m_Animation_Type->ChangeAni(L"Lg_Jump");
		}

		if (Animation_Check(L"Lg_Air_DownTrans", 7) || Animation_Check(L"Lg_Air_UpTrans", 19))
		{
			m_bCheck = false;
		}
	}

	if (m_JumpCheck)
	{
		TRANS()->WMOVE(JUMPVEC * Game_Time::DeltaTime());
		JUMPVEC -= GRAVITYVEC * Game_Time::DeltaTime();
	}

	if (TRANS()->LPOS().Y < Floor + (m_Collision->LSCALE().Y / 2) - m_Collision->LPOS().Y)
	{
		TRANS()->LPOS({ TRANS()->LPOS().X , Floor + (m_Collision->LSCALE().Y / 2) - m_Collision->LPOS().Y ,TRANS()->LPOS().Z });
		GRAVITYVEC = { 0,450.0F,0 };
		JUMPVEC = { 0,500.0F,0 };
		m_JumpCheck = false;
		m_bCheck = false;
	}
}

void Slime::PunchCheck()
{
	TimeCount += Game_Time::DeltaTime();

	if (TimeCount >= 3.f  //쿨타임
		&& TRANS()->LPOS().Y <= Floor + (m_Collision->LSCALE().Y / 2) - m_Collision->LPOS().Y //땅바닥에 있는가?
		)
	{
		switch (m_Phase)
		{
		case Slime_Phase::Mini:
			m_eState = Slime_State::Mn_Atk;
			break;
		case Slime_Phase::Big:
			m_eState = Slime_State::Lg_Atk;
			break;
		default:
			break;
		}

		TimeCount = 0.f;
	}
}

void Slime::Punch()
{
	m_Animation_Type->ChangeAni(L"Mn_Punch");
	m_Animation->LSCALE({ 1200.f, 1200.f, 1.0f });

	if (m_Animation_Type->Get_Ani() == L"Mn_Punch")
	{
		if (m_Animation_Type->Get_Num() == 15)
		{
			m_eState = Slime_State::Mn_Jump;
			m_Animation->LSCALE(Slime_Size_Default);

			m_Collision->LSCALE(Slime_Collision_Scale_Default);
			m_Collision->LPOS(Slime_Collision_LPos_Default);
		}

		else if(m_Animation_Type->Get_Num() >= 8 && m_Animation_Type->Get_Num() <= 12)
		{
			m_Collision->LSCALE({ 300.f, 200.f, 1.f });
			m_Collision->LPOS({ -150.0F, +50.0F, -1.0F });
		}

		else
		{
			m_Collision->LSCALE(Slime_Collision_Scale_Default);
			m_Collision->LPOS(Slime_Collision_LPos_Default);
		}
	}

}

void Slime::Mn_Intro()
{
	if (m_Animation_Type->Get_Ani() == L"Mn_Intro")
	{
		if (m_Animation_Type->Get_Num() == 26)
		{
			m_eState = Slime_State::Mn_Jump;
		}
	}
}

void Slime::Mn_Trans()
{
	m_Animation_Type->ChangeAni(L"Morph");
	m_Animation->LSCALE(Slime_Size_X2);

	if (m_Animation_Type->Get_Num() == 47)
	{
		m_eState = Slime_State::Lg_Jump;
		m_Phase = Slime_Phase::Big;
		m_Animation->LSCALE(Slime_Size_Default);
		m_Collision->LSCALE(Lg_Slime_Collision_Scale_Default);
		m_Collision->LPOS(Lg_Slime_Collision_LPos_Default);
	}
}

void Slime::Lg_Punch()
{
	m_Animation_Type->ChangeAni(L"Lg_Punch");
	m_Animation->LSCALE(Slime_Size_X2);

	if (m_Animation_Type->Get_Ani() == L"Lg_Punch")
	{
		if (m_Animation_Type->Get_Num() == 18)
		{
			m_eState = Slime_State::Lg_Jump;
			m_Animation->LSCALE(Slime_Size_Default);

			m_Collision->LSCALE(Lg_Slime_Collision_Scale_Default);
			m_Collision->LPOS(Lg_Slime_Collision_LPos_Default);
		}

		else if (m_Animation_Type->Get_Num() >= 12 && m_Animation_Type->Get_Num() <= 18)
		{
			m_Collision->LSCALE({ 450.f, 200.f, 1.f });
			m_Collision->LPOS({ -200.0F, +150.0F, 0.0F });
		}

		else
		{
			m_Collision->LSCALE(Lg_Slime_Collision_Scale_Default);
			m_Collision->LPOS(Lg_Slime_Collision_LPos_Default);
		}
	}
}

void Slime::Lg_Trans()
{
	TimeCount_Death += Game_Time::DeltaTime();

	if (TimeCount_Death < 4.0f && m_bCheck)
	{
		m_Animation->LSCALE({ 600.f, 600.f, 1.f });
		m_Animation_Type->ChangeAni(L"Lg_Death");
		m_Collision->LSCALE(Lg_Slime_Collision_Scale_Default);
		m_Collision->LPOS(Lg_Slime_Collision_LPos_Default);
	}

	else if (TimeCount_Death >= 4.f || !m_bCheck)
	{
		m_Animation->LSCALE({ 1800.f, 1800.f, 1.f });
		m_Effect_Animation->On();
		m_Effect_Animation_Type->On();
		m_Effect_Animation->WPOS({ TRANS()->WPOS().X, TRANS()->WPOS().Y, -1.F });

		if (m_bCheck) { //떨어지는거 보여주려고 위치 높게 설정. (초기 1회)
			TRANS()->WPOS({ TRANS()->WPOS().X, 250.f , 1.f });
			m_bCheck = false;
		}

		TRANS()->WMOVE(Game_Vector::DOWN * Game_Time::DeltaTime(m_Speed*2));

		if (TRANS()->WPOS().Y <= Floor + (m_Collision->LSCALE().Y / 2)) //땅까지 내려왔으면 애니매이션 변경
		{
			m_Animation_Type->ChangeAni(L"To_Slam");
			TRANS()->WPOS({ TRANS()->WPOS().X, Floor + m_Collision->LSCALE().Y / 2, 1.f });
		}

		else //떨어지는 중에는 이 애니매이션
		{
			m_Animation_Type->ChangeAni(L"To_Trans");
		}

		if (m_Effect_Animation_Type->Get_Num() == 14)
		{
			m_eState = Slime_State::To_Move;
			To_HitCheck = 0;

			m_Collision_1->LSCALE(Slime_Collision_Scale_Default);
			m_Collision_1->LPOS({ m_Collision_1->LPOS().X , m_Collision_1->LPOS().Y + 200.f ,m_Collision_1->LPOS().Z});

			m_Collision->LSCALE({ 360.f, 200.f, 1.f });
			m_Collision->LPOS(To_Slime_Collision_LPos_Default);
			m_Collision->Off();

			m_Collision_ForTo->On();

			m_Effect_Animation->Off();
			m_Effect_Animation_Type->Off();
		}
	}
}

void Slime::To_AtkCheck(Game_Collision* _This, Game_Collision* _Other)
{
	To_HitCheck++;
}

void Slime::To_AtkCheck()
{
	if (To_HitCheck >= 2)
	{
		m_eState = Slime_State::To_Atk;
		To_HitCheck = 0;
	}
}

void Slime::To_Atk()
{
	m_Animation_Type->ChangeAni(L"To_Smash");

	if (Animation_Check(L"To_Smash", 14))
	{
		m_eState = Slime_State::To_Move;
		To_HitCheck = 0;
		m_Collision->Off();

	}
	else if (Animation_Check(L"To_Smash", 8, 11))
	{
		m_Collision->On();
	}
	else
	{
		m_Collision->Off();
	}
}

void Slime::To_Death()
{
	TimeCount_Death += Game_Time::DeltaTime();

	m_Animation_Type->ChangeAni(L"To_Death");

	if (TimeCount_Death >= 2.5f)
	{
		ACTOR()->Death();
		Game_Scene::ChangeScene(L"WorldMap");
	}
}

void Slime::To_Move()
{
	if (TRANS()->LSCALE().X <= 0)
	{
		TRANS()->Reverse();
	}

	if (m_bLeft)
	{
		TRANS()->WMOVE(Game_Vector::LEFT * Game_Time::DeltaTime(m_Speed));
		m_Animation_Type->ChangeAni(L"To_Move_L");
	}

	else
	{
		TRANS()->WMOVE(Game_Vector::RIGHT * Game_Time::DeltaTime(m_Speed));
		m_Animation_Type->ChangeAni(L"To_Move_R");
	}

	if (TRANS()->WPOS().X < -675.f + (m_Collision->LSCALE().X / 2))
	{
		m_bLeft = false;
		m_bCheck = true;
	}
	else if (TRANS()->WPOS().X > 675.f - (m_Collision->LSCALE().X / 2))
	{
		m_bLeft = true;
		m_bCheck = true;
		//TRANS()->LSCALE(TRANS()->LSCALE() * Game_Vector { -1.f, 1.f, 1.f });
	}
}
