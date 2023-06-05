
matrix g_matWorld, g_matView, g_matProj;

struct VS_IN
{
	float3	vPosition : POSITION;
	float3	vTextureUV : TEXCOORD0;
};

struct VS_OUT
{
	float4	vPosition : SV_POSITION;
	float3	vTextureUV : TEXCOORD0;
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
	float3	vTextureUV : TEXCOORD0;
};

struct PS_OUT
{
	float4	vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = float4(0.1f, 1.f, 1.f, 1.f);

	return Out;
}

technique11	DefaultTechnique
{
	pass Square
	{
		vertexshader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}