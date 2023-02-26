#pragma once
#include <iostream>
#include <fstream>
#include "../GameInfo.h"

class CTextManager
{
private:
    std::ifstream   m_File;
    MessageBoxType  m_BoxType;
    bool            m_IsDialogue;
    std::function<void()>	m_pEndDialogueFunc;

    std::string     m_curDialCode;

    bool            m_bEnableAllStop;

public:
    void SetEnableAllStop(bool Value)
    {
        m_bEnableAllStop = Value;
    }

    void SetDialCode(const std::string& dialCode)
    {
        m_curDialCode = dialCode;
    }

public:
    bool GetEnableAllStop() const
    {
        return m_bEnableAllStop;
    }

    std::string GetDialCode() const
    {
        return m_curDialCode;
    }

public:
    bool Init();

    MessageBoxType GetBoxType() const
    {
        return m_BoxType;
    }

    bool IsPlayDialogue()   const
    {
        return m_IsDialogue;
    }

    template<typename T>
    void SetEndDialogueFunc(T* pObj, void(T::* pFunc)())
    {
        m_pEndDialogueFunc = std::bind(pFunc, pObj);
    }

    void FindDialogueCursor();
    std::string GetText();

    DECLARE_SINGLE(CTextManager)
};
