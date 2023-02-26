
#include "StartSceneWidget.h"
#include "Button.h"
#include "UIImage.h"
#include "UIProgressBar.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
//#include "../Scene/Stage00.h"
#include "../GameManager.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Texture.h"

CStartSceneWidget::CStartSceneWidget()
{
}

CStartSceneWidget::~CStartSceneWidget()
{
}

bool CStartSceneWidget::Init()
{
    if (!CUIWidget::Init())
        return false;

	m_pScene->GetResourceManager()->SoundPlay("title");

	CSharedPtr<CUIImage> pMainUIImage = CreateControl<CUIImage>("UI_TitleImage");
	pMainUIImage->SetTexture("Title", TEXT("Title_.bmp"));
	pMainUIImage->SetPivot(0.5f, 0.5f);
	Vector2 vSize = Vector2(m_pScene->GetResourceManager()->FindTexture("Title")->GetTextureInfo()->BmpInfo.bmWidth,
		m_pScene->GetResourceManager()->FindTexture("Title")->GetTextureInfo()->BmpInfo.bmHeight);
	pMainUIImage->SetSize(vSize);
	pMainUIImage->SetPos(Vector2(GET_RESOLUTION.Width / 2, GET_RESOLUTION.Height /2));
	pMainUIImage->SetZOrder(1);
	//CSharedPtr<CButton> pButton = CreateControl<CButton>("StartButton");

	//pButton->SetNormalTexture("StartButton", TEXT("StartButton.bmp"));
	//pButton->SetMouseOnTexture("StartButton", TEXT("StartButton.bmp"));
	//pButton->SetClickTexture("StartButton", TEXT("StartButton.bmp"));
	//pButton->SetDisableTexture("StartButton", TEXT("StartButton.bmp"));

	//pButton->SetNormalTextureFrame(Vector2(0.f, 0.f), Vector2(200.f, 100.f));
	//pButton->SetMouseOnTextureFrame(Vector2(200.f, 0.f), Vector2(200.f, 100.f));
	//pButton->SetClickTextureFrame(Vector2(400.f, 0.f), Vector2(200.f, 100.f));
	//pButton->SetDisableTextureFrame(Vector2(600.f, 0.f), Vector2(200.f, 100.f));

	////pButton->SetMouseOnSound("ButtonMouseOn", "TeemoSmile.mp3");
	////pButton->SetClickSound("ButtonClick", "TeemoStartClicck.mp3");

	//pButton->SetPos(500.f, 600.f);
	//pButton->SetSize(200.f, 100.f);
	//pButton->SetClickCallback(this, &CStartSceneWidget::StartButtonClick);

	//CSharedPtr<CButton> pButton1 = CreateControl<CButton>("EditorButton");

	//pButton1->SetNormalTexture("StartButton", TEXT("StartButton.bmp"));
	//pButton1->SetMouseOnTexture("StartButton", TEXT("StartButton.bmp"));
	//pButton1->SetClickTexture("StartButton", TEXT("StartButton.bmp"));
	//pButton1->SetDisableTexture("StartButton", TEXT("StartButton.bmp"));

	//pButton1->SetNormalTextureFrame(Vector2(0.f, 0.f), Vector2(200.f, 100.f));
	//pButton1->SetMouseOnTextureFrame(Vector2(200.f, 0.f), Vector2(200.f, 100.f));
	//pButton1->SetClickTextureFrame(Vector2(400.f, 0.f), Vector2(200.f, 100.f));
	//pButton1->SetDisableTextureFrame(Vector2(600.f, 0.f), Vector2(200.f, 100.f));

	////pButton->SetMouseOnSound("ButtonMouseOn", "TeemoSmile.mp3");
	////pButton->SetClickSound("ButtonClick", "TeemoStartClicck.mp3");

	//pButton1->SetPos(800.f, 600.f);
	//pButton1->SetSize(200.f, 100.f);
	//pButton1->SetClickCallback(this, &CStartSceneWidget::EditorButtonClick);

    return true;
}