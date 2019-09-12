#pragma once

#include "CommonHeader.h"

class CShader
{
public:
	CShader(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext) : m_Device{ Device }, m_DeviceContext{ DeviceContext } {};
	~CShader() {};

	void Create(EShaderType ShaderType, LPCWSTR ShaderFileName, LPCSTR EntryPoint,
		const D3D11_INPUT_ELEMENT_DESC* VSInputElements = nullptr, UINT VSInputElementCount = 0);

	void Use();

private:
	ID3D11Device*				m_Device{};
	ID3D11DeviceContext*		m_DeviceContext{};

	EShaderType					m_eShaderType{};

	ComPtr<ID3D11VertexShader>	m_VertexShader{};
	ComPtr<ID3D11PixelShader>	m_PixelShader{};

	ComPtr<ID3D11InputLayout>	m_InputLayout{};

	ComPtr<ID3D10Blob>			m_ShaderBlob{};
};