#pragma once

#include "CommonHeader.h"

class CTexture
{
public:
	CTexture(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext) : m_Device{ Device }, m_DeviceContext{ DeviceContext } {};
	~CTexture() {};

	void CreateTexture(const string& TextureFileName);

	void Use() const;

	const XMFLOAT2& GetTextureSize() const;

private:
	ID3D11Device*						m_Device{};
	ID3D11DeviceContext*				m_DeviceContext{};

	ComPtr<ID3D11Resource>				m_Resource{};
	ComPtr<ID3D11ShaderResourceView>	m_ShaderResourceView{};
	ComPtr<ID3D11SamplerState>			m_SamplerState{};

	XMFLOAT2							m_TextureSize{};
};