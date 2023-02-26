
#include "UIControl.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "UIWidget.h"
#include "MouseWidget.h"

CUIControl::CUIControl()    :
    m_pScene(nullptr),
    m_pOwner(nullptr),
    m_pAnimation(nullptr),
    m_Start(false),
	m_bCollisionMouse(false),
    m_ZOrder(0),
	m_Shape(UIShape::Rect)
{
}

CUIControl::~CUIControl()
{
	//if (m_bCollisionMouse)
	//{
	//	if (m_pScene)
	//	{
	//		CMouseWidget* pMouseWidget = (CMouseWidget*)m_pScene->GetMouseWidget();

	//		pMouseWidget->SetCollisionControl(nullptr);
	//	}
	//}

	SAFE_DELETE(m_pAnimation);
}

void CUIControl::SetTexture(const std::string& Name)
{
    m_Texture = m_pScene->GetResourceManager()->FindTexture(Name);
}

void CUIControl::SetTexture(const std::string& Name, const TCHAR* pFileName, const std::string& PathName)
{
    m_pScene->GetResourceManager()->LoadTexture(Name, pFileName, PathName);
	m_Texture = m_pScene->GetResourceManager()->FindTexture(Name);
}

void CUIControl::SetTexture(const std::string& Name, const Vector2& ImageStartPos, const Vector2& PixelSize, const std::string& PathName)
{
	m_bIsTextureImageStartPos = true;
	m_vTextureImageStartPos = ImageStartPos;
	m_vTexturePixelSize = PixelSize;
	m_vSize = PixelSize;
	m_Texture = m_pScene->GetResourceManager()->FindTexture(Name);
}

void CUIControl::SetColorKey(unsigned char r, unsigned char g, unsigned char b, int Index)
{
    if (m_Texture)
        m_Texture->SetColorKey(r, g, b, Index);
}

bool CUIControl::AddAnimation(const std::string& Name, AnimationType Type,
	bool Loop, CTexture* pTexture,
	const std::vector<AnimationFrameData>& vecFrame, float PlayTime)
{
	if (!m_pAnimation)
	{
		m_pAnimation = new CAnimationUI;

		m_pAnimation->m_pScene = m_pScene;
		m_pAnimation->m_pOwner = this;
	}

	return m_pAnimation->AddAnimation(Name, Type, Loop, pTexture,
		vecFrame, PlayTime);
}

bool CUIControl::AddAnimation(const std::string& Name, AnimationType Type,
	bool Loop, CTexture* pTexture, float PlayTime)
{
	if (!m_pAnimation)
	{
		m_pAnimation = new CAnimationUI;

		m_pAnimation->m_pScene = m_pScene;
		m_pAnimation->m_pOwner = this;
	}

	return m_pAnimation->AddAnimation(Name, Type, Loop, pTexture, PlayTime);
}

bool CUIControl::AddAnimation(const std::string& Name, AnimationType Type,
	bool Loop, const std::string& TextureName, float PlayTime)
{
	if (!m_pAnimation)
	{
		m_pAnimation = new CAnimationUI;

		m_pAnimation->m_pScene = m_pScene;
		m_pAnimation->m_pOwner = this;
	}

	return m_pAnimation->AddAnimation(Name, Type, Loop, TextureName, PlayTime);
}

bool CUIControl::AddAnimation(const std::string& Name, AnimationType Type,
	bool Loop, const std::string& TextureName,
	const TCHAR* pTexFileName, float PlayTime)
{
	if (!m_pAnimation)
	{
		m_pAnimation = new CAnimationUI;

		m_pAnimation->m_pScene = m_pScene;
		m_pAnimation->m_pOwner = this;
	}

	return m_pAnimation->AddAnimation(Name, Type, Loop, TextureName, pTexFileName, PlayTime);
}

bool CUIControl::AddAnimation(const std::string& Name, AnimationType Type, bool Loop, 
	const std::string& TextureName, const std::vector<std::wstring>& vecFileName, float PlayTime)
{
	if (!m_pAnimation)
	{
		m_pAnimation = new CAnimationUI;

		m_pAnimation->m_pScene = m_pScene;
		m_pAnimation->m_pOwner = this;
	}

	return m_pAnimation->AddAnimation(Name, Type, Loop, TextureName, vecFileName, PlayTime);
}

bool CUIControl::AddAnimationFrameData(const std::string& Name, unsigned int x, unsigned int y, unsigned int Count, const Vector2& FramePixelSize, const Vector2& Pivot)
{
	if (!m_pAnimation)
		return false;

	return m_pAnimation->AddAnimationFrameData(Name, x, y, Count, FramePixelSize, Pivot);
}


bool CUIControl::AddAnimationFrameData(const std::string& Name,
	const Vector2& ImageStartPos, const Vector2& Size)
{
	if (!m_pAnimation)
		return false;

	return m_pAnimation->AddAnimationFrameData(Name, ImageStartPos, Size);
}

bool CUIControl::AddAnimationFrameData(const std::string& Name,
	const Vector2& ImageStartPos, const Vector2& Size, int Count)
{
	if (!m_pAnimation)
		return false;

	return m_pAnimation->AddAnimationFrameData(Name, ImageStartPos, Size, Count);
}

void CUIControl::SetPlayScale(const std::string& Name, float Scale)
{
	if (!m_pAnimation)
		return;

	m_pAnimation->SetPlayScale(Name, Scale);
}

void CUIControl::SetCurrentAnimation(const std::string& Name)
{
	if (!m_pAnimation)
		return;

	m_pAnimation->SetCurrentAnimation(Name);
}

void CUIControl::ChangeAnimation(const std::string& Name)
{
	if (!m_pAnimation)
		return;

	m_pAnimation->ChangeAnimation(Name);
}

void CUIControl::SetAnimationReverse(const std::string& Name, bool bReverse)
{
	if (!m_pAnimation)
		return;

	m_pAnimation->SetAnimationReverse(Name, bReverse);
}

void CUIControl::InitializeAnimationFrame()
{
	if (!m_pAnimation) return;
	
	m_pAnimation->m_pCurrentAnim->Frame = 0;
	m_pAnimation->m_pCurrentAnim->bEndFunction = false;
}

bool CUIControl::Init()
{
	m_vPivot = Vector2(0.5f, 0.5f);
	return true;
}

void CUIControl::Update(float DeltaTime)
{
	if (m_pAnimation)
		m_pAnimation->Update(DeltaTime);
}

bool CUIControl::CollisionMouse(const Vector2& MousePos)
{
	switch (m_Shape)
	{
	case UIShape::Rect:
	{
		Vector2 vLT = m_vPos - m_vPivot * m_vSize;

		if (vLT.x <= MousePos.x && MousePos.x <= vLT.x + m_vSize.x &&
			vLT.y <= MousePos.y && MousePos.y <= vLT.y + m_vSize.y)
		{
			m_bCollisionMouse = true;
			return true;
		}

		m_bCollisionMouse = false;
	}
		break;
	case UIShape::Sphere:
	{
		Vector2	Center = m_vPos - (Vector2(0.5f, 0.5f) - m_vPivot) * m_vSize;
		float	Radius = sqrtf(m_vSize.x * m_vSize.x + m_vSize.y * m_vSize.y) / 2.f;

		// 마우스 위치와 중점과의 거리를 구한다.
		Vector2 vDist = Center - MousePos;
		float   Dist = sqrtf(vDist.x * vDist.x + vDist.y * vDist.y);

		m_bCollisionMouse = Dist <= Radius;

		return m_bCollisionMouse;
	}
		break;
	}

	return false;
}

void CUIControl::Collision()
{
}

void CUIControl::PrevRender()
{
}

void CUIControl::Render(HDC hDC, float DeltaTime)
{
	if (m_bIsTextureImageStartPos)
		return Render(hDC, DeltaTime, m_vTextureImageStartPos.x, m_vTextureImageStartPos.y);

	
	Vector2	vRenderPos = m_vPos - m_vPivot * m_vSize;

	// 애니메이션이 있을 경우와 없을 경우로 나누어준다.
	if (m_pAnimation)
	{
		const AnimationFrameData& FrameData =
			m_pAnimation->m_pCurrentAnim->vecFrameData[m_pAnimation->m_pCurrentAnim->Frame];
		
		vRenderPos -= ((FrameData.Size - m_vSize) * Vector2(FrameData.Pivot.x, FrameData.Pivot.y));

		if (m_pAnimation->m_pCurrentAnim->AnimType == AnimationType::Sprite)
		{
			m_pAnimation->m_pCurrentAnim->pTexture->Render(hDC, vRenderPos,
				FrameData.ImageStartPos, FrameData.Size);
		}

		else
		{
			m_pAnimation->m_pCurrentAnim->pTexture->Render(hDC, vRenderPos,
				FrameData.ImageStartPos, FrameData.Size,
				m_pAnimation->m_pCurrentAnim->Frame);
		}
	}

	else
	{
		// Texture가 있을 경우 Texture로 출력을 하고 Texture가 없을 경우 Rect로 출력을
		// 해주도록 한다.
		if (m_Texture)
		{
			m_Texture->Render(hDC, vRenderPos, Vector2(0.f, 0.f), m_vSize);
		}

		else
		{
			Rectangle(hDC, (int)vRenderPos.x, (int)vRenderPos.y, (int)(vRenderPos.x + m_vSize.x),
				(int)(vRenderPos.y + m_vSize.y));
		}
	}
}


void CUIControl::Render(HDC hDC, float DeltaTime, const Vector2& vPos)
{
	if (m_bIsTextureImageStartPos)
		return Render(hDC, DeltaTime, vPos, m_vTextureImageStartPos.x, m_vTextureImageStartPos.y);
	
	Vector2	vRenderPos = (vPos + m_vPos) - m_vPivot * m_vSize;

	// 애니메이션이 있을 경우와 없을 경우로 나누어준다.
	if (m_pAnimation)
	{
		const AnimationFrameData& FrameData =
			m_pAnimation->m_pCurrentAnim->vecFrameData[m_pAnimation->m_pCurrentAnim->Frame];

		if (m_pAnimation->m_pCurrentAnim->AnimType == AnimationType::Sprite)
		{
			m_pAnimation->m_pCurrentAnim->pTexture->Render(hDC, vRenderPos,
				FrameData.ImageStartPos, FrameData.Size);
		}

		else
		{
			m_pAnimation->m_pCurrentAnim->pTexture->Render(hDC, vRenderPos,
				FrameData.ImageStartPos, FrameData.Size,
				m_pAnimation->m_pCurrentAnim->Frame);
		}
	}

	else
	{
		// Texture가 있을 경우 Texture로 출력을 하고 Texture가 없을 경우 Rect로 출력을
		// 해주도록 한다.
		if (m_Texture)
		{
			m_Texture->Render(hDC, vRenderPos, Vector2(0.f, 0.f), m_vSize);
		}

		else
		{
			Rectangle(hDC, (int)vRenderPos.x, (int)vRenderPos.y, (int)(vRenderPos.x + m_vSize.x),
				(int)(vRenderPos.y + m_vSize.y));
		}
	}
}


void CUIControl::Render(HDC hDC, float DeltaTime, float ImageStartPosX, float ImageStartPosY)
{
	Vector2	vRenderPos = m_vPos - m_vPivot * m_vSize;

	if (m_Texture)
		m_Texture->Render(hDC, vRenderPos, Vector2(ImageStartPosX, ImageStartPosY), m_vSize);

	else
		Rectangle(hDC, (int)vRenderPos.x, (int)vRenderPos.y, (int)(vRenderPos.x + m_vSize.x),
			(int)(vRenderPos.y + m_vSize.y));
}

void CUIControl::Render(HDC hDC, float DeltaTime, const Vector2& vPos, float ImageStartPosX, float ImageStartPosY)
{
	Vector2	vRenderPos = (vPos + m_vPos) - m_vPivot * m_vSize;

	if (m_Texture)
		m_Texture->Render(hDC, vRenderPos, Vector2(ImageStartPosX, ImageStartPosY), m_vSize);

	else
		Rectangle(hDC, (int)vRenderPos.x, (int)vRenderPos.y, (int)(vRenderPos.x + m_vSize.x),
			(int)(vRenderPos.y + m_vSize.y));
}


void CUIControl::CollisionEnd()
{
}

void CUIControl::RenderRatio(HDC hDC, float DeltaTime, float Ratio)
{
	Vector2	vRenderPos = m_vPos - m_vPivot * m_vSize;

	// 애니메이션이 있을 경우와 없을 경우로 나누어준다.
	if (m_pAnimation)
	{
		const AnimationFrameData& FrameData =
			m_pAnimation->m_pCurrentAnim->vecFrameData[m_pAnimation->m_pCurrentAnim->Frame];

		vRenderPos -= ((FrameData.Size - m_vSize) * Vector2(FrameData.Pivot.x, FrameData.Pivot.y));

		Vector2	Size = FrameData.Size;
		Size.x *= Ratio;

		if (m_pAnimation->m_pCurrentAnim->AnimType == AnimationType::Sprite)
		{
			m_pAnimation->m_pCurrentAnim->pTexture->Render(hDC, vRenderPos,
				FrameData.ImageStartPos, Size);
		}

		else
		{
			m_pAnimation->m_pCurrentAnim->pTexture->Render(hDC, vRenderPos,
				FrameData.ImageStartPos, Size,
				m_pAnimation->m_pCurrentAnim->Frame);
		}
	}

	else
	{
		Vector2	Size = m_vSize;
		Size.x *= Ratio;

		// Texture가 있을 경우 Texture로 출력을 하고 Texture가 없을 경우 Rect로 출력을
		// 해주도록 한다.
		if (m_Texture)
		{
			m_Texture->Render(hDC, vRenderPos, Vector2(0.f, 0.f), Size);
		}

		else
		{
			Rectangle(hDC, (int)vRenderPos.x, (int)vRenderPos.y, (int)(vRenderPos.x + Size.x),
				(int)(vRenderPos.y + Size.y));
		}
	}
}

void CUIControl::RenderRatio(HDC hDC, float DeltaTime, const Vector2& vPos, float Ratio)
{
	Vector2	vRenderPos = vPos - m_vPivot * m_vSize;

	// 애니메이션이 있을 경우와 없을 경우로 나누어준다.
	if (m_pAnimation)
	{
		const AnimationFrameData& FrameData =
			m_pAnimation->m_pCurrentAnim->vecFrameData[m_pAnimation->m_pCurrentAnim->Frame];

		Vector2	Size = FrameData.Size;
		Size.x *= Ratio;

		if (m_pAnimation->m_pCurrentAnim->AnimType == AnimationType::Sprite)
		{
			m_pAnimation->m_pCurrentAnim->pTexture->Render(hDC, vRenderPos,
				FrameData.ImageStartPos, Size);
		}

		else
		{
			m_pAnimation->m_pCurrentAnim->pTexture->Render(hDC, vRenderPos,
				FrameData.ImageStartPos, Size,
				m_pAnimation->m_pCurrentAnim->Frame);
		}
	}

	else
	{
		Vector2	Size = m_vSize;
		Size.x *= Ratio;

		// Texture가 있을 경우 Texture로 출력을 하고 Texture가 없을 경우 Rect로 출력을
		// 해주도록 한다.
		if (m_Texture)
		{
			m_Texture->Render(hDC, vRenderPos, Vector2(0.f, 0.f), Size);
		}

		else
		{
			Rectangle(hDC, (int)vRenderPos.x, (int)vRenderPos.y, (int)(vRenderPos.x + Size.x),
				(int)(vRenderPos.y + Size.y));
		}
	}
}
