#pragma once
#include <Game_Logic.h>

class Scene_WorldMap : public Game_Scene_Com
{
public:
	Scene_WorldMap() {}
	~Scene_WorldMap() {}

public:
	void Init() override;
	void Update() override;
	void SceneChangeStart() override;
	void SceneChangeEnd() override;
	void Debug();
private:
	bool ShiftCheck = false;
	bool m_DebugCheck = true;

	Game_Ptr<Game_Actor> Shift;
	Game_Ptr<Game_Actor> Shift_1;
	Game_Ptr<Game_Actor> Shift_2;
	Game_Ptr<Game_Actor> Shift_3;

	Game_Ptr<Game_Sprite_Renderer> Shift_Renderer;
	Game_Ptr<Game_Sprite_Renderer> Shift_Renderer_1;
	Game_Ptr<Game_Sprite_Renderer> Shift_Renderer_2;
	Game_Ptr<Game_Sprite_Renderer> Shift_Renderer_3;


	Game_Ptr<Game_Actor> Background_Up;
	Game_Ptr<Game_Sprite_Renderer> Background_Up_Renderer;

	Game_Ptr<Game_Actor> Noise;
	Game_Ptr<Game_Sprite_Renderer> Noise_Renderer;
	Game_Ptr<Game_Animation> Noise_Ani;
};

