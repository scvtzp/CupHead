#pragma once
#include "Game_Shader.h"
class Game_Pixel_Shader : public Game_Shader, public HFILEMGR<Game_Pixel_Shader>
{
public:
	static Game_Ptr<Game_Pixel_Shader> Load(const Game_String& _Path, const Game_String& _FuncName, unsigned int _VH = 5, unsigned int _VL = 0)
	{
		Game_Ptr<Game_Pixel_Shader> Res = CreateToInsertToFile(_Path);
		Res->Load(_FuncName, _VH, _VL);
		return Res;
	}

public:
	ID3D11PixelShader* m_ShaderPtr;


public:
	void Load(const Game_String& _FuncName, unsigned int _VH = 5, unsigned int _VL = 0);
	void Setting();
public:
	Game_Pixel_Shader() : m_ShaderPtr(nullptr)
	{
		m_Type = SHADERTYPE::SHADER_VS;
	}

	~Game_Pixel_Shader()
	{
		m_ShaderPtr->Release();
	}
};

