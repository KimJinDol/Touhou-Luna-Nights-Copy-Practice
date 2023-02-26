#pragma once
#include "StaticObject.h"
class CSetsmei :
    public CStaticObject
{
	friend class CScene;

protected:
	CSetsmei();
	CSetsmei(const CSetsmei& obj);
	virtual ~CSetsmei();

private:
	float	m_BlinkTime;
	float	m_CurBlinkTime;
	std::string			m_dialCode;
	std::function<void()>	m_pEndDialogueFunc;

public:
	void SetDialCode(const std::string& dialCode)
	{
		m_dialCode = dialCode;
	}

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void Collision();
	virtual void Render(HDC hDC, float DeltaTime);
	virtual CSetsmei* Clone() const;

public:
	void BeginCollision(class CCollider* pSrc, class CCollider* pDest, float DeltaTime);
	void EndCollision(class CCollider* pSrc, class CCollider* pDest, float DeltaTime);
};

