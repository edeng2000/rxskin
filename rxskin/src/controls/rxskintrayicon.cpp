#include "pch.h"
#include "controls/RXSkinTrayIcon.h"

#include <map>

using namespace std;
#define TRAY_WINDOW_MESSAGE (WM_USER+100)


namespace
{
	class CIdToTrayIconMap
	{
	public:
		typedef UINT KeyType;
		typedef CRXSkinTrayIcon* ValueType;

		// typedef didn't work with VC++6
		struct StdMap : public std::map<KeyType,ValueType> {};
		typedef StdMap::iterator iterator;

		CIdToTrayIconMap() : m_Empty(true) {}
		ValueType& operator[](KeyType k)
		{
			return GetOrCreateStdMap()[k];
		}
		ValueType* find(KeyType k)
		{
			if (m_Empty)
				return NULL;
			StdMap::iterator it = GetStdMap().find(k);
			if (it == GetStdMap().end())
				return NULL;
			return &it->second;
		}
		int erase(KeyType k)
		{
// 			if (m_Empty)
// 				return 0;
// 			StdMap& m = GetStdMap();
// 			int res = (int)m.erase(k);
// 			if (m.empty())
// 			{
// 				m.~StdMap();
// 				m_Empty = true;
// 			}
			return 0;
		}
		bool empty() const
		{
			return m_Empty;
		}
		// Call this only when the container is not empty!!!
		iterator begin()
		{
			return m_Empty ? iterator() : GetStdMap().begin();
		}
		// Call this only when the container is not empty!!!
		iterator end()
		{
			return m_Empty ? iterator() : GetStdMap().end();
		}

	private:
		StdMap &GetStdMap()
		{
			return (StdMap&)m_MapBuffer;
		}
		StdMap &GetOrCreateStdMap()
		{
			if (m_Empty)
			{
				new ((void*)&m_MapBuffer) StdMap();
				m_Empty = false;
			}
			return (StdMap&)m_MapBuffer;
		}
	private:
		bool m_Empty;
		char m_MapBuffer[sizeof(StdMap)];
	};

	static CIdToTrayIconMap& GetIdToTrayIconMap()
	{
		// This hack prevents running the destructor of our map, so it isn't problem if someone tries to reach this from a static destructor.
		// Because of using MyMap this will not cause a memory leak if the user removes all items from the container before exiting.
		static char id_to_tray_icon_buffer[sizeof(CIdToTrayIconMap)];
		static bool initialized = false;
		if (!initialized)
		{
			initialized = true;
			new ((void*)id_to_tray_icon_buffer) CIdToTrayIconMap();
		}
		return (CIdToTrayIconMap&)id_to_tray_icon_buffer;
	}

	static UINT GetNextTrayIconId()
	{
		static UINT next_id = 1;
		return next_id++;
	}
}


static const UINT g_WndMsgTaskbarCreated = RegisterWindowMessage(TEXT("TaskbarCreated"));
LRESULT CALLBACK CRXSkinTrayIcon::MessageProcessorWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
 	if (uMsg == TRAY_WINDOW_MESSAGE)
	{
		if (CRXSkinTrayIcon** ppIcon = GetIdToTrayIconMap().find((UINT)wParam))
			(*ppIcon)->OnMessage((UINT)lParam);
		return 0;
	}
	else if (uMsg == g_WndMsgTaskbarCreated)
	{
		CIdToTrayIconMap &id_to_tray = GetIdToTrayIconMap();
		if (!id_to_tray.empty())
		{
			for (std::map<UINT,CRXSkinTrayIcon*>::const_iterator it=id_to_tray.begin(),eit=id_to_tray.end(); it!=eit; ++it)
			{
				CRXSkinTrayIcon *pTrayIcon = it->second;
				pTrayIcon->OnTaskbarCreated();
			}
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HWND CRXSkinTrayIcon::GetMessageProcessorHWND()
{
	static HWND hWnd = NULL;
	if (!hWnd)
	{
		static const TCHAR TRAY_ICON_MESSAGE_PROCESSOR_WND_CLASSNAME[] = _T("TRAY_ICON_MESSAGE_PROCESSOR_WND_CLASS");
		HINSTANCE hInstance = m_poApp->GetInstance();

		WNDCLASSEX wc;
		wc.cbSize = sizeof(wc);
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hIconSm = NULL;
		wc.hInstance = hInstance;
		wc.lpfnWndProc = MessageProcessorWndProc;
		wc.lpszClassName = TRAY_ICON_MESSAGE_PROCESSOR_WND_CLASSNAME;
		wc.lpszMenuName = NULL;
		wc.style = 0;
		if (!RegisterClassEx(&wc))
			return NULL;

		hWnd = CreateWindowEx(
			0,
			TRAY_ICON_MESSAGE_PROCESSOR_WND_CLASSNAME,
			_T("TRAY_ICON_MESSAGE_PROCESSOR_WND"),
			WS_POPUP,
			0, 0, 0, 0,
			NULL,
			NULL,
			hInstance,
			NULL
			);
	}
	return hWnd;
}

CRXSkinTrayIcon::CRXSkinTrayIcon(const TCHAR* name, bool visible, HICON hIcon, bool destroy_icon_in_destructor)
: m_Id(GetNextTrayIconId())
, m_Name(name)
, m_hIcon(hIcon)
, m_Visible(false)
, m_DestroyIconInDestructor(destroy_icon_in_destructor)
, m_pOnMessageFunc(NULL)
, m_pListener(NULL)
{
	GetIdToTrayIconMap()[m_Id] = this;
	SetVisible(visible);
}

CRXSkinTrayIcon::~CRXSkinTrayIcon()
{
	SetVisible(false);
	SetIcon(NULL, m_DestroyIconInDestructor);
	GetIdToTrayIconMap().erase(m_Id);
}

HICON CRXSkinTrayIcon::InternalGetIcon() const
{
	return m_hIcon ? m_hIcon : ::LoadIcon(NULL, IDI_APPLICATION);
}

bool CRXSkinTrayIcon::AddIcon()
{
	NOTIFYICONDATA data;
	FillNotifyIconData(data);
	data.uFlags |= NIF_MESSAGE|NIF_ICON|NIF_TIP;
	data.uCallbackMessage = TRAY_WINDOW_MESSAGE;
	data.hIcon = InternalGetIcon();

	size_t tip_len = max(sizeof(data.szTip)-1, _tcslen(m_Name.c_str()));
	memcpy(data.szTip, m_Name.c_str(), tip_len*sizeof(TCHAR));
	data.szTip[tip_len] = 0;

	return FALSE != Shell_NotifyIcon(NIM_ADD, &data);
}

bool CRXSkinTrayIcon::RemoveIcon()
{
	NOTIFYICONDATA data;
	FillNotifyIconData(data);
	return FALSE != Shell_NotifyIcon(NIM_DELETE, &data);
}

void CRXSkinTrayIcon::OnTaskbarCreated()
{ 
	if (m_Visible)
		AddIcon();
}

void CRXSkinTrayIcon::SetName(const TCHAR* name)
{
	m_Name = name;
	if (m_Visible)
	{
		NOTIFYICONDATA data;
		FillNotifyIconData(data);
		data.uFlags |= NIF_TIP;

		size_t tip_len = max(sizeof(data.szTip)-1, _tcslen(name));
		memcpy(data.szTip, name, tip_len*sizeof(TCHAR));
		data.szTip[tip_len] = 0;

		Shell_NotifyIcon(NIM_MODIFY, &data);
	}
}

bool CRXSkinTrayIcon::SetVisible(bool visible)
{
	if (m_Visible == visible)
		return true;
	m_Visible = visible;
	if (m_Visible)
		return AddIcon();
	return RemoveIcon();
}

void CRXSkinTrayIcon::SetIcon(HICON hNewIcon, bool destroy_current_icon)
{
	if (m_hIcon == hNewIcon)
		return;
	if (destroy_current_icon && m_hIcon)
		DestroyIcon(m_hIcon);
	m_hIcon = hNewIcon;

	if (m_Visible)
	{
		NOTIFYICONDATA data;
		FillNotifyIconData(data);
		data.uFlags |= NIF_ICON;
		data.hIcon = InternalGetIcon();
		Shell_NotifyIcon(NIM_MODIFY, &data);
	}
}

bool CRXSkinTrayIcon::ShowBalloonTooltip(const TCHAR* title, const TCHAR* msg, ETooltipIcon icon)
{
#ifndef NOTIFYICONDATA_V2_SIZE
	return false;
#else
	if (!m_Visible)
		return false;

	NOTIFYICONDATA data;
	FillNotifyIconData(data);
	data.cbSize = NOTIFYICONDATA_V2_SIZE;	// win2k and later
	data.uFlags |= NIF_INFO;
	data.dwInfoFlags = icon;
	data.uTimeout = 10000;	// deprecated as of Windows Vista, it has a min(10000) and max(30000) value on previous Windows versions.

	size_t title_len = max(sizeof(data.szInfoTitle)-1, _tcslen(title));
	memcpy(data.szInfoTitle, title, title_len*sizeof(TCHAR));
	data.szInfoTitle[title_len] = 0;

	size_t msg_len = max(sizeof(data.szInfo)-1, _tcslen(msg));
	memcpy(data.szInfo, msg, msg_len*sizeof(TCHAR));
	data.szInfo[msg_len] = 0;

	return FALSE != Shell_NotifyIcon(NIM_MODIFY, &data);
#endif
}

void CRXSkinTrayIcon::OnMessage(UINT uMsg)
{
	if (m_pOnMessageFunc)
		m_pOnMessageFunc(this, uMsg);
	if (m_pListener)
		m_pListener->OnTrayIconMessage(this, uMsg);
}

void CRXSkinTrayIcon::FillNotifyIconData(NOTIFYICONDATA& data)
{
	memset(&data, 0, sizeof(data));
	// the basic functions need only V1
#ifdef NOTIFYICONDATA_V1_SIZE
	data.cbSize = NOTIFYICONDATA_V1_SIZE;
#else
	data.cbSize = sizeof(data);
#endif
	data.hWnd = GetMessageProcessorHWND();
	data.uID = m_Id;
}
