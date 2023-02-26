#include "Ref.h"
#include "GarbageCollector.h"

CRef::CRef()	:
	m_iRefCount(1),
	m_bDestroy(false),
	m_bEnable(true)
{
}

CRef::~CRef()
{
}

void CRef::AddRef()
{
	++m_iRefCount;
}

void CRef::Release()
{
	--m_iRefCount;
	
	// 더이상 이 객체를 참조하는 곳이 없다면 제거한다.
	if (m_iRefCount == 0)
	{
		if (!m_bDestroy)
			CGarbageCollector::GetInst()->AddMemory(this);
		m_bDestroy = true;
	}
}

void CRef::Destroy()
{
	if (m_bDestroy) return;

	--m_iRefCount;

	CGarbageCollector::GetInst()->AddMemory(this);

	m_bDestroy = true;
}
