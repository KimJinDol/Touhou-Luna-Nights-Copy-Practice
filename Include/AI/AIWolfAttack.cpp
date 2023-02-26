#include "AIWolfAttack.h"
#include "../Object/WolfAI.h"

CAIWolfAttack::CAIWolfAttack()
{
}

CAIWolfAttack::~CAIWolfAttack()
{
}

void CAIWolfAttack::Run(CAIController* pController, CAICharacter* pOwner, CScene* pScene, float DeltaTime)
{
    // 점프하면서 어택
    CWolfAI* pWolf = Cast<CWolfAI*, CAICharacter*>(pOwner); 

    pWolf->AttackAnim();
}
