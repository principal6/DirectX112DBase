#include "CEntityPool.h"

CEntity* CEntityPool::AddEntity()
{
	m_vEntities.emplace_back(make_unique<CEntity>(m_DirectX->m_Device.Get(), m_DirectX->m_DeviceContext.Get()));

	return m_vEntities.back().get();
}

CEntity* CEntityPool::GetEntity(size_t Index)
{
	return m_vEntities[Index].get();
}

void CEntityPool::DrawEntities()
{
	for (auto& i : m_vEntities)
	{
		i->m_MatrixWorld = XMMatrixTranspose(XMMatrixTranslation(i->WorldPosition.x, i->WorldPosition.y, 0.0f));

		m_DirectX->m_CBWorld->Update(&(i->m_MatrixWorld));

		i->Draw();
	}
}