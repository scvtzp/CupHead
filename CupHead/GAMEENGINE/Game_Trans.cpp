#include "Game_Trans.h"
#include "Game_Actor.h"
#include "Game_Scene.h"
#include "Game_Cam.h"

#include <Game_Debug.h>
#include "Game_Sprite.h"

bool (*Game_Trans::COLFUNC[(int)COLTYPE::MAX][(int)COLTYPE::MAX])(Game_Ptr<Game_Trans> _Left, Game_Ptr<Game_Trans> _Right) = { nullptr, };

Game_Trans::HGAMECOLSTART Game_Trans::Starter;

Game_Trans::HGAMECOLSTART::HGAMECOLSTART()
{
	Game_Trans::COLFUNC[(int)COLTYPE::AABB2D][(int)COLTYPE::AABB2D] = AABB2DCOLAABB2D;
	Game_Trans::COLFUNC[(int)COLTYPE::AABB2D][(int)COLTYPE::SPHERE2D] = AABB2DCOLSPHERE2D;
	Game_Trans::COLFUNC[(int)COLTYPE::AABB2D][(int)COLTYPE::OBB2D] = AABB2DCOLOBB2D;

	Game_Trans::COLFUNC[(int)COLTYPE::OBB2D][(int)COLTYPE::AABB2D] = OBB2DCOLAABB2D;
	Game_Trans::COLFUNC[(int)COLTYPE::OBB2D][(int)COLTYPE::SPHERE2D] = OBB2DCOLSPHERE2D;
	Game_Trans::COLFUNC[(int)COLTYPE::OBB2D][(int)COLTYPE::OBB2D] = OBB2DCOLOBB2D;

	Game_Trans::COLFUNC[(int)COLTYPE::SPHERE2D][(int)COLTYPE::AABB2D] = SPHERE2DCOLAABB2D;
	Game_Trans::COLFUNC[(int)COLTYPE::SPHERE2D][(int)COLTYPE::SPHERE2D] = SPHERE2DCOLSPHERE2D;
	Game_Trans::COLFUNC[(int)COLTYPE::SPHERE2D][(int)COLTYPE::OBB2D] = SPHERE2DCOLOBB2D;
}

Game_Trans::Game_Trans() 
	: m_LPOS(Game_Vector::ZERO), m_LSCALE(Game_Vector::ONE), m_LROT(Game_Vector::ZERO)
	, m_WPOS(Game_Vector::ZERO), m_WSCALE(Game_Vector::ONE), m_WROT(Game_Vector::ZERO)
	, m_TRANSDATA(), CALARRMATCHECK{true,}
{
	memset(&CALARRMATCHECK, 1, sizeof(CALARRMATCHECK));
	m_TRANSDATA.CALIDEN();
}

// 누군가 내 부모가 되어준다는거죠?
void Game_Trans::PARENT(Game_Ptr<Game_Trans> _Parent) 
{
	// 나한테 부모가 생기면 나는 트랜스폼리스트에서 빠지는거다.
	ACTOR()->SCENE()->m_AllTrans.remove(this);
	// 부모님의????? 부모의 자식으로 들어간다.
	_Parent->m_CHILDLIST.push_back(this);

	// 부모님이 생겼으니 한번은 재계산 하자.
	CALARRMATCHECK[(int)MATTYPE::MATTYPE_PARENT] = true;
	// 나의 월드행렬을 자식의 부모행렬에 입력해주고
	m_TRANSDATA.PARENT = _Parent->m_TRANSDATA.WWORLD;
	// 주명학생이 알아냄 완전 실수함.
	// 내가 나의 부모?
	m_PARENT = _Parent;
	// m_PARENT = this;
}

void Game_Trans::Init()
{
	if (nullptr != ACTOR()->m_TRANS)
	{
		AMSG(L"한 액터에 트랜스폼이 2개가 들어갈수는 없습니다.");
	}

	ACTOR()->m_TRANS = this;
	ACTOR()->SCENE()->m_AllTrans.push_back(this);
}

void Game_Trans::TransUpdate()
{
	// 크기가 변경...
	if (true == CALARRMATCHECK[(int)MATTYPE::MATTYPE_SCALE])
	{
		m_TRANSDATA.SCALE.Scale3D(m_LSCALE);
		CALARRMATCHECK[(int)MATTYPE::MATTYPE_LWORLD] = true;
	}

	// 회전이 변경...
	if (true == CALARRMATCHECK[(int)MATTYPE::MATTYPE_ROT])
	{
		m_TRANSDATA.ROT.RotDeg(m_LROT);
		CALARRMATCHECK[(int)MATTYPE::MATTYPE_LWORLD] = true;
	}

	if (true == CALARRMATCHECK[(int)MATTYPE::MATTYPE_POS])
	{
		m_TRANSDATA.POS.Pos3D(m_LPOS);
		CALARRMATCHECK[(int)MATTYPE::MATTYPE_LWORLD] = true;
	}

	if (true == CALARRMATCHECK[(int)MATTYPE::MATTYPE_LWORLD] || 
		true == CALARRMATCHECK[(int)MATTYPE::MATTYPE_PARENT])
	{
		// CALWORLD를 한다고 해서 Wpos가 변경되나요?
		m_TRANSDATA.CALLWORLD();
		m_TRANSDATA.CALWWORLD();
		// 나의 WWORLD가 변경되었다는 의미로 이걸 TRUE로 변경
		CALARRMATCHECK[(int)MATTYPE::MATTYPE_WWORLD] = true;
	}

	// 부모님이 바뀌었다는 이야기죠?
	if (nullptr != m_PARENT && true == CALARRMATCHECK[(int)MATTYPE::MATTYPE_PARENT])
	{
		LPOS(LPOS());
		LSCALE(LSCALE());
		LROT(LROT());
	}

	// 자식들
	for (auto& _Child : m_CHILDLIST)
	{
		// 이번프레임이 행렬의 변경이 있었다는 이야기죠?
		if (true == CALARRMATCHECK[(int)MATTYPE::MATTYPE_WWORLD])
		{
			_Child->CALARRMATCHECK[(int)MATTYPE::MATTYPE_PARENT] = true;
			// 나의 월드행렬을 자식의 부모행렬에 입력해주고
			// 자식에게 월드행렬을 입력해주고
			_Child->m_TRANSDATA.PARENT = m_TRANSDATA.WWORLD;
		}

		// 내걸 곱해서 자신만의 월드행렬을 만든다.

		_Child->TransUpdate();
	}
	memset(&CALARRMATCHECK, 0, sizeof(CALARRMATCHECK));
}

void Game_Trans::CamUpdate(Game_Ptr<Game_Cam> _Cam) 
{
	m_TRANSDATA.PROJ = _Cam->m_PROJ;
	m_TRANSDATA.VIEW = _Cam->m_VIEW;
	m_TRANSDATA.CALWVP();
}

bool Game_Trans::Col(COLTYPE _ThisType, Game_Ptr<Game_Trans> _Trans, COLTYPE _OtherType)
{
	if (nullptr == COLFUNC[(int)(_ThisType)][(int)(_OtherType)])
	{
		AMSG(L"충돌 함수가 구현되지 않은 충돌을 하려고 했습니다.");
	}

	return COLFUNC[(int)(_ThisType)][(int)(_OtherType)](this, _Trans);

	//// AABB 회전하지 않은 박스
	//DirectX::BoundingOrientedBox ColThis;
	//DirectX::BoundingBox ColOther;

	//// XMFLOAT3   Game_Vector
	//ColThis.Center = WPOS();
	//// 여기다가 그냥 각도를 넣어주면 회전을 인식하지 못한다.
	//// X Y Z W가 꽉꽉차있는 사원수를 넣어줘야
	//// 그걸로 한방에 회전행렬을 만들어서 회전이 인식된다.
	//// WROT(); 오일러각을 넣어줘봐야 아무런 의미도 없다.
	//// Orientation 사원수를 넣어달라고 하는거니까.
	//// 사원수를 쓴다는 것은 
	//// 우리는 이해하기 어려운 복소수 허수 등등이 조합된 행렬을 사용하고 있는 것.
	//// 나는 속력 필요없다.
	//// 직관적인게 좋다. 
	//// 회전에서 디그리가 한번 라디안으로 바뀌고 그걸 다시 쿼터니온을 해서 회전하는데.
	//// WROT() 자체는 그냥 디그리일 뿐이다.
	//ColThis.Orientation = WROT().DegToQ();
	//// DirectX::XMQuaternionRotationRollPitchYaw()
	//ColThis.Extents = WSCALE().HalfVector();
	//ColOther.Center = _Trans->WPOS();
	//ColOther.Extents = _Trans->WSCALE().HalfVector();
	//if ((int)_ThisType < (int)COLTYPE::SPHERE3D)
	//{
	//	ColThis.Center.z = 0.0f;
	//}
	//if ((int)_OtherType < (int)COLTYPE::SPHERE3D)
	//{
	//	ColOther.Center.z = 0.0f;
	//}
	// 요 함수가 알아서 해줍니다.
	// return ColThis.Intersects(ColOther);

}

DirectX::BoundingBox Game_Trans::AABB2D()
{
	return DirectX::BoundingBox(WPOS2D(), WSCALE().HalfVector().AbsVector());
}
DirectX::BoundingSphere Game_Trans::SPHERE2D()
{
	return DirectX::BoundingSphere(WPOS2D(), WSCALE().HalfVector().X);
}
DirectX::BoundingOrientedBox Game_Trans::OBB2D()
{
	Game_Vector Pos = WPOS2D();
	Game_Vector Scale = WSCALE();
	Game_Vector Rot = WROT();

	return DirectX::BoundingOrientedBox(WPOS2D(), WSCALE().HalfVector(), WROT().DegToQ());
}

//// 텍스처 본래의 크기로 세팅해주는 함수
//void Game_Trans::OriginScale(const Game_String& _TexName)
//{
//	Game_Ptr<Game_Sprite> _Sprite = Game_Sprite::Find(_TexName);
//
//	if (nullptr == _Sprite)
//	{
//		AMSG(_TexName + L"의 텍스처를 찾지 못했습니다.");
//	}
//
//	SetLScale(_Sprite->Tex()->Size());
//}