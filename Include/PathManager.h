#pragma once

#include "GameInfo.h"

struct PathInfo
{
    TCHAR* Path;
    char* PathMultibyte;

    PathInfo()
    {
        Path = new TCHAR[MAX_PATH];
        PathMultibyte = new char[MAX_PATH];
    }

    ~PathInfo()
    {
        SAFE_DELETE_ARRAY(Path);
        SAFE_DELETE_ARRAY(PathMultibyte);
    }
};

class CPathManager
{
private:
    std::unordered_map<std::string, PathInfo*>   m_mapPath;

public:
    bool Init();
    bool AddPath(const std::string& Name, const TCHAR* pAddPath,
        const std::string& BaseName = ROOT_PATH);
    const PathInfo* FindPath(const std::string& Name);

    DECLARE_SINGLE(CPathManager)
};

