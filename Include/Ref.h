#pragma once

#include "GameInfo.h"

class CRef
{
public:
	CRef();
	virtual ~CRef() = 0;

protected:
	int		m_iRefCount;
	bool	m_bDestroy;
	bool	m_bEnable;

public:
	bool IsDestroy()	const
	{
		return m_bDestroy;
	}

	bool IsEnable()	const
	{
		return m_bEnable;
	}

public:
	void AddRef();
	void Release();
	void Enable(bool bEnable)
	{
		m_bEnable = bEnable;
	}

	void Destroy();
};

