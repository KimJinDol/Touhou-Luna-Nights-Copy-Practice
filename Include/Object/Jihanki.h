#pragma once
#include "StaticObject.h"

class CJihanki :
    public CStaticObject
{
	friend class CScene;

protected:
	CJihanki();
	CJihanki(const CJihanki& obj);
	virtual ~CJihanki();

private:
	bool	m_bCanBuyJuse;
	static bool	m_bCheckFirst;

public:
	virtual bool Init();
	virtual CJihanki* Clone() const;

public:
	void CanBuyJuse();

public:
	void BeginCollision(class CCollider* pSrc, class CCollider* pDest, float DeltaTime);

	void PlayBuyJuse();
	void PlayAkikan();
	void PlayAkikan2();
	void PlayGoku();
	void PlayItem_get();
};

