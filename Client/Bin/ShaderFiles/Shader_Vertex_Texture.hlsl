

#include "Shader_Client_Defines.h"

matrix g_matWorld, g_matView, g_matProj;

texture2D g_Texture;


struct VS_IN
{
	float3	vPosition : POSITION;
	float2	vTextureUV : TEXCOORD0;
};

struct VS_OUT
{
	float4	vPosition : SV_POSITION;
	float2	vTextureUV : TEXCOORD0;
};

VS_OUT	VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	matrix matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTextureUV = In.vTextureUV;

	return Out;
}

struct PS_IN
{
	float4	vPosition : SV_POSITION;
	float2	vTextureUV : TEXCOORD0;
};

struct PS_OUT
{
	float4	vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;


	Out.vColor = g_Texture.Sample(LinearSampler, In.vTextureUV);

	return Out;
}

technique11	DefaultTechnique
{
	pass Rect
	{
		//SetRasterizerState(RS_Default);
		//SetDepthStencilState(DS_Default, 0);
		//SetBlendState(BS_Default, float4(0.0f, 0.f, 0.f, 0.f), 0xffffffff);


		vertexshader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}