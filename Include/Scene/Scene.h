#pragma once

#include "../GameInfo.h"
#include "../Object/GameObject.h"
#include "../UI/UIWidget.h"

struct LayerInfo
{
	std::string						m_Name;
	std::list<CSharedPtr<class CGameObject>>	m_ObjList;
	int								m_iZOrder;

	LayerInfo() :
		m_iZOrder(0)
	{
	}

	~LayerInfo()
	{
	}
};

class CScene
{
	friend class CSceneManager;

	// 생성, 소멸자를 private으로 하면 외부에서 생성하거나 제거할 수 없다.
	// Scene은 SceneManager를 통해서만 생성, 제거를 할 수 있도록 한다.
	// 그렇게 하기 위해서 SceneManager를 friend 처리를 해두도록 한다.
private:
	CScene();
	~CScene();

private:
	class CSceneMode* m_pSceneMode;
	class CSceneCollision* m_pSceneCollision;
	class CSceneResource* m_pSceneResource;
	std::unordered_map<std::string, class CGameObject*>	m_mapPrototype;
	
	std::list<LayerInfo*> m_LayerList;
	std::list<class CGameObject*>	m_ObjList;

	// 출력될 오브젝트를 모아놓을 배열을 만들어준다.
	class CGameObject** m_pRenderArray;
	int				m_RenderArrayCount;
	int				m_RenderArrayCapacity;

	int				m_BackRenderArrayCount;

	std::vector<class CUIWidget*>	m_WidgetList;
	class CUIWidget* m_pMouseWidget;
	class CStage* m_pStage;

private:
	std::unordered_map<std::string, class CCamera*>	m_mapCamera;
	class CCamera* m_pMainCamera;

public:
	LayerInfo* CreateLayer(const std::string& Name, int iZOrder = 0);
	LayerInfo* FindLayer(const std::string& Name);
	static bool LayerSort(LayerInfo* pLayer1, LayerInfo* pLayer2);

public:
	class CUIWidget* GetMouseWidget()	const
	{
		return m_pMouseWidget;
	}

	class CCamera* GetMainCamera()	const
	{
		return m_pMainCamera;
	}

	class CSceneCollision* GetCollision()	const
	{
		return m_pSceneCollision;
	}

	class CSceneResource* GetResourceManager()	const
	{
		return m_pSceneResource;
	}

	class CSceneMode* GetSceneMode()	const
	{
		return m_pSceneMode;
	}

	class CStage* GetStage()	const
	{
		return m_pStage;
	}

public:
	bool Init();
	void Update(float DeltaTime);
	void Collision(float DeltaTime);
	void Render(HDC hDC, float DeltaTime);

public:
	class CGameObject* FindObject(const std::string& Name);
	class CUIWidget* FindWidget(const std::string& Name);

public:
	static int SortY(const void* pSrc, const void* pDest);
	static int SortZOrder(const void* pSrc, const void* pDest);

public:
	template <typename T>
	T* CreateGameObject(const std::string& Name, const Vector2& vPos = Vector2(0.f, 0.f),
		const Vector2& vSize = Vector2(0.f, 0.f), const std::string& LayerName = "Default")
	{
		T* pObj = new T;

		pObj->m_pScene = this;	
		pObj->SetName(Name);
		pObj->SetPos(vPos);
		pObj->SetSize(vSize);
		
		if (!pObj->Init())
		{		
			SAFE_DELETE(pObj);
			return nullptr;
		}

		LayerInfo* pLayer = FindLayer(LayerName);

		if (!pLayer) return NULL;
	
		if (pLayer->m_Name == "BackObj1")
			pObj->SetCameraInfluend(0.1f);
		else if (pLayer->m_Name == "BackObj2")
			pObj->SetCameraInfluend(0.5f);

		m_ObjList.push_back(pObj);
		pLayer->m_ObjList.push_back(pObj);

		return pObj;
	}

	template <typename T>
	T* CreateGameObject(const std::string& PrototypeName,
		const std::string& Name, const Vector2& vPos = Vector2(0.f, 0.f),
		const Vector2& vSize = Vector2(0.f, 0.f), const std::string& LayerName = "Default")
	{
		auto	iter = m_mapPrototype.find(PrototypeName);

		if (iter == m_mapPrototype.end())
			return nullptr;

		T* pObj = (T*)iter->second->Clone();

		pObj->SetScene(this);
		pObj->SetName(Name);
		pObj->SetPos(vPos);
		pObj->SetSize(vSize);

		LayerInfo* pLayer = FindLayer(LayerName);

		if (!pLayer) return NULL;

		m_ObjList.push_back(pObj);
		pLayer->m_ObjList.push_back(pObj);

		return pObj;
	}

	class CGameObject* CreateGameObjectNoType(const std::string& PrototypeName,
		const std::string& Name, const Vector2& vPos = Vector2(0.f, 0.f),
		const Vector2& vSize = Vector2(0.f, 0.f));

	template <typename T>
	T* CreatePrototype(const std::string& Name)
	{
		auto	iter = m_mapPrototype.find(Name);

		if (iter != m_mapPrototype.end())
			return nullptr;

		T* pObj = new T;

		pObj->SetScene(this);
		pObj->SetName(Name);

		if (!pObj->Init())
		{
			SAFE_DELETE(pObj);
			return nullptr;
		}

		m_mapPrototype.insert(std::make_pair(Name, pObj));

		return pObj;
	}

	template <typename T>
	T* FindProtoType(const std::string& PrototypeName)
	{
		auto	iter = m_mapPrototype.find(PrototypeName);

		if (iter == m_mapPrototype.end())
			return nullptr;

		return iter->second;
	}


	template <typename T>
	T* CreateSceneMode()
	{
		T* pSceneMode = new T;

		pSceneMode->m_pScene = this;

		if (!pSceneMode->Init())
		{
			SAFE_DELETE(pSceneMode);
			return nullptr;
		}

		m_pSceneMode = (CSceneMode*)pSceneMode;

		return pSceneMode;
	}

	template <typename T>
	T* CreateWidget(const std::string& Name)
	{
		T* pWidget = new T;

		pWidget->SetName(Name);
		pWidget->m_pScene = this;

		if (!pWidget->Init())
		{
			SAFE_DELETE(pWidget);
			return nullptr;
		}

		m_WidgetList.push_back(pWidget);

		return pWidget;
	}

	template <typename T>
	T* FindWidget(const std::string& Name)
	{
		auto	iter = m_WidgetList.begin();
		auto	iterEnd = m_WidgetList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->GetName() == Name)
				return (T*)*iter;
		}

		return nullptr;
	}

	template <typename T>
	T* CreateStage()
	{
		T* pStage = new T;

		pStage->m_pScene = this;

		if (!pStage->Init())
		{
			SAFE_DELETE(pStage);
			return nullptr;
		}

		SAFE_DELETE(m_pStage);
		m_pStage = pStage;

		return pStage;
	}
};

