#include "Game_Trans.h"
#include "Game_Actor.h"
#include "Game_Scene.h"

#include <Game_Debug.h>

bool Game_Trans::SPHERE2DCOLOBB2D(Game_Ptr<Game_Trans> _Left, Game_Ptr<Game_Trans> _Right)
{
	return _Left->SPHERE2D().Intersects(_Right->OBB2D());
}
bool Game_Trans::SPHERE2DCOLAABB2D(Game_Ptr<Game_Trans> _Left, Game_Ptr<Game_Trans> _Right) {
	return _Left->SPHERE2D().Intersects(_Right->AABB2D());;
}
bool Game_Trans::SPHERE2DCOLSPHERE2D(Game_Ptr<Game_Trans> _Left, Game_Ptr<Game_Trans> _Right) {
	return _Left->SPHERE2D().Intersects(_Right->SPHERE2D());;
}

bool Game_Trans::AABB2DCOLOBB2D(Game_Ptr<Game_Trans> _Left, Game_Ptr<Game_Trans> _Right) {
	return _Left->AABB2D().Intersects(_Right->OBB2D());;
}
bool Game_Trans::AABB2DCOLAABB2D(Game_Ptr<Game_Trans> _Left, Game_Ptr<Game_Trans> _Right) {
	return _Left->AABB2D().Intersects(_Right->AABB2D());;
}
bool Game_Trans::AABB2DCOLSPHERE2D(Game_Ptr<Game_Trans> _Left, Game_Ptr<Game_Trans> _Right) {
	return _Left->AABB2D().Intersects(_Right->SPHERE2D());;
}

bool Game_Trans::OBB2DCOLOBB2D(Game_Ptr<Game_Trans> _Left, Game_Ptr<Game_Trans> _Right)
{
	return _Left->OBB2D().Intersects(_Right->OBB2D());;
}
bool Game_Trans::OBB2DCOLAABB2D(Game_Ptr<Game_Trans> _Left, Game_Ptr<Game_Trans> _Right) {
	return _Left->OBB2D().Intersects(_Right->AABB2D());;
}
bool Game_Trans::OBB2DCOLSPHERE2D(Game_Ptr<Game_Trans> _Left, Game_Ptr<Game_Trans> _Right)
{
	return _Left->OBB2D().Intersects(_Right->SPHERE2D());;
}
