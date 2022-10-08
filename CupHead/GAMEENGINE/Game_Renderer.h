#pragma once
#include "Game_Trans.h"

class Game_Cam;
class Game_Renderer : public Game_Trans
{
public:
	void Init(int _Order = 0);
	virtual void Render(Game_Ptr<Game_Cam> _Cam);

public:
	Game_Renderer();
	~Game_Renderer();
};
