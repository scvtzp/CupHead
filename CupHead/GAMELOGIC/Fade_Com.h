#pragma once
#include "Game_Logic.h"

enum class FADESTATE
{
	FADEOUT,
	FADEIN,
	FADEMAX,
};

class Fade_Com : public Game_Actor_Com
{
private:
	FADESTATE m_FADESTATE;
	float m_Alpha;
	Game_Ptr<Game_Sprite_Renderer> m_SRENDER;

public:
	void FadeOutStart() {
		m_FADESTATE = FADESTATE::FADEOUT;
		m_Alpha = 1.0f;
	}

	void Init() override;
	void Update() override;
};

