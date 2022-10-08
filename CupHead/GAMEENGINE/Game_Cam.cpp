#include "Game_Cam.h"
#include <Game_Debug.h>
#include <GAMEWINDOW.h>
#include "Game_Actor.h"
#include "Game_Trans.h"
#include "Game_Scene.h"


void Game_Cam::StartSetting(int _Order)
{
	// 트랜스폼이 없는 엑터에 들어간것.
	if (nullptr == ACTOR()->TRANS())
	{
		AMSG(L"트랜스폼이 없는 액터에 캠을 넣었습니다.");
	}
	Order(_Order);

	m_CAMFOV = 60.0F;
	m_CAMFAR = 1000.0F;
	m_CAMNEAR = 0.1F;
	m_MODE = Cam_Mode::ORTH;
	m_CAMSIZE = GAMEWINDOW::MAINOBJ()->Size();

	ACTOR()->SCENE()->PushCam(this);
}

void Game_Cam::RenderPrev() 
{
	//CBTRANSDATA.V.VIEWLH(CamPos, Game_Vector::FOWARD, Game_Vector::UP);
	//CBTRANSDATA.P.ORTHLH(CamScale, 0.1f, 100.0f);
	// 카메라를 들고 있는 액터의 위치가 곧 카메라가 보고있는 위치이다

	// 이녀석이 뷰행렬을 만드는 방법.
	// 회전행렬에서 얻어올수 있다.

	Game_Vector Pos = ACTOR()->TRANS()->WPOS();
	Game_Vector F = ACTOR()->TRANS()->WFOWARD();
	Game_Vector U = ACTOR()->TRANS()->WUP();

	m_VIEW.VIEWLH(ACTOR()->TRANS()->WPOS(), ACTOR()->TRANS()->WFOWARD(), ACTOR()->TRANS()->WUP());

	switch (m_MODE)
	{
	case Cam_Mode::ORTH:
		m_PROJ.ORTHLH(m_CAMSIZE, m_CAMNEAR, m_CAMFAR);
		break;
	case Cam_Mode::PERS:
		m_PROJ.PERSDEG(m_CAMFOV, m_CAMSIZE, m_CAMNEAR, m_CAMFAR);
		break;
	default:
		AMSG(L"캠 모드 설정이 잘못됐습니다.");
		break;
	}

}
