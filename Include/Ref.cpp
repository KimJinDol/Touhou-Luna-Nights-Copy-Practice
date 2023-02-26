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
	
	// ���̻� �� ��ü�� �����ϴ� ���� ���ٸ� �����Ѵ�.
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
