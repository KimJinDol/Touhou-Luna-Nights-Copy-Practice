#pragma once
#include "UIWidget.h"

class CStartSceneWidget :
    public CUIWidget
{
    friend class CScene;

protected:
    CStartSceneWidget();
    virtual ~CStartSceneWidget();

public:
    virtual bool Init();

};