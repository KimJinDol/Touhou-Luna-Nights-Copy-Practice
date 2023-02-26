
#include "SpawnPoint.h"
//#include "../Object/Goblin.h"
#include "../Object/Archer.h"
#include "../Object/Bullet.h"
#include "../Object/GuideBullet.h"
#include "../Object/TornadoBullet.h"
#include "../Object/Portal.h"
#include "../Object/PlayerRotationObj.h"
#include "../Object/PlayerSubObj.h"
#include "../Object/BulletEffect.h"
#include "../Object/Smoke.h"
#include "Scene.h"

CSpawnPoint::CSpawnPoint() :
    m_pSpawnObj(nullptr),
    m_pScene(nullptr),
    m_SpawnTime(0.f),
    m_SpawnTimeMax(5.f),
    m_Enable(true),
    m_Active(true)
{
}

CSpawnPoint::CSpawnPoint(const CSpawnPoint& obj)
{
}

CSpawnPoint::~CSpawnPoint()
{
}

void CSpawnPoint::Update(float DeltaTime)
{
    // 생성된 오브젝트가 없을 경우
    if (m_SpawnTimeMax == -1.f)
    {
        if (!m_PrototypeName.empty())
        {
            m_pSpawnObj = m_pScene->CreateGameObjectNoType(m_PrototypeName, m_Name,
                m_vPos);

            //m_pSpawnObj->SetSpawnPoint(this);
            this->Destroy();
        }

        else
        {
            //if (m_ClassType == typeid(CGoblin).hash_code())
            //{
            //    m_pSpawnObj = m_pScene->CreateGameObject<CGoblin>("Goblin", m_vPos);
            //    //m_pSpawnObj->SetSpawnPoint(this);
            //}
        }
    }

    else
    {
        if (!m_pSpawnObj)
        {
            m_SpawnTime += DeltaTime;

            if (m_SpawnTime >= m_SpawnTimeMax)
            {
                m_SpawnTime = 0.f;

                if (!m_PrototypeName.empty())
                {
                    m_pSpawnObj = m_pScene->CreateGameObjectNoType(m_PrototypeName, m_Name,
                        m_vPos);

                    m_pSpawnObj->SetSpawnPoint(this);
                }

                else
                {
                    //if (m_ClassType == typeid(CGoblin).hash_code())
                    //{
                    //    m_pSpawnObj = m_pScene->CreateGameObject<CGoblin>("Goblin", m_vPos);
                    //    m_pSpawnObj->SetSpawnPoint(this);
                    //}
                }
            }
        }
    }
}

void CSpawnPoint::Render(HDC hDC, float DeltaTime)
{
}
