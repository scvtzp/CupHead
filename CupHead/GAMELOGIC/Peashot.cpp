#include "Logic_Enum.h"
#include "Peashot.h"

#define Peashot_Collision_Size {50.f, 50.f, 1.f}
#define Peashot_Collision_LPos {0.f, 0.f, -1.f}
void Peashot::Init()
{
	m_Speed = 400.0F;
	m_Cam = SCENE()->MainCam();

	{
		m_Collision = ACTOR()->CreateCom<Game_Collision>((int)COLORDER::COLORDER_Bullet);
		m_Collision->ColType(COLTYPE::AABB2D);
		m_Collision->LSCALE(Peashot_Collision_Size);
		m_Collision->LPOS(Peashot_Collision_LPos);
		m_Collision->PushEnterFunc(this, &Peashot::Hit);

		//�浹 ���� �׽�Ʈ�� ���
		m_Collision_Debug_Render = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Debug);
		m_Collision_Debug_Render->LSCALE(Peashot_Collision_Size);
		m_Collision_Debug_Render->LPOS(Peashot_Collision_LPos);
		m_Collision_Debug_Render->SPRITE(L"Col.png");
	}

	{
		m_Animation = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_ACTOR);
		m_Animation->LSCALE({ 400.f, 400.f, 1.0f });

		m_Animation_Type = ACTOR()->CreateCom<Game_Animation>(m_Animation);

		m_Animation_Type->CreateAni(L"Death", L"Peashot.png", 0, 5, 0.05f, true);
		m_Animation_Type->CreateAni(L"Intro", L"Peashot.png", 6, 11, 0.15f, false);
		m_Animation_Type->CreateAni(L"Main", L"Peashot.png", 12, 17, 0.05f, true);
		m_Animation_Type->CreateAni(L"Spark", L"Peashot.png", 18, 21, 0.05f, false);

		m_Animation_Type->ChangeAni(L"Spark");
	}
}

void Peashot::Update()
{
	if (m_Animation_Type->Get_Ani() == L"Spark")
	{
		if (m_Animation_Type->Get_Num() == 21)
		{
			m_Animation_Type->ChangeAni(L"Main");
		}
	}

	//�������� ����
	if (TRANS()->WPOS().X <= -640.f ||
		TRANS()->WPOS().X >= 640.f ||
		TRANS()->WPOS().Y <= -360.f ||
		TRANS()->WPOS().Y >= 360.f)
	{
		Bullet_Death();
		return;
	}
	else if (DeathCheck) //��Ƶ� ����
	{
		Bullet_Death();
		return;
	}
	Debug();
	Move();

}

void Peashot::Hit(Game_Collision* _This, Game_Collision* _Other)
{
	DeathCheck = true;
}

void Peashot::Move()
{
	//��
	if (TRANS()->WROT().Z == 0.f)
	{
		TRANS()->WMOVE(Game_Vector::RIGHT * Game_Time::DeltaTime(m_Speed));
	}
	//��
	else if (TRANS()->WROT().Z == 180.f)
	{
		TRANS()->WMOVE(Game_Vector::LEFT * Game_Time::DeltaTime(m_Speed));
	}
	//��
	else if (TRANS()->WROT().Z == 90.f)
	{
		TRANS()->WMOVE(Game_Vector::UP * Game_Time::DeltaTime(m_Speed));
	}
	//��
	else if (TRANS()->WROT().Z == 270.f)
	{
		TRANS()->WMOVE(Game_Vector::DOWN * Game_Time::DeltaTime(m_Speed));
	}
	//���
	else if (TRANS()->WROT().Z == 45.f)
	{
		TRANS()->WMOVE(Game_Vector::UP * Game_Time::DeltaTime(m_Speed));
		TRANS()->WMOVE(Game_Vector::RIGHT * Game_Time::DeltaTime(m_Speed));
	}
	//����
	else if (TRANS()->WROT().Z == 315.f)
	{
		TRANS()->WMOVE(Game_Vector::DOWN * Game_Time::DeltaTime(m_Speed));
		TRANS()->WMOVE(Game_Vector::RIGHT * Game_Time::DeltaTime(m_Speed));
	}
	//�»�
	else if (TRANS()->WROT().Z == 135.f)
	{
		TRANS()->WMOVE(Game_Vector::UP * Game_Time::DeltaTime(m_Speed));
		TRANS()->WMOVE(Game_Vector::LEFT * Game_Time::DeltaTime(m_Speed));
	}
	//����
	else if (TRANS()->WROT().Z == 225.f)
	{
		TRANS()->WMOVE(Game_Vector::DOWN * Game_Time::DeltaTime(m_Speed));
		TRANS()->WMOVE(Game_Vector::LEFT * Game_Time::DeltaTime(m_Speed));
	}
}

void Peashot::Debug()
{
	//�����
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
			m_Collision_Debug_Render->On();
			//m_Collision_Debug->TRANS()->LPOS({ m_Collision->WPOS().X,m_Collision->WPOS().Y, -2.f });
		}
		else
		{
			m_Collision_Debug_Render->Off();
		}
	}
}

void Peashot::Bullet_Death()
{
	m_Animation_Type->ChangeAni(L"Death");

	if (m_Animation_Type->Get_Num() >= m_Animation_Type->Get_EndNum())
	{
		ACTOR()->Death();
		m_Collision_Debug_Render->Death();
	}
}
