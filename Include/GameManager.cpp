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

    // 백버퍼에 원래 지정되어 있던 도구를 지정한다.
    SelectObject(m_hBackDC, m_hPrevBackBmp);
    // 백버퍼용 비트맵 도구를 제거한다.
    DeleteObject(m_hBackBmp);

    // 백버퍼용 DC를 제거한다.
    DeleteDC(m_hBackDC);

    ReleaseDC(m_hWnd, m_hDC);
}

ATOM CGameManager::MyRegisterClass(const TCHAR* pClassName, int IconID, int SmallIconID)
{
    // 레지스터에 등록하기 위한 정보를 담아두는 구조체이다.
    // 윈도우 클래스 구조체로 어떤 스타일로 창을 만들지, 윈도우 메세지가 들어올 경우 어떤 함수를
    // 호출해줄것인지 등을 지정해줄 수 있다.
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInst;
    // 실행파일 아이콘을 설정한다.
    wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IconID));
    // 이 프로그램이 동작될때 마우스 커서를 어떤 모양으로 동작시킬지를 결정한다.
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    // 아래는 메뉴를 표현한다.
    wcex.lpszMenuName = nullptr;
    // 레지스터에 등록할 이름을 지정한다. 유니코드로 지정해야 한다.
    wcex.lpszClassName = pClassName;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(SmallIconID));

    // 설정된 정보를 등록한다.
    return RegisterClassExW(&wcex);
}

bool CGameManager::Create(const TCHAR* pClassName, const TCHAR* pTitle)
{
    // 윈도우의 좌표계 : 좌측 상단이 0, 0 이다.오른쪽으로 갈수록 X 플러스값이 되고 아래로 갈수록 Y플러스
    // 값이 된다.
    // 스크린 좌표계 : 바탕화면의 좌표계를 말한다. 바탕화면에서 좌, 상단이 0, 0이다.
    // 클라이언트 좌표계 : 윈도우 클라이언트 영역의 좌, 상단이 0, 0이 된다.
    // CreateWindow : 이 함수는 윈도우 창을 생성해주고 생성된 창의 윈도우 핸들을 반환해주는 함수이다.
    // 1번인자 : 레지스터에 등록된 이름을 지정한다.
    // 2번인자 : 윈도우창 타이틀바의 이름을 지정한다.
    // 3번인자 : 이 윈도우 창을 어떤 형태로 생성할지 구성요소를 지정해준다.
    // 4번인자 : 이 윈도우 창이 스크린좌표계에서 어디에 생성될지를 지정하는 X 값이다.
    // 5번인자 : 이 윈도우 창이 스크린좌표계에서 어디에 생성될지를 지정하는 Y 값이다.
    // 6번인자 : 윈도우 창의 가로 크기를 지정한다.
    // 7번인자 : 윈도우 창의 세로 크기를 지정한다.
    m_hWnd = CreateWindowW(pClassName, pTitle, WS_OVERLAPPED |
        WS_CAPTION |
        WS_SYSMENU |
        WS_MINIMIZEBOX,
        0, 0, m_Resolution.Width, m_Resolution.Height,
        nullptr, nullptr, m_hInst, nullptr);

    // 생성된 핸들이 nullptr이라면 윈도우 창이 제대로 생성된것이 아니라는 의미이다.
    if (!m_hWnd)
        return false;
    
    // 생성하고자 하는 클라이언트 영역의 크기를 Rect로 만들어준다.
    RECT    rc = { 0, 0, m_Resolution.Width, m_Resolution.Height };

    // 클라이언트 창의 크기가 위의 크기가 되기 위해서 필요한 정확한 최종
    // 크기를 얻어온다.
    // 2번인자에 원하는 윈도우 스타일을 넣어주고 3번인자에 메뉴를 사용할
    // 것인지 여부를 넣어주면 위에서 지정한 크기대로 클라이언트 영역이
    // 만들어질 수 있도록 최종윈도우 크기를 구해주게 된다.
    AdjustWindowRect(&rc, WS_OVERLAPPED |
        WS_CAPTION |
        WS_SYSMENU |
        WS_MINIMIZEBOX, false);

    // 위에서 구해준 최종 윈도우의 크기로 윈도우 창을 새롭게 정의한다.
    SetWindowPos(m_hWnd, nullptr, 0, 0, rc.right - rc.left,
        rc.bottom - rc.top, SWP_NOZORDER | SWP_NOMOVE);

    // ShowWindow : 화면에 이 윈도우 창을 보여줄지 말지를 결정한다.
    ShowWindow(m_hWnd, SW_SHOW);

    // 클라이언트 영역을 업데이트 해준다. 갱신해준다는 것이다.
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

    // 메모리 관리자 초기화
    if (!CGarbageCollector::GetInst()->Init())
        return false;

    // 경로 관리자 초기화
    if (!CPathManager::GetInst()->Init())
        return false;

    // Text 관리자 초기화
    if (!CTextManager::GetInst()->Init())
        return false;

    // 리소스 관리자 초기화
    if (!CResourceManager::GetInst()->Init())
        return false;

    // 충돌 관리자 초기화
    if (!CCollisionManager::GetInst()->Init())
        return false;

    // 입력 관리자 초기화
    if (!CInput::GetInst()->Init())
        return false;

    // 장면관리자를 초기화한다.
    if (!CSceneManager::GetInst()->Init())
        return false;

    m_pTimer = new CTimer;

    m_pTimer->Init();


    /*
    HDC : 윈도우 클라이언트 영역에 무언가를 그리고자 할 경우
    DC를 이용해서 그리게 된다.
    DC는 윈도우 창에 그림을 그리고자 하는 도구이다.
    GetDC함수는 인자로 들어간 윈도우 핸들에대한 HDC를 만들어주는
    함수이다.
    GetDC를 이용해서 생성된 DC는 반드시 ReleaseDC 함수를 이용해서
    제거해주어야 한다.
    */
    m_hDC = GetDC(m_hWnd);

    // 화면 DC를 복제한 백버퍼 DC를 만들어준다.
    m_hBackDC = CreateCompatibleDC(m_hDC);

    // 화면 DC를 이용하여 비트맵 도구를 만들어준다. 이 도구가 백버퍼가 될것이다.
    m_hBackBmp = CreateCompatibleBitmap(m_hDC, m_Resolution.Width, m_Resolution.Height);

    // 위에서 만들어준 비트맵 도구를 백버퍼용 DC에 지정해준다.
    m_hPrevBackBmp = SelectObject(m_hBackDC, m_hBackBmp);

    /*
    HBRUSH : 윈도우 클라이언트창에 무언가를 그릴고자 할때 지정할
    색상을 말한다.
    HPEN : 출력할때 선의 색상을 지정할때 사용한다.
    r, g, b는 0 ~ 255 의 값을 가지고 조합하여 최종 색을 만들어낸다.
    */
    m_hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
    m_hGreenBrush = CreateSolidBrush(RGB(0, 255, 0));
    m_hRedPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    m_hGreenPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    m_hPurplePen = CreatePen(PS_SOLID, 1, RGB(100, 0, 255));

    // MainSceneMode 생성
#ifdef _DEBUG
    // MainSceneMode 생성
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
    // 메세지 루프를 돌려준다.
    MSG msg;

    /*
    윈도우는 메세지 기반의 운영체제이다. 운영체제는 어떤 이벤트가 발생할때마다 메세지를 만들어서
    해당 윈도우에게 전달해주게 된다.
    이벤트는 마우스 클릭, 이동, 키보드 입력 등을 모두 통칭해서 이벤트라 한다.
    각각의 윈도우들은 내부적으로 메세지 큐를 가지고 있다.
    운영체제는 이벤트가 발생하면 해당 이벤트의 정보를 메세지로 만들고 그 메세지를 윈도우의 메세지 큐에
    넣어주게 된다.
    GetMessage는 바로 메세지 큐에 있는 메세지를 꺼내오는 함수이다.
    단, 메세지 큐가 비어있다면 메세지가 들어올때까지 이 함수 안에서 대기하고 있는것이다.
    */
    while (m_bLoop)
    {
        // PeekMessage는 메세지큐 안에 메세지가 있을 경우 해당 메세지를
        // 가져오고 true가 되며 메세지큐 안에 메세지가 없을 경우
        // false가 된다.
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // 해당 메세지가 WM_KEYDOWN인지, 눌려진 키가 문자키인지를 맞을 경우 WM_CHAR메세지를
            // 메세지 큐에 넣어주는 기능이다.
            TranslateMessage(&msg);

            // 이 메세지를 WndProc 에 전달해준다.
            DispatchMessage(&msg);
        }

        // 메세지가 없을 경우
        else
        {
            Logic();
        }
    }

    return (int)msg.wParam;
}

/*
게임은 크게 4가지의 단계를 거치게 된다.
1. 입력
2. 입력받은 내용을 토대로 업데이트
3. 업데이트한 내용을 토대로 충돌처리
4. 출력
*/
void CGameManager::Logic()
{
    // 타이머를 갱신한다.
    m_pTimer->Update();

    CResourceManager::GetInst()->Update();

    // 입력을 처리한다.
    Input(m_pTimer->GetDeltaTime());

    // 데이터를 업데이트한다.
    if (Update(m_pTimer->GetDeltaTime()))
    {
        CGarbageCollector::GetInst()->Update();
        return;
    }

    // 충돌처리를 진행한다.
    if (Collision(m_pTimer->GetDeltaTime()))
    {
        CGarbageCollector::GetInst()->Update();
        return;
    }

    // 화면에 출력한다.
    Render(m_pTimer->GetDeltaTime());

    // 지워져야 할 메모리를 모두 지워준다.
    CGarbageCollector::GetInst()->Update();

    char    TimeText[256] = {};
    wchar_t TimeTextLPCW[256] = {};
    size_t cn;

    // sprintf_s : 문자열을 만들어줄때 사용한다.
    // 1번인자에 들어간 문자 배열을 2, 3번 인자의 내용으로 채워주도록 한다.
#ifdef _DEBUG
    sprintf_s(TimeText, "FPS : %.5f, DeltaTime : %.5f", m_pTimer->GetFPS(), m_pTimer->GetDeltaTime());
#else
    sprintf_s(TimeText, "Tougou Luna Nights");
#endif
    mbstowcs_s(&cn, TimeTextLPCW, TimeText, (int)strlen(TimeText));
    SetWindowText(m_hWnd, TimeTextLPCW);
    /*TextOutA(m_hBackDC, 700, 50, TimeText, (int)strlen(TimeText));*/

    // 백버퍼에 그려진 내용을 화면에 고속복사를 해준다.
    // 1번 인자 : 복사될 버퍼
    // 2번 인자 : 복사될 버퍼의 시작 x좌표
    // 3번 인자 : 복사될 버퍼의 시작 y좌표
    // 4번 인자 : 복사될 버퍼에 복사될 가로 크기
    // 5번 인자 : 복사될 버퍼에 복사될 세로 크기
    // 6번 인자 : 복사할 버퍼
    // 7번 인자 : 복사할 버퍼의 시작 x좌표
    // 8번 인자 : 복사할 버퍼의 시작 y좌표
    // 9번 인자 : 복사 플래그 지정. SRCCOPY : 소스정보 고속복사.(복사할 버퍼를 고속복사해준다.)
    BitBlt(m_hDC, 0, 0, m_Resolution.Width, m_Resolution.Height, m_hBackDC, 0, 0, SRCCOPY);

    // 모두 그렸으면 화면을 갱신한다.
    // 지웠다가 다시 그리게 TRUE로 주게되면 화면을 지우고 다시그려서 잔상은 제거가 되지만
    // 매 프레임마다 그리고 지우기를 반복하기 때문에 화면에 깜빡임 현상이 나타날 수 있다.
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
    // 화면에 출력하기 전에 백버퍼를 흰색으로 초기화해준다.
    PatBlt(m_hBackDC, 0, 0, m_Resolution.Width, m_Resolution.Height, WHITENESS);


    CSceneManager::GetInst()->Render(m_hBackDC, DeltaTime);
}

/*
void CGameManager::Logic()
{
    // GetAsyncKeyState : 윈도우의 키 입력을 받을때 사용하는 함수이다.
    // 이 함수는 리턴값이 특이한데 0, 0x1, 0x8000 3가지가 조합되어 나온다.
    // 0, 0x1, 0x8000, 0x8001 이 4가지의 반환값이 나오게 되는데
    // 0은 지정한 키를 안눌렀을때 반환된다.
    // 0x1은 이전 프레임에 키를 눌렀을 경우 반환된다.
    // 0x8000은 현재 키를 눌렀을때 반환된다.
    // 0x8001은 이전 프레임에도 키를 눌렀고 현재 프레임에도 키를 눌렀을
    // 경우 반환되는 값이다.
    // VK_RETURN : 엔터키이다.
    // 0 & 0x8000 = 0
    // 0x1 & 0x8000 = 0
    // 0x8000 & 0x8000 = 0x8000
    // 0x8001 & 0x8000 = 0x8000
    if (GetAsyncKeyState(VK_RETURN) & 0x8000)
    {
        // MessageBox : 메세지를 팝업창으로 출력해주는 함수이다.
        MessageBox(0, TEXT("Msg Text"), TEXT("Title"), MB_OK);
    }

    // 문자키를 받고자 할 경우 대문자로 처리해야 한다.
    if (GetAsyncKeyState('P') & 0x8000)
    {
        // lstrlen : 유니코드 문자열의 길이를 얻어오는 함수이다.
        int Length = lstrlen(TEXT("출력되냐?"));
        TextOut(m_hDC, 0, 0, TEXT("출력되냐?"), Length);

        // SelectObject : DC에 원하는 도구를 지정해줄 때 사용한다.
        // 이 함수는 도구를 지정하고 기존에 가지고 있던 도구를
        // 반환하게 된다.
        // 반드시 다 썼다면 원래대로 되돌려줘야 한다.
        HBRUSH hPrevBrush = (HBRUSH)SelectObject(m_hDC, m_hBrush);

        Rectangle(m_hDC, 150, 150, 250, 250);

        SelectObject(m_hDC, hPrevBrush);

        HPEN hPrevPen = (HPEN)SelectObject(m_hDC, m_hPen);

        Rectangle(m_hDC, 100, 100, 200, 200);

        SelectObject(m_hDC, hPrevPen);

        // MoveToEx, LineTo 함수를 이용해 선을 그릴 수 있다.
        // MoveToEx 함수는 선의 시작점을 정의할때 사용한다.
        // LineTo는 선의 끝점을 정의할때 사용한다.
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
    // 메세지가 어떤 메세지인지에 따라 switch문으로 분기하여 메세지를 처리해준다.
    // 여기서 사용 안하는 메세지들은 default에서 처리해준다.
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        EndPaint(hWnd, &ps);
    }
    break;
    // WM_DESTROY는 윈도우 창 종료 메세지이다. 이 메세지가 들어오면
    // 윈도우 창이 종료되고 프로그램이 종료된다는 의미이다.
    // 그래서 메세지 루프를 여기에서 종료시켜주어야 한다.
    case WM_DESTROY:
        m_bLoop = false;
        PostQuitMessage(0);
        break;
    default:
        // DefWindowProc 함수를 호출하여 여기서 사용을 안하는 메세지들은 기본 윈도우에서 해당 메세지에
        // 맞는 동작을 하도록 한다.
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
