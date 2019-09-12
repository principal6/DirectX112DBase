#pragma once

#include "CommonHeader.h"
#include "CShader.h"
#include "CObject2D.h"
#include "CConstantBuffer.h"

class CDirectX
{
	friend class CEntityPool;

public:
	CDirectX() {};
	~CDirectX() {};

	void Create(LPCTSTR WindowTitle, const XMFLOAT2& WindowSize, WNDPROC WndProc, HINSTANCE hInstance, int nShowCmd, const string& FontFileName);
	
	CShader* AddShader(EShaderType ShaderType, LPCWSTR ShaderFileName, LPCSTR EntryPoint,
		const D3D11_INPUT_ELEMENT_DESC* VSInputElements = nullptr, UINT VSInputElementCount = 0);
	CShader* GetShader(size_t Index);

	void BeginRendering(const float* ClearColorArray);
	void EndRendering();
	void RenderText(const wstring& Text, const XMFLOAT2& Position, const FXMVECTOR& Color);

private:
	void CreateMainWindow(LPCTSTR Title, const XMFLOAT2& WindowSize, WNDPROC WndProc, HINSTANCE hInstance, int nShowCmd);
	void CreateDirect3DObjects(const string& FontFileName);

	void CreateDevice();
	void CreateAndSetRenderTargetView();
	void CreateBaseConstantBuffers();
	void CreateImageBlendState();
	void SetViewport();

private:
	HWND							m_hWnd{};
	HINSTANCE						m_hInstance{};
	XMFLOAT2						m_WindowSize{};

	ComPtr<IDXGISwapChain>			m_SwapChain{};
	ComPtr<ID3D11Device>			m_Device{};
	ComPtr<ID3D11DeviceContext>		m_DeviceContext{};
	ComPtr<ID3D11RenderTargetView>	m_RenderTargetView{};
	ComPtr<ID3D11BlendState>		m_BlendImageTransparency{};

	vector<unique_ptr<CShader>>		m_vShaders{};

	XMMATRIX						m_MatrixProjection{};
	unique_ptr<CConstantBuffer>		m_CBProjection{};
	unique_ptr<CConstantBuffer>		m_CBWorld{};

	unique_ptr<SpriteBatch>			m_SpriteBatch{};
	unique_ptr<SpriteFont>			m_SpriteFont{};
};