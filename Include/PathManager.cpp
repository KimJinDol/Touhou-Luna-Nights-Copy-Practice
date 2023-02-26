#include "PathManager.h"

DEFINITION_SINGLE(CPathManager)

CPathManager::CPathManager()
{
}

CPathManager::~CPathManager()
{
    auto    iter = m_mapPath.begin();
    auto    iterEnd = m_mapPath.end();

    for (; iter != iterEnd; ++iter)
    {
        SAFE_DELETE(iter->second);
    }
}

bool CPathManager::Init()
{
    TCHAR   Path[MAX_PATH] = {};

    // GetModuleFile�� ���� ���������� �ִ� ��� + �������� �̸�.exe �̷�������
    // ��ü��ΰ� ������ �ȴ�.
    // ���⿡�� �������� �̸��� ���� Bin���������� ���ͼ�
    // Root ������ ����Ѵ�.
    GetModuleFileName(0, Path, MAX_PATH);

    // �������� �̸��� �����Ѵ�.
    int  Length = lstrlen(Path);

    for (int i = Length - 1; i > 0; --i)
    {
        if (Path[i] == '\\')
        {
            memset(&Path[i + 1], 0, sizeof(TCHAR) * (MAX_PATH - 1 - i));
            break;
        }
    }

    PathInfo* pPathInfo = new PathInfo;

    lstrcpy(pPathInfo->Path, Path);

#ifdef UNICODE

    // �����ڵ� ���ڿ��� ��Ƽ����Ʈ ���ڿ��� ��ȯ�Ѵ�.
    // CP_ACP : OS�� �⺻ ���� ������ ��� �ڵ带 ����Ͽ� ��ȯ�Ѵ�.
    int ConvertLength = WideCharToMultiByte(CP_ACP, 0, pPathInfo->Path, -1, nullptr, 0, 0, 0);

    WideCharToMultiByte(CP_ACP, 0, pPathInfo->Path, -1,
        pPathInfo->PathMultibyte, ConvertLength, 0, 0);
#else

    strcpy_s(pPathInfo->PathMultibyte, pPathInfo->Path);

#endif // UNICODE

    m_mapPath.insert(std::make_pair(ROOT_PATH, pPathInfo));

    AddPath(TEXTURE_PATH, TEXT("Texture\\"));
    AddPath(SOUND_PATH, TEXT("Sound\\"));
    AddPath(DATA_PATH, TEXT("Data\\"));

    return true;
}

bool CPathManager::AddPath(const std::string& Name, const TCHAR* pAddPath,
    const std::string& BaseName)
{
    if (FindPath(Name))
        return false;

    const PathInfo* pBasePath = FindPath(BaseName);

    if (!pBasePath)
        return false;

    PathInfo* pPathInfo = new PathInfo;

    lstrcpy(pPathInfo->Path, pBasePath->Path);
    lstrcat(pPathInfo->Path, pAddPath);

#ifdef UNICODE
    // �����ڵ� ���ڿ��� ��Ƽ����Ʈ ���ڿ��� ��ȯ�Ѵ�.
    // CP_ACP : OS�� �⺻ ���� ������ ��� �ڵ带 ����Ͽ� ��ȯ�Ѵ�.
    int ConvertLength = WideCharToMultiByte(CP_ACP, 0, pPathInfo->Path, -1, nullptr, 0, 0, 0);

    WideCharToMultiByte(CP_ACP, 0, pPathInfo->Path, -1,
        pPathInfo->PathMultibyte, ConvertLength, 0, 0);
#else

    strcpy_s(pPathInfo->PathMultibyte, pPathInfo->Path);

#endif // UNICODE

    m_mapPath.insert(std::make_pair(Name, pPathInfo));

    return true;
}

const PathInfo* CPathManager::FindPath(const std::string& Name)
{
    auto    iter = m_mapPath.find(Name);

    if (iter == m_mapPath.end())
        return nullptr;

    return iter->second;
}
