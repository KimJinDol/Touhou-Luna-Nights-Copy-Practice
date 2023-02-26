#include "GameObject.h"
#include "../Collision/Collider.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneCollision.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Animation.h"
#include "../Scene/Camera.h"
#include "../UI/UIWidgetWorld.h"
#include "../Stage/Stage.h"
#include "../Stage/Tile.h"
#include "../Collision/Collision.h"
#include "../AI/AIBase.h"
#include "../AI/AIController.h"
#include "../Scene/SpawnPoint.h"

CGameObject::CGameObject()	:
	m_vPos{},
	m_vSize{},
	m_vPivot{},
	m_pScene(nullptr),
	m_Start(false), 
	m_pAnimation(nullptr),
	m_fLifeTime(0.f),
	m_Culling(false),
	m_bIsRenderInfluendCameraPos(true),
	m_vTextureImageStartPos{},
	m_vTexturePixelSize{},
	m_pSpawnPoint(nullptr),
	RectComponent(nullptr),
	m_bIsRender(true),
	m_CameraInfluend(0.f)
{
}

CGameObject::CGameObject(const CGameObject& obj)
{
	*this = obj;

	m_Start = false;

	m_iRefCount = 1;

	m_pAnimation = nullptr;

	if (obj.m_pAnimation)
	{
		m_pAnimation = obj.m_pAnimation->Clone();
		m_pAnimation->m_pOwner = this;
	}

	m_ColliderList.clear();

	auto	iter = obj.m_ColliderList.begin();
	auto	iterEnd = obj.m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		CCollider* pCollider = (*iter)->Clone();

		pCollider->m_Owner = this;

		m_ColliderList.push_back(pCollider);
	}

	m_WorldWidgetList.clear();
}

CGameObject::~CGameObject()
{
	if (m_pSpawnPoint)
		m_pSpawnPoint->SetSpawnObject(nullptr);

	SAFE_DELETE(m_pAnimation);

	RectComponent = nullptr;

	auto	iter = m_ColliderList.begin();
	auto	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->ClearCollisionList();
		SAFE_RELEASE((*iter));
	}

	auto	iter1 = m_WorldWidgetList.begin();
	auto	iter1End = m_WorldWidgetList.end();

	for (; iter1 != iter1End; ++iter1)
	{
		SAFE_RELEASE((*iter1));
	}
}

void CGameObject::SetScene(class CScene* pScene)
{
	m_pScene = pScene;
	
	if (m_pAnimation)
		m_pAnimation->m_pScene = m_pScene;

	auto	iter = m_ColliderList.begin();
	auto	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->m_pScene = m_pScene;
	}
}

void CGameObject::SetTexture(const std::string& Name)
{
	m_Texture = m_pScene->GetResourceManager()->FindTexture(Name);
}

void CGameObject::SetTexture(const std::string& Name, const TCHAR* pFileName, const std::string& PathName)
{
	m_pScene->GetResourceManager()->LoadTexture(Name, pFileName, PathName);
	m_Texture = m_pScene->GetResourceManager()->FindTexture(Name);
}

void CGameObject::SetTexture(const std::string& Name, const TCHAR* pFileName, const Vector2& ImageStartPos, const Vector2& PixelSize, const std::string& PathName)
{
	m_bIsTextureImageStartPos = true;
	m_vTextureImageStartPos = ImageStartPos;
	m_vTexturePixelSize = PixelSize;
	m_pScene->GetResourceManager()->LoadTexture(Name, pFileName, PathName);
	m_Texture = m_pScene->GetResourceManager()->FindTexture(Name);
}

void CGameObject::SetTexture(const std::string& Name, const Vector2& ImageStartPos, const Vector2& PixelSize, const std::string& PathName)
{
	m_bIsTextureImageStartPos = true;
	m_vTextureImageStartPos = ImageStartPos;
	m_vTexturePixelSize = PixelSize;
	m_Texture = m_pScene->GetResourceManager()->FindTexture(Name);
}

void CGameObject::SetColorKey(unsigned char r, unsigned char g, unsigned char b, int Index)
{
	if (m_Texture)
		m_Texture->SetColorKey(r, g, b, Index);
}

void CGameObject::Start()
{
}

void CGameObject::SetInput()
{
}

void CGameObject::Update(float DeltaTime)
{
	if (m_fLifeTime > 0.f)
	{
		m_fLifeTime -= DeltaTime;

		if (m_fLifeTime <= 0.f)
		{
			CRef::Destroy();
			return;
		}
	}


	auto	iter = m_ColliderList.begin();
	auto	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->IsEnable())
			(*iter)->Update(DeltaTime);
	}

	if (m_pAnimation)
		m_pAnimation->Update(DeltaTime);

	auto	iter1 = m_WorldWidgetList.begin();
	auto	iter1End = m_WorldWidgetList.end();

	for (; iter1 != iter1End; ++iter1)
	{
		if ((*iter1)->IsEnable())
			(*iter1)->Update(DeltaTime);
	}
}

void CGameObject::Collision()
{
	auto	iter = m_ColliderList.begin();
	auto	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		// 장면의 충돌관리자에 모든 충돌체들을 등록해준다.
		if ((*iter)->IsEnable())
			m_pScene->GetCollision()->AddCollider(*iter);
	}
}

void CGameObject::PrevRender()
{
	CCamera* pCamera = m_pScene->GetMainCamera();
	if (m_bIsRenderInfluendCameraPos)
	{
		Vector2 CameraInfluendVec = (m_vPos - m_pScene->GetMainCamera()->GetPos()) * m_CameraInfluend;
		m_vRenderPos = m_vPos - pCamera->GetPos() - m_vPivot * m_vSize - CameraInfluendVec;
	}
	else
	{
		m_vRenderPos = m_vPos - m_vPivot * m_vSize;
	}
	// 실제 출력 좌표가 화면에 들어오는지를 판단해야 한다.
	Vector2	vSize = m_vSize;

	if (m_pAnimation)
	{
		const AnimationFrameData& FrameData =
			m_pAnimation->m_pCurrentAnim->vecFrameData[m_pAnimation->m_pCurrentAnim->Frame];

		m_vRenderPos -= ((FrameData.Size - m_vSize) * Vector2(FrameData.Pivot.x, FrameData.Pivot.y));
		vSize = FrameData.Size;
	}

	//Vector2 RenderPos = m_vPos - pCamera->GetPos() - m_vPivot * m_vSize;
	m_Culling = false;

	// 출력되는 오른쪽이 0보다 작거나 같을 경우 화면에 안보이는 상태이므로 출력에서
	// 제외시켜준다.
	if (m_vRenderPos.x + vSize.x <= 0.f)
		m_Culling = true;

	else if (m_vRenderPos.x >= pCamera->GetResolution().x)
		m_Culling = true;

	else if (m_vRenderPos.y + vSize.y <= 0.f)
		m_Culling = true;

	else if (m_vRenderPos.y >= pCamera->GetResolution().y)
		m_Culling = true;

	auto	iter1 = m_WorldWidgetList.begin();
	auto	iter1End = m_WorldWidgetList.end();

	for (; iter1 != iter1End; ++iter1)
	{
		if ((*iter1)->IsEnable())
			(*iter1)->PrevRender();
	}
}

void CGameObject::Render(HDC hDC, float DeltaTime)
{
	if (m_bIsRender)
	{
		Vector2 CameraInfluendVec = (m_vPos - m_pScene->GetMainCamera()->GetPos()) * m_CameraInfluend;
		// 애니메이션이 있을 경우와 없을 경우로 나누어준다.
		if (m_pAnimation)
		{
			const AnimationFrameData& FrameData =
				m_pAnimation->m_pCurrentAnim->vecFrameData[m_pAnimation->m_pCurrentAnim->Frame];


			if (m_pAnimation->m_pCurrentAnim->AnimType == AnimationType::Sprite)
			{
				m_pAnimation->m_pCurrentAnim->pTexture->Render(hDC, m_vRenderPos,
					FrameData.ImageStartPos, FrameData.Size);
			}

			else
			{
				m_pAnimation->m_pCurrentAnim->pTexture->Render(hDC, m_vRenderPos,
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
				if (m_bIsTextureImageStartPos)
				{
					m_vRenderPos -= (m_vTexturePixelSize - m_vSize) * m_vPivot;
					m_Texture->Render(hDC, m_vRenderPos, m_vTextureImageStartPos, m_vTexturePixelSize);
				}
				else
					m_Texture->Render(hDC, m_vRenderPos, Vector2(0.f, 0.f), m_vSize);
			}

			else
			{
				Rectangle(hDC, (int)m_vRenderPos.x, (int)m_vRenderPos.y, (int)(m_vRenderPos.x + m_vSize.x),
					(int)(m_vRenderPos.y + m_vSize.y));
			}
		}

		auto	iter1 = m_WorldWidgetList.begin();
		auto	iter1End = m_WorldWidgetList.end();

		for (; iter1 != iter1End; ++iter1)
		{
			if ((*iter1)->IsEnable())
				(*iter1)->Render(hDC, DeltaTime);
		}
	}

	auto	iter = m_ColliderList.begin();
	auto	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->IsEnable())
			(*iter)->Render(hDC, DeltaTime);
	}
}

float CGameObject::TakeDamage(float Damage, CGameObject* DamageObject)
{
	// 기본 데미지 처리~
	return Damage;
}

CAnimation* CGameObject::CreateAnimation()
{
	SAFE_RELEASE(m_pAnimation);

	m_pAnimation = new CAnimation;

	m_pAnimation->m_pScene = m_pScene;
	m_pAnimation->m_pOwner = this;

	return m_pAnimation;
}

bool CGameObject::AddAnimation(const std::string& Name, AnimationType Type,
	bool Loop, CTexture* pTexture, 
	const std::vector<AnimationFrameData>& vecFrame, float PlayTime)
{
	if (!m_pAnimation)
	{
		m_pAnimation = new CAnimation;

		m_pAnimation->m_pScene = m_pScene;
		m_pAnimation->m_pOwner = this;
	}

	return m_pAnimation->AddAnimation(Name, Type, Loop, pTexture,
		vecFrame, PlayTime);
}

bool CGameObject::AddAnimation(const std::string& Name, AnimationType Type,
	bool Loop, CTexture* pTexture, float PlayTime)
{
	if (!m_pAnimation)
	{
		m_pAnimation = new CAnimation;

		m_pAnimation->m_pScene = m_pScene;
		m_pAnimation->m_pOwner = this;
	}

	return m_pAnimation->AddAnimation(Name, Type, Loop, pTexture, PlayTime);
}

bool CGameObject::AddAnimation(const std::string& Name, AnimationType Type,
	bool Loop, const std::string& TextureName, float PlayTime)
{
	if (!m_pAnimation)
	{
		m_pAnimation = new CAnimation;

		m_pAnimation->m_pScene = m_pScene;
		m_pAnimation->m_pOwner = this;
	}

	return m_pAnimation->AddAnimation(Name, Type, Loop, TextureName, PlayTime);
}

bool CGameObject::AddAnimation(const std::string& Name, AnimationType Type,
	bool Loop, const std::string& TextureName, 
	const TCHAR* pTexFileName, float PlayTime)
{
	if (!m_pAnimation)
	{
		m_pAnimation = new CAnimation;

		m_pAnimation->m_pScene = m_pScene;
		m_pAnimation->m_pOwner = this;
	}

	return m_pAnimation->AddAnimation(Name, Type, Loop, TextureName, pTexFileName, PlayTime);
}

bool CGameObject::AddAnimationFrameData(const std::string& Name, 
	const Vector2& ImageStartPos, const Vector2& Size)
{
	if (!m_pAnimation)
		return false;

	return m_pAnimation->AddAnimationFrameData(Name, ImageStartPos, Size);
}
 
bool CGameObject::AddAnimationFrameData(const std::string& Name, 
	const Vector2& ImageStartPos, const Vector2& Size, int Count)
{
	if (!m_pAnimation)
		return false;

	return m_pAnimation->AddAnimationFrameData(Name, ImageStartPos, Size, Count);
}

bool CGameObject::AddAnimationFrameData(const std::string& Name, unsigned int x, unsigned int y, unsigned int Count, const Vector2 FramePixelSize /*= Vector2(192.f, 192.f)*/,
	const Vector2 Pivot /*= Vector2(0.5f, 1.f)*/)
{
	if (!m_pAnimation)
		return false;

	return m_pAnimation->AddAnimationFrameData(Name, x, y, Count, FramePixelSize, Pivot);
}

void CGameObject::SetPlayScale(const std::string& Name, float Scale)
{
	if (!m_pAnimation)
		return;

	m_pAnimation->SetPlayScale(Name, Scale);
}

void CGameObject::SetCurrentAnimation(const std::string& Name)
{
	if (!m_pAnimation)
		return;

	m_pAnimation->SetCurrentAnimation(Name);
}

void CGameObject::ChangeAnimation(const std::string& Name)
{
	if (!m_pAnimation)
		return;

	m_pAnimation->ChangeAnimation(Name);
}

void CGameObject::SetAnimationReverse(const std::string& Name, bool bReverse)
{
	if (!m_pAnimation)
		return;

	m_pAnimation->SetAnimationReverse(Name, bReverse);
}

void CGameObject::SetNextAnimation(const std::string& SrcName, const std::string& DestName)
{
	if (!m_pAnimation)
		return;

	m_pAnimation->SetNextAnimation(SrcName, DestName);
}