#include "Monkey.h"
#include "Monkey_Card.h"
#include "Monkey_Bullet.h"
#include "Logic_Enum.h"

#define Monkey_Size_Def {500.f, 500.f, 1.f}
#define Monkey_Collision_Scale_Def {200.f, 200.f, 1.f}
#define Monkey_Collision_LPos_Def {0.f, 0.f, 0.f}

bool Monkey::State_Check = true;
bool Monkey::Shake_Check = true;

void Monkey::Init()
{
	if (nullptr == Game_Input::FindKey(L"Test"))
		Game_Input::CreateKey(L"Test", 'T');

	m_Hp = 200;

	m_Gravity = 0.5F;
	m_Speed = 300.0F;
	m_Cam = SCENE()->MainCam();

	GRAVITYVEC = { 0,450.0F,0 };
	JUMPVEC = { 0,500.0F,0 };

	m_eState = Monkey_State::Intro;

	{
		m_Collision = ACTOR()->CreateCom<Game_Collision>((int)COLORDER::COLORDER_MONSTER_ForPlayer);
		m_Collision->ColType(COLTYPE::AABB2D);
		m_Collision->LSCALE(Monkey_Collision_Scale_Def);
		m_Collision->LPOS(Monkey_Collision_LPos_Def);

		m_Collision_1 = ACTOR()->CreateCom<Game_Collision>((int)COLORDER::COLORDER_Monster_ForBullet);
		m_Collision_1->ColType(COLTYPE::AABB2D);
		m_Collision_1->LSCALE(Monkey_Collision_Scale_Def);
		m_Collision_1->LPOS(Monkey_Collision_LPos_Def);
		m_Collision_1->PushEnterFunc(this, &Monkey::Hit);

		//충돌 범위 테스트용 출력
		m_Collision_Debug_Render = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Debug);
		m_Collision_Debug_Render->LSCALE(Monkey_Collision_Scale_Def);
		m_Collision_Debug_Render->LPOS(Monkey_Collision_LPos_Def);
		m_Collision_Debug_Render->SPRITE(L"Col.png");
	}


	//Monkey_card 세팅
	{
		for (int i = 0; i < 12; ++i)
		{
			Game_Ptr<Game_Actor> Ptr = SCENE()->CreateActor(L"Monkey_Card");
			Ptr->TRANS()->WSCALE({ 1.f, 1.f, 1.f });
			Game_Ptr<Monkey_Card> Card = Ptr->CreateCom<Monkey_Card>();

			if (i == 1 || i == 2)
				Card->Set_CardType(Monkey_CardType::Cuphead);
			if (i == 3 || i == 4)
				Card->Set_CardType(Monkey_CardType::MugMan);
			if (i == 5 || i == 6)
				Card->Set_CardType(Monkey_CardType::Chips);
			if (i == 7 || i == 8)
				Card->Set_CardType(Monkey_CardType::Flowers);
			if (i == 9 || i == 10)
				Card->Set_CardType(Monkey_CardType::Shield);
			if (i == 11 || i == 0)
				Card->Set_CardType(Monkey_CardType::Spindle);

			Deck.emplace_back(Card);
		}

		random_shuffle(Deck.begin(), Deck.end());		
		int i = 0;
		for (auto* itor : Deck)
		{
			itor->TRANS()->WPOS({ (float)(i % 4) * -250 + 375 , (float)(i / 4) * -200 + 200 , 1.f });	
			++i;
		}
	}

	Renderer_Setting();
}

void Monkey::Update()
{
	if(m_eState != Monkey_State::Intro && m_eState != Monkey_State::Death)
		Move();

	switch (m_eState)
	{
	case Monkey_State::Intro:
		Intro();
		break;
	case Monkey_State::Idle_Closed:
		m_Collision_1->Off();
		CardDown();
		Idle_Closed();
		break;
	case Monkey_State::Idle_Open:
		m_Collision_1->On();
		CardUp();
		Idle_Open();
		break;
	case Monkey_State::Atk:
		CardUp();
		Atk();
		break;
	case Monkey_State::Closed_to_Open:
		Closed_to_Open();
		break;
	case Monkey_State::Open_to_Closed:
		Open_to_Closed();
		break;
	case Monkey_State::Shake:
		Shake();
		break;
	case Monkey_State::Death:
		Death();
		break;
	default:
		break;
	}

	if (!Shake_Check)
	{
		m_eState = Monkey_State::Shake;
	}
	if (m_Hp <= 0)
	{
		m_eState = Monkey_State::Death;
	}
}

void Monkey::Hit(Game_Collision* _This, Game_Collision* _Other)
{
	if (m_eState != Monkey_State::Idle_Closed && m_eState != Monkey_State::Shake)
	{
		m_Hp--;
	}
}

void Monkey::Renderer_Setting()
{
	m_Animation = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_ACTOR);
	m_Animation->LSCALE(Monkey_Size_Def);

	m_Animation_Type = ACTOR()->CreateCom<Game_Animation>(m_Animation);

	m_Animation_Type->CreateAni(L"Close_to_Open", L"Monkey_Closed_to_Open.png", 0, 5, 0.3f, true);
	m_Animation_Type->CreateAni(L"Open_to_Close", L"Monkey_Open_to_Close.png", 0, 7, 0.3f, true);
	m_Animation_Type->CreateAni(L"Idle_Closed", L"Monkey_Idle_Closed.png", 0, 10, 0.1f, true);
	m_Animation_Type->CreateAni(L"Idle_Open", L"Monkey_Idle_Open.png", 0, 10, 0.1f, true);
	m_Animation_Type->CreateAni(L"Shake", L"Monkey_Shake.png", 0, 4, 0.1f, true);
	m_Animation_Type->CreateAni(L"Intro", L"Monkey_Intro.png", 0, 21, 0.1f, false);
	m_Animation_Type->CreateAni(L"Death", L"Monkey_Death.png", 0, 10, 0.1f, true);
	m_Animation_Type->CreateAni(L"Attack", L"Monkey_Attack.png", 0, 10, 0.3f, true); //어택1
	m_Animation_Type->CreateAni(L"Open_Idle", L"Monkey_open_idle.png", 0, 4, 0.3f, true); //어택2

	m_Animation_Type->ChangeAni(L"Idle_Closed");


	//Claw
	m_Animation_Claw = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_ACTOR);
	m_Animation_Claw->LSCALE({300.f, 1500.f, 1.f});
	m_Animation_Claw->LPOS({0.f, 640.f, 1.f});

	m_Animation_Type_Claw = ACTOR()->CreateCom<Game_Animation>(m_Animation_Claw);
					
	m_Animation_Type_Claw->CreateAni(L"Claw_open", L"Monkey_Claw__open(11)_Closed(22).png", 0, 10, 0.1f, true);
	m_Animation_Type_Claw->CreateAni(L"Claw_Closed", L"Monkey_Claw__open(11)_Closed(22).png", 11, 32, 0.1f, true);
	m_Animation_Type_Claw->CreateAni(L"Claw_Attack", L"Monkey_Claw_Attack.png", 0, 10, 0.1f, true);
	m_Animation_Type_Claw->CreateAni(L"Claw_Death", L"Monkey_Claw_Death.png", 0, 10, 0.1f, true);
	m_Animation_Type_Claw->CreateAni(L"Claw_Closed_to_Open", L"Monkey_Claw_Closed_to_Open.png", 0, 5, 0.1f, true);
	m_Animation_Type_Claw->CreateAni(L"Claw_Open_Idle", L"Monkey_Claw_Open_Idle.png", 0, 4, 0.1f, true);
	m_Animation_Type_Claw->CreateAni(L"Claw_Open_to_Closed", L"Monkey_Claw_Open_to_Closed.png", 0, 7, 0.1f, true);
	m_Animation_Type_Claw->CreateAni(L"Claw_Shake", L"Monkey_Claw_Shake.png", 0, 4, 0.1f, true);

	m_Animation_Type_Claw->ChangeAni(L"Claw_open");
}

void Monkey::Move()
{
	//좌우
	switch (m_LR)
	{
	case Monky_Course::Left:
		TRANS()->WMOVE(Game_Vector::LEFT * Game_Time::DeltaTime(m_Speed));
		break;
	case Monky_Course::Right:
		TRANS()->WMOVE(Game_Vector::RIGHT * Game_Time::DeltaTime(m_Speed));
		break;
	default:
		break;
	}

	if (TRANS()->LPOS().X < -640.f || TRANS()->LPOS().X > 640.f)
	{
		TRANS()->Reverse();
		switch (m_LR)
		{
		case Monky_Course::Left:
			m_LR = Monky_Course::Right;
			break;
		case Monky_Course::Right:
			m_LR = Monky_Course::Left;
			break;
		default:
			break;
		}
	}

	//상하
	switch (m_UD)
	{
	case Monky_Course::Up:
		TRANS()->WMOVE(Game_Vector::UP * Game_Time::DeltaTime(m_Speed));
		break;
	case Monky_Course::Down:
		TRANS()->WMOVE(Game_Vector::DOWN * Game_Time::DeltaTime(m_Speed));
		break;
	default:
		break;
	}

	if (TRANS()->LPOS().Y < -360.f || TRANS()->LPOS().Y > 360.f)
	{
		switch (m_UD)
		{
		case Monky_Course::Up:
			m_UD = Monky_Course::Down;
			break;
		case Monky_Course::Down:
			m_UD = Monky_Course::Up;
			break;
		default:
			break;
		}
	}
}

void Monkey::Intro()
{
	m_Animation_Type->ChangeAni(L"Intro");

	if (m_Animation_Type->Get_Num() == 21)
	{
		m_eState = Monkey_State::Idle_Closed;
	}
}

void Monkey::Idle_Closed()
{
	m_Animation_Type->ChangeAni(L"Idle_Closed");
	m_Animation_Type_Claw->ChangeAni(L"Claw_open");
	if (!State_Check)
	{
		m_eState = Monkey_State::Closed_to_Open;
		State_Check = true;
	}
}

void Monkey::Idle_Open()
{
	m_Animation_Type->ChangeAni(L"Idle_Open");
	m_Animation_Type_Claw->ChangeAni(L"Claw_Closed");

	OpenCount += Game_Time::DeltaTime();
	TimeCount += Game_Time::DeltaTime();	
	if (TimeCount >= 2.f) //쿨타임
	{
		m_eState = Monkey_State::Atk;
		TimeCount = 0.f;
	}
	if (OpenCount >= 15.f)
	{
		m_eState = Monkey_State::Open_to_Closed;
		OpenCount = 0.f;
	}
}

void Monkey::Closed_to_Open()
{
	m_Animation_Type->ChangeAni(L"Close_to_Open");
	if(m_Animation_Type->Get_Num() == 4)
		m_eState = Monkey_State::Idle_Open;
}

void Monkey::Open_to_Closed()
{
	m_Animation_Type->ChangeAni(L"Open_to_Close");
	if (m_Animation_Type->Get_Num() == 6)
		m_eState = Monkey_State::Idle_Closed;

}

void Monkey::Atk()
{
	
	if(Atk_Ani_Check)
		m_Animation_Type->ChangeAni(L"Open_Idle");
	else
		m_Animation_Type->ChangeAni(L"Attack");
	
	if (m_Animation_Type->Get_Num() == 3 && !Atk_Ani_Check)
	{
		//총알 발사
		{
			for (int i = 0; i < 8; ++i)
			{
				Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"Bullet");
				PTR->TRANS()->WSCALE({ 1.f, 1.f, 1.f });
				PTR->TRANS()->WROT({ 0.f, 0.f, i * 45.0f });
				PTR->TRANS()->WPOS(TRANS()->WPOS());
				PTR->CreateCom<Monkey_Bullet>();
			}
		}
		Atk_Ani_Check = true;
	}



	if (m_Animation_Type->Get_Num() == 4 && Atk_Ani_Check)
	{
		Atk_Ani_Check = false;
		m_eState = Monkey_State::Idle_Open;
	}
}

void Monkey::Shake()
{
	m_Animation_Type->ChangeAni(L"Shake");
	m_Animation_Type_Claw->ChangeAni(L"Claw_Shake");

	m_Speed = 400.f; //발업
	Shake_Time += Game_Time::DeltaTime();
	if(Shake_Time >= 2.f)
	{
		Shake_Check = true;
		Shake_Time = 0.f;
		m_Speed = 300.f; //복구
		m_eState = Monkey_State::Idle_Closed;
	}
}

void Monkey::Death()
{
	m_Animation_Type->ChangeAni(L"Death");
	m_Animation_Type_Claw->ChangeAni(L"Claw_Death");

	Shake_Time += Game_Time::DeltaTime();
	if (Shake_Time >= 3.f) //ShakeTime -> DeathTime 재활용 
	{
		Game_Scene::ChangeScene(L"WorldMap");
	}
}

void Monkey::CardUp()
{
	bool Ch = true;
	if (CardUpDown_Check)
		return;

	for (auto* itor : Deck)
	{
		itor->TRANS()->WMOVE(Game_Vector::UP * Game_Time::DeltaTime(m_Speed*2));
		if (Ch)
		{
			Ch = false;
			if (itor->TRANS()->LPOS().Y >= 920.f)
			{
				CardUpDown_Check = true;
			}
		}
	}

}

void Monkey::CardDown()
{
	bool Ch = true;
	if (!CardUpDown_Check)
	{
		int i = 0;
		for (auto* itor : Deck)
		{
			itor->TRANS()->WPOS({ (float)(i % 4) * -250 + 375 , (float)(i / 4) * -200 + 200 , 1.f });
			++i;
		}
		return;
	}

	for (auto* itor : Deck)
	{
		itor->TRANS()->WMOVE(Game_Vector::DOWN * Game_Time::DeltaTime(m_Speed*2));
		if (Ch)
		{
			Ch = false;
			if (itor->TRANS()->LPOS().Y <= 200)
			{
				CardUpDown_Check = false;
			}
		}
	}

}