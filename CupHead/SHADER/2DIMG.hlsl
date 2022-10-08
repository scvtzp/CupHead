struct VTX2DIMGIN
{
    float4 Pos : POSITION;
    float4 Uv : TEXCOORD;
    float4 Color : COLOR;
};

struct VTX2DIMGOUT
{
    float4 Pos : SV_Position;
    float4 Uv : TEXCOORD;
    float4 Color : COLOR;
};

// ������۴� ������� �ؾ� �Ѵ�.
// b�� �νĵǴµ� �������̳ĸ�?

// �� �����ʹ� cpu���� ���ƿðŴ�.
// 
cbuffer Game_TransDATA : register(b0)
{
    matrix POS;
    matrix SCALE;
    matrix ROT;
    matrix REVOL;
    matrix PARENT;
    matrix LWORLD;
    matrix WWORLD;
    matrix VIEW;
    matrix PROJ;
    matrix WV;
    matrix VP;
    matrix WVP;
}



cbuffer CUTDATA : register(b1)
{
    float4 CUTVECTOR;
}

// HVTX2DCOLOR
VTX2DIMGOUT VS_2DIMG(VTX2DIMGIN _In)
{
    VTX2DIMGOUT Out = (VTX2DIMGOUT)0;

    Out.Pos = mul(_In.Pos, WVP);
    Out.Uv = _In.Uv;

    // ���ؽ� ���̴��� 4�� ���ϱ�.

    // (0 * 1 / 21.0f) + 1 / 21.0f,0 , (0 * 1 / 21.0f) + 1 / 21.0f,0
    // (0 * 1 / 21.0f) + 1 / 21.0f,1 , (0 * 1 / 21.0f) + 1 / 21.0f,1 

    float XSize = CUTVECTOR.z;
    float YSize = CUTVECTOR.w;
    float XStart = CUTVECTOR.x;
    float YStart = CUTVECTOR.y;

    Out.Uv.x = (Out.Uv.x * XSize) + XStart;
    Out.Uv.y = (Out.Uv.y * YSize) + YStart;


    Out.Color = _In.Color;
    return Out;
}

cbuffer DRAWCOLOR : register(b0)
{
    float4 RENDERCOLOR;
}

// �ؽ�ó�� ����ϰڴٰ� �Ѱ�.
Texture2D Tex : register(t0);
SamplerState Smp : register(s0);

float4 PS_2DIMG(VTX2DIMGOUT _In) : SV_Target0
{
    // ���⼭ �ؽ�ó�� �˾ƾ���?
    // ���⿡�� ó���Ǽ� ȭ�鿡 �ȼ��� ���̴°���?
    // _In.Pos = mul(_In.Pos, WVP);

    // float2 Uv = float2(_In.Uv.x, _In.Uv.y);

    float4 Color = Tex.Sample(Smp, _In.Uv.xy);

    // 1, 1, 1 * 1, 0, 0 == 1, 1, 1
    Color.xyz *= RENDERCOLOR.xyz;
    Color.a *= RENDERCOLOR.a;

    if (Color.a == 0.0f)
    {
        clip(-1);
    }

    return Color;
}