#pragma once
#include "StaticObject.h"


class CTimeStopItem :
    public CStaticObject
{
	friend class CScene;

protected:
	CTimeStopItem();
	CTimeStopItem(const CTimeStopItem& obj);
	virtual ~CTimeStopItem();



public:
	virtual bool Init();
	virtual CTimeStopItem* Clone() const;


public:
	void BeginCollision(class CCollider* pSrc, class CCollider* pDest, float DeltaTime);
};

