#pragma once
#include "UIWidget.h"
class CMainSceneWidget :
    public CUIWidget
{
    friend class CScene;

protected:
    CMainSceneWidget();
    virtual ~CMainSceneWidget();

public:
    virtual bool Init();
};

