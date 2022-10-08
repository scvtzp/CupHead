#pragma once
#include "Game_Com.h"
#include "Game_Header.h"

enum class COLTYPE
{
	SPHERE2D, // API�� ��Ŭ�̴�.
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

//�浹ó��
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
	Game_Vector m_LSCALE; // �������� ���� 0�̸� ��Ʈ���ڴ�. // ũ�Ⱑ 0�ΰ� �������� �ʴ´ٴ� ���̴ϱ�.
	Game_Vector m_LROT;
	Game_Vector m_WPOS;
	Game_Vector m_WSCALE; // �������� ���� 0�̸� ��Ʈ���ڴ�. // ũ�Ⱑ 0�ΰ� �������� �ʴ´ٴ� ���̴ϱ�.
	Game_Vector m_WROT;
	Game_Vector m_HitScale; //�浹ó�� ���� ������

	bool CALARRMATCHECK[(int)MATTYPE::MATTYPE_MAX];
	TransData m_TRANSDATA;

	// Ʈ���� ���� �θ� ������ �ִ�.
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
	// m_LPOS�� ������ �־�� ��� ����.
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
	// �̰͵� �� �ٲ��� �Ѵ�.

	// SetLScale 
	// : �ڽ��� LScale �������ִ� �Լ�
	// -->  ����� �ڽ��� WScale�� ������ִ� �Լ� (�ڽ��� LScale�� ��ȭ X)
	void SetLScale(const Game_Vector& _Value)
	{
		if (nullptr == m_PARENT)
		{
			m_LSCALE = m_WSCALE = _Value;
			CALARRMATCHECK[(int)MATTYPE::MATTYPE_SCALE] = true;
			return;
		}
		// 1) Scale ����
		m_LSCALE = _Value;
		// WScale �� ���ϱ�
		m_WSCALE = m_PARENT->WSCALE() * m_LSCALE;

		// [ �� �� ]
		// Q. �� �θ� Ʈ������ �� WSCALE�� ���ϸ� �ڽ� Ʈ�������� WORLD������ TRANSFORM ���� �� �Ӱ� ���ұ�?
		// A. SCALE, POS �� 2���� ������ ��ȭ�� ���� --> ���� 0617 ���� ����.pptx

		// 2) Pos ����
		m_WPOS = m_PARENT->WPOS() + (m_LPOS * m_PARENT->WSCALE());
		// �ڽ��� WPOS�� (�θ��� WPOS) + ( �ڽ��� LPOS * �θ��� WSCALE)
		// ��. �θ��� WPOS�� ��ġ�� x������ 1��ŭ �̵��߰�, WSCLE �� 2�� Ŀ���� ���, �ڽ��� LPOS ( 3, 2, 0)
		// �θ� : WPOS( 1, 0, 0 ), WSCALE( 2, 2, 2 )
		// ---> �ڽ� : LPOS( 3, 2, 0), LSCALE( 1, 1, 1 ), 
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

		// ���� �������
		// ������ �³�?
		// �����е��� �ణ�� ������ ����� �ִٴ°��� ����ض�.
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

		// �������� � �Ӽ��� 0�ΰ� ������� �ʴ´�.

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

		// �̿����� �����Ϸ��� ������ ���� �����ؼ� �װɷ� ����� �غ��� �Ѵ�.
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
	
	//����
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


	// �׸��� ��������? ���̰� ����?
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
	//      ���� _ThisType��    ��� Ʈ������        ���� _OtherType��
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