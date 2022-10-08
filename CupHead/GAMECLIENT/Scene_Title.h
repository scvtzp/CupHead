#pragma once
#include <Game_Logic.h>

class Scene_Title : public Game_Scene_Com
{
public:
	Scene_Title() {}
	~Scene_Title() {}

public:
	void Init() override;
	void Update() override;
};

