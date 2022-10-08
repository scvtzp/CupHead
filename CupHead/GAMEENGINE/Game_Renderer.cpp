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
	// ������ �������鼭 Ʈ���� ���̴�.
	// �ڽŸ��� ��ġ�� ũ�⸦ ������ �ִ�.

	// ���� �ڿ������� ��� ����?
	// �θ��� �ڽ����� ���� �ȴ�.
	// �θ� Ʈ�������� �ڽ����� ���� �ȴ�.
	// ������ �ڽ��� �ɰŴϱ� ���� ���������� �ʽ��ϴ�.
	// ������ ���� ���� �ʴ´�.
	PARENT(ACTOR()->TRANS());
	// ���ڽ��� Ʈ���� ���̴ϱ�

	// �̰� ������ �ִ�.
	// m_TRANSDATA
	Order(_Order);
	ACTOR()->SCENE()->PushRender(this);

	// m_AllRender.
}

void Game_Renderer::Render(Game_Ptr<Game_Cam> _Cam) 
{

}