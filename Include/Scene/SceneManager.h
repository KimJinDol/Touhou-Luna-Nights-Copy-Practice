#pragma once

#include "Scene.h"

class CSceneManager
{
private:
	CScene* m_pScene;
	CScene* m_pNextScene;

public:
	bool Init();
	// Update의 반환타입이 bool인 이유는 장면이 다른 장면으로 전환될 것인지를 판단해주기 위해서이다.
	bool Update(float DeltaTime);
	bool Collision(float DeltaTime);
	void Render(HDC hDC, float DeltaTime);

public:
	CScene* GetCurrentScene()	const
	{
		return m_pScene;
	}

	CScene* GetNextScene()	const 
	{
		return m_pNextScene;
	}

public:
	class CScene* CreateNextScene();

private:
	bool ChangeScene();

public:
	template <typename T>
	T* CreateSceneMode(bool bCurrent = true)
	{
		if (bCurrent)
			return m_pScene->CreateSceneMode<T>();

		return m_pNextScene->CreateSceneMode<T>();
	}

	DECLARE_SINGLE(CSceneManager)
};

