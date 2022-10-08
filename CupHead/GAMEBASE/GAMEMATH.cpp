#include "GAMEMATH.h"

const float GAMEMATH::PI = 3.14159265358979323846264338f;
const float GAMEMATH::PI2 = 3.14159265358979323846264338f * 2;
const float GAMEMATH::PI2D1 = 3.14159265358979323846264338f * 0.5f;
const float GAMEMATH::DTOR = GAMEMATH::PI / 180.0f;
const float GAMEMATH::RTOD = 180.0f / GAMEMATH::PI;


const Game_Vector Game_Vector::WHITE = { 1.0F, 1.0F, 1.0f, 1.0f };
const Game_Vector Game_Vector::BLUE = { 0.0F, 0.0F, 1.0f, 1.0f };
const Game_Vector Game_Vector::BLACK = { 0.0F, 0.0F, 0.0f, 1.0f };
const Game_Vector Game_Vector::RED = { 1.0F, 0.0F, 0.0f, 1.0f };

const Game_Vector Game_Vector::NONE = { 0.0F, 0.0F, 0.0f, 0.0f };
const Game_Vector Game_Vector::ZERO = { 0.0F, 0.0F, 0.0f, 1.0f };
const Game_Vector Game_Vector::ONE = { 1.0F, 1.0F, 1.0f, 1.0f };


const Game_Vector Game_Vector::RIGHT = { 1.0F, 0.0F };
const Game_Vector Game_Vector::LEFT = { -1.F, 0.0F };
const Game_Vector Game_Vector::UP = { 0.0F, 1.0F };
const Game_Vector Game_Vector::DOWN = { 0.0F, -1.0F };


const Game_Vector Game_Vector::FOWARD = { 0.0F, 0.0F, 1.0f };
const Game_Vector Game_Vector::BACK = { 0.0F, 0.0F, -1.0f };

Game_Vector& Game_Vector::operator*=(const Game_Matrix& _Value)
{
	Game_Vector NewResult;

	DXVEC = DirectX::XMVector4Transform(DXVEC, _Value);
	return *this;
}


Game_Vector Game_Vector::operator*(const Game_Matrix& _Value)
{
	Game_Vector NewResult;

	for (size_t x = 0; x < 4; x++)
	{
		NewResult.Arr2D[0][x] = 0.0f;
		for (size_t CalCount = 0; CalCount < 4; CalCount++)
		{
			NewResult.Arr2D[0][x] += Arr2D[0][CalCount] * _Value.ArrF2D[CalCount][x];
		}
	}

	return NewResult;
}