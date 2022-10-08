#include "Fade_Com.h"

#include "Game_FreeCam.h"
#include <Game_Time.h>


void Fade_Com::Init()
{
	m_SRENDER = ACTOR()->FindComType<Game_Sprite_Renderer>();

	if (nullptr == m_SRENDER)
	{
		AMSG(L"��������Ʈ �������� nullptr�̹Ƿ� ���̵� ����� ����� �� �����ϴ�.");
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
		// ���⸸ ������ ��������
		break;
	}
	case FADESTATE::FADEIN:
	{
		// �̰� �����غ�����.
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