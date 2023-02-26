#include "GameManager.h"
#include "Timer.h"
#include "Scene/SceneManager.h"
#include "GarbageCollector.h"
#include "Scene/StageSceneMode.h"
#include "Scene/StartScene.h"
#include "Scene/Stage00Mode.h"
#include "Input.h"
#include "Collision/CollisionManager.h"
#include "PathManager.h"
#include "Resource/ResourceManager.h"
#include "Scene/EditorSceneMode.h"
#include "Scene/Camera.h"
#include "Scene/Scene.h"
#include "UI/TextManager.h"

DEFINITION_SINGLE(CGameManager)
bool CGameManager::m_bLoop = true;

CGameManager::CGameManager()
{
}

CGameManager::~CGameManager()
{
    CSceneManager::DestroyInst();

    CAIManager::DestroyInst();

    CInput::DestroyInst();

    CCollisionManager::DestroyInst();

    CPathManager::DestroyInst();

    CTextManager::DestroyInst();

    CGarbageCollector::DestroyInst();

    CResourceManager::DestroyInst();

    SAFE_DELETE(m_pTimer);

    DeleteObject(m_hRedPen);
    DeleteObject(m_hGreenPen);
    DeleteObject(m_hPurplePen);
    DeleteObject(m_hRedBrush);
    DeleteObject(m_hGreenBrush);

    // ����ۿ� ���� �����Ǿ� �ִ� ������ �����Ѵ�.
    SelectObject(m_hBackDC, m_hPrevBackBmp);
    // ����ۿ� ��Ʈ�� ������ �����Ѵ�.
    DeleteObject(m_hBackBmp);

    // ����ۿ� DC�� �����Ѵ�.
    DeleteDC(m_hBackDC);

    ReleaseDC(m_hWnd, m_hDC);
}

ATOM CGameManager::MyRegisterClass(const TCHAR* pClassName, int IconID, int SmallIconID)
{
    // �������Ϳ� ����ϱ� ���� ������ ��Ƶδ� ����ü�̴�.
    // ������ Ŭ���� ����ü�� � ��Ÿ�Ϸ� â�� ������, ������ �޼����� ���� ��� � �Լ���
    // ȣ�����ٰ����� ���� �������� �� �ִ�.
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInst;
    // �������� �������� �����Ѵ�.
    wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IconID));
    // �� ���α׷��� ���۵ɶ� ���콺 Ŀ���� � ������� ���۽�ų���� �����Ѵ�.
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    // �Ʒ��� �޴��� ǥ���Ѵ�.
    wcex.lpszMenuName = nullptr;
    // �������Ϳ� ����� �̸��� �����Ѵ�. �����ڵ�� �����ؾ� �Ѵ�.
    wcex.lpszClassName = pClassName;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(SmallIconID));

    // ������ ������ ����Ѵ�.
    return RegisterClassExW(&wcex);
}

bool CGameManager::Create(const TCHAR* pClassName, const TCHAR* pTitle)
{
    // �������� ��ǥ�� : ���� ����� 0, 0 �̴�.���������� ������ X �÷������� �ǰ� �Ʒ��� ������ Y�÷���
    // ���� �ȴ�.
    // ��ũ�� ��ǥ�� : ����ȭ���� ��ǥ�踦 ���Ѵ�. ����ȭ�鿡�� ��, ����� 0, 0�̴�.
    // Ŭ���̾�Ʈ ��ǥ�� : ������ Ŭ���̾�Ʈ ������ ��, ����� 0, 0�� �ȴ�.
    // CreateWindow : �� �Լ��� ������ â�� �������ְ� ������ â�� ������ �ڵ��� ��ȯ���ִ� �Լ��̴�.
    // 1������ : �������Ϳ� ��ϵ� �̸��� �����Ѵ�.
    // 2������ : ������â Ÿ��Ʋ���� �̸��� �����Ѵ�.
    // 3������ : �� ������ â�� � ���·� �������� ������Ҹ� �������ش�.
    // 4������ : �� ������ â�� ��ũ����ǥ�迡�� ��� ���������� �����ϴ� X ���̴�.
    // 5������ : �� ������ â�� ��ũ����ǥ�迡�� ��� ���������� �����ϴ� Y ���̴�.
    // 6������ : ������ â�� ���� ũ�⸦ �����Ѵ�.
    // 7������ : ������ â�� ���� ũ�⸦ �����Ѵ�.
    m_hWnd = CreateWindowW(pClassName, pTitle, WS_OVERLAPPED |
        WS_CAPTION |
        WS_SYSMENU |
        WS_MINIMIZEBOX,
        0, 0, m_Resolution.Width, m_Resolution.Height,
        nullptr, nullptr, m_hInst, nullptr);

    // ������ �ڵ��� nullptr�̶�� ������ â�� ����� �����Ȱ��� �ƴ϶�� �ǹ��̴�.
    if (!m_hWnd)
        return false;
    
    // �����ϰ��� �ϴ� Ŭ���̾�Ʈ ������ ũ�⸦ Rect�� ������ش�.
    RECT    rc = { 0, 0, m_Resolution.Width, m_Resolution.Height };

    // Ŭ���̾�Ʈ â�� ũ�Ⱑ ���� ũ�Ⱑ �Ǳ� ���ؼ� �ʿ��� ��Ȯ�� ����
    // ũ�⸦ ���´�.
    // 2�����ڿ� ���ϴ� ������ ��Ÿ���� �־��ְ� 3�����ڿ� �޴��� �����
    // ������ ���θ� �־��ָ� ������ ������ ũ���� Ŭ���̾�Ʈ ������
    // ������� �� �ֵ��� ���������� ũ�⸦ �����ְ� �ȴ�.
    AdjustWindowRect(&rc, WS_OVERLAPPED |
        WS_CAPTION |
        WS_SYSMENU |
        WS_MINIMIZEBOX, false);

    // ������ ������ ���� �������� ũ��� ������ â�� ���Ӱ� �����Ѵ�.
    SetWindowPos(m_hWnd, nullptr, 0, 0, rc.right - rc.left,
        rc.bottom - rc.top, SWP_NOZORDER | SWP_NOMOVE);

    // ShowWindow : ȭ�鿡 �� ������ â�� �������� ������ �����Ѵ�.
    ShowWindow(m_hWnd, SW_SHOW);

    // Ŭ���̾�Ʈ ������ ������Ʈ ���ش�. �������شٴ� ���̴�.
    UpdateWindow(m_hWnd);

    return true;
}

bool CGameManager::Init(HINSTANCE hInst, const TCHAR* pTitle,
	const TCHAR* pClassName, int Width, int Height,
	int IconID, int SmallIconID)
{
	m_hInst = hInst;

	m_Resolution.Width = Width;
	m_Resolution.Height = Height;

    MyRegisterClass(pClassName, IconID, SmallIconID);

    if (!Create(pClassName, pTitle))
        return false;

    // �޸� ������ �ʱ�ȭ
    if (!CGarbageCollector::GetInst()->Init())
        return false;

    // ��� ������ �ʱ�ȭ
    if (!CPathManager::GetInst()->Init())
        return false;

    // Text ������ �ʱ�ȭ
    if (!CTextManager::GetInst()->Init())
        return false;

    // ���ҽ� ������ �ʱ�ȭ
    if (!CResourceManager::GetInst()->Init())
        return false;

    // �浹 ������ �ʱ�ȭ
    if (!CCollisionManager::GetInst()->Init())
        return false;

    // �Է� ������ �ʱ�ȭ
    if (!CInput::GetInst()->Init())
        return false;

    // �������ڸ� �ʱ�ȭ�Ѵ�.
    if (!CSceneManager::GetInst()->Init())
        return false;

    m_pTimer = new CTimer;

    m_pTimer->Init();


    /*
    HDC : ������ Ŭ���̾�Ʈ ������ ���𰡸� �׸����� �� ���
    DC�� �̿��ؼ� �׸��� �ȴ�.
    DC�� ������ â�� �׸��� �׸����� �ϴ� �����̴�.
    GetDC�Լ��� ���ڷ� �� ������ �ڵ鿡���� HDC�� ������ִ�
    �Լ��̴�.
    GetDC�� �̿��ؼ� ������ DC�� �ݵ�� ReleaseDC �Լ��� �̿��ؼ�
    �������־�� �Ѵ�.
    */
    m_hDC = GetDC(m_hWnd);

    // ȭ�� DC�� ������ ����� DC�� ������ش�.
    m_hBackDC = CreateCompatibleDC(m_hDC);

    // ȭ�� DC�� �̿��Ͽ� ��Ʈ�� ������ ������ش�. �� ������ ����۰� �ɰ��̴�.
    m_hBackBmp = CreateCompatibleBitmap(m_hDC, m_Resolution.Width, m_Resolution.Height);

    // ������ ������� ��Ʈ�� ������ ����ۿ� DC�� �������ش�.
    m_hPrevBackBmp = SelectObject(m_hBackDC, m_hBackBmp);

    /*
    HBRUSH : ������ Ŭ���̾�Ʈâ�� ���𰡸� �׸����� �Ҷ� ������
    ������ ���Ѵ�.
    HPEN : ����Ҷ� ���� ������ �����Ҷ� ����Ѵ�.
    r, g, b�� 0 ~ 255 �� ���� ������ �����Ͽ� ���� ���� ������.
    */
    m_hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
    m_hGreenBrush = CreateSolidBrush(RGB(0, 255, 0));
    m_hRedPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    m_hGreenPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    m_hPurplePen = CreatePen(PS_SOLID, 1, RGB(100, 0, 255));

    // MainSceneMode ����
#ifdef _DEBUG
    // MainSceneMode ����
    CStage00Mode* pSceneMode = CSceneManager::GetInst()->CreateSceneMode<CStage00Mode>();
    CSceneManager::GetInst()->GetCurrentScene()->GetMainCamera()->SetPos(0.f, 350.f);
#else
    CStageSceneMode* pSceneMode = CSceneManager::GetInst()->CreateSceneMode<CStageSceneMode>();
	CSceneManager::GetInst()->GetCurrentScene()->GetMainCamera()->SetPos(0.f, 0.f);
#endif
    

	return true;
}

int CGameManager::Run()
{
    // �޼��� ������ �����ش�.
    MSG msg;

    /*
    ������� �޼��� ����� �ü���̴�. �ü���� � �̺�Ʈ�� �߻��Ҷ����� �޼����� ����
    �ش� �����쿡�� �������ְ� �ȴ�.
    �̺�Ʈ�� ���콺 Ŭ��, �̵�, Ű���� �Է� ���� ��� ��Ī�ؼ� �̺�Ʈ�� �Ѵ�.
    ������ ��������� ���������� �޼��� ť�� ������ �ִ�.
    �ü���� �̺�Ʈ�� �߻��ϸ� �ش� �̺�Ʈ�� ������ �޼����� ����� �� �޼����� �������� �޼��� ť��
    �־��ְ� �ȴ�.
    GetMessage�� �ٷ� �޼��� ť�� �ִ� �޼����� �������� �Լ��̴�.
    ��, �޼��� ť�� ����ִٸ� �޼����� ���ö����� �� �Լ� �ȿ��� ����ϰ� �ִ°��̴�.
    */
    while (m_bLoop)
    {
        // PeekMessage�� �޼���ť �ȿ� �޼����� ���� ��� �ش� �޼�����
        // �������� true�� �Ǹ� �޼���ť �ȿ� �޼����� ���� ���
        // false�� �ȴ�.
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // �ش� �޼����� WM_KEYDOWN����, ������ Ű�� ����Ű������ ���� ��� WM_CHAR�޼�����
            // �޼��� ť�� �־��ִ� ����̴�.
            TranslateMessage(&msg);

            // �� �޼����� WndProc �� �������ش�.
            DispatchMessage(&msg);
        }

        // �޼����� ���� ���
        else
        {
            Logic();
        }
    }

    return (int)msg.wParam;
}

/*
������ ũ�� 4������ �ܰ踦 ��ġ�� �ȴ�.
1. �Է�
2. �Է¹��� ������ ���� ������Ʈ
3. ������Ʈ�� ������ ���� �浹ó��
4. ���
*/
void CGameManager::Logic()
{
    // Ÿ�̸Ӹ� �����Ѵ�.
    m_pTimer->Update();

    CResourceManager::GetInst()->Update();

    // �Է��� ó���Ѵ�.
    Input(m_pTimer->GetDeltaTime());

    // �����͸� ������Ʈ�Ѵ�.
    if (Update(m_pTimer->GetDeltaTime()))
    {
        CGarbageCollector::GetInst()->Update();
        return;
    }

    // �浹ó���� �����Ѵ�.
    if (Collision(m_pTimer->GetDeltaTime()))
    {
        CGarbageCollector::GetInst()->Update();
        return;
    }

    // ȭ�鿡 ����Ѵ�.
    Render(m_pTimer->GetDeltaTime());

    // �������� �� �޸𸮸� ��� �����ش�.
    CGarbageCollector::GetInst()->Update();

    char    TimeText[256] = {};
    wchar_t TimeTextLPCW[256] = {};
    size_t cn;

    // sprintf_s : ���ڿ��� ������ٶ� ����Ѵ�.
    // 1�����ڿ� �� ���� �迭�� 2, 3�� ������ �������� ä���ֵ��� �Ѵ�.
#ifdef _DEBUG
    sprintf_s(TimeText, "FPS : %.5f, DeltaTime : %.5f", m_pTimer->GetFPS(), m_pTimer->GetDeltaTime());
#else
    sprintf_s(TimeText, "Tougou Luna Nights");
#endif
    mbstowcs_s(&cn, TimeTextLPCW, TimeText, (int)strlen(TimeText));
    SetWindowText(m_hWnd, TimeTextLPCW);
    /*TextOutA(m_hBackDC, 700, 50, TimeText, (int)strlen(TimeText));*/

    // ����ۿ� �׷��� ������ ȭ�鿡 ��Ӻ��縦 ���ش�.
    // 1�� ���� : ����� ����
    // 2�� ���� : ����� ������ ���� x��ǥ
    // 3�� ���� : ����� ������ ���� y��ǥ
    // 4�� ���� : ����� ���ۿ� ����� ���� ũ��
    // 5�� ���� : ����� ���ۿ� ����� ���� ũ��
    // 6�� ���� : ������ ����
    // 7�� ���� : ������ ������ ���� x��ǥ
    // 8�� ���� : ������ ������ ���� y��ǥ
    // 9�� ���� : ���� �÷��� ����. SRCCOPY : �ҽ����� ��Ӻ���.(������ ���۸� ��Ӻ������ش�.)
    BitBlt(m_hDC, 0, 0, m_Resolution.Width, m_Resolution.Height, m_hBackDC, 0, 0, SRCCOPY);

    // ��� �׷����� ȭ���� �����Ѵ�.
    // �����ٰ� �ٽ� �׸��� TRUE�� �ְԵǸ� ȭ���� ����� �ٽñ׷��� �ܻ��� ���Ű� ������
    // �� �����Ӹ��� �׸��� ����⸦ �ݺ��ϱ� ������ ȭ�鿡 ������ ������ ��Ÿ�� �� �ִ�.
    //InvalidateRect(m_hWnd, nullptr, TRUE);
}

void CGameManager::Input(float DeltaTime)
{
    CInput::GetInst()->Update(DeltaTime);
}

bool CGameManager::Update(float DeltaTime)
{
    return CSceneManager::GetInst()->Update(DeltaTime);
}

bool CGameManager::Collision(float DeltaTime)
{
    return CSceneManager::GetInst()->Collision(DeltaTime);
}

void CGameManager::Render(float DeltaTime)
{
    // ȭ�鿡 ����ϱ� ���� ����۸� ������� �ʱ�ȭ���ش�.
    PatBlt(m_hBackDC, 0, 0, m_Resolution.Width, m_Resolution.Height, WHITENESS);


    CSceneManager::GetInst()->Render(m_hBackDC, DeltaTime);
}

/*
void CGameManager::Logic()
{
    // GetAsyncKeyState : �������� Ű �Է��� ������ ����ϴ� �Լ��̴�.
    // �� �Լ��� ���ϰ��� Ư���ѵ� 0, 0x1, 0x8000 3������ ���յǾ� ���´�.
    // 0, 0x1, 0x8000, 0x8001 �� 4������ ��ȯ���� ������ �Ǵµ�
    // 0�� ������ Ű�� �ȴ������� ��ȯ�ȴ�.
    // 0x1�� ���� �����ӿ� Ű�� ������ ��� ��ȯ�ȴ�.
    // 0x8000�� ���� Ű�� �������� ��ȯ�ȴ�.
    // 0x8001�� ���� �����ӿ��� Ű�� ������ ���� �����ӿ��� Ű�� ������
    // ��� ��ȯ�Ǵ� ���̴�.
    // VK_RETURN : ����Ű�̴�.
    // 0 & 0x8000 = 0
    // 0x1 & 0x8000 = 0
    // 0x8000 & 0x8000 = 0x8000
    // 0x8001 & 0x8000 = 0x8000
    if (GetAsyncKeyState(VK_RETURN) & 0x8000)
    {
        // MessageBox : �޼����� �˾�â���� ������ִ� �Լ��̴�.
        MessageBox(0, TEXT("Msg Text"), TEXT("Title"), MB_OK);
    }

    // ����Ű�� �ް��� �� ��� �빮�ڷ� ó���ؾ� �Ѵ�.
    if (GetAsyncKeyState('P') & 0x8000)
    {
        // lstrlen : �����ڵ� ���ڿ��� ���̸� ������ �Լ��̴�.
        int Length = lstrlen(TEXT("��µǳ�?"));
        TextOut(m_hDC, 0, 0, TEXT("��µǳ�?"), Length);

        // SelectObject : DC�� ���ϴ� ������ �������� �� ����Ѵ�.
        // �� �Լ��� ������ �����ϰ� ������ ������ �ִ� ������
        // ��ȯ�ϰ� �ȴ�.
        // �ݵ�� �� ��ٸ� ������� �ǵ������ �Ѵ�.
        HBRUSH hPrevBrush = (HBRUSH)SelectObject(m_hDC, m_hBrush);

        Rectangle(m_hDC, 150, 150, 250, 250);

        SelectObject(m_hDC, hPrevBrush);

        HPEN hPrevPen = (HPEN)SelectObject(m_hDC, m_hPen);

        Rectangle(m_hDC, 100, 100, 200, 200);

        SelectObject(m_hDC, hPrevPen);

        // MoveToEx, LineTo �Լ��� �̿��� ���� �׸� �� �ִ�.
        // MoveToEx �Լ��� ���� �������� �����Ҷ� ����Ѵ�.
        // LineTo�� ���� ������ �����Ҷ� ����Ѵ�.
        MoveToEx(m_hDC, 300, 100, nullptr);
        LineTo(m_hDC, 400, 200);
        LineTo(m_hDC, 500, 200);

        MoveToEx(m_hDC, 300, 500, nullptr);
        LineTo(m_hDC, 400, 600);

        Ellipse(m_hDC, 100, 300, 300, 400);

        RECT    rc = { 300, 450, 400, 550 };

        FrameRect(m_hDC, &rc, m_hBrush);
    }
}
*/
LRESULT CGameManager::WndProc(HWND hWnd, UINT message, 
    WPARAM wParam, LPARAM lParam)
{
    // �޼����� � �޼��������� ���� switch������ �б��Ͽ� �޼����� ó�����ش�.
    // ���⼭ ��� ���ϴ� �޼������� default���� ó�����ش�.
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...
        EndPaint(hWnd, &ps);
    }
    break;
    // WM_DESTROY�� ������ â ���� �޼����̴�. �� �޼����� ������
    // ������ â�� ����ǰ� ���α׷��� ����ȴٴ� �ǹ��̴�.
    // �׷��� �޼��� ������ ���⿡�� ��������־�� �Ѵ�.
    case WM_DESTROY:
        m_bLoop = false;
        PostQuitMessage(0);
        break;
    default:
        // DefWindowProc �Լ��� ȣ���Ͽ� ���⼭ ����� ���ϴ� �޼������� �⺻ �����쿡�� �ش� �޼�����
        // �´� ������ �ϵ��� �Ѵ�.
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
