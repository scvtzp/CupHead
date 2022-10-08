#pragma once
#include "Game_Logic.h"

// 자유로운 캠이동을 담당한다.
// 캠에 넣어주면 캠을 움직이게 하는 녀석입니다.
// 캠은 어디에 들어가 있죠? 기능
class Game_FreeCam : public Game_Actor_Com
{
private:
	Game_Ptr<Game_Cam> m_Cam;
	float m_Speed;
	float m_RotSpeed;

public:
	void Init() override;
	void Update() override;

};

