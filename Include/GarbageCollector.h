#pragma once

#include "GameInfo.h"

class CGarbageCollector
{
private:
	class CRef** m_pMemoryArray;
	int			m_Count;
	int			m_Capacity;

public:
	bool Init();
	void AddMemory(class CRef* pMemory);
	void Update();

	DECLARE_SINGLE(CGarbageCollector)
};

