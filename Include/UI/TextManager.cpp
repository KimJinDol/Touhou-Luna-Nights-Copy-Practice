#include "TextManager.h"
#include "../PathManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/SceneMode.h"''
DEFINITION_SINGLE(CTextManager)

CTextManager::CTextManager()    :
    m_BoxType(MessageBoxType::MBT_1)
{
}

CTextManager::~CTextManager()
{
    if (m_File.is_open())
        m_File.close();
}

bool CTextManager::Init()
{
    return true;
}

void CTextManager::FindDialogueCursor()
{
    if (m_File.is_open())
        m_File.close();

    char    FullPath[MAX_PATH] = {};

    const char* pPath = CPathManager::GetInst()->FindPath(DATA_PATH)->PathMultibyte;

    if (pPath)
        strcpy_s(FullPath, pPath);

    strcat_s(FullPath, "dial_k.txt");

    m_File.open(FullPath);

    std::string Line;
    int Offset = 0;

    while (!m_File.eof())
    {
        std::getline(m_File, Line);

        // 해당 고유번호를 찾았다면 아래 실행
        if (Offset = Line.find(m_curDialCode, 0) != std::string::npos)
            return;
    }
}

std::string CTextManager::GetText()
{
    m_IsDialogue = true;
    char Line[256] = {};
    std::string strLine;
    m_File.getline(Line, 256);
    strLine = Line;

    if ((strLine.find("$0101", 0)) != std::string::npos)
    {
        m_BoxType = MessageBoxType::MBT_1;
        m_File.getline(Line, 256);
    }
    else if ((strLine.find("$0505", 0)) != std::string::npos)
    {
        if (m_curDialCode.find("#sa", 0) != std::string::npos)
            m_BoxType = MessageBoxType::MBT_3;
        else
            m_BoxType = MessageBoxType::MBT_2;
        m_File.getline(Line, 256);
    }
    else if ((strLine.find("#end", 0)) != std::string::npos)
    {
        m_IsDialogue = false;
        m_pEndDialogueFunc();
    }

    return Line;
}
