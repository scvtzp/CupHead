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

// 언어는 버전이 있다.
// C++ 17버전
class Game_Shader 
{
protected:
	// 사용권한이 아니다.
	// char[]안에 그걸 담는다.
	// 쉐이더의 코드 그자체
	ID3DBlob* m_pBlob; // 컴파일된 코드의 바이너리 포인터
	// 에러도 담아준다.
	ID3DBlob* m_pErrBlob; // 컴파일된 코드의 바이너리 포인터
	unsigned int m_VH;
	unsigned int m_VL;
	SHADERTYPE m_Type;


};

