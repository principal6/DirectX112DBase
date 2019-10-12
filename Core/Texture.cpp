#include "Texture.h"

void CTexture::CreateFromFile(const wstring& TextureFileName)
{
	size_t found{ TextureFileName.find_last_of(L'.') };
	wstring Ext{ TextureFileName.substr(found) };

	for (auto& c : Ext)
	{
		c = toupper(c);
	}
	if (Ext == L".DDS")
	{
		assert(SUCCEEDED(CreateDDSTextureFromFile(m_PtrDevice, TextureFileName.c_str(),
			(ID3D11Resource**)m_Texture2D.GetAddressOf(), &m_ShaderResourceView)));
	}
	else
	{
		assert(SUCCEEDED(CreateWICTextureFromFile(m_PtrDevice, TextureFileName.c_str(), 
			(ID3D11Resource**)m_Texture2D.GetAddressOf(), &m_ShaderResourceView)));
	}
}

void CTexture::CreateWICFromMemory(const vector<uint8_t>& RawData)
{
	assert(SUCCEEDED(CreateWICTextureFromMemory(m_PtrDevice, &RawData[0], RawData.size(), 
		(ID3D11Resource**)m_Texture2D.GetAddressOf(), &m_ShaderResourceView)));
}

void CTexture::CreateNoiseTexture(const XMFLOAT2& TextureSize)
{
	srand(static_cast<uint32_t>(GetTickCount64()));

	D3D11_TEXTURE2D_DESC Texture2DDesc{};
	Texture2DDesc.ArraySize = 1;
	Texture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	Texture2DDesc.CPUAccessFlags = 0;
	Texture2DDesc.Format = DXGI_FORMAT_R32_FLOAT;
	Texture2DDesc.Height = static_cast<UINT>(TextureSize.y);
	Texture2DDesc.MipLevels = 1;
	Texture2DDesc.SampleDesc.Count = 1;
	Texture2DDesc.SampleDesc.Quality = 0;
	Texture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	Texture2DDesc.Width = static_cast<UINT>(TextureSize.x);

	vector<float> vRawData{};
	static constexpr int KNoiseMax{ 255 };
	static constexpr int KNoiseStep{ 60 };
	static constexpr float KNoiseStepF{ static_cast<float>(KNoiseStep) / KNoiseMax };
	size_t TextureWidth{ static_cast<size_t>(TextureSize.x) };
	size_t TextureHeight{ static_cast<size_t>(TextureSize.y) };
	vRawData.resize(TextureWidth * TextureHeight);

	for (size_t x = 1; x < TextureWidth; ++x)
	{
		float Deviation{ static_cast<float>(rand() % (KNoiseStep + 1) - KNoiseStep / 2) / KNoiseMax };
		float RawValue{ vRawData[x - 1] + Deviation };
		RawValue = min(RawValue, 1.0f);
		RawValue = max(RawValue, 0.0f);

		vRawData[x] = static_cast<float>(RawValue);
	}

	for (size_t y = 1; y < TextureHeight; ++y)
	{
		for (size_t x = 1; x < TextureWidth; ++x)
		{
			float Deviation{ static_cast<float>(rand() % (KNoiseStep + 1) - KNoiseStep / 2) / KNoiseMax };
			float RawValue{ (vRawData[y * TextureWidth + x - 1] + vRawData[(y - 1) * TextureWidth + x]) / 2.0f + Deviation };
			RawValue = min(RawValue, 1.0f);
			RawValue = max(RawValue, 0.0f);

			vRawData[y * TextureWidth + x] = static_cast<float>(RawValue);
		}
	}

	D3D11_SUBRESOURCE_DATA SubresourceData{};
	SubresourceData.pSysMem = &vRawData[0];
	SubresourceData.SysMemPitch = static_cast<UINT>(sizeof(float) * TextureWidth);
	SubresourceData.SysMemSlicePitch = 0;
	m_PtrDevice->CreateTexture2D(&Texture2DDesc, &SubresourceData, m_Texture2D.GetAddressOf());

	m_PtrDevice->CreateShaderResourceView(m_Texture2D.Get(), nullptr, m_ShaderResourceView.GetAddressOf());
}

void CTexture::Use() const
{
	m_PtrDeviceContext->PSSetShaderResources(0, 1, m_ShaderResourceView.GetAddressOf());
}