#pragma once

#include "../GameInfo.h"
#include "../Collision/ColliderLine.h"

struct HitResult
{
	Vector2 hitPoint;
	class CCollider* hitCollider; // 충돌된 콜라이더
	std::function<void(const HitResult& hitResult)>		Callback;

	HitResult() :
		Callback(nullptr),
		hitCollider(NULL)
	{
		hitPoint.x = -1.f;
		hitPoint.y = -1.f;
	}

	HitResult(const HitResult& hitResult)
	{
		hitPoint = hitResult.hitPoint;
		Callback = hitResult.Callback;
		hitCollider = hitResult.hitCollider;
	}
	
	~HitResult()
	{
	}
};

struct RaycastInfo
{
	class CGameObject* Owner;
	HitResult hitResult;
	class CColliderLine* Line; // 레이캐스트를 실행할 Line 콜라이더

	RaycastInfo(HitResult& _hitResult)	:
		Line(nullptr),
		Owner(nullptr),
		hitResult(_hitResult)
	{
	}
	~RaycastInfo()
	{
		SAFE_DELETE(Line);
	}
};

class CSceneCollision
{
	friend class CScene;

private:
	CSceneCollision();
	~CSceneCollision();

private:
	class CScene* m_pScene;
	std::vector<class CCollider*>	m_vecCollider;
	std::list<RaycastInfo*>			m_listRaycast;

public:
	void SetScene(class CScene* Scene)
	{
		m_pScene = Scene;
	}

	void AddCollider(class CCollider* pCollider);
	void CollisionMouse(float DeltaTime, std::vector<class CUIWidget*>& WidgetList);
	void Collision(float DeltaTime);
	void Render(HDC hDC, float DeltaTime);

public:
	void Raycast(HitResult hitResult, CGameObject* Owner, CollisionChannel Channel, const Vector2& StartPos, const Vector2& RayDir, float Distance);
};

