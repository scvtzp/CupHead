#pragma once
#include "Game_Device.h"
#include <Game_Mgr.h>

enum class SHADERTYPE
{
	SHADER_VS,
	SHADER_HS,
	SHADER_DS,
	SHADER_GS,
	SHADER_PS,
	SHADER_END,
};

// ���� ������ �ִ�.
// C++ 17����
class Game_Shader 
{
protected:
	// �������� �ƴϴ�.
	// char[]�ȿ� �װ� ��´�.
	// ���̴��� �ڵ� ����ü
	ID3DBlob* m_pBlob; // �����ϵ� �ڵ��� ���̳ʸ� ������
	// ������ ����ش�.
	ID3DBlob* m_pErrBlob; // �����ϵ� �ڵ��� ���̳ʸ� ������
	unsigned int m_VH;
	unsigned int m_VL;
	SHADERTYPE m_Type;


};

