#include "Game_Com.h"
#include "Game_Trans.h"
#include "Game_Actor.h"

Game_Ptr<Game_Scene> Game_Scene_Com::SCENE()
{
	return m_Scene;
}

Game_Ptr<Game_Actor> Game_Actor_Com::ACTOR()
{
	return m_Actor;
}

Game_Ptr<Game_Scene> Game_Actor_Com::SCENE() {
	return m_Actor->SCENE();
}

Game_Ptr<Game_Trans> Game_Actor_Com::TRANS() 
{
	return m_Actor->TRANS();
}

bool Game_Actor_Com::IsUpdate()
{
	// 액터의 bool값도 true고 나의 bool값도 true여야 나는 나의 일을할수 있다. 
	return true == Game_Virtual_Progress::IsUpdate() && true == m_Actor->IsUpdate();
}

bool Game_Actor_Com::IsDeath()
{
	// 액터의 bool값도 true고 나의 bool값도 true여야 나는 나의 일을할수 있다. 
	return true == Game_Virtual_Progress::IsDeath() || true == m_Actor->IsDeath();
}
