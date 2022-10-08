#pragma once
#include <Game_Ptr.h>
#include <GAMEMATH.h>
#include <list>
#include "Game_Virtual_Progress.h"
//#include <Logic_Enum.h>

class Game_Com : public Game_Ref, public Game_Virtual_Progress
{
};


// �з��Ѵ�.
class Game_Scene;
class Game_Scene_Com : public Game_Com
{
	friend Game_Scene;
private:
	Game_Scene* m_Scene;

public:
	Game_Ptr<Game_Scene> SCENE();
	float ClearUi_Time = 0.f;
	float Clear_Time = 0.f;

private:
	// ������ ���ټ��� �ֳ��� ������?
	void Scene(Game_Scene* _Ptr)
	{
		m_Scene = _Ptr;
	}
};

class Game_Trans;
class Game_Actor;
class Game_Actor_Com : public Game_Com
{
private:
	friend Game_Actor;

private:
	Game_Actor* m_Actor;

public:
	Game_Ptr<Game_Actor> ACTOR();
	Game_Ptr<Game_Scene> SCENE();
	Game_Ptr<Game_Trans> TRANS();

	bool IsUpdate() override;
	bool IsDeath() override;


private:
	// ������ ���ټ��� �ֳ��� ������?
	void ACTOR(Game_Actor* _Ptr)
	{
		m_Actor = _Ptr;
	}
};

