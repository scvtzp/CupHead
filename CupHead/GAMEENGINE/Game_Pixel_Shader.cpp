#include "Game_Pixel_Shader.h"


void Game_Pixel_Shader::Load(const Game_String& _FuncName, unsigned int _VH, unsigned int _VL)
{
	m_VH = _VH;
	m_VL = _VL;

	unsigned int Flag = 0;

#ifdef _DEBUG
	// 디버그 모드로 쉐이더 기동
	Flag = D3D10_SHADER_DEBUG;
#endif

	char Arr[200];

	sprintf_s(Arr, "ps_%d_%d", m_VH, m_VL);

	std::string FuncName = _FuncName;

	if (S_OK != D3DCompileFromFile(
		GameFile.FullPath(),
		nullptr,  // 매크로도 그냥 쉐이더 안에 있는걸
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // 인크루드를 그냥 알아서 하겠다.
		FuncName.c_str(), // 컴파일하고 싶은 함수이름
		Arr, // 버전
		D3DCOMPILE_PACK_MATRIX_ROW_MAJOR,
		0,// 기본 옵션으로 컴파일
		&m_pBlob,
		&m_pErrBlob)
		)
	{
		Game_String ErrorText = (char*)m_pErrBlob->GetBufferPointer();
		AMSG(ErrorText + L"쉐이더 컴파일 에러");
		return;
	}

	if (S_OK != Game_Device::MAINOBJ()->Device()->CreatePixelShader(
		m_pBlob->GetBufferPointer(),
		m_pBlob->GetBufferSize(),
		nullptr,
		&m_ShaderPtr))
	{
		AMSG(L"버텍스 쉐이더 인터페이스 생성에 실패했습니다");
	}

	//Game_Vector Src;
	//Game_Vector SrcFactor;

	//Game_Vector Dest;
	//Game_Vector DestFactor;
	//Game_Vector Color = { (Src.X * SrcFactor.X) + (Dest.X * DestFactor.X), };
}

void Game_Pixel_Shader::Setting() 
{
	// m_ShaderPtr
	Game_Device::MAINOBJ()->Context()->PSSetShader(m_ShaderPtr, 0, 0);
}