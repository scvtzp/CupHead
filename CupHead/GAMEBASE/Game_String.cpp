#include "Game_String.h"

Game_String operator+(const wchar_t* _Left, const Game_String& _Right)
{
	Game_String Str = _Left;
	Str += _Right;
	return Str;
}

bool operator==(const wchar_t* _Left, const Game_String& _Right) {
	return _Right == _Left;
}


bool operator!=(const wchar_t* _Left, const Game_String& _Right) {
	return _Right != _Left;
}