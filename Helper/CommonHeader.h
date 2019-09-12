#pragma once

#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include <unordered_map>
#include <windows.h>
#include <cassert>
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "../DirectXTK/pch.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DirectXTK.lib")

using namespace DirectX;
using namespace Microsoft::WRL;
using std::vector;
using std::string;
using std::wstring;
using std::to_wstring;
using std::unique_ptr;
using std::make_unique;
using std::chrono::steady_clock;
using std::unordered_map;

enum class EShaderType
{
	Invalid,

	VertexShader,
	PixelShader
};