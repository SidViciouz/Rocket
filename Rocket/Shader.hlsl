struct Light
{
	float3 position;
	int type;
	float3 direction;
	int pad1;
	float3 color;
	int pad2;
};

struct VertexOut
{
	float4 pos : SV_POSITION;
	float2 tex : TEXTURE;
	float3 normal : NORMAL;
	float3 posW : POSITION;
};

struct VertexIn
{
	float3 pos : POSITION;
	float2 tex : TEXTURE;
	float3 normal : NORMAL;
};

cbuffer obj : register(b0)
{
	float4x4 world;
	float3 diffuseAlbedo;
	float roughness;
	float3 fresnel;
	int pad;
}

cbuffer trans : register(b1)
{
	float4x4 view;
	float4x4 projection;
	Light lights[3];
	float3 cameraPosition;
	int pad1;
}

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	float4 posH = mul(float4(vin.pos,1.0f),mul(transpose(world),mul(transpose(view),transpose(projection))));

	//homogeneous clip space로 ps에 보내야함.(SV_POSITION semantic으로 지정된 값)
	//vout.pos = mul(posW, viewProjection);
	vout.pos = posH;
	
	//반영 조명에서 halfway vector를 계산하기 위해서 필요함.
	vout.posW = mul(vin.pos, world);

	//non uniform scaling이라고 가정했을때 world matrix를 곱한다. 그렇지 않은 경우에는 inverse-transpose를 이용해야함.
	//빛 계산에 이용할 것이므로 world space로 변환
	vout.normal = mul(vin.normal,(float3x3)world);
	
	//uv좌표계
	vout.tex = vin.tex;

	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	float3 L = {0.0f,0.0f,1.0f};
	float rambertTerm = 0.0f;
	float4 color = { 0.0f,0.0f,0.0f,0.0f };
	float3 fresnelTerm;
	float roughnessTerm;

	for (int i = 0; i < 3; ++i)
	{
		if (lights[i].type == 0) //directional light
		{
			L = -normalize(lights[i].direction);
		}
		else if (lights[i].type == 1) // point light
		{
			L = normalize(lights[i].position - pin.pos);
		}
		else //spot light
		{

		}

		//빛의 입사각에 따라 단위면적당 들어오는 빛의 양이 달라짐을 표현
		rambertTerm = max(dot(L, pin.normal), 0.0f);

		//빛의 입사각에 따라 반사되는 빛의 양이 달라지는 것을 표현(반영 조명에서 사용)
		float cosTerm = 1.0f - saturate(dot(pin.normal, L));
		fresnelTerm = fresnel + (1.0f - fresnel) * (cosTerm * cosTerm * cosTerm * cosTerm * cosTerm);

		//표면 거칠기를 표현(반영 조명에서 사용)
		float3 halfway = normalize(normalize(cameraPosition - pin.posW) + L);
		float m = (1.0f - roughness) * 32.0f;
		roughnessTerm = (m + 8.0f) * pow(max(dot(halfway, pin.normal), 0.0f), m) / 8.0f;
		
		color += float4(rambertTerm * lights[i].color * (diffuseAlbedo + fresnelTerm*roughnessTerm), 1.0f);
	}

	return color;
}