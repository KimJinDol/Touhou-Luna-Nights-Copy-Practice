#include "AIGoblinAttack.h"
#include "../Object/GoblinAI.h"

CAIGoblinAttack::CAIGoblinAttack()
{
}

CAIGoblinAttack::~CAIGoblinAttack()
{
}

void CAIGoblinAttack::Run(CAIController* pController, CAICharacter* pOwner, CScene* pScene, float DeltaTime)
{
    // 점프하면서 어택
    CGoblinAI* pWolf = Cast<CGoblinAI*, CAICharacter*>(pOwner);

    pWolf->AttackAnim();
}
 