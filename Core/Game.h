#pragma once

#define NOMINMAX 0

#include <Windows.h>
#include "Object2D.h"
#include "PrimitiveGenerator.h"
#include "Texture.h"
#include "Shader.h"
#include "GameObject2D.h"
#include "TinyXml2/tinyxml2.h"

struct SCBVS2DSpaceData
{
	XMMATRIX	Projection{};
	XMMATRIX	World{};
};

struct SCBPS2DFlagsData
{
	BOOL		bUseTexture{};
	BOOL		Pad[3]{};
};

class CGame
{
public:
	CGame(HINSTANCE hInstance, const XMFLOAT2& WindowSize) : m_hInstance{ hInstance }, m_WindowSize{ WindowSize } {}
	~CGame() {}

public:
	void CreateWin32(WNDPROC WndProc, LPCTSTR WindowName, const wstring& FontFileName, bool bWindowed);
	void Destroy();

// Object pool
public:
	CShader* AddShader();
	CShader* GetShader(size_t Index);

	CObject2D* AddObject2D();
	CObject2D* GetObject2D(size_t Index);

	CTexture* AddTexture();
	CTexture* GetTexture(size_t Index);

	CGameObject2D* AddGameObject2D(const string& Name);
	CGameObject2D* GetGameObject2D(const string& Name);
	CGameObject2D* GetGameObject2D(size_t Index);

public:
	void BeginRendering(const FLOAT* ClearColor);
	void Animate();
	void Draw(float DeltaTime);
	void EndRendering();

public:
	Keyboard::State GetKeyState();
	Mouse::State GetMouseState();
	SpriteBatch* GetSpriteBatchPtr() { return m_SpriteBatch.get(); }
	SpriteFont* GetSpriteFontPtr() { return m_SpriteFont.get(); }

private:
	void CreateWin32Window(WNDPROC WndProc, LPCTSTR WindowName);
	void InitializeDirectX(const wstring& FontFileName, bool bWindowed);

private:
	void DrawGameObject2Ds(float DeltaTime);

private:
	void CreateSwapChain(bool bWindowed);
	void CreateSetViews();
	void SetViewports();
	void CreateInputDevices();
	void CreateShaders();

private:
	static constexpr float KDefaultFOV{ 50.0f / 360.0f * XM_2PI };
	static constexpr float KDefaultNearZ{ 0.1f };
	static constexpr float KDefaultFarZ{ 1000.0f };
	static constexpr XMVECTOR KColorWhite{ 1.0f, 1.0f, 1.0f, 1.0f };

public:
	unique_ptr<CShader>	VSBase2D{};
	unique_ptr<CShader>	PSBase2D{};

public:
	SCBVS2DSpaceData	cbVS2DSpaceData{};
	SCBPS2DFlagsData	cbPS2DFlagsData{};

private:
	vector<unique_ptr<CShader>>				m_vShaders{};
	vector<unique_ptr<CObject2D>>			m_vObject2Ds{};
	vector<unique_ptr<CTexture>>			m_vTextures{};
	vector<unique_ptr<CGameObject2D>>		m_vGameObject2Ds{};

	unordered_map<string, size_t>			m_mapGameObject2DNameToIndex{};

private:
	vector<D3D11_VIEWPORT>	m_vViewports{};

private:
	HWND		m_hWnd{};
	HINSTANCE	m_hInstance{};
	XMFLOAT2	m_WindowSize{};

private:
	XMMATRIX		m_MatrixProjection2D{};

private:
	ComPtr<IDXGISwapChain>			m_SwapChain{};
	ComPtr<ID3D11Device>			m_Device{};
	ComPtr<ID3D11DeviceContext>		m_DeviceContext{};
	ComPtr<ID3D11RenderTargetView>	m_RenderTargetView{};

	unique_ptr<Keyboard>			m_Keyboard{};
	unique_ptr<Mouse>				m_Mouse{};
	unique_ptr<SpriteBatch>			m_SpriteBatch{};
	unique_ptr<SpriteFont>			m_SpriteFont{};
	unique_ptr<CommonStates>		m_CommonStates{};
};