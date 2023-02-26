
#include "GameManager.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(2272);

    // TEXT : �����ڵ��� ��� �����ڵ� ���ڿ��� ������ֱ� ���� ���ڿ� �տ�
    // L�� �ٿ��ְ� �ȴ�. ��Ƽ����Ʈ�� ��� ���ڿ��� �״�� ���� �ȴ�.
    if (!CGameManager::GetInst()->Init(hInstance, TEXT("GameFramework"),
        TEXT("GameFramework"), 1280, 720, IDI_ICON1, IDI_ICON1))
    {
        CGameManager::DestroyInst();
        return 0;
    }

    // �ʱ�ȭ�� �����ߴٸ� ������ ���۽�Ų��.
    int iRet = CGameManager::GetInst()->Run();

    CGameManager::DestroyInst();

    return iRet;
}
