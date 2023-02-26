#pragma once
#include "AI.h"

class CAIWill_o :
    public CAICharacter
{
	friend class CScene;

protected:
	CAIWill_o();
	CAIWill_o(const CAIWill_o& obj);
	virtual ~CAIWill_o();

public:
	virtual void Die();

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual CAIWill_o* Clone()	const;

public:
	void BeginCollision(class CCollider* pSrc, class CCollider* pDest, float DeltaTime);

};

