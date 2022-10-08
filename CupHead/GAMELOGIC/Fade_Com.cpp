#include "Fade_Com.h"

#include "Game_FreeCam.h"
#include <Game_Time.h>


void Fade_Com::Init()
{
	m_SRENDER = ACTOR()->FindComType<Game_Sprite_Renderer>();

	if (nullptr == m_SRENDER)
	{
		AMSG(L"스프라이트 랜더러가 nullptr이므로 페이드 기능을 사용할 수 없습니다.");
	}

	m_FADESTATE = FADESTATE::FADEMAX;
}

void Fade_Com::Update()
{
	switch (m_FADESTATE)
	{
	case FADESTATE::FADEOUT:
	{
		if (0 >= m_Alpha)
		{
			m_FADESTATE = FADESTATE::FADEMAX;
		}

		m_Alpha -= Game_Time::DeltaTime();
		m_SRENDER->Alpha(m_Alpha);
		// 여기만 구현해 놓을께요
		break;
	}
	case FADESTATE::FADEIN:
	{
		// 이건 구현해보세요.
		break;
	}
	case FADESTATE::FADEMAX:
	{
		break;
	}
	default:
		break;
	}
}