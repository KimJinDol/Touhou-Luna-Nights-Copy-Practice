
#include "MainSceneWidget.h"
#include "Button.h"

CMainSceneWidget::CMainSceneWidget()
{
}

CMainSceneWidget::~CMainSceneWidget()
{
}

bool CMainSceneWidget::Init()
{
    if (!CUIWidget::Init())
        return false;

    CSharedPtr<CButton> pButton = CreateControl<CButton>("StartButton");

    pButton->SetNormalTexture("StartButton", TEXT("StartButton.bmp"));
    pButton->SetMouseOnTexture("StartButton", TEXT("StartButton.bmp"));
    pButton->SetClickTexture("StartButton", TEXT("StartButton.bmp"));
    pButton->SetDisableTexture("StartButton", TEXT("StartButton.bmp"));

    pButton->SetNormalTextureFrame(Vector2(0.f, 0.f), Vector2(200.f, 100.f));
    pButton->SetMouseOnTextureFrame(Vector2(200.f, 0.f), Vector2(200.f, 100.f));
    pButton->SetClickTextureFrame(Vector2(400.f, 0.f), Vector2(200.f, 100.f));
    pButton->SetDisableTextureFrame(Vector2(600.f, 0.f), Vector2(200.f, 100.f));

    //pButton->SetMouseOnSound("ButtonMouseOn", "TeemoSmile.mp3");
    //pButton->SetClickSound("ButtonClick", "TeemoStartClicck.mp3");

    pButton->SetPos(500.f, 600.f);
    pButton->SetSize(200.f, 100.f);

    CSharedPtr<CButton> pButton1 = CreateControl<CButton>("StartButton1");

    pButton1->SetNormalTexture("StartButton", TEXT("StartButton.bmp"));
    pButton1->SetMouseOnTexture("StartButton", TEXT("StartButton.bmp"));
    pButton1->SetClickTexture("StartButton", TEXT("StartButton.bmp"));
    pButton1->SetDisableTexture("StartButton", TEXT("StartButton.bmp"));

    pButton1->SetNormalTextureFrame(Vector2(0.f, 0.f), Vector2(200.f, 100.f));
    pButton1->SetMouseOnTextureFrame(Vector2(200.f, 0.f), Vector2(200.f, 100.f));
    pButton1->SetClickTextureFrame(Vector2(400.f, 0.f), Vector2(200.f, 100.f));
    pButton1->SetDisableTextureFrame(Vector2(600.f, 0.f), Vector2(200.f, 100.f));

    //pButton1->SetMouseOnSound("ButtonMouseOn", "TeemoSmile.mp3");
    //pButton1->SetClickSound("ButtonClick", "TeemoStartClicck.mp3");

    pButton1->SetPos(600.f, 550.f);
    pButton1->SetSize(200.f, 100.f);
    pButton1->SetZOrder(1);

    return true;
}
