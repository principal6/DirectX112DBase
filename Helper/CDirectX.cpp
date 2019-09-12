#include "CDirectX.h"

void CDirectX::Create(LPCTSTR WindowTitle, const XMFLOAT2& WindowSize, WNDPROC WndProc, HINSTANCE hInstance, int nShowCmd, const string& FontFileName)
{
	CreateMainWindow(WindowTitle, WindowSize, WndProc, hInstance, nShowCmd);
	CreateDirect3DObjects(FontFileName);
}

void CDirectX::CreateMainWindow(LPCTSTR Title, const XMFLOAT2& WindowSize, WNDPROC WndProc, HINSTANCE hInstance, int nShowCmd)
{
	assert(!m_hWnd);

	m_hInstance = hInstance;
	m_WindowSize = WindowSize;

	WNDCLASSEX window_class{};
	window_class.cbSize = sizeof(WNDCLASSEX);
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = WndProc;
	window_class.hInstance = m_hInstance;
	window_class.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	window_class.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
	window_class.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	window_class.lpszClassName = TEXT("game_window");
	window_class.lpszMenuName = nullptr;
	assert(RegisterClassEx(&window_class));

	RECT window_rect{};
	window_rect.left = 0;
	window_rect.top = 0;
	window_rect.right = static_cast<LONG>(m_WindowSize.x);
	window_rect.bottom = static_cast<LONG>(m_WindowSize.y);
	AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, FALSE);
	
	assert(m_hWnd = CreateWindowEx(0, window_class.lpszClassName, Title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top,
		nullptr, nullptr, m_hInstance, nullptr));

	ShowWindow(m_hWnd, nShowCmd);
	
	UpdateWindow(m_hWnd);
}

void CDirectX::CreateDirect3DObjects(const string& FontFileName)
{
	CreateDevice();

	CreateAndSetRenderTargetView();

	CreateBaseConstantBuffers();

	CreateImageBlendState();

	SetViewport();

	wstring w_font_file_name{ FontFileName.begin(), FontFileName.end() };
	m_SpriteBatch = make_unique<SpriteBatch>(m_DeviceContext.Get());
	m_SpriteFont = make_unique<SpriteFont>(m_Device.Get(), w_font_file_name.c_str());
}

void CDirectX::CreateDevice()
{
	assert(!m_SwapChain);

	DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
	swap_chain_desc.BufferDesc.Width = static_cast<UINT>(m_WindowSize.x);
	swap_chain_desc.BufferDesc.Height = static_cast<UINT>(m_WindowSize.y);
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.BufferCount = 1;
	swap_chain_desc.OutputWindow = m_hWnd;
	swap_chain_desc.Windowed = TRUE;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	assert(SUCCEEDED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
		D3D11_SDK_VERSION, &swap_chain_desc, &m_SwapChain, &m_Device, nullptr, &m_DeviceContext)));
}

void CDirectX::CreateAndSetRenderTargetView()
{
	assert(!m_RenderTargetView);

	ComPtr<ID3D11Texture2D> back_buffer{};
	assert(SUCCEEDED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer)));
	assert(SUCCEEDED(m_Device->CreateRenderTargetView(back_buffer.Get(), nullptr, &m_RenderTargetView)));

	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), nullptr);
}

void CDirectX::CreateBaseConstantBuffers()
{
	m_CBProjection = make_unique<CConstantBuffer>(m_Device.Get(), m_DeviceContext.Get());
	m_MatrixProjection = XMMatrixOrthographicLH(m_WindowSize.x, m_WindowSize.y, 0.0f, 1.0f);
	m_CBProjection->Create(0, sizeof(XMMATRIX), &m_MatrixProjection);
	m_CBProjection->Use(EShaderType::VertexShader);

	m_CBWorld = make_unique<CConstantBuffer>(m_Device.Get(), m_DeviceContext.Get());
	m_CBWorld->Create(1, sizeof(XMMATRIX));
}

void CDirectX::CreateImageBlendState()
{
	// 중복 생성 방지
	assert(m_BlendImageTransparency == nullptr);

	D3D11_BLEND_DESC blendDesc{};
	blendDesc.AlphaToCoverageEnable = false;
	D3D11_RENDER_TARGET_BLEND_DESC render_target_blend_desc{};
	render_target_blend_desc.BlendEnable = true;
	render_target_blend_desc.SrcBlend = render_target_blend_desc.SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	render_target_blend_desc.DestBlend = render_target_blend_desc.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	render_target_blend_desc.BlendOp = render_target_blend_desc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	render_target_blend_desc.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0] = render_target_blend_desc;

	m_Device->CreateBlendState(&blendDesc, &m_BlendImageTransparency);
}

void CDirectX::SetViewport()
{
	D3D11_VIEWPORT view_port{};
	view_port.Width = m_WindowSize.x;
	view_port.Height = m_WindowSize.y;
	view_port.TopLeftX = 0.0f;
	view_port.TopLeftY = 0.0f;
	view_port.MinDepth = 0.0f;
	view_port.MaxDepth = 1.0f;

	m_DeviceContext->RSSetViewports(1, &view_port);
}

void CDirectX::BeginRendering(const float* ClearColorArray)
{
	m_CBProjection->Use(EShaderType::VertexShader);
	m_CBWorld->Use(EShaderType::VertexShader);

	m_DeviceContext->OMSetBlendState(m_BlendImageTransparency.Get(), nullptr, 0xFFFFFFFF);

	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), ClearColorArray);

	m_SpriteBatch->Begin();
}

void CDirectX::EndRendering()
{
	m_SpriteBatch->End();

	m_SwapChain->Present(0, 0);
}

void CDirectX::RenderText(const wstring& Text, const XMFLOAT2& Position, const FXMVECTOR& Color)
{
	m_SpriteFont->DrawString(m_SpriteBatch.get(), Text.c_str(), Position, Color);
}

CShader* CDirectX::AddShader(EShaderType ShaderType, LPCWSTR ShaderFileName, LPCSTR EntryPoint,
	const D3D11_INPUT_ELEMENT_DESC* VSInputElements, UINT VSInputElementCount)
{
	m_vShaders.emplace_back(make_unique<CShader>(m_Device.Get(), m_DeviceContext.Get()));

	m_vShaders.back()->Create(ShaderType, ShaderFileName, EntryPoint, VSInputElements, VSInputElementCount);

	return m_vShaders.back().get();
}

CShader* CDirectX::GetShader(size_t Index)
{
	return m_vShaders[Index].get();
}