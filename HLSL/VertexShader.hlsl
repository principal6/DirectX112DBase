#include "Header.hlsli"

cbuffer cbSpace : register(b0)
{
	matrix Projection;
};

cbuffer cbSpace : register(b1)
{
	matrix World;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	output.pos = mul(input.pos, World);
	output.pos = mul(output.pos, Projection);
	output.color = input.color;
	output.uv = input.uv;

	return output;
}