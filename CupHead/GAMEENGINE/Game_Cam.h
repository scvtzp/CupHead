#pragma once
#include "Game_Com.h"
#include <set>
#include <Game_Debug.h>

enum class Cam_Mode
{
	ORTH, // 직교투영
	PERS, // 원근투영
};

class Game_Renderer;
class Game_Cam : public Game_Actor_Com
{
public:
	friend Game_Trans;
	friend Game_Scene;

private:
	Cam_Mode m_MODE;
	Game_Matrix m_VIEW;
	Game_Matrix m_PROJ;
	Game_Vector m_CAMSIZE;
	float m_CAMNEAR;
	float m_CAMFAR;
	float m_CAMFOV;

	// 내가 바라보는 랜더러들의 그룹을 카메라가 알고 있어야 한다.
	std::set<int> m_VIEWRENDERERS;

public:
	Game_Vector CAMSIZE() { return m_CAMSIZE; }
	void CAMSIZE(const Game_Vector _Value) { m_CAMSIZE = _Value; }

	float CAMFOV() { return m_CAMFOV; }
	float CAMNEAR() { return m_CAMNEAR; }
	float CAMFAR() { return m_CAMFAR; }

	void CAMFOV(float _Value) { m_CAMFAR = _Value; }
	void CAMNEAR(float _Value) { m_CAMNEAR = _Value; }
	void CAMFAR(float _Value) { m_CAMFAR = _Value; }

	//CBTRANSDATA.V.VIEWLH(CamPos, Game_Vector::FOWARD, Game_Vector::UP);
	//CBTRANSDATA.P.ORTHLH(CamScale, 0.1f, 100.0f);

private:
	void StartSetting(int _Order);

public:
	Cam_Mode MODE() { return m_MODE; }
	void MODE(Cam_Mode _MODE) { m_MODE = _MODE; }

public:
	// 그 뒤로 몇개가 들어올지
	// 가변인자이다.
	template<typename ... REST>
	void Init(int _Order, REST ... _Arg) 
	{
		StartSetting(_Order);
		AddRenderGroup(_Arg...);
	}

	template<typename ... REST>
	void AddRenderGroup(int _GroupIndex, REST ... _Arg)
	{
		if (m_VIEWRENDERERS.end() != m_VIEWRENDERERS.find(_GroupIndex))
		{
			AMSG(L"카메라가 같은 그룹을 두번 보려고 했습니다.");
		}

		m_VIEWRENDERERS.insert(_GroupIndex);
		AddRenderGroup(_Arg...);
	}

	void AddRenderGroup(int _GroupIndex)
	{
		if (m_VIEWRENDERERS.end() != m_VIEWRENDERERS.find(_GroupIndex))
		{
			AMSG(L"카메라가 같은 그룹을 두번 보려고 했습니다.");
		}

		m_VIEWRENDERERS.insert(_GroupIndex);
	}

	void AddRenderGroup()	{}

	void RenderPrev() override;
};