#include "Inventory.h"
Inventory_Bullet_Type Inventory::Inven_1 = Inventory_Bullet_Type::End;
Inventory_Bullet_Type Inventory::Inven_2 = Inventory_Bullet_Type::End;
Inventory_Bullet_Type Inventory::Now_Type = Inventory_Bullet_Type::End;
Inventory_Ability_Type Inventory::Inven_3 = Inventory_Ability_Type::End;
vector<Inventory_Bullet_Type> Inventory::Inventory_Vector;
vector<Inventory_Ability_Type> Inventory::Inventory3_Vector;
bool Inventory::Peashot_Check = false;
bool Inventory::Spread_Check = false;
bool Inventory::Bouncer_Check = false;
bool Inventory::Charge_Check = false;

void Inventory::Init()
{
}

void Inventory::Update()
{
}

void Inventory::Change_Inventory_Type()
{
	if (Game_Input::Down(L"1"))
	{
		switch (Inven_1)
		{
		case Inventory_Bullet_Type::Peashot:
			Set_Inven(1, Inventory_Bullet_Type::Spread);
			break;
		case Inventory_Bullet_Type::Spread:
			if(Bouncer_Check)
				Set_Inven(1, Inventory_Bullet_Type::Bouncer);
			else if(Charge_Check)
				Set_Inven(1, Inventory_Bullet_Type::Charge);
			else
				Set_Inven(1, Inventory_Bullet_Type::Peashot);
			break;
		case Inventory_Bullet_Type::Bouncer:
			if (Charge_Check)
				Set_Inven(1, Inventory_Bullet_Type::Charge);
			else
				Set_Inven(1, Inventory_Bullet_Type::Peashot);
			break;
		case Inventory_Bullet_Type::Charge:
			Set_Inven(1, Inventory_Bullet_Type::Peashot);
			break;
		case Inventory_Bullet_Type::End:
			break;
		default:
			break;
		}
	}
	else if (Game_Input::Down(L"2"))
	{
		switch (Inven_2)
		{
		case Inventory_Bullet_Type::Peashot:
			Set_Inven(2, Inventory_Bullet_Type::Spread);
			break;
		case Inventory_Bullet_Type::Spread:
			if (Bouncer_Check)
				Set_Inven(2, Inventory_Bullet_Type::Bouncer);
			else if (Charge_Check)
				Set_Inven(2, Inventory_Bullet_Type::Charge);
			else
				Set_Inven(2, Inventory_Bullet_Type::Peashot);
			break;
		case Inventory_Bullet_Type::Bouncer:
			if (Charge_Check)
				Set_Inven(2, Inventory_Bullet_Type::Charge);
			else
				Set_Inven(2, Inventory_Bullet_Type::Peashot);
			break;
		case Inventory_Bullet_Type::Charge:
			Set_Inven(2, Inventory_Bullet_Type::Peashot);
			break;
		case Inventory_Bullet_Type::End:
			break;
		default:
			break;
		}
	}
	else if (Game_Input::Down(L"3"))
	{

	}
}

void Inventory::Inven_Change()
{
	if (Now_Type == Inven_1)
		Now_Type = Inven_2;
	else
		Now_Type = Inven_1;
}

Inventory_Bullet_Type Inventory::Get_Inven()
{
	return Now_Type;
}

Inventory_Ability_Type Inventory::Get_Ability_Inven()
{
	return Inven_3;
}

void Inventory::Set_Inven(int Num, Inventory_Bullet_Type Type)
{
	switch (Num)
	{
	case 1:
		Inven_1 = Type;
		break;
	case 2:
		Inven_2 = Type;
		break;
	default:
		break;
	}
}
