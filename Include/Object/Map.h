#pragma once
#include "StaticObject.h"

class CMap :
	public CStaticObject
{
	friend class CScene;

protected:
	CMap();
	CMap(const CMap& obj);
	virtual ~CMap();

private:
	int		m_Alpha;
	float	m_AlphaTime;
	float	m_curAlphaTime;
	bool	m_Reverse;

public:
	void SetReverse(bool Value)
	{
		m_Reverse = Value;
	}

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual CMap* Clone() const;

};