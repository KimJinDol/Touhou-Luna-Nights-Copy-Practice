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

    // GetModuleFile은 현재 실행파일이 있는 경로 + 실행파일 이름.exe 이런식으로
    // 전체경로가 들어오게 된다.
    // 여기에서 실행파일 이름을 빼고 Bin폴더까지만 얻어와서
    // Root 폴더로 사용한다.
    GetModuleFileName(0, Path, MAX_PATH);

    // 실행파일 이름을 제거한다.
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

    // 유니코드 문자열을 멀티바이트 문자열로 변환한다.
    // CP_ACP : OS에 기본 언어로 설정된 언어 코드를 사용하여 변환한다.
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
    // 유니코드 문자열을 멀티바이트 문자열로 변환한다.
    // CP_ACP : OS에 기본 언어로 설정된 언어 코드를 사용하여 변환한다.
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
