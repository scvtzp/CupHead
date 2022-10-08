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
		AMSG(L"프레임 애니메이션은 스프라이트 랜더러가 있어야 만들수 있습니다.");
	}
}

void Game_Animation::Update()
{
	// float Time = Game_Time::DeltaTime();
	if (nullptr == m_CurAni)
	{
		AMSG(L"애니메이션이 지정되지 않았습니다.");
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
		AMSG(_AniName + L"이미 만들어진 애니메이션 이름입니다");
	}

	if (nullptr == Game_Sprite::Find(_SpriteName))
	{
		AMSG(_AniName + L"스프라이트가 존재하지 않아 애니메이션을 만들수 없습니다.");
	}

	// 바로 키와 함께 만들어진다.
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
		AMSG(_AniName + L"애니메이션이 존재하지 않습니다");
	}

	//바꿀려는 상태가 기존과 동일하면 변경 X
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

