#pragma once

#include "CommonHeader.h"

class CConstantBuffer
{
public:
	CConstantBuffer(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext) : m_Device{ Device }, m_DeviceContext{ DeviceContext } {};
	~CConstantBuffer() {};

	void Create(UINT ConstantBufferSlot, size_t ConstantBufferByteWitdth, const void* PtrInitialData = nullptr);
	void Update(const void* PtrData);

	void Use(EShaderType ShaderType);

private:
	ID3D11Device*			m_Device{};
	ID3D11DeviceContext*	m_DeviceContext{};

	ComPtr<ID3D11Buffer>	m_ConstantBuffer{};

	UINT					m_ConstantBufferSlot{};
	size_t					m_ConstantBufferByteSize{};
};