#pragma once
#include "UIWidgetWorld.h"
#include "UIImage.h"

class CUIPlayerWidgetWorld :
	public CUIWidgetWorld
{
public:
	CUIPlayerWidgetWorld();
	CUIPlayerWidgetWorld(const CUIPlayerWidgetWorld& pWidgetWorld);
	~CUIPlayerWidgetWorld();

private:
	CSharedPtr<class CUIImage> m_pSnailSocket;
	CSharedPtr<class CUIImage> m_pSnailGauge1;
	CSharedPtr<class CUIImage> m_pSnailGauge2;
	CSharedPtr<class CUIImage> m_pSnailGauge3;
	CSharedPtr<class CUIImage> m_pSnailGauge4;
	CSharedPtr<class CUIImage> m_pNoMP;
	CSharedPtr<class CUIImage> m_pFullMP;
	CSharedPtr<class CUIImage> m_pFullStat;
	CSharedPtr<class CUIImage> m_pNoTimeGauge;
	CSharedPtr<class CUIImage> m_pFullTimeGauge;
	CSharedPtr<class CUIImage> m_pGraze;
	CSharedPtr<class CUITextNumber>  m_pTimeStopKnifeNum;
	CSharedPtr<class CUIImage> m_pUpVision;


public:
	bool Init();
	void FillSnailGauge(int idx);
	void SnailGaugeEnable(bool Value);

	void ShowNoMP();
	void HideNoMP();
	void ShowFullMP();
	void HideFullMP();

	void ShowFullStat();
	void HideFullStat();
	
	void ShowNoTimeGauge();
	void HideNoTimeGauge();
	void ShowFullTimeGauge();
	void HideFullTimeGauge();

	void SetKnifeNum(int KnifeNum);
	void HideKnifeNum();

	void ShowUpVision(bool Value);
	bool IsShowUpVision()	const
	{
		return m_pUpVision.Get()->IsEnable();
	}

};