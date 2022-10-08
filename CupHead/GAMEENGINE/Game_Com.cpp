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
	// ������ bool���� true�� ���� bool���� true���� ���� ���� �����Ҽ� �ִ�. 
	return true == Game_Virtual_Progress::IsUpdate() && true == m_Actor->IsUpdate();
}

bool Game_Actor_Com::IsDeath()
{
	// ������ bool���� true�� ���� bool���� true���� ���� ���� �����Ҽ� �ִ�. 
	return true == Game_Virtual_Progress::IsDeath() || true == m_Actor->IsDeath();
}
