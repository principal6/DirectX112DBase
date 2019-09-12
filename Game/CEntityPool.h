#pragma once

#include "../Helper/CDirectX.h"
#include "CEntity.h"

class CEntityPool
{
public:
	CEntityPool(CDirectX& DirectX) : m_DirectX{ &DirectX } {};
	~CEntityPool() {};

	CEntity* AddEntity();
	CEntity* GetEntity(size_t Index);
	void DrawEntities();

private:
	CDirectX*					m_DirectX{};
	vector<unique_ptr<CEntity>>	m_vEntities{};
};