#include "Game_Sprite_Renderer.h"
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

void Game_Sprite_Renderer::Init(int _Order) {

	// 부모의 함수 호출하는 방법.
	Game_Renderer::Init(_Order);

	if (nullptr == Game_CBuffer::Find(L"TRANSDATA"))
	{
		Game_CBuffer::Create<TransData>(L"TRANSDATA");
		Game_CBuffer::Create<Game_Vector>(L"COLOR");
		Game_CBuffer::Create<Game_Vector>(L"CUTDATA");
	}

	m_Color = Game_Vector::WHITE;
}
void Game_Sprite_Renderer::Render(Game_Ptr<Game_Cam> _Cam) 
{
	DebugCheck();

	if (nullptr == m_SPRITE)
	{
		AMSG(L"스프라이트가 세팅되지 않았습니다.");
	}

	// 부모인 트랜스폼의 기능
	CamUpdate(_Cam);

	Game_Ptr<Game_Rasterizer> RS = Game_Rasterizer::Find(L"NONE");
	Game_Ptr<Game_DepthStencil> DS = Game_DepthStencil::Find(L"DEFDEPTH");
	Game_Ptr<Game_blend> BLEND = Game_blend::Find(L"AlphaBlend");

	RS->Setting();
	DS->Setting();
	BLEND->Setting();

	// Game_Ptr<Game_Texture> TEX = Game_Texture::Find(L"WALKLEFT.png");
	Game_Ptr<Game_Sampler> SMP = Game_Sampler::Find(L"LWSMP");
	// Game_Ptr<Game_Sampler> SMP = Game_Sampler::Find(L"PMSMP");

	m_SPRITE->Tex()->Setting(SHADERTYPE::SHADER_PS, 0);
	// TEX->Setting(SHADERTYPE::SHADER_PS, 0);
	SMP->Setting(SHADERTYPE::SHADER_PS, 0);


	Game_Ptr<Game_Vertex_Shader> VTXS = Game_Vertex_Shader::Find(L"2DIMG.hlsl");
	Game_Ptr<Game_Pixel_Shader> ITXS = Game_Pixel_Shader::Find(L"2DIMG.hlsl");

	Game_Ptr<Game_CBuffer> CBTRANS = Game_CBuffer::Find(L"TRANSDATA");
	Game_Ptr<Game_CBuffer> CBCOLOR = Game_CBuffer::Find(L"COLOR");
	Game_Ptr<Game_CBuffer> CUTDATA = Game_CBuffer::Find(L"CUTDATA");

	CutData = m_SPRITE->SpriteData(m_SpriteIndex);

	CBTRANS->DataChange(m_TRANSDATA);
	CBCOLOR->DataChange(m_Color);
	CUTDATA->DataChange(CutData);

	CBTRANS->Setting(SHADERTYPE::SHADER_VS, 0);
	CUTDATA->Setting(SHADERTYPE::SHADER_VS, 1);
	CBCOLOR->Setting(SHADERTYPE::SHADER_PS, 0);

	VTXS->Setting();
	ITXS->Setting();

	Game_Ptr<Game_VB_Buffer> VB = Game_VB_Buffer::Find(L"2DCOLORRECT");
	Game_Ptr<Game_IB_Buffer> IB = Game_IB_Buffer::Find(L"2DCOLORRECT");
	VB->Setting();
	IB->Setting();

	Game_Device::MAINOBJ()->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Game_Device::MAINOBJ()->Context()->DrawIndexed(IB->Count(), 0, 0);

}