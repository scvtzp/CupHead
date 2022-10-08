#pragma once
#include <Game_Logic.h>
class Scene_Ghost : public Game_Scene_Com
{
public:
	Scene_Ghost() {}
	~Scene_Ghost() {}
public:
	Game_Ptr<Game_Actor> PLAYER;
	Game_Ptr<Game_Actor> MONSTER;

public:
	void Init() override;
	void Update() override;
	void SceneChangeStart() override;
	void SceneChangeEnd() override;

	float Spawn_Time = 0.f;
	float Stage_Time = 0.f;


	//¼º¹è²¨
	bool EndTrigger = false;
	Game_Ptr<Game_Sprite_Renderer> m_Collision_Debug_Render;
	void Hit(Game_Collision* _This, Game_Collision* _Other)
	{
		EndTrigger = true;
	}
};

