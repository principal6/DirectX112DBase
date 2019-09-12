#include "Header.hlsli"

SamplerState current_sampler_state : register(s0);
Texture2D current_texture : register(t0);

float4 main(VS_OUTPUT input) : SV_TARGET
{
	return current_texture.Sample(current_sampler_state, input.uv);
}