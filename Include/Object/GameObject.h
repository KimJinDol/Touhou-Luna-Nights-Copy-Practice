#pragma once

#include "../Ref.h"
#include "../Resource/Texture.h"
#include "../Resource/Animation.h"
#include "../AI/AIController.h"
#include "../Collision/ColliderLine.h"
#include "../Collision/ColliderRect.h"

class CGameObject	:
	public CRef
{
	friend class CScene;

protected:
	CGameObject();
	CGameObject(const CGameObject& obj);
	virtual ~CGameObject();

protected:
	class CScene* m_pScene;

	class CAnimation* m_pAnimation;
	
	std::string	m_Name;
	Vector2		m_vPos;
	Vector2		m_vSize;
	Vector2		m_vPivot;	
	bool		m_bIsRenderInfluendCameraPos;	// 렌더 위치가 카메라 위치에 대한 영향을 받는지 (고정 위치인지)
	float		m_CameraInfluend;				// 카메라 위치에 영향을 받는 정도
	Vector2		m_vRenderPos;
	bool		m_bIsTextureImageStartPos;
	bool		m_bIsRender;
	Vector2		m_vTextureImageStartPos;
	Vector2		m_vTexturePixelSize;
	bool		m_Start;
	bool		m_Culling;
	std::list<class CCollider*>	m_ColliderList;
	std::list<class CUIWidgetWorld*>	m_WorldWidgetList;
	float		m_fLifeTime;
	CSharedPtr<CTexture>	m_Texture;
	class CSpawnPoint* m_pSpawnPoint;

	CSharedPtr<class CColliderRect> RectComponent;


public:
	void SetCameraInfluend(float Influend)
	{
		m_CameraInfluend = Influend;
	}

	void SetSpawnPoint(class CSpawnPoint* pPoint)
	{
		m_pSpawnPoint = pPoint;
	}

	void SetTextureImagePosAndSize(const Vector2& ImageStartPos, const Vector2& PixelSize)
	{
		m_bIsTextureImageStartPos = true;
		m_vTextureImageStartPos = ImageStartPos;
		m_vTexturePixelSize = PixelSize;
	}

	void SetIsInfluenceCameraPos(bool value)
	{
		m_bIsRenderInfluendCameraPos = value;
	}

	bool IsCulling()	const
	{
		return m_Culling;
	}

	void SetScene(class CScene* pScene);


public:
	Vector2 GetCenterPos()	const
	{
		return m_vPos + m_vSize * (Vector2(0.5f, 0.5f) - m_vPivot);
	}

	bool IsStart()	const
	{
		return m_Start;
	}

	Vector2 GetPos()	const
	{
		return m_vPos;
	}

	Vector2 GetSize()	const
	{
		return m_vSize;
	}

	Vector2 GetPivot()	const
	{
		return m_vPivot;
	}

	const std::string& GetName()	const
	{
		return m_Name;
	}

public:
	void SetEnableRender(bool Value)
	{
		m_bIsRender = Value;
	}

	void SetLifeTime(float Time)
	{
		m_fLifeTime = Time;
	}

	void SetPos(const Vector2& vPos)
	{
		m_vPos = vPos;
	}

	void SetPos(float x, float y)
	{
		m_vPos.x = x;
		m_vPos.y = y;
	}

	void SetSize(const Vector2& vSize)
	{
		m_vSize = vSize;
	}

	void SetSize(float x, float y)
	{
		m_vSize.x = x;
		m_vSize.y = y;
	}

	void SetPivot(const Vector2& vPivot)
	{
		m_vPivot = vPivot;
	}

	void SetPivot(float x, float y)
	{
		m_vPivot.x = x;
		m_vPivot.y = y;
	}

	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}

public:
	void SetTexture(const std::string& Name);
	void SetTexture(const std::string& Name, const TCHAR* pFileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetTexture(const std::string& Name, const TCHAR* pFileName, const Vector2& ImageStartPos, const Vector2& PixelSize,
		const std::string& PathName = TEXTURE_PATH);
	void SetTexture(const std::string& Name, const Vector2& ImageStartPos, const Vector2& PixelSize,
		const std::string& PathName = TEXTURE_PATH);
	void SetColorKey(unsigned char r, unsigned char g, unsigned char b,
		int Index = 0);

public:
	// Init 함수는 이 객체가 생성됨과 동시에 호출되는 초기화 함수이다.
	virtual bool Init() = 0;
	// Start 함수는 장면에 배치가 되고 그 후에 이 객체가
	// 동작되기 직전에 한번 호출되는 함수이다.
	virtual void Start();
	// 이 객체가 장면에 배치가 된 후에 시작되기 전에 입력등록을
	// 위해 제공되는 함수이다.
	virtual void SetInput();
	virtual void Update(float DeltaTime);
	virtual void Collision();
	virtual void PrevRender();
	virtual void Render(HDC hDC, float DeltaTime);
	virtual CGameObject* Clone() const = 0;
	virtual float TakeDamage(float Damage, CGameObject* DamageObject);

public:
	template <typename T>
	T* AddCollider(const std::string& Name)
	{
		T* Collider = new T;

		Collider->SetName(Name);
		Collider->m_Owner = this;
		Collider->m_pScene = m_pScene;
		Collider->m_vOffsetPos = m_vSize * (Vector2(0.5f, 0.5f) - m_vPivot);
			
		if (!Collider->Init())
		{
			SAFE_RELEASE(Collider);
			return nullptr;
		}

		m_ColliderList.push_back(Collider);

		return Collider;
	}

public:
	class CAnimation* CreateAnimation();
	bool AddAnimation(const std::string& Name, AnimationType Type, bool Loop,
		class CTexture* pTexture, const std::vector<AnimationFrameData>& vecFrame,
		float PlayTime = 1.f);
	bool AddAnimation(const std::string& Name, AnimationType Type, bool Loop,
		class CTexture* pTexture, float PlayTime = 1.f);
	bool AddAnimation(const std::string& Name, AnimationType Type, bool Loop,
		const std::string& TextureName, float PlayTime = 1.f);
	bool AddAnimation(const std::string& Name, AnimationType Type, bool Loop,
		const std::string& TextureName, const TCHAR* pTexFileName,
		float PlayTime = 1.f);
	bool AddAnimationFrameData(const std::string& Name,
		const Vector2& ImageStartPos, const Vector2& Size);
	bool AddAnimationFrameData(const std::string& Name,
		const Vector2& ImageStartPos, const Vector2& Size,
		int Count);
	bool AddAnimationFrameData(const std::string& Name, unsigned int x, unsigned int y,     // 아틀라스 기준.
		unsigned int Count, const Vector2 FramePixelSize = Vector2(512.f, 512.f), const Vector2 Pivot = Vector2(0.5f, 1.f));
	void SetPlayScale(const std::string& Name, float Scale);
	void SetCurrentAnimation(const std::string& Name);
	void ChangeAnimation(const std::string& Name);
	void SetAnimationReverse(const std::string& Name, bool bReverse);
	void SetNextAnimation(const std::string& SrcName, const std::string& DestName); // Loop돌지 않는 애니메이션일 경우, 다음으로 이어질 애니메이션을 세팅해주는 함수.

public:
	template <typename T>
	void SetAnimationEndFunction(const std::string& Name,
		T* pObj, void (T::* pFunc)())
	{
		if (!m_pAnimation)
			return;

		m_pAnimation->SetEndFunction<T>(Name, pObj, pFunc);
	}

	template <typename T>
	void SetAnimationNotifyFunction(const std::string& Name, int Frame,
		T* pObj, void (T::* pFunc)())
	{
		if (!m_pAnimation)
			return;

		m_pAnimation->SetNotifyFunction<T>(Name, Frame, pObj, pFunc);
	}

	template <typename T>
	T* CreateWorldWidget()
	{
		T* pWidget = new T;

		pWidget->SetOwner(this);
		pWidget->SetScene(m_pScene);

		if (!pWidget->Init())
		{
			SAFE_DELETE(pWidget);
			return nullptr;
		}

		m_WorldWidgetList.push_back(pWidget);

		return pWidget;
	}
};

