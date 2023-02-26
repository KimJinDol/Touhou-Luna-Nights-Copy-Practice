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
	bool		m_bIsRenderInfluendCameraPos;	// ���� ��ġ�� ī�޶� ��ġ�� ���� ������ �޴��� (���� ��ġ����)
	float		m_CameraInfluend;				// ī�޶� ��ġ�� ������ �޴� ����
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
	// Init �Լ��� �� ��ü�� �����ʰ� ���ÿ� ȣ��Ǵ� �ʱ�ȭ �Լ��̴�.
	virtual bool Init() = 0;
	// Start �Լ��� ��鿡 ��ġ�� �ǰ� �� �Ŀ� �� ��ü��
	// ���۵Ǳ� ������ �ѹ� ȣ��Ǵ� �Լ��̴�.
	virtual void Start();
	// �� ��ü�� ��鿡 ��ġ�� �� �Ŀ� ���۵Ǳ� ���� �Էµ����
	// ���� �����Ǵ� �Լ��̴�.
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
	bool AddAnimationFrameData(const std::string& Name, unsigned int x, unsigned int y,     // ��Ʋ�� ����.
		unsigned int Count, const Vector2 FramePixelSize = Vector2(512.f, 512.f), const Vector2 Pivot = Vector2(0.5f, 1.f));
	void SetPlayScale(const std::string& Name, float Scale);
	void SetCurrentAnimation(const std::string& Name);
	void ChangeAnimation(const std::string& Name);
	void SetAnimationReverse(const std::string& Name, bool bReverse);
	void SetNextAnimation(const std::string& SrcName, const std::string& DestName); // Loop���� �ʴ� �ִϸ��̼��� ���, �������� �̾��� �ִϸ��̼��� �������ִ� �Լ�.

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

