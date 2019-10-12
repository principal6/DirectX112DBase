#include <chrono>
#include "Core/Game.h"

using std::chrono::steady_clock;

LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	constexpr float KClearColor[4]{ 0.2f, 0.6f, 0.9f, 1.0f };
	
	CGame Game{ hInstance, XMFLOAT2(800, 600) };
	Game.CreateWin32(WndProc, TEXT("Game"), L"Asset\\dotumche_10_korean.spritefont", true);

	CTexture* TextureGround{ Game.AddTexture() };
	{
		TextureGround->CreateFromFile(L"Asset\\ground.png");
	}

	CGameObject2D* go2DRectangle{ Game.AddGameObject2D("2DRectangle") };
	{
		go2DRectangle->ComponentTransform.Translation = XMVectorSet(0, 100, 0, 0);
		go2DRectangle->ComponentRender.PtrObject2D = Game.AddObject2D();
		go2DRectangle->ComponentRender.PtrObject2D->CreateDynamic(Generate2DRectangle(XMFLOAT2(100, 50)));
		go2DRectangle->ComponentRender.PtrTexture = TextureGround;
		go2DRectangle->bIsVisible = true;
	}

	while (true)
	{
		static MSG Msg{};
		static char KeyDown{};

		if (PeekMessage(&Msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (Msg.message == WM_KEYDOWN) KeyDown = (char)Msg.wParam;
			if (Msg.message == WM_QUIT) break;

			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		else
		{
			static steady_clock Clock{};
			long long TimeNow{ Clock.now().time_since_epoch().count() };
			static long long TimePrev{ TimeNow };
			float DeltaTimeF{ (TimeNow - TimePrev) * 0.000'000'001f };

			Game.BeginRendering(Colors::CornflowerBlue);

			const Keyboard::State& KeyState{ Game.GetKeyState() };
			if (KeyState.Escape)
			{
				Game.Destroy();
			}
			
			const Mouse::State& MouseState{ Game.GetMouseState() };
			static int PrevMouseX{ MouseState.x };
			static int PrevMouseY{ MouseState.y };
			if (MouseState.leftButton)
			{
			
			}
			if (MouseState.x != PrevMouseX || MouseState.y != PrevMouseY)
			{

				PrevMouseX = MouseState.x;
				PrevMouseY = MouseState.y;
			}
			if (MouseState.scrollWheelValue)
			{
				
			}

			Game.Animate();
			Game.Draw(DeltaTimeF);

			static SpriteBatch* PtrSpriteBatch{ Game.GetSpriteBatchPtr() };
			static SpriteFont* PtrSpriteFont{ Game.GetSpriteFontPtr() };
			PtrSpriteBatch->Begin();
			PtrSpriteFont->DrawString(PtrSpriteBatch, to_string(DeltaTimeF).c_str(), XMVectorSet(0, 0, 0, 1));
			PtrSpriteBatch->End();

			Game.EndRendering();

			KeyDown = 0;
			TimePrev = TimeNow;
		}
	}
	return 0;
}

LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (Msg)
	{
	case WM_ACTIVATEAPP:
		Keyboard::ProcessMessage(Msg, wParam, lParam);
		break;
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		Mouse::ProcessMessage(Msg, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard::ProcessMessage(Msg, wParam, lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return 0;
}