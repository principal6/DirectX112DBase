#include "CConstantBuffer.h"

void CConstantBuffer::Create(UINT ConstantBufferSlot, size_t ConstantBufferByteWitdth, const void* PtrInitialData)
{
	assert(!m_ConstantBuffer);
	m_ConstantBufferSlot = ConstantBufferSlot;
	m_ConstantBufferByteSize = ConstantBufferByteWitdth;

	D3D11_BUFFER_DESC constant_buffer_description{};
	constant_buffer_description.ByteWidth = static_cast<UINT>(m_ConstantBufferByteSize);
	constant_buffer_description.Usage = D3D11_USAGE_DYNAMIC;
	constant_buffer_description.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constant_buffer_description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constant_buffer_description.MiscFlags = 0;
	constant_buffer_description.StructureByteStride = 0;

	if (PtrInitialData)
	{
		D3D11_SUBRESOURCE_DATA subresource_data{};
		subresource_data.pSysMem = PtrInitialData;

		assert(SUCCEEDED(m_Device->CreateBuffer(&constant_buffer_description, &subresource_data, &m_ConstantBuffer)));
	}
	else
	{
		assert(SUCCEEDED(m_Device->CreateBuffer(&constant_buffer_description, nullptr, &m_ConstantBuffer)));
	}
}

void CConstantBuffer::Update(const void* PtrData)
{
	assert(m_ConstantBuffer);

	D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
	if (SUCCEEDED(m_DeviceContext->Map(m_ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource)))
	{
		memcpy(mapped_subresource.pData, PtrData, m_ConstantBufferByteSize);

		m_DeviceContext->Unmap(m_ConstantBuffer.Get(), 0);
	}
}

void CConstantBuffer::Use(EShaderType ShaderType)
{
	switch (ShaderType)
	{
	case EShaderType::VertexShader:
		m_DeviceContext->VSSetConstantBuffers(m_ConstantBufferSlot, 1, m_ConstantBuffer.GetAddressOf());
		break;
	case EShaderType::PixelShader:
		m_DeviceContext->PSSetConstantBuffers(m_ConstantBufferSlot, 1, m_ConstantBuffer.GetAddressOf());
		break;
	default:
		break;
	}
}