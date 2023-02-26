#pragma once
#include "UIWidget.h"

class CPlayerWidget :
    public CUIWidget
{
    friend class CScene;

protected:
    CPlayerWidget();
    virtual ~CPlayerWidget();

private:
    CSharedPtr<class CUIImage>       m_pMainUIImage;
    CSharedPtr<class CUITextNumber>  m_pLevelText;
    CSharedPtr<class CUIProgressBar> m_pHPBar;
    CSharedPtr<class CUITextNumber>  m_pHPText;
    CSharedPtr<class CUIProgressBar> m_pMPBar;
    CSharedPtr<class CUITextNumber>  m_pMPText;
    CSharedPtr<class CUITextNumber>  m_pTimeStopTextNumber;
    CSharedPtr<class CUIImage>       m_pTimeState;
    CSharedPtr<class CUIImage>       m_pItemBack;
    CSharedPtr<class CUIImage>       m_pBlackMask;
    CSharedPtr<class CUIImage>       m_pItemImage;
    CSharedPtr<class CUIText>        m_pText;


    bool    m_bSecondHandSound;
    float   m_fSecondHandTime;
    float   m_fCurrentSecondHandTime;

public:
    void SetLevel(int level);
    void SetHP(int hp);
    void SetMP(int mp);
    void SetTimeStopGauge(int TimeStopGauge);
    void PlayTimeGaugeEffect();
    void SetTimeState(TimeMode eMode);

    void PlaySecondeHandSound();

    void SetText(const std::string& Text);
    void HideText();


    // Enable
    void SetEnableTimeStopWidget(bool Value);
    void SetWidgetEnable(bool Value);
    void SetEnableItemWidget(bool Value);

    void GetItem(const std::string& Name);

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);

};