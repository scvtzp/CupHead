#include "Game_Actor.h"

Game_Trans* Game_Actor::TRANS()
{
	return m_TRANS;
}

Game_Scene* Game_Actor::SCENE() 
{
	return m_SCENE;
}


void Game_Actor::UpdatePrev() {
	Game_UpDate_ListObj_Mgr<Game_Actor_Com>::UpdatePrev();
}
void Game_Actor::Update() {
	Game_UpDate_ListObj_Mgr<Game_Actor_Com>::Update();
}
void Game_Actor::UpdateNext() {
	Game_UpDate_ListObj_Mgr<Game_Actor_Com>::UpdateNext();
}
void Game_Actor::RenderPrev() {
	Game_UpDate_ListObj_Mgr<Game_Actor_Com>::RenderPrev();
}
void Game_Actor::RenderNext() {
	Game_UpDate_ListObj_Mgr<Game_Actor_Com>::RenderNext();
}
void Game_Actor::CollisionPrev() {
	Game_UpDate_ListObj_Mgr<Game_Actor_Com>::CollisionPrev();
}
void Game_Actor::CollisionNext() {
	Game_UpDate_ListObj_Mgr<Game_Actor_Com>::CollisionNext();
}
void Game_Actor::SceneChangeEnd() {
	Game_UpDate_ListObj_Mgr<Game_Actor_Com>::SceneChangeEnd();
}
void Game_Actor::SceneChangeStart() {
	Game_UpDate_ListObj_Mgr<Game_Actor_Com>::SceneChangeStart();
}
void Game_Actor::Release() {
	Game_UpDate_ListObj_Mgr<Game_Actor_Com>::Release();
}
