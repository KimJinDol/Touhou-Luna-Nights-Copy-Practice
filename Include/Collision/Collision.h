#pragma once

#include "../GameInfo.h"

class CCollision
{
public:
    static bool CollisionRectToRect(class CColliderRect* pSrc, class CColliderRect* pDest);
    static bool CollisionRectToRect(const RectInfo& Src, const RectInfo& Dest);

    static bool CollisionSphereToSphere(class CColliderSphere* pSrc, class CColliderSphere* pDest);
    static bool CollisionSphereToSphere(const SphereInfo& Src, const SphereInfo& Dest);

    static bool CollisionRectToSphere(class CColliderRect* pSrc, class CColliderSphere* pDest);
    static bool CollisionRectToSphere(const RectInfo& Src, const SphereInfo& Dest);

    static bool CollisionRectToPixel(class CColliderRect* pSrc, class CColliderPixel* pDest);
    static bool CollisionRectToPixel(const RectInfo& pSrc, class CColliderPixel* pDest);

	static bool CollisionSphereToPixel(class CColliderSphere* pSrc, class CColliderPixel* pDest);
	static bool CollisionSphereToPixel(const SphereInfo& pSrc, class CColliderPixel* pDest);

	static bool CollisionLineToLine(Vector2& vInterect, const Vector2& vStart1, const Vector2& vEnd1,
		const Vector2& vStart2, const Vector2& vEnd2);
    static bool CollisionRectToLine(Vector2& vInterect, class CColliderRect* pRect, class CColliderLine* pLine);
};

