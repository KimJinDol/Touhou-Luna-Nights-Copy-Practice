#pragma once
#include "GameObject.h"

class CStaticObject :
    public CGameObject
{
	friend class CScene;

protected:
	CStaticObject();
	CStaticObject(const CStaticObject& obj);
	virtual ~CStaticObject();

public:
	void SetEnableRender(bool Value)
	{
		m_bIsRender = Value;
	}

public:
	virtual bool Init();
	virtual void Start();
	virtual void SetInput();
	virtual void Update(float DeltaTime);
	virtual void Collision();
	virtual void Render(HDC hDC, float DeltaTime);
	virtual CStaticObject* Clone() const;
};

