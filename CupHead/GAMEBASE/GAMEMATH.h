#pragma once
#include <math.h>

// dirxtx���� �����ϴ� �Լ����� SIMD ������ �����ϱ� �����̴�.
// SIMD float 4���� �Ѳ����� ���ϰų� ���Ѵ�.
#include <DirectXCollision.h>
#include <DirectXCollision.inl>
#include <DirectXPackedVector.h>	// SIMD ������ ����� ���Ϳ� �Լ��� ������ ���.
#include <d3d11_4.h>				// ���̷�Ʈ ����̽� �� �Լ����� ����ִ� ���
#include <d3dcompiler.h>			// ���̴������� �Լ� 

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")

// �̷��� �־���� ���̷�Ʈ�� ����Ҽ� �ִ�.

#define MATHMSG(MSSAGE) _wassert(MSSAGE, _CRT_WIDE(__FILE__), (unsigned)(__LINE__))

#include <Windows.h>

class Game_Matrix;
class GAMEMATH 
{
public:
	// PI ������
	// D ��׸� => 360�� ����
	// R ���� => ȣ������ ����
	// ���п����� �ٵ� 3.14�� ���ؼ� ������ �� ����� �����.
	// ���� 90���� ȸ����Ű�� �ʹ�.
	// ȸ����Ű�� �Լ�(������ �־��ּ���)
	// ȸ����Ű�� �Լ�(90 * 3.14 / 180/0f)
	// �⺻������ ������ 180 => 3.14�� ǥ���Ѵ�.
	static const float PI;
	static const float DTOR;
	static const float RTOD;
	static const float PI2;
	static const float PI2D1;
};

class Game_Vector 
{
public:
	const static Game_Vector FOWARD;
	const static Game_Vector BACK;
	const static Game_Vector RIGHT;
	const static Game_Vector LEFT;
	const static Game_Vector UP;
	const static Game_Vector DOWN;

	const static Game_Vector RED;
	const static Game_Vector BLUE;
	const static Game_Vector WHITE;
	const static Game_Vector BLACK;

	const static Game_Vector NONE;
	const static Game_Vector ZERO;
	const static Game_Vector ONE;


public:
	void ZEROCHECK() const
	{
		if (X == 0.0F) {} //MATHMSG(L"X ũ�� �Ӽ��� 0�Դϴ�");		}
		if (Y == 0.0F)		{ MATHMSG(L"Y ũ�� �Ӽ��� 0�Դϴ�");		}
		if (Z == 0.0F)		{ MATHMSG(L"Z ũ�� �Ӽ��� 0�Դϴ�");		}
	}

	// ���ͷ� �÷��� ǥ���Ϸ��� 0~1������ ���� �־��ּ���
	static unsigned int ColorToUint(float _R, float _G, float _B, float _A)
	{
		unsigned int uColor = 0;
		__int8* Ptr = (__int8*)&uColor;
		//       0.5 * 255
		Ptr[0] = (__int8)(_R * 255.0f);
		Ptr[1] = (__int8)(_G * 255.0f);
		Ptr[2] = (__int8)(_B * 255.0f);
		Ptr[3] = (__int8)(_A * 255.0f);
		return uColor;
	}
	unsigned int ColorToUint()
	{
		unsigned int uColor = 0;
		__int8* Ptr = (__int8*)&uColor;
		//       0.5 * 255
		Ptr[0] = (__int8)(R * 255.0f);
		Ptr[1] = (__int8)(G * 255.0f);
		Ptr[2] = (__int8)(B * 255.0f);
		Ptr[3] = (__int8)(A * 255.0f);
		return uColor;
	}
	static unsigned int ColorToUintInt8(unsigned int _R, unsigned int _G, unsigned int _B, unsigned int _A)
	{
		unsigned int uColor = 0;
		__int8* Ptr = (__int8*)&uColor;
		//       0.5 * 255
		Ptr[0] = (__int8)_R;
		Ptr[1] = (__int8)_G;
		Ptr[2] = (__int8)_B;
		Ptr[3] = (__int8)_A;
		return uColor;
	}
	static Game_Vector ColorToUintInt8(unsigned char _R, unsigned char _G, unsigned char _B, unsigned char _A)
	{
		unsigned int uColor = 0;
		unsigned char* Ptr = (unsigned char*)&uColor;
		//       0.5 * 255
		Ptr[0] = _R;
		Ptr[1] = _G;
		Ptr[2] = _B;
		Ptr[3] = _A;

		Game_Vector Color;
		Color = UintToColor(uColor);
		return Color;
	}

	// *** �߰� ***

	static Game_Vector UintToColor(unsigned int _R)
	{
		// RGBA �� �� ����ִ�.

		Game_Vector vColor;
		unsigned __int8* Ptr = (unsigned __int8*)&_R;
		//       0.5 * 255
		vColor.X = (float)((float)Ptr[0] / 255.0F); // r
		vColor.Y = (float)((float)Ptr[1] / 255.0F); // g
		vColor.Z = (float)((float)Ptr[2] / 255.0F); // b
		vColor.W = (float)((float)Ptr[3] / 255.0F); // a
		return vColor;
	}

public:
	// 2���������� X�� Y�� ǥ���Ҽ� �ְԵȴ�.

	union 
	{
		struct 
		{
			float X; // r
			float Y; // g
			float Z; // b
			float W; // a
		};
		struct
		{
			float R; // r
			float G; // g
			float B; // b
			float A; // a
		};
		struct
		{
			float Arr[4]; // a
		};

		struct
		{
			float Arr2D[1][4]; // a
		};

		DirectX::XMVECTOR DXVEC;
	};


	// �������� ��ο� �Լ����� ��κ�
	// int�� �䱸�ϹǷ� ��ȯ�Լ��� ����ڴ�.
	// �׷��� �� �׷� ���ۺ��� int�� �������
	// float���� �����Ѵ���
	// int �� ��ȯ�ϳ���???

	// �̷������� ����� �ִ�.
	// int�� 0.1�� �̵��ߴٴ°��� ǥ���Ҽ� �������?
	// �Ұ����ϴ�.
	// �׷��Ƿ� ���� ��ġ�� ũ���
	// float���� ǥ���ϰ�
	// �װ��� �������� �ȼ������� ��������
	// int�� ��ȯ�ؼ� ����Ѵ�.

public:
	static Game_Vector WindowPosTo3DPos(Game_Vector _WindowPos, const Game_Vector& _ScreenSize) 
	{
		Game_Vector ConPos = _WindowPos;
		ConPos.Y *= -1.0f;
		ConPos += _ScreenSize.HalfVector();
		return ConPos;
	}

	static Game_Vector ReversWindowPosTo3DPos(Game_Vector _WindowPos, const Game_Vector& _ScreenSize)
	{
		Game_Vector ConPos = _WindowPos;
		ConPos.Y *= -1.0f;
		ConPos -= _ScreenSize.HalfVector();
		return ConPos;
	}


	static Game_Vector Corss3D(const Game_Vector& _Left, const Game_Vector& _Right)
	{
		return Game_Vector(
			_Left.Y * _Right.Z - _Left.Z * _Right.Y, 
			_Left.Z * _Right.X - _Left.X * _Right.Z,
			_Left.X * _Right.Y - _Left.Y * _Right.X
		);
	}


	static float Dot(const Game_Vector& _Left, const Game_Vector& _Right)
	{
		return _Left.X * _Right.X + _Left.Y * _Right.Y + _Left.Z * _Right.Z;
	}

	operator DirectX::XMVECTOR() const
	{
		return DXVEC;
	}

	operator DirectX::XMFLOAT3() const
	{
		return DirectX::XMFLOAT3(X, Y, Z);
	}

	operator DirectX::XMFLOAT4() const
	{
		return DirectX::XMFLOAT4(X, Y, Z, W);
	}

	float Dot(const Game_Vector& _Other)
	{
		return X * _Other.X + Y * _Other.Y + Z * _Other.Z;
	}

	float DirToRadZ()
	{
		Game_Vector Dir = *this;
		Dir.NORMAL3D();
		return acosf(Dir.X);
	}

	float DirToDegZ()
	{
		return DirToRadZ() * GAMEMATH::RTOD;
	}

	// � 

	static Game_Vector RadToDirZ(float _Rad) 
	{
		// ������ �־��ָ�
		// �׿� �´� ���� ���͸� ������ִ°�.
		// ���̰� 1�� ����

		// CalPos.X = (1 * cosf(_Rad)) ;
		// CalPos.Y = (1 * sinf(_Rad)) ;
		return { cosf(_Rad) , sinf(_Rad)};
	}

	int IX() const {		return (int)X;	}
	int IY() const {		return (int)Y;	}

	unsigned int UIX() const { return (unsigned int)X; }
	unsigned int UIY() const { return (unsigned int)Y; }

	// �����⺸�� ���ϱⰡ �׻� ������.
	// ���ϱ� 0.5f�� ó���Ѵ�.
	float HX() const { return X * 0.5f; }
	float HY() const { return Y * 0.5f; }

	int HIX() const { return (int)HX(); }
	int HIY() const { return (int)HY(); }

	// ���ʹ� => �ﰢ��
	// ��Ÿ����� ������ ����ϰ�
	// ������ �����ִ� �Լ��� ����ϱ⸸ �ϸ�
	// �ս��� ������ ���̸� �˼� �ִ�.
	float LEN3D() const 
	{ 
		DirectX::XMVECTOR Vec = DirectX::XMVector3Length(DXVEC);
		return Vec.m128_f32[0];
	}

	// ������ ���̷�
	// X�� Y�� ������ �Ǹ�
	// ���̰� 1�� ���Ͱ� ������ �ǰ�
	// �������Ͷ�� �Ѵ�.

	Game_Vector HalfVector() const
	{
		Game_Vector HV = *this;
		// ���� ���Ͱ� �ȴ�.
		HV *= 0.5f;
		return HV;
	}

	Game_Vector AbsVector() const
{
		Game_Vector HV = *this;
		HV.X = abs(HV.X);
		HV.Y= abs(HV.Y);
		HV.Z = abs(HV.Z);

		return HV;
	}

	Game_Vector DegToQ() const
	{
		Game_Vector Vector = *this;
		Vector *= GAMEMATH::DTOR;
		return DirectX::XMQuaternionRotationRollPitchYaw(Vector.X, Vector.Y, Vector.Z);
	}

	Game_Vector RadToQ() const
	{
		return DirectX::XMQuaternionRotationRollPitchYaw(X, Y, Z);
	}

	// ������ ���̸� 1�� �����ϴ� 
	void NORMAL3D()
	{
		DXVEC = DirectX::XMVector3Normalize(DXVEC);
		return;
	}

	Game_Vector NORMAL3DTORETURN()
	{
		Game_Vector V = DirectX::XMVector3Normalize(DXVEC);
		return V;
	}

	Game_Vector& operator=(const Game_Vector& _Value)
	{
		X = _Value.X;
		Y = _Value.Y;
		Z = _Value.Z;
		W = _Value.W;

		return *this;
	}


	bool operator==(const Game_Vector& _Value) const
	{
		return X == _Value.X && Y == _Value.Y && Z == _Value.Z;
	}

	bool operator!=(const Game_Vector& _Value) const
	{
		return X != _Value.X || Y != _Value.Y || Z != _Value.Z;
	}

	Game_Vector operator+(const Game_Vector& _Value) const
	{
		return { X + _Value.X, Y + _Value.Y, Z + _Value.Z };
	}

	Game_Vector operator*(const Game_Vector& _Value) const
	{
		return { X * _Value.X, Y * _Value.Y, Z * _Value.Z };
	}

	Game_Vector operator-(const Game_Vector& _Value) const
	{
		return { X - _Value.X, Y - _Value.Y,  Z - _Value.Z };
	}

	Game_Vector operator/(const Game_Vector& _Value) const
	{
		return { X / _Value.X, Y / _Value.Y,  Z / _Value.Z };
	}

	Game_Vector operator-()
	{
		return { -X, -Y, -Z};
	}


	Game_Vector& operator-=(const Game_Vector& _Value)
	{
		X -= _Value.X;
		Y -= _Value.Y;
		Z -= _Value.Z;

		return *this;
	}

	Game_Vector& operator+=(const Game_Vector& _Value)
	{
		X += _Value.X;
		Y += _Value.Y;
		Z += _Value.Z;

		return *this;
	}

	Game_Vector& operator*=(const Game_Matrix& _Value);
	Game_Vector operator*(const Game_Matrix& _Value);

	Game_Vector& operator*=(const float _Value)
	{
		X *= _Value;
		Y *= _Value;
		Z *= _Value;

		return *this;
	}

	Game_Vector& operator*=(const Game_Vector& _Value)
	{
		X *= _Value.X;
		Y *= _Value.Y;
		Z *= _Value.Z;

		return *this;
	}

	Game_Vector operator*(const float& _Value) const
	{
		return { X * _Value, Y * _Value, Z * _Value };
	}

	operator POINT() 
	{
		return POINT{IX(), IY()};
	}

	void Set3D(const Game_Vector& _Other) 
	{
		X = _Other.X;
		Y = _Other.Y;
		Z = _Other.Z;
	}

	Game_Vector RotDegReturn(const Game_Vector& _Deg) {
		return RotRadReturn(_Deg * GAMEMATH::DTOR);
	}
	Game_Vector RotRadReturn(const Game_Vector& _Rad)
	{
		return DirectX::XMVector3Rotate(*this, _Rad.RadToQ());
	}

	Game_Vector RotDegZReturn(float _Rad) {
		return RotRadZReturn(_Rad * GAMEMATH::DTOR);
	}
	Game_Vector RotRadZReturn(float _Rad)
	{
		Game_Vector CalPos = *this;
		CalPos.X = (X * cosf(_Rad)) - (Y * sinf(_Rad));
		CalPos.Y = (X * sinf(_Rad)) + (Y * cosf(_Rad));
		return CalPos;
	}

	Game_Vector RotDegYReturn(float _Rad) {
		return RotRadYReturn(_Rad * GAMEMATH::DTOR);
	}
	Game_Vector RotRadYReturn(float _Rad)
	{
		Game_Vector CalPos = *this;
		CalPos.X = (X * cosf(_Rad)) - (Z * sinf(_Rad));
		CalPos.Z = (X * sinf(_Rad)) + (Z * cosf(_Rad));
		return CalPos;
	}

	Game_Vector RotDegXReturn(float _Rad) {
		return RotRadXReturn(_Rad * GAMEMATH::DTOR);
	}
	Game_Vector RotRadXReturn(float _Rad)
	{
		Game_Vector CalPos = *this;
		CalPos.Y = (Y * cosf(_Rad)) - (Z * sinf(_Rad));
		CalPos.Z = (Y * sinf(_Rad)) + (Z * cosf(_Rad));
		return CalPos;
	}

	void RotDegZ(float _Rad) {
		RotRadZ(_Rad * GAMEMATH::DTOR);
	}
	void RotRadZ(float _Rad) 
	{
		Game_Vector CalPos = *this;
		CalPos.X = (X * cosf(_Rad)) - (Y * sinf(_Rad));
		CalPos.Y = (X * sinf(_Rad)) + (Y * cosf(_Rad));
		*this = CalPos;
	}

	void RotDegY(float _Rad) {
		RotRadY(_Rad * GAMEMATH::DTOR);
	}
	void RotRadY(float _Rad)
	{
		Game_Vector CalPos = *this;
		CalPos.X = (X * cosf(_Rad)) - (Z * sinf(_Rad));
		CalPos.Z = (X * sinf(_Rad)) + (Z * cosf(_Rad));
		*this = CalPos;
	}

	void RotDegX(float _Rad) {
		RotRadX(_Rad * GAMEMATH::DTOR);
	}
	void RotRadX(float _Rad)
	{
		Game_Vector CalPos = *this;
		CalPos.Y = (Y * cosf(_Rad)) - (Z * sinf(_Rad));
		CalPos.Z = (Y * sinf(_Rad)) + (Z * cosf(_Rad));
		*this = CalPos;
	}

public:
	Game_Vector() : X(0.0f), Y(0.0f), Z(0.0f), W(1.0f)	{	}
	Game_Vector(const int _X, const int _Y) : X((float)_X), Y((float)_Y), Z(0.0f), W(1.0f) {	}
	Game_Vector(const float _X, const  float _Y) : X(_X), Y(_Y), Z(0.0f), W(1.0f) {	}
	Game_Vector(const float _X, const  float _Y, const  float _Z) : X(_X), Y(_Y), Z(_Z), W(1.0f) {	}
	Game_Vector(const float _X, const  float _Y, const  float _Z, const  float _W) : X(_X), Y(_Y), Z(_Z), W(_W) {	}
	Game_Vector(const Game_Vector& _Other) : X(_Other.X), Y(_Other.Y), Z(_Other.Z), W(_Other.W) 	{		}
	Game_Vector(const DirectX::XMVECTOR& _Other) : DXVEC(_Other) {		}
};

class Game_Matrix
{
public:
	union 
	{
		struct 
		{
			float ArrF2D[4][4];
		};

		struct
		{
			float ArrF1D[16];
		};


		struct
		{
			Game_Vector ArrV[4];
		};

		DirectX::XMMATRIX DXMAT;
	};

public:
	Game_Matrix() { IDEN(); }
	~Game_Matrix() {}

	Game_Matrix(const Game_Matrix& _Other) : ArrF2D{ 0, }
	{
		memcpy_s(this, sizeof(Game_Matrix), &_Other, sizeof(Game_Matrix));
	}

	Game_Matrix(const DirectX::XMMATRIX& _Other) 
	{
		DXMAT = _Other;
	}

public:
	operator DirectX::XMMATRIX() const
	{
		return DXMAT;
	}

	void IDEN()
	{
		memset(this, 0, sizeof(Game_Matrix));
		DXMAT = DirectX::XMMatrixIdentity();
	}

	void RotDeg(const Game_Vector& _Value)
	{
		RotRad(_Value * GAMEMATH::DTOR);
	}

	void RotRad(const Game_Vector& _Value)
	{
		DXMAT = DirectX::XMMatrixRotationRollPitchYaw(_Value.X, _Value.Y, _Value.Z);
	}

	void RotXDeg(float _Angle) 
	{
		RotXRad(_Angle * GAMEMATH::DTOR);
	}

	void RotXRad(float _Angle)
	{
		IDEN();
		DXMAT = DirectX::XMMatrixRotationX(_Angle);
	}

	void RotYDeg(float _Angle) 
	{
		RotYRad(_Angle * GAMEMATH::DTOR);
	}

	void RotYRad(float _Angle)
	{
		IDEN();
		DXMAT = DirectX::XMMatrixRotationY(_Angle);
	}

	void RotZDeg(float _Angle) 
	{
		RotZRad(_Angle * GAMEMATH::DTOR);
	}

	void RotZRad(float _Angle)
	{
		IDEN();
		DXMAT = DirectX::XMMatrixRotationZ(_Angle);
	}

	void Pos3D(const Game_Vector& _Value)
	{
		IDEN();

		// ����� �̰� 64��Ʈ������ �ȴ�.
		// 32��Ʈ�� �����ϸ� �ȵȴ�.
		DXMAT = DirectX::XMMatrixTranslation(_Value.X, _Value.Y, _Value.Z);
		// DXMAT = DirectX::XMLoadFloat4x4(DirectX::XMMatrixTranslation(_Value.X, _Value.Y, _Value.Z));
		// #ifdef X64
		// #else
		// #endif
		// ArrV[3].Set3D(_Value);
	}

	void Scale3D(const Game_Vector& _Value)
	{
		IDEN();
		_Value.ZEROCHECK();
		DXMAT = DirectX::XMMatrixScaling(_Value.X, _Value.Y, _Value.Z);
	}

	void VIEWLH(const Game_Vector& _Pos, const Game_Vector& _EyeDir, const Game_Vector& _EyeUp)
	{
		IDEN();
		// ���̸� 1�� ����� ����


		// LookAt�� ��ġ��ü�� �־��ָ� �ȴ�. 
		// �̳༮�� ���������� 
		DXMAT = DirectX::XMMatrixLookToLH(_Pos, _EyeDir, _EyeUp);
		return;
	}

	// _FOV : ���� �þ߰�
	// _Width : ����ϰ� ���� ȭ���� �ʺ�
	// _Height : ����ϰ� ���� ȭ���� ����
	// _Near : ������� �Ÿ�
	// _Far : ������� �Ÿ�

	void VIEWPORT(float _StartX, float _StartY, float _W, float _H, float _MinZ, float _MaxZ) 
	{
		IDEN();

		ArrF2D[0][0] = _W / 2;
		ArrF2D[1][1] = -(_H / 2);
		ArrF2D[2][2] = _MinZ - _MaxZ;

		ArrF2D[3][0] = _StartX + ArrF2D[0][0];
		ArrF2D[3][1] = _StartY + (_H / 2);
		ArrF2D[3][2] = _MinZ;

		
	}

	void ORTHLH(const Game_Vector& _Size, float _Near, float _Far)
	{
		IDEN();
		DXMAT = DirectX::XMMatrixOrthographicLH(_Size.X, _Size.Y, _Near, _Far);
		return;
	}

	void PERSDEG(float _Fov, const Game_Vector& _Size, float _Near, float _Far)
	{
		PERSRAD(_Fov, _Size.X, _Size.Y, _Near, _Far);
	}

	void PERSRAD(float _Fov, float _Width, float _Height, float _Near, float _Far)
	{
		// � ȭ�鿡 �츮�� ������ �𸣴ϱ�
		// ȭ�� ��ü�� -1~1

		// 1.28 / 0.72
		// 0.72 / 1.28;

		// 2 / 1
		// x 0.5  y
		// ���� �þ߰�.

		IDEN();
		DXMAT = DirectX::XMMatrixPerspectiveFovLH(_Fov * GAMEMATH::DTOR, _Width/ _Height, _Near, _Far);


		return;
	}


	void TransPos() 
	{
		DXMAT = DirectX::XMMatrixTranspose(DXMAT);
	}

public:
	Game_Matrix& operator=(const Game_Matrix& _Value)
	{
		memcpy_s(this, sizeof(Game_Matrix),&_Value, sizeof(Game_Matrix));

		return *this;
	}

	Game_Matrix operator*(const Game_Matrix& _Value) const
	{
		Game_Matrix Mat = DirectX::XMMatrixMultiply(DXMAT, _Value.DXMAT);
		return Mat;
	}

};

enum class MATTYPE
{
	MATTYPE_POS,
	MATTYPE_SCALE,
	MATTYPE_ROT,
	MATTYPE_REVOL,
	MATTYPE_PARENT,
	MATTYPE_LWORLD,
	MATTYPE_WWORLD,
	MATTYPE_VIEW,
	MATTYPE_PROJ,
	MATTYPE_WV,
	MATTYPE_VP,
	MATTYPE_WVP,
	MATTYPE_MAX,
};

class TransData
{
public:
	union
	{
		struct
		{
			Game_Matrix POS;
			Game_Matrix SCALE;
			Game_Matrix ROT;
			Game_Matrix REVOL;
			Game_Matrix PARENT;
			Game_Matrix LWORLD;
			Game_Matrix WWORLD;
			Game_Matrix VIEW;
			Game_Matrix PROJ;
			Game_Matrix WV;
			Game_Matrix VP;
			Game_Matrix WVP;
		};

		Game_Matrix ARRMAT[(int)MATTYPE::MATTYPE_MAX];
	};

public:
	void CALIDEN()
	{
		for (auto& _Value : ARRMAT)
		{
			_Value.IDEN();
		}
	}

	void CALLWORLD()
	{
		LWORLD = SCALE * ROT * POS * REVOL;
	}

	void CALWWORLD( )
	{
		WWORLD = LWORLD * PARENT;
	}

	void SETVIEWPROJ(Game_Matrix _VIEW, Game_Matrix _PROJ)
	{
		VIEW = _VIEW;
		PROJ = _PROJ;
	}

	void CALWVP() 
	{
		WV = WWORLD * VIEW;
		VP = VIEW * PROJ;

		// �̰� ���� �߿��� ���.
		// WWORLD == �÷��̾��� ��ġ�� ��� �ִ�. ũ�� ȸ��
		// View == ī�޶��� ��ġ�� �ٶ󺸴� ����� ȸ����
		WVP = WWORLD * VIEW * PROJ;
	}

	void CALTrans() 
	{
		CALLWORLD();
		CALWWORLD();
		CALWVP();
	}

	TransData() : LWORLD()
	{

	}

	~TransData()
	{

	}
};