#pragma once
#include "GameObject.h"
#include "../Interface.h"

class CPortal :
    public CGameObject, public ITimeStopInterface
{
	friend class CScene;

protected:
	CPortal();
	CPortal(const CPortal& obj);
	virtual ~CPortal();

private:
	SceneType	m_CurrentSceneType;
	SceneType	m_NextSceneType;

public:
	void SetCurrentSceneType(SceneType Type)
	{
		m_CurrentSceneType = Type;
	}

	void SetNextSceneType(SceneType Type)
	{
		m_NextSceneType = Type;
	}

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void Update(float DeltaTime, TimeMode eMode);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual CPortal* Clone()	const;

public:
	virtual void SetTimeMode(TimeMode eMode);
	void BeginCollision(class CCollider* pSrc, class CCollider* pDest, float DeltaTime);
};

