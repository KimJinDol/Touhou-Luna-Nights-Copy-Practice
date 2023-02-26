#pragma once

#include "GameInfo.h"

class CGameManager
{
private:
	HINSTANCE		m_hInst;
	HWND			m_hWnd;
	HDC				m_hDC;
	HDC				m_hBackDC;
	HBITMAP			m_hBackBmp;
	HGDIOBJ			m_hPrevBackBmp;
	Resolution		m_Resolution;
	static bool		m_bLoop;
	HBRUSH			m_hRedBrush;
	HBRUSH			m_hGreenBrush;
	HPEN			m_hRedPen;
	HPEN			m_hGreenPen;
	HPEN			m_hPurplePen;
	class CTimer*	m_pTimer;

public:
	CTimer* GetTimer()		const
	{
		return m_pTimer;
	}

	HWND GetWindowHandle()	const
	{
		return m_hWnd;
	}

	HINSTANCE GetWindowInstance()	const
	{
		return m_hInst;
	}

	HDC GetWindowDC()	const
	{
		return m_hDC;
	}

	Resolution GetResolution()	const
	{
		return m_Resolution;
	}

	HBRUSH GetRedBrush()	const
	{
		return m_hRedBrush;
	}

	HBRUSH GetGreenBrush()	const
	{
		return m_hGreenBrush;
	}

	HPEN GetRedPen()	const
	{
		return m_hRedPen;
	}

	HPEN GetGreenPen()	const
	{
		return m_hGreenPen;
	}

	HPEN GetPurplePen()	const
	{
		return m_hPurplePen;
	}

private:
	ATOM MyRegisterClass(const TCHAR* pClassName, int IconID, int SmallIconID);
	bool Create(const TCHAR* pClassName, const TCHAR* pTitle);

public:
	// TCHAR : �����ڵ��϶��� 2����Ʈ ����, ��Ƽ����Ʈ �϶��� 1����Ʈ ���ڸ�
	// ����ϰ� ���ش�.
	bool Init(HINSTANCE hInst, const TCHAR* pTitle,
		const TCHAR* pClassName, int Width, int Height,
		int IconID, int SmallIconID);
	int Run();

private:
	void Logic();
	void Input(float DeltaTime);
	bool Update(float DeltaTime);
	bool Collision(float DeltaTime);
	void Render(float DeltaTime);

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_SINGLE(CGameManager)
};

