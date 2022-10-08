#pragma once
#include <Game_Logic.h>
class Scene_Monkey : public Game_Scene_Com
{
public:
	Scene_Monkey() {}
	~Scene_Monkey() {}
public:
	Game_Ptr<Game_Actor> PLAYER;
	Game_Ptr<Game_Actor> MONSTER;

public:
	void Init() override;
	void Update() override;
	void SceneChangeStart() override;
	void SceneChangeEnd() override;

	float Stage_Time = 0.f;
};

