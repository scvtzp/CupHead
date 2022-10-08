#pragma once
#include "Game_Com.h"
#include "Game_Header.h"

enum class COLTYPE
{
	SPHERE2D, // API의 서클이다.
	AABB2D,
	OBB2D,
	SPHERE3D,
	AABB3D,
	OBB3D,
	MAX,
};

class Game_Cam;
class Game_Trans : public Game_Actor_Com
{

//충돌처리
private:
	class HGAMECOLSTART
	{
	public:
		HGAMECOLSTART();
	};
	friend HGAMECOLSTART;
	static HGAMECOLSTART Starter;

public:
	static bool __stdcall SPHERE2DCOLOBB2D(Game_Ptr<Game_Trans> _Left, Game_Ptr<Game_Trans> _Right);
	static bool __stdcall SPHERE2DCOLAABB2D(Game_Ptr<Game_Trans> _Left, Game_Ptr<Game_Trans> _Right);
	static bool __stdcall SPHERE2DCOLSPHERE2D(Game_Ptr<Game_Trans> _Left, Game_Ptr<Game_Trans> _Right);
	static bool __stdcall AABB2DCOLOBB2D(Game_Ptr<Game_Trans> _Left, Game_Ptr<Game_Trans> _Right);
	static bool __stdcall AABB2DCOLAABB2D(Game_Ptr<Game_Trans> _Left, Game_Ptr<Game_Trans> _Right);
	static bool __stdcall AABB2DCOLSPHERE2D(Game_Ptr<Game_Trans> _Left, Game_Ptr<Game_Trans> _Right);
	static bool __stdcall OBB2DCOLOBB2D(Game_Ptr<Game_Trans> _Left, Game_Ptr<Game_Trans> _Right);
	static bool __stdcall OBB2DCOLAABB2D(Game_Ptr<Game_Trans> _Left, Game_Ptr<Game_Trans> _Right);
	static bool __stdcall OBB2DCOLSPHERE2D(Game_Ptr<Game_Trans> _Left, Game_Ptr<Game_Trans> _Right);

	static bool(__stdcall* COLFUNC[(int)COLTYPE::MAX][(int)COLTYPE::MAX])(Game_Ptr<Game_Trans> _Left, Game_Ptr<Game_Trans> _Right);

public:



protected:
	Game_Vector m_LPOS;
	Game_Vector m_LSCALE; // 스케일이 어디라도 0이면 터트리겠다. // 크기가 0인건 존재하지 않는다는 말이니까.
	Game_Vector m_LROT;
	Game_Vector m_WPOS;
	Game_Vector m_WSCALE; // 스케일이 어디라도 0이면 터트리겠다. // 크기가 0인건 존재하지 않는다는 말이니까.
	Game_Vector m_WROT;
	Game_Vector m_HitScale; //충돌처리 전용 스케일

	bool CALARRMATCHECK[(int)MATTYPE::MATTYPE_MAX];
	TransData m_TRANSDATA;

	// 트랜스 폼은 부모를 가질수 있다.
	Game_Trans* m_PARENT;
	std::list<Game_Trans*> m_CHILDLIST;

public:
	Game_Trans* PARENT()
	{
		return m_PARENT;
	}
	void PARENT(Game_Ptr<Game_Trans> _Parent);


public:
	Game_Trans();

public:
	Game_Vector LPOS() { return m_LPOS; }
	Game_Vector LSCALE() { return m_LSCALE; }
	Game_Vector LROT() { return m_LROT; }
	Game_Vector WPOS() { return m_WPOS; }
	Game_Vector WPOS2D()
	{
		return Game_Vector(m_WPOS.X, m_WPOS.Y, 0.0f, m_WPOS.W);
	}

	Game_Vector WSCALE() { return m_WSCALE; }
	Game_Vector WROT() { return m_WROT; }
	Game_Vector Hit_Scale() { return m_HitScale; }


private:
	// m_LPOS가 결정나 있어야 계산 가능.
	void CALWPOS()
	{
		DEBUGCHECK;

		Game_Vector Pos = m_LPOS * m_PARENT->WSCALE();
		Game_Vector Rot = WROT();
		Game_Vector Last = Pos.RotDegReturn(Rot);

		m_WPOS = m_PARENT->WPOS() + ((m_LPOS * m_PARENT->WSCALE()).RotDegReturn(m_PARENT->WROT()));
	}

	void CALCHILDPOS() {
		for (auto& _Child : m_CHILDLIST)
		{
			_Child->CALWPOS();
		}
	}

	void CALWROT()
	{
		m_WROT = m_PARENT->WROT() + m_LROT;
	}

	void CALCHILDROT() {
		for (auto& _Child : m_CHILDLIST)
		{
			_Child->CALWROT();
		}
	}

	void CALWSCALE()
	{
		m_WSCALE = m_PARENT->WSCALE() * m_LSCALE;
	}

	void CALCHILDSCALE() {
		for (auto& _Child : m_CHILDLIST)
		{
			_Child->CALWSCALE();
		}
	}

public:
	// 이것들 다 바뀌어야 한다.

	// SetLScale 
	// : 자식의 LScale 세팅해주는 함수
	// -->  변경된 자식의 WScale을 계산해주는 함수 (자식의 LScale는 변화 X)
	void SetLScale(const Game_Vector& _Value)
	{
		if (nullptr == m_PARENT)
		{
			m_LSCALE = m_WSCALE = _Value;
			CALARRMATCHECK[(int)MATTYPE::MATTYPE_SCALE] = true;
			return;
		}
		// 1) Scale 세팅
		m_LSCALE = _Value;
		// WScale 은 곱하기
		m_WSCALE = m_PARENT->WSCALE() * m_LSCALE;

		// [ 중 요 ]
		// Q. 왜 부모 트랜스폼 의 WSCALE이 변하면 자식 트랜스폼의 WORLD에서의 TRANSFORM 정보 중 머가 변할까?
		// A. SCALE, POS 에 2개의 정보에 변화가 생김 --> 내용 0617 수업 내용.pptx

		// 2) Pos 세팅
		m_WPOS = m_PARENT->WPOS() + (m_LPOS * m_PARENT->WSCALE());
		// 자신의 WPOS는 (부모의 WPOS) + ( 자신의 LPOS * 부모의 WSCALE)
		// 예. 부모의 WPOS의 위치가 x축으로 1만큼 이동했고, WSCLE 이 2배 커졌을 경우, 자식의 LPOS ( 3, 2, 0)
		// 부모 : WPOS( 1, 0, 0 ), WSCALE( 2, 2, 2 )
		// ---> 자식 : LPOS( 3, 2, 0), LSCALE( 1, 1, 1 ), 
		//             WPOS(1 + 3 * 2(= 7), 0 + 2 * 2(= 4), 0), WSCALE( 4.5, 4.5, 4.5)

		WPOS(m_WPOS);
	}

	void LPOS(const Game_Vector& _Value)
	{
		if (nullptr == m_PARENT)
		{
			m_LPOS = m_WPOS = _Value;
			CALARRMATCHECK[(int)MATTYPE::MATTYPE_POS] = true;
			CALCHILDPOS();
			return;
		}

		m_LPOS = _Value;
		CALWPOS();
		CALCHILDPOS();
		CALARRMATCHECK[(int)MATTYPE::MATTYPE_POS] = true;
	}
	void LSCALE(const Game_Vector& _Value)
	{
		if (nullptr == m_PARENT)
		{
			m_LSCALE = m_WSCALE = _Value;
			CALCHILDSCALE();
			CALCHILDPOS();
			CALARRMATCHECK[(int)MATTYPE::MATTYPE_SCALE] = true;
			return;
		}

		m_LSCALE = _Value;
		CALWSCALE();
		CALCHILDSCALE();
		CALCHILDPOS();
		CALARRMATCHECK[(int)MATTYPE::MATTYPE_SCALE] = true;

	}
	void LROT(const Game_Vector& _Value)
	{
		if (nullptr == m_PARENT)
		{
			m_LROT = m_WROT = _Value;
			CALARRMATCHECK[(int)MATTYPE::MATTYPE_ROT] = true;
			CALCHILDROT();
			CALCHILDPOS();
			return;
		}

		m_LROT = _Value;
		CALWROT();
		CALCHILDROT();
		CALCHILDPOS();

		CALARRMATCHECK[(int)MATTYPE::MATTYPE_ROT] = true;

	}
	void WPOS(const Game_Vector& _Value)
	{
		if (nullptr == m_PARENT)
		{
			m_LPOS = m_WPOS = _Value;
			CALARRMATCHECK[(int)MATTYPE::MATTYPE_POS] = true;
			CALCHILDPOS();
			return;
		}

		// 나온 결과물이
		// 순서가 맞나?
		// 여러분들이 약간의 오차가 생길수 있다는것을 기억해라.
		m_LPOS = ((_Value - m_PARENT->WPOS()) / m_PARENT->WSCALE()).RotDegReturn(-m_PARENT->WROT());
		CALWPOS();
		CALCHILDPOS();


		CALARRMATCHECK[(int)MATTYPE::MATTYPE_POS] = true;
	}

	void WSCALE(const Game_Vector& _Value)
	{
		if (nullptr == m_PARENT)
		{
			m_LSCALE = m_WSCALE = _Value;
			CALCHILDSCALE();
			CALCHILDPOS();
			CALARRMATCHECK[(int)MATTYPE::MATTYPE_SCALE] = true;
			return;
		}

		// 스케일의 어떤 속성이 0인걸 허용하지 않는다.

		m_LSCALE = _Value / m_PARENT->WPOS();
		CALWSCALE();
		CALCHILDSCALE();
		CALCHILDPOS();

		CALARRMATCHECK[(int)MATTYPE::MATTYPE_SCALE] = true;
	}
	void WROT(const Game_Vector& _Value)
	{
		if (nullptr == m_PARENT)
		{
			m_LROT = m_WROT = _Value;
			CALCHILDROT();
			CALCHILDPOS();
			CALARRMATCHECK[(int)MATTYPE::MATTYPE_ROT] = true;
			return;
		}

		// 이연산을 이해하려면 가상의 수를 연상해서 그걸로 계산을 해봐야 한다.
		//0 0 0  0 0 90 - 0 0 90
		m_LROT = _Value - m_PARENT->WROT();
		CALWROT();
		CALCHILDROT();
		CALCHILDPOS();

		CALARRMATCHECK[(int)MATTYPE::MATTYPE_ROT] = true;
	}

	void Hit_Scale(const Game_Vector& _Value) { m_HitScale = _Value; }

	void LMOVE(const Game_Vector& _Value) { LPOS(LPOS() + _Value); }
	void WMOVE(const Game_Vector& _Value) { WPOS(WPOS() + _Value); }
	
	//내꺼
	void Set_LScaleY(const float Y) { LSCALE({LSCALE().X, Y, LSCALE().Z}); }
	void Set_LScaleX(const float X) { LSCALE({ X,  LSCALE().Y, LSCALE().Z }); }
	void Set_LPosY(const float Y) { LPOS({ LPOS().X, Y, LPOS().Z }); }

	//void Set_Y(const float& _Value = Floor) { LPOS((LPOS() - Game_Vector{ 0.f, LPOS().Y ,0.f }) + Game_Vector{0.f, _Value + m_HitScale.Y / 2 ,0.f});}
	void Reverse() { LSCALE(LSCALE() * Game_Vector{ -1.f, 1.f ,1.f }); }
	void Reverse_L() 
	{ 
		if (WSCALE().X > 0)
			Reverse();
	}
	void Reverse_R()
	{
		if (WSCALE().X < 0)
			Reverse();
	}

	void LROTXADDX(float _Value) { m_LROT.X += _Value;		LROT(m_LROT); }
	void LROTXADDY(float _Value) { m_LROT.Y += _Value;		LROT(m_LROT); }
	void LROTXADDZ(float _Value) { m_LROT.Z += _Value;		LROT(m_LROT); }
	void WROTXADDX(float _Value) {
		m_WROT.X += _Value;		WROT(m_WROT);
	}
	void WROTXADDY(float _Value) {
		m_WROT.Y += _Value;		WROT(m_WROT);
	}
	void WROTXADDZ(float _Value) { m_WROT.Z += _Value;		WROT(m_WROT); }


	// 그리고 방향이죠? 길이가 얼마죠?
	Game_Vector LFOWARD() { return m_TRANSDATA.ROT.ArrV[2]; }
	Game_Vector LBACK() { return -m_TRANSDATA.ROT.ArrV[2]; }
	Game_Vector LRIGHT() { return m_TRANSDATA.ROT.ArrV[0]; }
	Game_Vector LLEFT() { return -m_TRANSDATA.ROT.ArrV[0]; }
	Game_Vector LUP() { return m_TRANSDATA.ROT.ArrV[1]; }
	Game_Vector LDOWN() { return m_TRANSDATA.ROT.ArrV[1]; }

	Game_Vector WFOWARD() { return m_TRANSDATA.WWORLD.ArrV[2].NORMAL3DTORETURN(); }
	Game_Vector WBACK() { return -m_TRANSDATA.WWORLD.ArrV[2].NORMAL3DTORETURN(); }
	Game_Vector WRIGHT() { return m_TRANSDATA.WWORLD.ArrV[0].NORMAL3DTORETURN(); }
	Game_Vector WLEFT() { return -m_TRANSDATA.WWORLD.ArrV[0].NORMAL3DTORETURN(); }
	Game_Vector WUP() { return m_TRANSDATA.WWORLD.ArrV[1].NORMAL3DTORETURN(); }
	Game_Vector WDOWN() { return -m_TRANSDATA.WWORLD.ArrV[1].NORMAL3DTORETURN(); }

public:
	//      내가 _ThisType고    상대 트랜스폼        상대는 _OtherType고
	bool Col(COLTYPE _ThisType, Game_Ptr<Game_Trans> _Trans, COLTYPE _OtherType);

public:
	DirectX::BoundingBox AABB2D();
	DirectX::BoundingSphere SPHERE2D();
	DirectX::BoundingOrientedBox OBB2D();

public:
	void Init() override;
	void TransUpdate();
	void CamUpdate(Game_Ptr<Game_Cam> _Cam);
	//void OriginScale(const Game_String& _TexName);
};