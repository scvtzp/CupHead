#include "Game_Debug.h"

Game_Debug::Game_DebugSTART Game_Debug::Starter = Game_Debug::Game_DebugSTART();
bool Game_Debug::IsLeckCheck = false;

FILE* Game_Debug::stream;
int Game_Debug::m_FrameCount = 0;

bool Game_Debug::bCon = false;