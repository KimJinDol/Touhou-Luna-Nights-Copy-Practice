
#include "GameManager.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(2272);

    // TEXT : 유니코드일 경우 유니코드 문자열로 만들어주기 위해 문자열 앞에
    // L을 붙여주게 된다. 멀티바이트일 경우 문자열을 그대로 쓰게 된다.
    if (!CGameManager::GetInst()->Init(hInstance, TEXT("GameFramework"),
        TEXT("GameFramework"), 1280, 720, IDI_ICON1, IDI_ICON1))
    {
        CGameManager::DestroyInst();
        return 0;
    }

    // 초기화가 성공했다면 게임을 동작시킨다.
    int iRet = CGameManager::GetInst()->Run();

    CGameManager::DestroyInst();

    return iRet;
}
