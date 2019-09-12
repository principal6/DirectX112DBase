#include "Helper/CDirectX.h"
#include "Game/CEntityPool.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nShowCmd)
{
	static constexpr XMFLOAT2 KWindowSize{ 960.0f, 540.0f };
	static constexpr float KClearColor[]{ 0.0f, 0.6f, 1.0f, 1.0f };
	static constexpr D3D11_INPUT_ELEMENT_DESC KInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	static const string KAssetDir{ "Asset/" };

	CDirectX directx{};
	directx.Create(TEXT("Game"), KWindowSize, WndProc, hInstance, nShowCmd, KAssetDir + "dotumche_10_korean.spritefont");
	CShader* vs{ directx.AddShader(EShaderType::VertexShader, L"HLSL/VertexShader.hlsl", "main", KInputLayout, ARRAYSIZE(KInputLayout)) };
	CShader* ps{ directx.AddShader(EShaderType::PixelShader, L"HLSL/PixelShader.hlsl", "main") };

	CEntityPool entity_pool{ directx };

	CEntity* entity_bg{ entity_pool.AddEntity() };
	entity_bg->CreateRectangle(KWindowSize, KAssetDir + "bg.png");

	CEntity* entity_main_sprite{ entity_pool.AddEntity() };
	entity_main_sprite->CreateRectangle(XMFLOAT2(100, 100), KAssetDir + "rocky_brown_100x100_lexasset.png");
	entity_main_sprite->SetRectangleUVRange(XMFLOAT2(200, 0), XMFLOAT2(100, 100));
	{
		auto& animation{ entity_main_sprite->AddAnimation("Asleep") };
		animation.vFrames.emplace_back(XMFLOAT2(0, 0), XMFLOAT2(100, 100));
	}
	{
		auto& animation{ entity_main_sprite->AddAnimation("WakingUp") };
		animation.vFrames.emplace_back(XMFLOAT2(0, 0), XMFLOAT2(100, 100));
		animation.vFrames.emplace_back(XMFLOAT2(100, 0), XMFLOAT2(100, 100));
		animation.vFrames.emplace_back(XMFLOAT2(200, 0), XMFLOAT2(100, 100));
	}
	{
		auto& animation{ entity_main_sprite->AddAnimation("Idle", 90ll) };
		animation.vFrames.emplace_back(XMFLOAT2(300, 100), XMFLOAT2(100, 100));
		animation.vFrames.emplace_back(XMFLOAT2(0, 200), XMFLOAT2(100, 100));
		animation.vFrames.emplace_back(XMFLOAT2(0, 200), XMFLOAT2(100, 100));
		animation.vFrames.emplace_back(XMFLOAT2(0, 200), XMFLOAT2(100, 100));
		animation.vFrames.emplace_back(XMFLOAT2(0, 200), XMFLOAT2(100, 100));
		animation.vFrames.emplace_back(XMFLOAT2(0, 200), XMFLOAT2(100, 100));
		animation.vFrames.emplace_back(XMFLOAT2(0, 200), XMFLOAT2(100, 100));
		animation.vFrames.emplace_back(XMFLOAT2(0, 200), XMFLOAT2(100, 100));
	}
	{
		auto& animation{ entity_main_sprite->AddAnimation("Walk") };
		animation.vFrames.emplace_back(XMFLOAT2(200, 0), XMFLOAT2(100, 100));
		animation.vFrames.emplace_back(XMFLOAT2(300, 0), XMFLOAT2(100, 100));
		animation.vFrames.emplace_back(XMFLOAT2(0, 100), XMFLOAT2(100, 100));
		animation.vFrames.emplace_back(XMFLOAT2(100, 100), XMFLOAT2(100, 100));
		animation.vFrames.emplace_back(XMFLOAT2(200, 100), XMFLOAT2(100, 100));
	}
	entity_main_sprite->SetAnimation(2);
	entity_main_sprite->SetAnimationFlipping(FlipHorizontal);
	
	steady_clock clock{};
	long long time_prev{ clock.now().time_since_epoch().count() };
	long long time_now{};
	long long time_microsec{};
	long long timer_animation{};
	long long timer_movement{};
	float delta_time{};
	
	while (true)
	{
		static MSG msg{};

		time_now = clock.now().time_since_epoch().count();

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE)
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);

			DispatchMessage(&msg);
		}
		else
		{
			time_microsec = time_now / 1'000;

			delta_time = 0.000'000'001f * (time_now - time_prev);

			bool pressed_left{}, pressed_right{}, pressed_up{}, pressed_down{};
			if (GetKeyState(VK_RIGHT) < 0) pressed_left = true;
			if (GetKeyState(VK_LEFT) < 0) pressed_right = true;
			if (GetKeyState(VK_UP) < 0) pressed_up = true;
			if (GetKeyState(VK_DOWN) < 0) pressed_down = true;

			if (time_microsec >= timer_movement + 3'000)
			{
				if (pressed_left)
				{
					entity_main_sprite->WorldPosition.x += 1.0f;
					entity_main_sprite->SetAnimation("Walk");
					entity_main_sprite->SetAnimationFlipping(FlipHorizontal);
				}
				if (pressed_right)
				{
					entity_main_sprite->WorldPosition.x -= 1.0f;
					entity_main_sprite->SetAnimation("Walk");
					entity_main_sprite->SetAnimationFlipping(FlipNone);
				}
				if (pressed_up) entity_main_sprite->WorldPosition.y += 1.0f;
				if (pressed_down) entity_main_sprite->WorldPosition.y -= 1.0f;
				if (!pressed_left && !pressed_right) entity_main_sprite->SetAnimation("Idle");

				timer_movement = time_microsec;
			}

			if (time_microsec >= timer_animation + 1'500)
			{
				entity_main_sprite->Animate();

				timer_animation = time_microsec;
			}

			directx.BeginRendering(KClearColor);

			vs->Use();
			ps->Use();
			
			entity_pool.DrawEntities();

			directx.RenderText(L"Delta Time: " + to_wstring(delta_time) + L" √ ", XMFLOAT2(0, 0), Colors::Black);
			
			directx.EndRendering();

			time_prev = time_now;
		}
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) DestroyWindow(hWnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}