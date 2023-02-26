
#pragma once

#define	SAFE_DELETE(p)	if(p)	{ delete p; p = nullptr; }
#define	SAFE_DELETE_ARRAY(p)	if(p)	{ delete[] p; p = nullptr; }
#define	SAFE_RELEASE(p)	if(p)	{ p->Release(); p = nullptr; }

#define	DECLARE_SINGLE(Type)	\
private:\
	static Type*	m_pInst;\
public:\
	static Type* GetInst()\
	{\
		if (!m_pInst)\
			m_pInst = new Type;\
		return m_pInst;\
	}\
	static void DestroyInst()\
	{\
		SAFE_DELETE(m_pInst);\
	}\
private:\
	Type();\
	~Type();


#define	DEFINITION_SINGLE(Type)	Type* Type::m_pInst = nullptr;
#define GET_SINGLE(Type) Type::GetInst()

#define GET_TIMER CGameManager::GetInst()->GetTimer()
#define GET_RESOLUTION CGameManager::GetInst()->GetResolution()

#define SHOW_OBJPOS(str) char arr[256] = {};\
sprintf_s(arr, str);\
TextOutA(hDC, m_vPos.x - m_pScene->GetMainCamera()->GetPos().x, m_vPos.y - m_pScene->GetMainCamera()->GetPos().y, arr, (int)strlen(arr))

#define SHOW_UIPOS(str) char arr[256] = {};\
sprintf_s(arr, str);\
TextOutA(hDC, m_vPos.x, m_vPos.y, arr, (int)strlen(arr))


template <typename To, typename From>
To Cast(From from)
{
	To castObj = dynamic_cast<To>(from);
	if (!castObj) return NULL;
		
	return castObj;
}