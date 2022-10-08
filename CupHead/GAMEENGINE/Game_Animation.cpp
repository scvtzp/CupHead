#include "Game_Animation.h"
#include "Game_Actor.h"
#include <Game_Time.h>

void Game_Animation::Init(Game_Ptr<Game_Sprite_Renderer> _SpriteRender)
{
	if (nullptr == _SpriteRender)
	{
		m_SpriteRender = ACTOR()->FindComType<Game_Sprite_Renderer>();
	}
	else {
		m_SpriteRender = _SpriteRender;
	}

	if (nullptr == m_SpriteRender)
	{
		AMSG(L"������ �ִϸ��̼��� ��������Ʈ �������� �־�� ����� �ֽ��ϴ�.");
	}
}

void Game_Animation::Update()
{
	// float Time = Game_Time::DeltaTime();
	if (nullptr == m_CurAni)
	{
		AMSG(L"�ִϸ��̼��� �������� �ʾҽ��ϴ�.");
	}


	m_CurAni->m_CurFrameTime -= Game_Time::DeltaTime();

	if (0 >= m_CurAni->m_CurFrameTime)
	{
		++m_CurAni->m_CurFrame;
		m_CurAni->m_CurFrameTime = m_CurAni->m_FrameTime;

		unsigned int Frame = m_CurAni->m_CurFrame;

		if (m_CurAni->m_End < m_CurAni->m_CurFrame)
		{
			if (true == m_CurAni->m_Loop)
			{
				m_CurAni->m_CurFrame = m_CurAni->m_Start;
			}
			else {
				--m_CurAni->m_CurFrame;
			}
		}
	}

	unsigned int Check = m_CurAni->m_CurFrame;

	m_SpriteRender->SPRITE(m_CurAni->m_SpriteName, m_CurAni->m_CurFrame);


}

void Game_Animation::CreateAni(
	const Game_String& _AniName,
	const Game_String& _SpriteName,
	unsigned int _Start,
	unsigned int _End,
	float _FrameTime,
	bool _Loop)
{
	if (m_Ani.end() != m_Ani.find(_AniName))
	{
		AMSG(_AniName + L"�̹� ������� �ִϸ��̼� �̸��Դϴ�");
	}

	if (nullptr == Game_Sprite::Find(_SpriteName))
	{
		AMSG(_AniName + L"��������Ʈ�� �������� �ʾ� �ִϸ��̼��� ����� �����ϴ�.");
	}

	// �ٷ� Ű�� �Բ� ���������.
	m_Ani[_AniName] = new Game_Frame_Animation();
	m_Ani[_AniName]->m_AniName = _AniName;
	m_Ani[_AniName]->m_SpriteName = _SpriteName;
	m_Ani[_AniName]->m_Start = _Start;
	m_Ani[_AniName]->m_End = _End;
	m_Ani[_AniName]->m_FrameTime = _FrameTime;
	m_Ani[_AniName]->m_Loop = _Loop;
}

void Game_Animation::ChangeAni(const Game_String& _AniName)
{
	if (m_Ani.end() == m_Ani.find(_AniName))
	{
		AMSG(_AniName + L"�ִϸ��̼��� �������� �ʽ��ϴ�");
	}

	//�ٲܷ��� ���°� ������ �����ϸ� ���� X
	if (m_CurAni != NULL && m_CurAni->m_AniName == _AniName)
	{
		return;
	}
		m_CurAni = m_Ani[_AniName];
		m_CurAni->Reset();
}

Game_String Game_Animation::Get_Ani() { return m_CurAni->m_AniName; }
int Game_Animation::Get_Num() { return m_CurAni->m_CurFrame; }
int Game_Animation::Get_EndNum(){ return m_CurAni->m_End; }

