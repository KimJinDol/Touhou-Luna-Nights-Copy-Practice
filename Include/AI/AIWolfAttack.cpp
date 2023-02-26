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
    // �����ϸ鼭 ����
    CWolfAI* pWolf = Cast<CWolfAI*, CAICharacter*>(pOwner); 

    pWolf->AttackAnim();
}
