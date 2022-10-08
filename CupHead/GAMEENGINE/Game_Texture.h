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
	// ���̺귯������ �������ִ� ����ü
	DirectX::ScratchImage m_Img;
	D3D11_TEXTURE2D_DESC Desc; // �ؽ�ó ����
	ID3D11Resource* m_pTex; // �ؽ�ó �޸� ����
	ID3D11RenderTargetView* m_RTV; // �ؽ�ó ����Ÿ�� ����
	ID3D11DepthStencilView* m_DSV; // �ؽ�ó ����Ÿ�� ����

	// �� ������ �̿��ؼ� �־���� �Ѵ�.
	ID3D11ShaderResourceView* m_SRV; // �ؽ�ó ���̴� ���� ����

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

