#pragma once
#include "GameObject.h"

class CEffect :
    public CGameObject
{
	friend class CScene;

protected:
	CEffect();
	CEffect(const CEffect& obj);
	virtual ~CEffect();

protected:
	CSharedPtr<CGameObject> m_pOwner;			// 이펙트 생성 객체
	CSharedPtr<class CCharacter>	m_pOwnerCharacter;
	
	bool		m_bOwnerIsCharacter;
	bool		m_bUseLocalSpace;	// 이펙트가 owner를 따라갈지 정한다.
	bool		m_bIsLoop;
	Vector2		m_vOffset;

public:
	void SetUseLocalSpace(bool value)
	{
		m_bUseLocalSpace = value;
	}

	void SetLoop(bool value)
	{
		m_bIsLoop = value;
	}

	void SetOwner(CGameObject* pOwner);

	void SetOffset(Vector2 vOffset)
	{
		m_vOffset = vOffset;
	}

	void SetOffset(float x, float y)
	{
		m_vOffset.x = x;
		m_vOffset.y = y;
	}

public:
	bool AddAnimation(const std::string& Name, AnimationType Type,
		class CTexture* pTexture, const std::vector<AnimationFrameData>& vecFrame,
		float PlayTime = 1.f);
	bool AddAnimation(const std::string& Name, AnimationType Type,
		class CTexture* pTexture, float PlayTime = 1.f);
	bool AddAnimation(const std::string& Name, AnimationType Type,
		const std::string& TextureName, float PlayTime = 1.f);
	bool AddAnimation(const std::string& Name, AnimationType Type,
		const std::string& TextureName, const TCHAR* pTexFileName,
		float PlayTime = 1.f);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual CEffect* Clone()	const;

public:
	void SetAutoDestroy();
	void EffectEnd();
};

