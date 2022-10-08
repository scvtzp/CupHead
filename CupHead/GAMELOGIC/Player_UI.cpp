#include "Player_UI.h"
#include "Logic_Enum.h"

int Player_UI::Hp = 3;
Bullet_Type Player_UI::BulletType = Bullet_Type::End;

void Player_UI::Init()
{
	Past_BulletType = BulletType;

	//무기 아이콘 출력.
	m_Collision_Debug_Render = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Debug);
	m_Collision_Debug_Render->LSCALE({100.f, 100.f, 1.f});
	m_Collision_Debug_Render->LPOS({ -570.f, -360.f, -100.f });
	m_Collision_Debug_Render->SPRITE(L"equip_icon_weapon_peashot.png");
	m_Collision_Debug_Render->Off();

	//Hp출력
	m_Collision_1_Debug_Render = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Debug);
	m_Collision_1_Debug_Render->LSCALE({ 90.f, 40.f, 1.f });
	m_Collision_1_Debug_Render->LPOS({ -570.f, -310.f, -90.f });
	m_Collision_1_Debug_Render->SPRITE(L"Hp_3.png");
}

void Player_UI::Update()
{
	//총알 교환
	if (Past_BulletType != BulletType)
	{
		m_Collision_Debug_Render->On();
		Bullet_PopUp_Timer += Game_Time::DeltaTime();
		if (Bullet_PopUp_Timer >= 0.7f)
		{
			Past_BulletType = BulletType;
			Bullet_PopUp_Timer = 0.f;
			m_Collision_Debug_Render->Set_LPosY(-360.f);
			m_Collision_Debug_Render->Off();
		}
		else
		{
			m_Collision_Debug_Render->Set_LPosY(m_Collision_Debug_Render->LPOS().Y + 1.f);
			if (m_Collision_Debug_Render->LPOS().Y >= -270.f)
			{
				m_Collision_Debug_Render->Set_LPosY(-270.f);
			}
			switch (BulletType)
			{
			case Bullet_Type::Peashot:
				m_Collision_Debug_Render->SPRITE(L"equip_icon_weapon_peashot.png");
				break;
			case Bullet_Type::Bouncer:
				break;
			case Bullet_Type::Charge:
				m_Collision_Debug_Render->SPRITE(L"equip_icon_weapon_charge.png");
				break;
			case Bullet_Type::Spread:
				m_Collision_Debug_Render->SPRITE(L"equip_icon_weapon_spread.png");
				break;
			case Bullet_Type::Sky_Bomb:
				m_Collision_Debug_Render->SPRITE(L"shmup_icon_bomb.png");
				break;
			case Bullet_Type::Sky_Peashot:
				m_Collision_Debug_Render->SPRITE(L"shmup_icon_bullet.png");
				break;
			case Bullet_Type::End:
				break;
			default:
				break;
			}
		}
	}

	//HP바
	switch (Hp)
	{
	case 1:
		HpTime += Game_Time::DeltaTime();
		if (HpTime >= 0.3f)
		{
			HpTime = 0.f;
			if (Hp1Check)
			{
				m_Collision_1_Debug_Render->SPRITE(L"Hp_1R.png");
				Hp1Check = false;
			}
			else if (!Hp1Check)
			{
				m_Collision_1_Debug_Render->SPRITE(L"Hp_1W.png");
				Hp1Check = true;
			}
		}
		break;
	case 2:
		m_Collision_1_Debug_Render->SPRITE(L"Hp_2.png");
		break;
	case 3:
		m_Collision_1_Debug_Render->SPRITE(L"Hp_3.png");
		break;
	default:
		break;
	}
}