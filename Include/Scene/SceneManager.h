#pragma once

#include "Scene.h"

class CSceneManager
{
private:
	CScene* m_pScene;
	CScene* m_pNextScene;

public:
	bool Init();
	// Update�� ��ȯŸ���� bool�� ������ ����� �ٸ� ������� ��ȯ�� �������� �Ǵ����ֱ� ���ؼ��̴�.
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

