
#include "AITrace.h"
#include "../Object/Player.h"
#include "../Resource/Animation.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneMode.h"
#include "../Object/AI.h"
#include "AIController.h"

CAITrace::CAITrace()
{
}

CAITrace::~CAITrace()
{
}

void CAITrace::Run(class CAIController* pController, CAICharacter* pOwner, CScene* pScene, float DeltaTime)
{
    CGameObject* pTarget = pController->GetGameObjectData("Target");

    if (!pTarget)
        return;

    Vector2 TargetPos = pTarget->GetCenterPos();

    Vector2 OwnerPos = pOwner->GetCenterPos();

    Vector2 MoveDir;
    MoveDir.x = TargetPos.x - OwnerPos.x;

    // Physics Simulate 가 비활성화라면 y좌표로도 움직이게.
    if (!pOwner->IsPhysicsSimulate())
        MoveDir.y = TargetPos.y - OwnerPos.y;

    MoveDir.Normalize();

    pOwner->SetMoveDir(MoveDir);
    pOwner->Move(MoveDir * pOwner->GetMoveSpeed() * DeltaTime);
}
