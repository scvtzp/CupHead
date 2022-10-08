#include "Game_Renderer.h"
#include "Game_Actor.h"
#include "Game_Scene.h"
#include "Game_Device.h"
#include "Game_VB_Buffer.h"
#include "Game_IB_Buffer.h"
#include "Game_CBuffer.h"
#include "Game_DepthStencil.h"
#include "Game_blend.h"
#include "Game_Rasterizer.h"
#include "Game_Vertex_Shader.h"
#include "Game_Pixel_Shader.h"
#include "Game_Sampler.h"
#include "Game_Sprite.h"
#include "Game_Cam.h"


Game_Renderer::Game_Renderer()
{
}

Game_Renderer::~Game_Renderer()
{
}

void Game_Renderer::Init(int _Order)
{
	// 랜더는 랜더러면서 트랜스 폼이다.
	// 자신만의 위치와 크기를 가지고 있다.

	// 나는 자연스럽게 어떻게 되죠?
	// 부모의 자식으로 들어가게 된다.
	// 부모 트랜스폼의 자식으로 들어가게 된다.
	// 어차피 자식이 될거니까 따로 관리하지는 않습니다.
	// 어차피 씬에 넣지 않는다.
	PARENT(ACTOR()->TRANS());
	// 나자신이 트랜스 폼이니까

	// 이걸 가지고 있다.
	// m_TRANSDATA
	Order(_Order);
	ACTOR()->SCENE()->PushRender(this);

	// m_AllRender.
}

void Game_Renderer::Render(Game_Ptr<Game_Cam> _Cam) 
{

}