
#include "Collision.h"
#include "ColliderRect.h"
#include "ColliderSphere.h"
#include "ColliderPixel.h"
#include "ColliderLine.h"
#include "../Object/Character.h"

bool CCollision::CollisionRectToRect(class CColliderRect* pSrc, class CColliderRect* pDest)
{
    if (CollisionRectToRect(pSrc->GetInfo(), pDest->GetInfo()))
    {
        return true;
    }

    return false;
}

bool CCollision::CollisionRectToRect(const RectInfo& Src, const RectInfo& Dest)
{
    if (Src.Left > Dest.Right)
        return false;

    else if (Src.Right < Dest.Left)
        return false;

    else if (Src.Top > Dest.Bottom)
        return false;

    else if (Src.Bottom < Dest.Top)
        return false;

    return true;
}

bool CCollision::CollisionSphereToSphere(CColliderSphere* pSrc, CColliderSphere* pDest)
{
    if (CollisionSphereToSphere(pSrc->GetInfo(), pDest->GetInfo()))
    {
        return true;
    }

    return false;
}

bool CCollision::CollisionSphereToSphere(const SphereInfo& Src, const SphereInfo& Dest)
{
    // 중점 사이의 거리를 구한다.
    Vector2 vDist = Src.Center - Dest.Center;
    float   Dist = sqrtf(vDist.x * vDist.x + vDist.y * vDist.y);

    return Dist <= Src.Radius + Dest.Radius;
}

bool CCollision::CollisionRectToSphere(CColliderRect* pSrc, CColliderSphere* pDest)
{
    if (CollisionRectToSphere(pSrc->GetInfo(), pDest->GetInfo()))
    {
        return true;
    }

    return false;
}

bool CCollision::CollisionRectToSphere(const RectInfo& Src, const SphereInfo& Dest)
{
    // 원의 중점이 사각형의 좌, 우, 위, 아래 영역에 존재하는지 판단한다.
    if ((Src.Left <= Dest.Center.x && Dest.Center.x <= Src.Right) ||
        (Src.Top <= Dest.Center.y && Dest.Center.y <= Src.Bottom))
    {
        RectInfo    RC = Src;
        RC.Left -= Dest.Radius;
        RC.Right += Dest.Radius;
        RC.Top -= Dest.Radius;
        RC.Bottom += Dest.Radius;

        // 확장된 사각형 안에 원의 중점이 들어오면 충돌이다.
        if (RC.Left > Dest.Center.x)
            return false;

        else if (RC.Right < Dest.Center.x)
            return false;

        else if (RC.Top > Dest.Center.y)
            return false;

        else if (RC.Bottom < Dest.Center.y)
            return false;

        return true;
    }

    // 원의 중점이 사각형의 대각선 영역에 존재할 경우 사각형의 4개의 점중 1개라도 원 안에 들어온다면
    // 충돌이다.
    Vector2 RectPos[4];

    RectPos[0] = Vector2(Src.Left, Src.Top);
    RectPos[1] = Vector2(Src.Right, Src.Top);
    RectPos[2] = Vector2(Src.Left, Src.Bottom);
    RectPos[3] = Vector2(Src.Right, Src.Bottom);

    for (int i = 0; i < 4; ++i)
    {
        // 점과 원의 중점사이의 거리를 구한다.
        Vector2 Length = Dest.Center - RectPos[i];
        float   Dist = sqrtf(Length.x * Length.x + Length.y * Length.y);

        if (Dist <= Dest.Radius)
            return true;
    }

    return false;
}

bool CCollision::CollisionRectToPixel(CColliderRect* pSrc, CColliderPixel* pDest)
{
	if (CollisionRectToPixel(pSrc->GetInfo(), pDest))
	{
		return true;
	}

	return false;
}

bool CCollision::CollisionRectToPixel(const RectInfo& pSrc, class CColliderPixel* pDest)
{
    pDest->SetHitPoint(Vector2(-1.f, -1.f));

	int iStartX, iEndX;
	int iStartY, iEndY;

	// 사각형의 left가 0 이하면 검사하고자 하는 픽셀 이미지 밖에 있다는 뜻이기에 그 부분은 검사 할 필요가 없다.
	iStartX = pSrc.Left < 0 ? 0 : pSrc.Left;
	iEndX = pSrc.Right >= pDest->GetWidth() ? pDest->GetWidth() - 1 : pSrc.Right;

	iStartY = pSrc.Top < 0 ? 0 : pSrc.Top;
	iEndY = pSrc.Bottom >= pDest->GetHeight() ? pDest->GetHeight() - 1 : pSrc.Bottom;

	for (int i = iStartY; i <= iEndY; ++i)
	{
		for (int j = iStartX; j <= iEndX; ++j)
		{
			int idx = i * pDest->GetWidth() + j;
			const PIXEL& pixel = pDest->GetPixel()[idx];
			if (pixel.r == 255 && pixel.g == 0 && pixel.b == 255)
			{
                pDest->SetHitPoint(Vector2(j, i));

				return true;
			}
		}
	}


	return false;
}

bool CCollision::CollisionSphereToPixel(CColliderSphere* pSrc, CColliderPixel* pDest)
{
	if (CollisionSphereToPixel(pSrc->GetInfo(), pDest))
	{
		return true;
	}

	return false;
}

bool CCollision::CollisionSphereToPixel(const SphereInfo& pSrc, CColliderPixel* pDest)
{
    return false;
}

bool CCollision::CollisionLineToLine(Vector2& vInterect, const Vector2& vStart1, const Vector2& vEnd1, const Vector2& vStart2, const Vector2& vEnd2)
{
	vInterect = Vector2(0.f, 0.f);

	float   Under = (vEnd2.y - vStart2.y) * (vEnd1.x - vStart1.x) - (vEnd2.x - vStart2.x) * (vEnd1.y - vStart1.y);

	if (Under == 0.f) // 두 선은 평행하다.
		return false;

	float   t, t1;

	t = ((vEnd2.x - vStart2.x) * (vStart1.y - vStart2.y) - (vEnd2.y - vStart2.y) * (vStart1.x - vStart2.x));
	t1 = (vEnd1.x - vStart1.x) * (vStart1.y - vStart2.y) - (vEnd1.y - vStart1.y) * (vStart1.x - vStart2.x);

	if (t == 0.f && t1 == 0.f)
		return false;

	float   Result1, Result2;
	Result1 = t / Under;
	Result2 = t1 / Under;

	if (Result1 < 0.f || Result1 > 1.f || Result2 < 0.f || Result2 > 1.f)
		return false;

	vInterect.x = vStart2.x + (Result1 * (vEnd2.x - vStart2.x));
    vInterect.y = vStart2.y + (Result2 * (vEnd2.y - vStart2.y));

	return true;
}

bool CCollision::CollisionRectToLine(Vector2& vInterect, CColliderRect* pRect, class CColliderLine* pLine)
{
    RectInfo Info = pRect->GetInfo();

	Vector2 LeftStart = Vector2(Info.Left, Info.Top);
	Vector2 LeftEnd = Vector2(Info.Left, Info.Bottom);

	Vector2 Interect;
	if (CollisionLineToLine(Interect, LeftStart, LeftEnd, pLine->GetStartPoint(), pLine->GetEndPoint()))
	{
		//if (pRect->GetOwner())
		//{
		//	CCharacter* Character = Cast<CCharacter*, CGameObject*>(pRect->GetOwner());
		//	if (Character && Character->GetPos().y >= Interect.y)
		//	{
		//		Character->JumpEnd();
		//		Character->SetPos(Character->GetPos().x, Interect.y);
		//	}
		//}
		return true;
	}

	Vector2 RightStart = Vector2(Info.Right, Info.Top);
	Vector2 RightEnd = Vector2(Info.Right, Info.Bottom);

	if (CollisionLineToLine(vInterect, RightStart, RightEnd, pLine->GetStartPoint(), pLine->GetEndPoint()))
	{
		//if (pRect->GetOwner())
		//{
		//	CCharacter* Character = Cast<CCharacter*, CGameObject*>(pRect->GetOwner());
		//	if (Character && Character->GetPos().y >= vInterect.y)
		//	{
		//		Character->JumpEnd();
		//		Character->SetPos(Character->GetPos().x, vInterect.y);
		//	}
		//}

		return true;
	}

    Vector2 BottomStart = Vector2(Info.Left, Info.Bottom);
    Vector2 BottomEnd = Vector2(Info.Right, Info.Bottom);

    // 아래 비교
    if (CollisionLineToLine(vInterect, BottomStart, BottomEnd, pLine->GetStartPoint(), pLine->GetEndPoint()))
        return true;

	Vector2 TopStart = Vector2(Info.Left, Info.Top);
	Vector2 TopEnd = Vector2(Info.Right, Info.Top);

	if (CollisionLineToLine(vInterect, TopStart, TopEnd, pLine->GetStartPoint(), pLine->GetEndPoint()))
		return true;

    return false;
}
