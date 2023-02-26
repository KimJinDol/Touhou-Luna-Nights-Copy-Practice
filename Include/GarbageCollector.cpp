#include "GarbageCollector.h"
#include "Ref.h"

DEFINITION_SINGLE(CGarbageCollector)

CGarbageCollector::CGarbageCollector()
{
	m_Count = 0;
	m_Capacity = 200;
	m_pMemoryArray = new CRef * [m_Capacity];
}

CGarbageCollector::~CGarbageCollector()
{
	for (int i = 0; i < m_Count; ++i)
	{
		SAFE_DELETE(m_pMemoryArray[i]);
	}

	delete[]	m_pMemoryArray;
}

bool CGarbageCollector::Init()
{
	return true;
}

void CGarbageCollector::AddMemory(CRef* pMemory)
{
	if (m_Count == m_Capacity)
	{
		m_Capacity *= 2;

		CRef** pArray = new CRef * [m_Capacity];

		memcpy(pArray, m_pMemoryArray, sizeof(CRef*) * m_Count);

		SAFE_DELETE_ARRAY(m_pMemoryArray);

		m_pMemoryArray = pArray;
	}

	m_pMemoryArray[m_Count] = pMemory;
	++m_Count;
}

void CGarbageCollector::Update()
{
	for (int i = 0; i < m_Count; ++i)
	{
		SAFE_DELETE(m_pMemoryArray[i]);
	}

	m_Count = 0;
}
