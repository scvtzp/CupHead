#pragma once
#include "Game_Logic.h"

// �����ο� ķ�̵��� ����Ѵ�.
// ķ�� �־��ָ� ķ�� �����̰� �ϴ� �༮�Դϴ�.
// ķ�� ��� �� ����? ���
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

