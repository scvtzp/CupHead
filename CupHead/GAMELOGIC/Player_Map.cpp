#include "Player_Map.h"
#include "Game_FreeCam.h"
#include "LOGIC_Value.h"
#include "Logic_Enum.h"
#include <Game_3D_Debug.h>

#define Player_Map_Size {150.f, 150.f, 1.f}
#define Player_Map_LPos {0.f, 0.f, 0.f}

Game_Vector Player_Map::Player_Pos = { -99999.f, -99999.f, -99999.f };
bool Player_Map::Player_Entrance_Check = false;
bool Player_Map::MoveCheck = true;
int Player_Map::Gold = 0;

void Player_Map::Init()
{
	m_Gravity = 0.5F;
	m_Speed = 500.0F;
	m_Cam = SCENE()->MainCam();

	ColorCheckDir[0] = Game_Vector{-20.f, 0.f}; // L
	ColorCheckDir[1] = Game_Vector{ 20.0F, 0.0F }; // R
	ColorCheckDir[2] = Game_Vector{ 0.0F, 0.0F, 0.0f, 1.0f }; //UP
	ColorCheckDir[3] = Game_Vector{ 0.0F, -40.0F }; // Down
	
	//충돌
	{
		m_Collision = ACTOR()->CreateCom<Game_Collision>((int)COLORDER::COLORDER_PLAYER_ForUnknown);
		m_Collision->ColType(COLTYPE::AABB2D);
		m_Collision->LSCALE({100.f, 100.f, 1.f});
		m_Collision->LPOS(Player_Map_LPos);
		m_Collision->PushStayFunc(this, &Player_Map::Hit);

		m_Collision_Debug_Render = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Debug);
		m_Collision_Debug_Render->LSCALE(m_Collision->LSCALE());
		m_Collision_Debug_Render->LPOS({ m_Collision->LPOS().X,m_Collision->LPOS().Y, -2.f });
		m_Collision_Debug_Render->SPRITE(L"Col.png");
	}

	Renderer_Setting();
}

void Player_Map::Update()
{
	//메뉴 여는 동안에는 업데이트 안함.
	if (!MoveCheck)
		return;

	//충돌체크
	Collision();
	Debug();
	//위치 동기화
	Player_Pos = TRANS()->WPOS();

	if (Game_Input::Press(L"PLAYERLEFT") && !ArrColorCheck[0])
	{
		Present_AnimationType = Player_Map_Animation_Type::Move_L;
		TRANS()->WMOVE(Game_Vector::LEFT * Game_Time::DeltaTime(m_Speed));
		if (TRANS()->WPOS().X >= 640.f && TRANS()->WPOS().X <= 3072.f)
			m_Cam->TRANS()->WMOVE(Game_Vector::LEFT * Game_Time::DeltaTime(m_Speed));

		if (Game_Input::Press(L"PLAYERUP") && !ArrColorCheck[2])
		{
			Present_AnimationType = Player_Map_Animation_Type::Move_L_Up;
			TRANS()->WMOVE(Game_Vector::UP * Game_Time::DeltaTime(m_Speed));
			if (TRANS()->WPOS().Y <= -360.f && TRANS()->WPOS().Y >= -1640.f)
				m_Cam->TRANS()->WMOVE(Game_Vector::UP * Game_Time::DeltaTime(m_Speed));
		}

		else if (Game_Input::Press(L"PLAYERDOWN") && !ArrColorCheck[3])
		{
			Present_AnimationType = Player_Map_Animation_Type::Move_L_Down;
			TRANS()->WMOVE(Game_Vector::DOWN * Game_Time::DeltaTime(m_Speed));
			if (TRANS()->WPOS().Y <= -360.f && TRANS()->WPOS().Y >= -1640.f)
				m_Cam->TRANS()->WMOVE(Game_Vector::DOWN * Game_Time::DeltaTime(m_Speed));
		}
	}

	else if (Game_Input::Press(L"PLAYERRIGHT") && !ArrColorCheck[1])
	{
		Present_AnimationType = Player_Map_Animation_Type::Move_R;
		TRANS()->WMOVE(Game_Vector::RIGHT * Game_Time::DeltaTime(m_Speed));

		if (TRANS()->WPOS().X >= 640.f && TRANS()->WPOS().X <= 3072.f)
			m_Cam->TRANS()->WMOVE(Game_Vector::RIGHT * Game_Time::DeltaTime(m_Speed));
		if (Game_Input::Press(L"PLAYERUP") && !ArrColorCheck[2])
		{
			Present_AnimationType = Player_Map_Animation_Type::Move_R_Up;
			TRANS()->WMOVE(TRANS()->WUP() * Game_Time::DeltaTime(m_Speed));
			if (TRANS()->WPOS().Y <= -360.f && TRANS()->WPOS().Y >= -1640.f)
				m_Cam->TRANS()->WMOVE(Game_Vector::UP * Game_Time::DeltaTime(m_Speed));
		}
		else if (Game_Input::Press(L"PLAYERDOWN") && !ArrColorCheck[3])
		{
			Present_AnimationType = Player_Map_Animation_Type::Move_R_Down;
			TRANS()->WMOVE(TRANS()->WDOWN() * Game_Time::DeltaTime(m_Speed));
			if (TRANS()->WPOS().Y <= -360.f && TRANS()->WPOS().Y >= -1640.f)
				m_Cam->TRANS()->WMOVE(Game_Vector::DOWN* Game_Time::DeltaTime(m_Speed));
		}
	}
	else if (Game_Input::Press(L"PLAYERUP") && !ArrColorCheck[2])
	{
		Present_AnimationType = Player_Map_Animation_Type::Move_Up;
		TRANS()->WMOVE(Game_Vector::UP * Game_Time::DeltaTime(m_Speed));
		if (TRANS()->WPOS().Y <= -360.f && TRANS()->WPOS().Y >= -1640.f)
			m_Cam->TRANS()->WMOVE(Game_Vector::UP * Game_Time::DeltaTime(m_Speed));
	}
	else if (Game_Input::Press(L"PLAYERDOWN") && !ArrColorCheck[3])
	{
		Present_AnimationType = Player_Map_Animation_Type::Move_Down;
		TRANS()->WMOVE(Game_Vector::DOWN * Game_Time::DeltaTime(m_Speed));
		if (TRANS()->WPOS().Y <= -360.f && TRANS()->WPOS().Y >= -1640.f)
			m_Cam->TRANS()->WMOVE(Game_Vector::DOWN * Game_Time::DeltaTime(m_Speed));
	}
	else
	{
		switch (Past_AnimationType)
		{
		case Player_Map_Animation_Type::Move_Up:
			break;
		case Player_Map_Animation_Type::Move_Down:
			break;
		case Player_Map_Animation_Type::Move_R:
		case Player_Map_Animation_Type::Move_L:
			break;
		case Player_Map_Animation_Type::Move_R_Up:
		case Player_Map_Animation_Type::Move_L_Up:
			break;
		case Player_Map_Animation_Type::Move_L_Down:
		case Player_Map_Animation_Type::Move_R_Down:
			break;		
		default:
			break;
		}
	}
	//애니메이션 변경
	if (Present_AnimationType != Past_AnimationType)
	{
		Past_AnimationType = Present_AnimationType;
		switch (Present_AnimationType)
		{
		case Player_Map_Animation_Type::Idle_Down:
			m_Animation_Type->ChangeAni(L"idle_Down");
			break;
		case Player_Map_Animation_Type::Idle_Up:
			m_Animation_Type->ChangeAni(L"idle_Up");
			break;
		case Player_Map_Animation_Type::Idle_R:
			m_Animation_Type->ChangeAni(L"idle_Side");
			TRANS()->Reverse_R();
			break;
		case Player_Map_Animation_Type::Idle_L:
			m_Animation_Type->ChangeAni(L"idle_Side");
			TRANS()->Reverse_L();
			break;
		case Player_Map_Animation_Type::Idle_R_Down_Side:
			m_Animation_Type->ChangeAni(L"idle_DownSide");
			TRANS()->Reverse_R();
			break;
		case Player_Map_Animation_Type::Idle_L_Down_Side:
			m_Animation_Type->ChangeAni(L"idle_DownSide");
			TRANS()->Reverse_L();
			break;
		case Player_Map_Animation_Type::Idle_R_Up_Side:
			m_Animation_Type->ChangeAni(L"idle_UpSide");
			TRANS()->Reverse_R();
			break;
		case Player_Map_Animation_Type::Idle_L_Up_Side:
			m_Animation_Type->ChangeAni(L"idle_UpSide");
			TRANS()->Reverse_L();
			break;

		case Player_Map_Animation_Type::Move_Up:
			m_Animation_Type->ChangeAni(L"Up");
			break;
		case Player_Map_Animation_Type::Move_Down:
			m_Animation_Type->ChangeAni(L"Down");
			break;
		case Player_Map_Animation_Type::Move_R:
			m_Animation_Type->ChangeAni(L"Side");
			TRANS()->Reverse_R();
			break;
		case Player_Map_Animation_Type::Move_L:
			m_Animation_Type->ChangeAni(L"Side");
			TRANS()->Reverse_L();
			break;
		case Player_Map_Animation_Type::Move_R_Up:
			m_Animation_Type->ChangeAni(L"UpSide");
			TRANS()->Reverse_R();
			break;
		case Player_Map_Animation_Type::Move_L_Up:
			m_Animation_Type->ChangeAni(L"UpSide");
			TRANS()->Reverse_L();
			break;
		case Player_Map_Animation_Type::Move_R_Down:
		case Player_Map_Animation_Type::Move_L_Down:
			m_Animation_Type->ChangeAni(L"DownSide");
			break;
		case Player_Map_Animation_Type::End:
			break;
		default:
			break;
		}
	}

	Game_Vector Pos = TRANS()->WPOS();

	//맵 입장용
	if (Game_Input::Press(L"JUMP"))
	{
		Player_Entrance_Check = true;
	}
	else
	{
		Player_Entrance_Check = false;
	}
}

void Player_Map::Collision()
{
	for (size_t i = 0; i < 4; i++)
	{
		ArrColorCheck[i] = false;
	}

	Game_Ptr<Game_Texture> IMG = Game_Texture::Find(L"WorldMap1_Hit.png");
	for (size_t i = 0; i < 4; i++)
	{
		Game_Vector POS = TRANS()->WPOS();
		POS += ColorCheckDir[i];
		POS.Y *= -1.0F;
		//POS *= LOGIC_Value::PIXELCOLRATIO;
		Game_Vector Color = IMG->GetPixel(POS);

		if (Color == Game_Vector::BLACK)
		{
			ArrColorCheck[i] = true;
		}
	}

}

void Player_Map::Hit(Game_Collision* _This, Game_Collision* _Other)
{

}

void Player_Map::Renderer_Setting()
{
	m_Animation = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_MAP);	
	m_Animation->LSCALE(Player_Map_Size);
	m_Animation_Type = ACTOR()->CreateCom<Game_Animation>(m_Animation);
	
	m_Animation_Type->CreateAni(L"idle_Down", L"World_idle_Down.png", 0, 4, 0.1f, true);
	m_Animation_Type->CreateAni(L"idle_DownSide", L"World_idle_DownSide.png", 0, 4, 0.1f, true);
	m_Animation_Type->CreateAni(L"idle_Side", L"World_idle_Side.png", 0, 4, 0.1f, true);
	m_Animation_Type->CreateAni(L"idle_Up", L"World_idle_Up.png", 0, 4, 0.1f, true);
	m_Animation_Type->CreateAni(L"idle_UpSide", L"World_idle_UpSide.png", 0, 4, 0.1f, true);
	
	m_Animation_Type->CreateAni(L"Down", L"World_Down.png", 0, 15, 0.1f, true);
	m_Animation_Type->CreateAni(L"DownSide", L"World_DownSide.png", 0, 15, 0.1f, true);
	m_Animation_Type->CreateAni(L"Side", L"World_Side.png", 0, 15, 0.1f, true);
	m_Animation_Type->CreateAni(L"Up", L"World_Up.png", 0, 15, 0.1f, true);
	m_Animation_Type->CreateAni(L"UpSide", L"World_UpSide.png", 0, 15, 0.1f, true);
	
	m_Animation_Type->CreateAni(L"Win", L"World_Win.png", 0, 11, 0.1f, true);
	
	m_Animation_Type->ChangeAni(L"idle_Down");
}

void Player_Map::Debug()
{
	if (Game_Input::Down(L"DebugOn"))
	{
		m_DebugCheck = true;
		m_Collision_Debug_Render->On();
		m_Collision_Debug_Render->On();
	}
	else if (Game_Input::Down(L"DebugOff"))
	{
		m_DebugCheck = false;
		m_Collision_Debug_Render->Off();
		m_Collision_Debug_Render->Off();
	}

	if (m_DebugCheck)
	{
		Game_3D_Debug::DrawDebugText(L"Player LPos : %f, %f", TRANS()->LPOS().X, TRANS()->LPOS().Y);
	}
}
