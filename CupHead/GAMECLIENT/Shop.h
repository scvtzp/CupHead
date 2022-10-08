#pragma once
#include <Game_Logic.h>

class Shop : public Game_Scene_Com
{
public:
	void Init() override;
	void Update() override;
	void SceneChangeStart() override;
	void SceneChangeEnd() override;

private:
	Game_Ptr<Game_Sprite_Renderer> Coffee_Renderer;
	Game_Ptr<Game_Sprite_Renderer> Hp1_Renderer;
	Game_Ptr<Game_Sprite_Renderer> Hp2_Renderer;
	Game_Ptr<Game_Sprite_Renderer> Cube_Renderer;
	Game_Ptr<Game_Sprite_Renderer> Bounce_Renderer;
	Game_Ptr<Game_Sprite_Renderer> Charge_Renderer;
	Game_Ptr<Game_Sprite_Renderer> Money;

	Game_Ptr<Game_Actor> Pig_Actor;
	Game_Ptr<Game_Sprite_Renderer> Pig;
	Game_Ptr<Game_Animation> Pig_Ani;

	Game_Ptr<Game_Actor> Coin_Actor;
	Game_Ptr<Game_Sprite_Renderer> Coin_Renderer;
	Game_Ptr<Game_Animation> Coin_Ani;


	static bool SoldCheck[7];
	int TargetNum = 0;
	bool EndTriger = false;
};

