#include "Collider.h"
#include "CollisionManager.h"
#include "ColliderPixel.h"

CCollider::CCollider()	:
	m_pProfile(nullptr),
	m_bIsBlock(false)
{
	SetCollisionProfile("Object");
}

CCollider::CCollider(const CCollider& collider)
{
	*this = collider;
	m_iRefCount = 1;

	m_BeginCollisionFunction = nullptr;
	m_EndCollisionFunction = nullptr;
}

CCollider::~CCollider()
{
	auto iter = m_CollisionList.begin();
	auto iterEnd = m_CollisionList.end();

	for (; iter != iterEnd; iter++)
	{
		(*iter)->DeleteCollisionList(this);
	}
}

void CCollider::SetCollisionProfile(const std::string& Name)
{
	m_pProfile = CCollisionManager::GetInst()->FindProfile(Name);
}

void CCollider::AddCollisionList(CCollider* pCollider)
{
	CSharedPtr<CCollider>	ptr = pCollider;

	m_CollisionList.push_back(ptr);
}

bool CCollider::CheckCollisionList(CCollider* pCollider)
{
	auto	iter = m_CollisionList.begin();
	auto	iterEnd = m_CollisionList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == pCollider)
			return true;
	}

	return false;
}

void CCollider::DeleteCollisionList(CCollider* pCollider)
{
	auto	iter = m_CollisionList.begin();
	auto	iterEnd = m_CollisionList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == pCollider)
		{
			m_CollisionList.erase(iter);
			return;
		}
	}
}

void CCollider::ClearCollisionList()
{
	auto	iter = m_CollisionList.begin();
	auto	iterEnd = m_CollisionList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->DeleteCollisionList(this);
	}

	m_CollisionList.clear();
}

bool CCollider::Init()
{
	return true;
}

void CCollider::Start()
{
}

void CCollider::Update(float DeltaTime)
{
	if (!m_bEnable)
	{
		auto iter = m_CollisionList.begin();
		auto iterEnd = m_CollisionList.end();

		for (; iter != iterEnd; iter++)
		{
			(*iter)->DeleteCollisionList(this);
			m_CollisionList.erase(iter);
		}

		m_CollisionList.clear();
		
	}
}

void CCollider::Render(HDC hDC, float DeltaTime)
{
}

CCollider* CCollider::Clone() const
{
	return nullptr;
}
