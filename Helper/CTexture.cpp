#include "CTexture.h"

void CTexture::CreateTexture(const string& TextureFileName)
{
	assert(!m_ShaderResourceView);
	assert(!m_Resource);
	assert(!m_SamplerState);

	wstring wfn{ TextureFileName.begin(), TextureFileName.end() };
	if (wfn.substr(wfn.size() - 3) == L"dds")
	{
		assert(SUCCEEDED(CreateDDSTextureFromFile(m_Device, wfn.c_str(), &m_Resource, &m_ShaderResourceView, 0)));
	}
	else
	{
		assert(SUCCEEDED(CreateWICTextureFromFile(m_Device, wfn.c_str(), &m_Resource, &m_ShaderResourceView, 0)));
	}

	auto texture{ static_cast<ID3D11Texture2D*>(m_Resource.Get()) };
	D3D11_TEXTURE2D_DESC texture_desc{};
	texture->GetDesc(&texture_desc);
	m_TextureSize.x = static_cast<float>(texture_desc.Width);
	m_TextureSize.y = static_cast<float>(texture_desc.Height);

	D3D11_SAMPLER_DESC sampler_desc{};
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MinLOD = 0.0f;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
	sampler_desc.MipLODBias = 0.0f;
	sampler_desc.MaxAnisotropy = 1;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampler_desc.BorderColor[0] = 1.0f;
	sampler_desc.BorderColor[1] = 1.0f;
	sampler_desc.BorderColor[2] = 1.0f;
	sampler_desc.BorderColor[3] = 1.0f;

	m_Device->CreateSamplerState(&sampler_desc, &m_SamplerState);
}

void CTexture::Use() const
{
	m_DeviceContext->PSSetSamplers(0, 1, m_SamplerState.GetAddressOf());
	m_DeviceContext->PSSetShaderResources(0, 1, m_ShaderResourceView.GetAddressOf());
}

const XMFLOAT2& CTexture::GetTextureSize() const
{
	return m_TextureSize; 
};