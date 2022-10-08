#pragma once
#include "Game_Com.h"
#include <set>
#include <Game_Debug.h>

enum class Cam_Mode
{
	ORTH, // ��������
	PERS, // ��������
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

	// ���� �ٶ󺸴� ���������� �׷��� ī�޶� �˰� �־�� �Ѵ�.
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
	// �� �ڷ� ��� ������
	// ���������̴�.
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
			AMSG(L"ī�޶� ���� �׷��� �ι� ������ �߽��ϴ�.");
		}

		m_VIEWRENDERERS.insert(_GroupIndex);
		AddRenderGroup(_Arg...);
	}

	void AddRenderGroup(int _GroupIndex)
	{
		if (m_VIEWRENDERERS.end() != m_VIEWRENDERERS.find(_GroupIndex))
		{
			AMSG(L"ī�޶� ���� �׷��� �ι� ������ �߽��ϴ�.");
		}

		m_VIEWRENDERERS.insert(_GroupIndex);
	}

	void AddRenderGroup()	{}

	void RenderPrev() override;
};