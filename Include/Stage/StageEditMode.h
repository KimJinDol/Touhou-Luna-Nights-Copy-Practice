#pragma once
#include "Stage.h"
class CStageEditMode :
    public CStage
{
    friend class CScene;
protected:
    CStageEditMode();
    virtual ~CStageEditMode();

private:
    bool    m_bIsVisibleTileResource;
    class CEditorDlg* m_pEditDlg;

public:
    void SetEidtDlg(CEditorDlg* pEditDlg)
    {
        m_pEditDlg = pEditDlg;
    }

    void SetVisibleTileResource(bool value)
    {
        m_bIsVisibleTileResource = value;
    }

    bool GetVisibleTileResource()   const
    {
        return m_bIsVisibleTileResource;
    }

    void TileClear(const Vector2& Pos);

public:
    virtual void Render(HDC hDC, float DeltaTime);
};

