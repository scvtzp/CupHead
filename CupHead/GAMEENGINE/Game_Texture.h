#pragma once
#include "Game_Shader.h"
#include <Game_Mgr.h>
#include <Game_Ptr.h>

#include <DirectXTex.h>

#pragma comment(lib, "DirectXTex.lib")

class Game_Texture : public HFILEMGR<Game_Texture>
{
public:
	static Game_Ptr<Game_Texture> Load(const Game_File& _File)
	{
		Game_Ptr<Game_Texture> Res = CreateToInsertToFile(_File.FullPath());
		Res->Load();
		return Res;
	}

	static Game_Ptr<Game_Texture> Load(const Game_String& _Path)
	{
		Game_Ptr<Game_Texture> Res = CreateToInsertToFile(_Path);
		Res->Load();
		return Res;
	}

private:
	// 라이브러리에서 지원해주는 구조체
	DirectX::ScratchImage m_Img;
	D3D11_TEXTURE2D_DESC Desc; // 텍스처 정보
	ID3D11Resource* m_pTex; // 텍스처 메모리 권한
	ID3D11RenderTargetView* m_RTV; // 텍스처 랜더타겟 권한
	ID3D11DepthStencilView* m_DSV; // 텍스처 깊이타겟 권한

	// 이 권한을 이용해서 넣어줘야 한다.
	ID3D11ShaderResourceView* m_SRV; // 텍스처 쉐이더 세팅 권한

public:
	Game_Vector Size();

public:
	Game_Texture();
	~Game_Texture();

public:
	void Load();
	void Setting(SHADERTYPE _Type, unsigned int _Reg);
	Game_Vector GetPixel(Game_Vector _Pos);
};

