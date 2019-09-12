#include "CShader.h"

void CShader::Create(EShaderType ShaderType, LPCWSTR ShaderFileName, LPCSTR EntryPoint,
	const D3D11_INPUT_ELEMENT_DESC* VSInputElements, UINT VSInputElementCount)
{
	assert(m_eShaderType == EShaderType::Invalid);
	assert(ShaderType != EShaderType::Invalid);
	m_eShaderType = ShaderType;

	switch (m_eShaderType)
	{
	case EShaderType::VertexShader:
		assert(SUCCEEDED(D3DCompileFromFile(
			ShaderFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryPoint, "vs_4_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &m_ShaderBlob, nullptr)));

		assert(SUCCEEDED(m_Device->CreateVertexShader(
			m_ShaderBlob->GetBufferPointer(), m_ShaderBlob->GetBufferSize(), nullptr, &m_VertexShader)));

		assert(SUCCEEDED(m_Device->CreateInputLayout(VSInputElements, VSInputElementCount,
			m_ShaderBlob->GetBufferPointer(), m_ShaderBlob->GetBufferSize(), &m_InputLayout)));
		break;
	case EShaderType::PixelShader:
		assert(SUCCEEDED(D3DCompileFromFile(
			ShaderFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryPoint, "ps_4_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &m_ShaderBlob, nullptr)));

		assert(SUCCEEDED(m_Device->CreatePixelShader(
			m_ShaderBlob->GetBufferPointer(), m_ShaderBlob->GetBufferSize(), nullptr, &m_PixelShader)));
		break;
	default:
		break;
	}
}

void CShader::Use()
{
	switch (m_eShaderType)
	{
	case EShaderType::VertexShader:
		m_DeviceContext->VSSetShader(m_VertexShader.Get(), 0, 0);

		m_DeviceContext->IASetInputLayout(m_InputLayout.Get());
		break;
	case EShaderType::PixelShader:
		m_DeviceContext->PSSetShader(m_PixelShader.Get(), 0, 0);
		break;
	default:
		break;
	}
}