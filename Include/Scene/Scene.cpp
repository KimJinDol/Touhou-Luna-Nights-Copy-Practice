
#include "Scene.h"
#include "../Object/GameObject.h"
#include "SceneMode.h"
#include "SceneCollision.h"
#include "SceneResource.h"
#include "Camera.h"
#include "../GameManager.h"
#include "../UI/UIWidget.h"
#include "../UI/MouseWidget.h"
#include "../Stage/Stage.h"
#include "../Interface.h"
#include "../Object/Player.h"

CScene::CScene()	:
	m_pSceneMode(nullptr),
	m_pMouseWidget(nullptr),
	m_pStage(nullptr)
{
	m_pSceneCollision = new CSceneCollision;
	m_pSceneCollision->SetScene(this);
	m_pSceneResource = new CSceneResource;

	m_RenderArrayCount = 0;
	m_RenderArrayCapacity = 1000;

	m_pRenderArray = new CGameObject * [m_RenderArrayCapacity];

	CreateLayer("BackGround");
	CreateLayer("BackObj1");
	CreateLayer("BackObj2");
	CreateLayer("Stage", 1);
	CreateLayer("Default", 2);
	CreateLayer("Collider", 3);
	
}

CScene::~CScene()
{
	delete[]	m_pRenderArray;

	SAFE_DELETE(m_pStage);

	auto iterLayer = m_LayerList.begin();
	auto iterLayerEnd = m_LayerList.end();

	for (; iterLayer != iterLayerEnd; ++iterLayer)
	{
		SAFE_DELETE(*iterLayer);
	}

	m_LayerList.clear();

	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->SetScene(nullptr);
		//(*iter)->m_pScene = nullptr;
		(*iter)->Release();
	}

	auto	iter1 = m_mapPrototype.begin();
	auto	iter1End = m_mapPrototype.end();

	for (; iter1 != iter1End; ++iter1)
	{
		iter1->second->SetScene(nullptr);
		SAFE_RELEASE(iter1->second);
	}

	auto	iter2 = m_mapCamera.begin();
	auto	iter2End = m_mapCamera.end();

	for (; iter2 != iter2End; ++iter2)
	{
		SAFE_DELETE(iter2->second);
	}

	auto	iter3 = m_WidgetList.begin();
	auto	iter3End = m_WidgetList.end();

	for (; iter3 != iter3End; ++iter3)
	{
		(*iter3)->SetScene(nullptr);
		SAFE_RELEASE((*iter3));
	}

	m_WidgetList.clear();

	//m_pMouseWidget->m_pScene = nullptr;
	//SAFE_RELEASE(m_pMouseWidget);

	SAFE_DELETE(m_pSceneResource);
	SAFE_DELETE(m_pSceneMode);
	SAFE_DELETE(m_pSceneCollision);
}

LayerInfo* CScene::CreateLayer(const std::string& Name, int iZOrder /*= 0*/)
{
	LayerInfo* pLayer = new LayerInfo;

	pLayer->m_Name = Name;
	pLayer->m_iZOrder = iZOrder;
	
	m_LayerList.push_back(pLayer);

	if (m_LayerList.size() >= 2)
		m_LayerList.sort(CScene::LayerSort);

	return pLayer;
}

LayerInfo* CScene::FindLayer(const std::string& Name)
{
	std::list<LayerInfo*>::iterator iter = m_LayerList.begin();
	std::list<LayerInfo*>::iterator iterEnd = m_LayerList.end();

	for (; iter != iterEnd; iter++)
	{
		if ((*iter)->m_Name == Name)
			return *iter;
	}

	return NULL;
}

bool CScene::LayerSort(LayerInfo* pLayer1, LayerInfo* pLayer2)
{
	return pLayer1->m_iZOrder < pLayer2->m_iZOrder;
}

bool CScene::Init()
{
	m_pMainCamera = new CCamera;

	m_pMainCamera->Init();

	m_mapCamera.insert(std::make_pair("MainCamera", m_pMainCamera));

	Resolution RS = CGameManager::GetInst()->GetResolution();

	m_pMainCamera->SetResolution((float)RS.Width, (float)RS.Height);
	m_pMainCamera->SetWorldSize((float)RS.Width, (float)RS.Height);

	//m_pMouseWidget = new CMouseWidget;

	//m_pMouseWidget->SetName("MouseWidget");
	//m_pMouseWidget->m_pScene = this;

	//if (!m_pMouseWidget->Init())
	//{
	//	SAFE_DELETE(m_pMouseWidget);
	//	return false;
	//}

	return true;
}

void CScene::Update(float DeltaTime)
{
	if (m_pSceneMode)
	{
		if (!m_pSceneMode->m_Start)
		{
			m_pSceneMode->Start();
		}

		m_pSceneMode->Update(DeltaTime);
	}

	if (m_pStage)
	{
		if (!m_pStage->IsStart())
			m_pStage->Start();
	}

	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; )
	{
		if ((*iter)->IsDestroy())
		{
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}
		++iter;
	}
	iter = m_ObjList.begin();
	iterEnd = m_ObjList.end();;
	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->m_Start)
		{
			(*iter)->m_Start = true;
			(*iter)->SetInput();
			(*iter)->Start();
		}

		if (!(*iter)->IsEnable())
			continue;

		if (!m_pSceneMode->m_bIsAllStop)
		{
			if (m_pSceneMode->GetTimeMode() == TimeMode::Move)
			{
				ITimeStopInterface* pTimeStopObj = Cast<ITimeStopInterface*>(*iter);
				if (pTimeStopObj)
				{
					if (pTimeStopObj->bTimeStop)
					{
						pTimeStopObj->bTimeStop = false;
						pTimeStopObj->SetTimeMode(TimeMode::Move);
					}
				}
				(*iter)->Update(DeltaTime);
			}
			else if (m_pSceneMode->GetTimeMode() == TimeMode::Stop)
			{
				ITimeStopInterface* pTimeStopObj = Cast<ITimeStopInterface*>(*iter);
				if (pTimeStopObj)
				{
					if (!pTimeStopObj->bTimeStop)
					{
						pTimeStopObj->bTimeStop = true;
						pTimeStopObj->SetTimeMode(TimeMode::Stop);
					}
					pTimeStopObj->Update(DeltaTime, TimeMode::Stop);
				}
			}
			else
			{
				ISlowTimeInterface* pSlowTimeObj = Cast<ISlowTimeInterface*>(*iter);
				if (pSlowTimeObj)
				{
					pSlowTimeObj->Update(DeltaTime, TimeMode::Snail);
				}
				else
				{
					(*iter)->Update(DeltaTime * m_pSceneMode->GetWorldTimeSpeed());
				}
			}
		}
		else
			(*iter)->Update(0.f);
	}

	if (m_pStage)
		m_pStage->Update(DeltaTime);

	m_pMainCamera->Update(DeltaTime);

	// 마우스 좌표를 이용하여 선택된 UI를 구하고 ZOrder가 우선순위가 높은 UI에 클릭되었을 경우
	// 하위 모든 UI에 대한 충돌처리를 제외시켜버린다.

	auto	iterUI = m_WidgetList.begin();
	auto	iterUIEnd = m_WidgetList.end();

	for (; iterUI != iterUIEnd; )
	{
		if ((*iterUI)->IsDestroy())
		{
			iterUI = m_WidgetList.erase(iterUI);
			iterUIEnd = m_WidgetList.end();
			continue;
		}

		++iterUI;
	}

	iterUI = m_WidgetList.begin();
	iterUIEnd = m_WidgetList.end();

	for (; iterUI != iterUIEnd; ++iterUI)
	{
		if (!(*iterUI)->IsEnable())
			continue;

		(*iterUI)->Update(DeltaTime);
	}

//	m_pMouseWidget->Update(DeltaTime);

	// Widget을 정렬한다.
	if (m_WidgetList.size() > 0)
		qsort(&m_WidgetList[0], m_WidgetList.size(), (size_t)sizeof(CUIWidget*),
			CScene::SortZOrder);
}

void CScene::Collision(float DeltaTime)
{
	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; )
	{
		if ((*iter)->IsDestroy())
		{
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		(*iter)->Collision();

		++iter;
	}

	if (m_pStage)
		m_pStage->Collision(DeltaTime);

	// 마우스와 UI, World 의 물체를 충돌해본다.
	m_pSceneCollision->CollisionMouse(DeltaTime, m_WidgetList);

	// 등록된 충돌체들끼리 서로 충돌처리를 진행한다.
	m_pSceneCollision->Collision(DeltaTime);
}

void CScene::Render(HDC hDC, float DeltaTime)
{
	m_RenderArrayCount = 0;
	m_BackRenderArrayCount = 0;


	auto	Layeriter = m_LayerList.begin();
	auto	LayeriterEnd = m_LayerList.end();

	for (; Layeriter != LayeriterEnd; Layeriter++)
	{
		auto	iter = (*Layeriter)->m_ObjList.begin();
		auto	iterEnd = (*Layeriter)->m_ObjList.end();

		for (; iter != iterEnd; )
		{
			if ((*iter)->IsDestroy())
			{
				iter = (*Layeriter)->m_ObjList.erase(iter);
				iterEnd = (*Layeriter)->m_ObjList.end();
				continue;
			}

			(*iter)->PrevRender();

			if (!(*iter)->IsCulling() && (*iter)->IsEnable())
			{
				if (m_RenderArrayCount == m_RenderArrayCapacity)
				{
					m_RenderArrayCapacity *= 2;

					CGameObject** pArray = new CGameObject * [m_RenderArrayCapacity];

					memcpy(pArray, m_pRenderArray, sizeof(CGameObject*) * m_RenderArrayCount);

					delete[]	m_pRenderArray;

					m_pRenderArray = pArray;
				}

					m_pRenderArray[m_RenderArrayCount] = *iter;
					++m_RenderArrayCount;
			}

			++iter;
		}

		if ((*Layeriter)->m_Name == "BackGround" || (*Layeriter)->m_Name == "BackObj1" || (*Layeriter)->m_Name == "BackObj2")
			m_BackRenderArrayCount = m_RenderArrayCount;
	}

	for (int i = 0; i < m_BackRenderArrayCount; ++i)
	{
		m_pRenderArray[i]->Render(hDC, DeltaTime);
	}

	if (m_pStage)
	{
		m_pStage->Render(hDC, DeltaTime);
	}

	// 실제 출력되는 배열을 Y소팅을 해준다.
	// 소팅은 Bottom값을 기준으로 정렬한다.
	//qsort(m_pRenderArray, (size_t)m_RenderArrayCount, (size_t)sizeof(CGameObject*),
	//	CScene::SortY);

	for (int i = m_BackRenderArrayCount; i < m_RenderArrayCount; ++i)
	{
		m_pRenderArray[i]->Render(hDC, DeltaTime);
	}

	// UI를 그려낸다.
	auto	iterUI = m_WidgetList.begin();
	auto	iterUIEnd = m_WidgetList.end();

	for (; iterUI != iterUIEnd; )
	{
		if ((*iterUI)->IsDestroy())
		{
			iterUI = m_WidgetList.erase(iterUI);
			iterUIEnd = m_WidgetList.end();
			continue;
		}

		++iterUI;
	}

	iterUI = m_WidgetList.begin();
	iterUIEnd = m_WidgetList.end();

	for (; iterUI != iterUIEnd; ++iterUI)
	{
		if (!(*iterUI)->IsEnable())
			continue;

		(*iterUI)->Render(hDC, DeltaTime);
	}

	m_pSceneCollision->Render(hDC, DeltaTime);

	// 마우스가 있을 경우 마우스를 그린다.
	//if (m_pMouseWidget)
	//	m_pMouseWidget->Render(hDC, DeltaTime);
#ifdef _DEBUG
	char Camera[MAX_PATH];
	Vector2 CameraPos = GetMainCamera()->GetPos();
	sprintf_s(Camera, "Camera : %d, %d", (int)CameraPos.x, (int)CameraPos.y);
	TextOutA(hDC, 20, 150, Camera, (int)strlen(Camera));

	char CurTimeMode[MAX_PATH];
	sprintf_s(CurTimeMode, "CurTimeMode : %d", m_pSceneMode->GetTimeMode());
	TextOutA(hDC, 20, 170, CurTimeMode, (int)strlen(CurTimeMode));
#endif
}

CGameObject* CScene::FindObject(const std::string& Name)
{
	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
			return *iter;
	}

	return nullptr;
}

CUIWidget* CScene::FindWidget(const std::string& Name)
{
	auto	iter = m_WidgetList.begin();
	auto	iterEnd = m_WidgetList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
			return *iter;
	}

	return nullptr;
}

int CScene::SortY(const void* pSrc, const void* pDest)
{
	CGameObject* pSrcObj = *(CGameObject**)pSrc;
	CGameObject* pDestObj = *(CGameObject**)pDest;

	// Bottom값을 구한다.
	float	SrcY = pSrcObj->GetPos().y + (1.f - pSrcObj->GetPivot().y) * pSrcObj->GetSize().y;
	float	DestY = pDestObj->GetPos().y + (1.f - pDestObj->GetPivot().y) * pDestObj->GetSize().y;

	if (SrcY < DestY)
		return -1;

	else if (SrcY > DestY)
		return 1;

	return 0;
}

int CScene::SortZOrder(const void* pSrc, const void* pDest)
{
	CUIWidget* pSrcUI = *(CUIWidget**)pSrc;
	CUIWidget* pDestUI = *(CUIWidget**)pDest;

	// Bottom값을 구한다.
	int	SrcZ = pSrcUI->GetZOrder();
	int	DestZ = pDestUI->GetZOrder();

	if (SrcZ < DestZ)
		return -1;

	else if (SrcZ > DestZ)
		return 1;

	return 0;
}

CGameObject* CScene::CreateGameObjectNoType(const std::string& PrototypeName, const std::string& Name, const Vector2& vPos, const Vector2& vSize)
{
	auto	iter = m_mapPrototype.find(PrototypeName);

	if (iter == m_mapPrototype.end())
		return nullptr;

	CGameObject* pObj = iter->second->Clone();

	pObj->SetScene(this);
	pObj->SetName(Name);
	pObj->SetPos(vPos);
	pObj->SetSize(vSize);

	m_ObjList.push_back(pObj);

	return pObj;
}
