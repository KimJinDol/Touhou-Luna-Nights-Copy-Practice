
#include "UIWidget.h"
#include "UIControl.h"
#include "../Input.h"
#include "MouseWidget.h"
#include "../Scene/Scene.h"
#include "../Resource/ResourceManager.h"

CUIWidget::CUIWidget()  :
    m_ZOrder(0),
    m_pScene(nullptr),
	m_vDefaultPos{},
	m_pOwner(nullptr)
{
}

CUIWidget::~CUIWidget()
{
    auto    iter = m_ControlList.begin();
    auto    iterEnd = m_ControlList.end();

    for (; iter != iterEnd; ++iter)
    {
        SAFE_RELEASE((*iter));
    }

    m_ControlList.clear();
}

void CUIWidget::SetScene(CScene* pScene)
{
	m_pScene = pScene;

	auto	iterUI = m_ControlList.begin();
	auto	iterUIEnd = m_ControlList.end();

	for (; iterUI != iterUIEnd; ++iterUI)
	{
		(*iterUI)->m_pScene = pScene;
	}
}

bool CUIWidget::Init()
{
    return true;
}

void CUIWidget::Update(float DeltaTime)
{
	auto	iterUI = m_ControlList.begin();
	auto	iterUIEnd = m_ControlList.end();

	for (; iterUI != iterUIEnd; )
	{
		if ((*iterUI)->IsDestroy())
		{
			iterUI = m_ControlList.erase(iterUI);
			iterUIEnd = m_ControlList.end();
			continue;
		}

		++iterUI;
	}

	iterUI = m_ControlList.begin();
	iterUIEnd = m_ControlList.end();

	for (; iterUI != iterUIEnd; ++iterUI)
	{
		if (!(*iterUI)->IsEnable())
			continue;

		(*iterUI)->Update(DeltaTime);
	}

	// Control을 정렬한다. 출력에 우선하여 정렬을 해야하므로 ZOrder 값이 큰게 나중에 출력되어야 한다.
	if (m_ControlList.size() > 0)
	{
		qsort(&m_ControlList[0], m_ControlList.size(), (size_t)sizeof(CUIControl*),
			CUIWidget::SortZOrder);
	}
}

bool CUIWidget::CollisionMouse()
{
	//auto	iterUI = m_ControlList.rbegin();
	//auto	iterUIEnd = m_ControlList.rend();

	//Vector2	MousePos = CInput::GetInst()->GetMousePos();

	//for (; iterUI != iterUIEnd; ++iterUI)
	//{
	//	if (!(*iterUI)->IsEnable())
	//		continue;

	//	if ((*iterUI)->CollisionMouse(MousePos))
	//	{
	//		CMouseWidget* pMouseWidget = (CMouseWidget*)m_pScene->GetMouseWidget();

	//		pMouseWidget->SetCollisionControl(*iterUI);

	//		return true;
	//	}
	//}

	return false;
}

void CUIWidget::Collision()
{
	auto	iterUI = m_ControlList.begin();
	auto	iterUIEnd = m_ControlList.end();

	for (; iterUI != iterUIEnd; )
	{
		if ((*iterUI)->IsDestroy())
		{
			iterUI = m_ControlList.erase(iterUI);
			iterUIEnd = m_ControlList.end();
			continue;
		}

		++iterUI;
	}

	iterUI = m_ControlList.begin();
	iterUIEnd = m_ControlList.end();

	for (; iterUI != iterUIEnd; ++iterUI)
	{
		if (!(*iterUI)->IsEnable())
			continue;

		(*iterUI)->Collision();
	}
}

void CUIWidget::PrevRender()
{
	auto	iterUI = m_ControlList.begin();
	auto	iterUIEnd = m_ControlList.end();

	for (; iterUI != iterUIEnd; )
	{
		if ((*iterUI)->IsDestroy())
		{
			iterUI = m_ControlList.erase(iterUI);
			iterUIEnd = m_ControlList.end();
			continue;
		}

		++iterUI;
	}

	iterUI = m_ControlList.begin();
	iterUIEnd = m_ControlList.end();

	for (; iterUI != iterUIEnd; ++iterUI)
	{
		if (!(*iterUI)->IsEnable())
			continue;

		(*iterUI)->PrevRender();
	}
}

void CUIWidget::Render(HDC hDC, float DeltaTime)
{
	auto	iterUI = m_ControlList.begin();
	auto	iterUIEnd = m_ControlList.end();

	for (; iterUI != iterUIEnd; )
	{
		if ((*iterUI)->IsDestroy())
		{
			iterUI = m_ControlList.erase(iterUI);
			iterUIEnd = m_ControlList.end();
			continue;
		}

		++iterUI;
	}

	iterUI = m_ControlList.begin();
	iterUIEnd = m_ControlList.end();

	for (; iterUI != iterUIEnd; ++iterUI)
	{
		if (!(*iterUI)->IsEnable())
			continue;

		(*iterUI)->Render(hDC, DeltaTime);
	}
}

void CUIWidget::Render(HDC hDC, float DeltaTime, const Vector2& vPos)
{
	auto	iterUI = m_ControlList.begin();
	auto	iterUIEnd = m_ControlList.end();

	for (; iterUI != iterUIEnd; )
	{
		if ((*iterUI)->IsDestroy())
		{
			iterUI = m_ControlList.erase(iterUI);
			iterUIEnd = m_ControlList.end();
			continue;
		}

		++iterUI;
	}

	iterUI = m_ControlList.begin();
	iterUIEnd = m_ControlList.end();

	for (; iterUI != iterUIEnd; ++iterUI)
	{
		if (!(*iterUI)->IsEnable())
			continue;

		(*iterUI)->Render(hDC, DeltaTime, vPos);
	}
}

CUIControl* CUIWidget::FindControl(const std::string& Name)
{
	auto	iter = m_ControlList.begin();
	auto	iterEnd = m_ControlList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
			return *iter;
	}

	return nullptr;
}

int CUIWidget::SortZOrder(const void* pSrc, const void* pDest)
{
	CUIControl* pSrcUI = *(CUIControl**)pSrc;
	CUIControl* pDestUI = *(CUIControl**)pDest;

	// Bottom값을 구한다.
	int	SrcZ = pSrcUI->GetZOrder();
	int	DestZ = pDestUI->GetZOrder();

	if (SrcZ < DestZ)
		return -1;

	else if (SrcZ > DestZ)
		return 1;

	return 0;
}
