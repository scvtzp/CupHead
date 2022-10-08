#pragma once
#include "HGAMEPLAYER.h"
#include <vector>
using namespace std;

enum class Inventory_Bullet_Type
{
	Peashot,
	Spread,
	Bouncer,
	Charge,
	End
};

enum class Inventory_Ability_Type
{
	Coffee,
	Hp1,
	Hp2,
	Cube,
	End
};

class Inventory : public MOVECOM
{
public:
	void Init() override;
	void Update() override;

public:
	static Inventory_Bullet_Type Inven_1;
	static Inventory_Bullet_Type Inven_2;
	static Inventory_Bullet_Type Now_Type;
	static Inventory_Ability_Type Inven_3;

	static bool Peashot_Check;
	static bool Spread_Check;
	static bool Bouncer_Check;
	static bool Charge_Check;

	//map에서 종류 교환.
	static vector<Inventory_Bullet_Type> Inventory_Vector;
	static void Set_Inventory_Type(Inventory_Bullet_Type Type) { Inventory_Vector.emplace_back(Type); }

	static vector<Inventory_Ability_Type> Inventory3_Vector;
	static void Set_Inventory_Ab_Type(Inventory_Ability_Type Type) { Inventory3_Vector.emplace_back(Type); }

	static void Change_Inventory_Type();


	//stage 에서 사용
	static void Inven_Change();
	static Inventory_Bullet_Type Get_Inven();
	static Inventory_Ability_Type Get_Ability_Inven();
	static void Set_Inven(int Num, Inventory_Bullet_Type Type);
	static float Get_CoolTime() { if (Now_Type == Inventory_Bullet_Type::Spread) { return 0.15f; } return 0.25f; }
};

