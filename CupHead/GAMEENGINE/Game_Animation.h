#pragma once
#include "Game_Sprite_Renderer.h"

class Game_Animation : public Game_Actor_Com
{
public:
	Game_Ptr<Game_Sprite_Renderer> m_SpriteRender;

public:
	void Init(Game_Ptr<Game_Sprite_Renderer> _SpriteRender = nullptr);
	void Update() override;

private:
	class Game_Frame_Animation : public Game_Ref
	{
	public:
		Game_String m_AniName = L"ABC";
		Game_String m_SpriteName;
		unsigned int m_Start;
		unsigned int m_End;
		float m_FrameTime;
		bool m_Loop;

		unsigned int m_CurFrame;
		float m_CurFrameTime;

	public:
		void Reset() {
			m_CurFrame = m_Start;
			m_CurFrameTime = m_FrameTime;
		}
	};

private:
	std::map<Game_String, Game_Ptr<Game_Frame_Animation>> m_Ani;
	Game_Ptr<Game_Frame_Animation> m_CurAni;


public:
	void CreateAni(
		const Game_String& _AniName, 
		const Game_String& _SpriteName, 
		unsigned int _Start, 
		unsigned int _End, 
		float _FrameTime,
		bool _Loop);


	void ChangeAni(const Game_String& _AniName);

	//д©╫╨ер
	Game_String Get_Ani();
	int Get_Num();
	int Get_EndNum();
};

