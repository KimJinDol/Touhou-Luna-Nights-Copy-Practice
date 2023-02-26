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
    // �����ϸ鼭 ����
    CGoblinAI* pWolf = Cast<CGoblinAI*, CAICharacter*>(pOwner);

    pWolf->AttackAnim();
}
 