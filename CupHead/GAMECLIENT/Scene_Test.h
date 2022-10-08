#pragma once
#include <Game_Logic.h>
class Fade_Com;
class Scene_Test : public Game_Scene_Com
{
public:
	Scene_Test();
	~Scene_Test();
public:
	Game_Ptr<Game_Actor> PLAYER;
	Game_Ptr<Game_Actor> MONSTER;
	Game_Ptr<Fade_Com>	m_FADE;
	Game_Ptr<Game_Sprite_Renderer> PLAYERRENDER;
public:
	void Init() override;
	void Update() override;
};

