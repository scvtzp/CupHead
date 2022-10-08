#pragma once
#include "HGAMEPLAYER.h"

enum class Map_Type
{
	Slime,
	Monkey,
	Ghost,
	Shop,
	End
};

class Map_Entrance : public MOVECOM
{
public:
	void Init() override;
	void Update() override;
	void Renderer_Setting();
	void Hit(Game_Collision* _This, Game_Collision* _Other);
	void Out(Game_Collision* _This, Game_Collision* _Other);

	void Set_Type(Map_Type type);
	Map_Type Get_Type() { return MapType; }
	void Debug();

private:
	Map_Type MapType = Map_Type::End;
	Game_Ptr<Game_Sprite_Renderer> Sign_Renderer;
};

