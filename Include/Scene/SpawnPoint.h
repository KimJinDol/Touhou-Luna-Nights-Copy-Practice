#pragma once

#include "../Object/GameObject.h"

class CSpawnPoint
{
	friend class CScene;

protected:
	CSpawnPoint();
	CSpawnPoint(const CSpawnPoint& obj);
	virtual ~CSpawnPoint();

protected:
	class CScene* m_pScene;
	std::string	m_Name;
	bool		m_Enable;
	bool		m_Active;
	Vector2		m_vPos;
	CGameObject* m_pSpawnObj;
	std::string	m_PrototypeName;
	float	m_SpawnTime;
	float	m_SpawnTimeMax;	// -1이 들어가있다면 딱 1번만 생성하는것으로 한다. 생성만 하고 제거된다.
	size_t	m_ClassType;

public:
	void SetPos(const Vector2& vPos)
	{
		m_vPos = vPos;
	}

	void SetPos(float x, float y)
	{
		m_vPos = Vector2(x, y);
	}

	void SetEnable(bool Enable)
	{
		m_Enable = Enable;
	}

	void Destroy()
	{
		m_Active = false;
	}

	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}

	const std::string& GetName()	const
	{
		return m_Name;
	}

	void SetSpawnTime(float Time)
	{
		m_SpawnTimeMax = Time;
	}

	void SetSpawnPrototype(const std::string& Name)
	{
		m_PrototypeName = Name;
	}

	void SetSpawnObject(CGameObject* pObject)
	{
		m_pSpawnObj = pObject;
	}

	template <typename T>
	void SetCreateClassType()
	{
		m_ClassType = typeid(T).hash_code();
	}

public:
	virtual void Update(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
};

