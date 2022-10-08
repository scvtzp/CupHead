#pragma once
#include "Game_String.h"

class Game_Name
{
private:
	Game_String m_Name;

public:
	Game_String NAME() 
	{
		return m_Name;
	}

	void NAME(const Game_String& _Name) 
	{
		m_Name = _Name;
	}

public:
	Game_Name() {}
	virtual ~Game_Name() {}
};

