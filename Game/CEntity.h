#pragma once

#include "../Helper/CObject2D.h"
#include "../Helper/CTexture.h"

static constexpr size_t KAnimationIndexInvalid{ std::string::npos };

enum EFlipOption : char
{
	FlipNone		= 0b00000000,
	FlipHorizontal	= 0b00000001,
	FlipVertical	= 0b00000010,
};

struct SAnimationFrame
{
	SAnimationFrame() {};
	SAnimationFrame(const XMFLOAT2& _UVOffset, const XMFLOAT2& _UVSize) : UVOffset{ _UVOffset }, UVSize{ _UVSize } {};

	XMFLOAT2 UVOffset{};
	XMFLOAT2 UVSize{};
};

struct SAnimation
{
	SAnimation(const string& _Name, size_t _TickPerFrame) : Name{ _Name }, TickPerFrame{ _TickPerFrame } {};

	string Name{};
	size_t TickPerFrame{};
	size_t CurrentFrameIndex{};
	vector<SAnimationFrame> vFrames{};
};

class CEntity
{
	friend class CEntityPool;

public:
	void* operator new(size_t size) { return _aligned_malloc(size, 16); }
	void operator delete(void* Ptr) { _aligned_free(Ptr); }
	
public:
	CEntity(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext) : m_Device{ Device }, m_DeviceContext{ DeviceContext } {};
	~CEntity() {};

	void CreateRectangle(const XMFLOAT2& RectangleSize);
	void CreateRectangle(const XMFLOAT2& RectangleSize, const string& TextureFileName);
	void CreateTextureSizeRectanlge(const string& TextureFileName);

	void AddTexture(const string& TextureFileName);

	void SetRectangleUVRange(const XMFLOAT2& OffsetInTexture, const XMFLOAT2& SizeInTexture, EFlipOption Flip = FlipNone);
	void SetRectangleUVRangeAndSize(const XMFLOAT2& OffsetInTexture, const XMFLOAT2& SizeInTexture, EFlipOption Flip = FlipNone);

	SAnimation& AddAnimation(const string& AnimationName, size_t TickPerFrame = 30);
	void SetAnimation(size_t AnimationIndex);
	void SetAnimation(const string& AnimationName);
	void SetAnimationFlipping(EFlipOption Flip);
	void Animate();

public:
	XMFLOAT2 WorldPosition{};

private:
	void Draw() const;
	void UpdateAnimationFrame();

private:
	ID3D11Device*					m_Device{};
	ID3D11DeviceContext*			m_DeviceContext{};

	vector<CObject2D>				m_vObjects{};
	vector<CTexture>				m_vTextures{};

	vector<SAnimation>				m_vAnimations{};
	size_t							m_AnimationIndex{ KAnimationIndexInvalid };
	size_t							m_AnimationTick{};
	EFlipOption						m_AnimationFlip{ FlipNone };
	unordered_map<string, size_t>	m_AnimationIndexMap{};

	XMMATRIX						m_MatrixWorld{ XMMatrixIdentity() };
};