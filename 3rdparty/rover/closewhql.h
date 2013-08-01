#ifndef _ROVERLIB_CLOSEWHQL_H_
#define _ROVERLIB_CLOSEWHQL_H_

#include <windows.h>

namespace roverlib
{
	//在 vista 下需要有 管理员权限才能正常使用

	typedef struct tagEnumReturn
	{
		TCHAR * tszName;
		BOOL   bRetCode;
	}EnumReturn;

	class CCloseWHQL
	{

	public:

		CCloseWHQL();
		~CCloseWHQL();

		//外部只需调用下面两个函数
		BOOL StartCloseThread();
		void EndCloseThread();

		void SetDlgName(LPTSTR lpDlgName);
		static DWORD WINAPI _CloseThread( LPVOID lpParameter );

	protected:

		HANDLE m_hThread;

	};
}

namespace roverlib
{
	wchar_t* g_pszWindow[8][2] = { 
		{_T("硬件安装"), _T("仍然继续(&C)")},
		{_T("Hardware Installation"), _T("&Continue Anyway")},
		{_T("软件安装"), _T("仍然继续(&C)")},
		{_T("Sofeware Installation"), _T("&Continue Anyway")},
		{_T("没有找到数字签名"), _T("是(&Y)")},
		{_T("Digital Signature Not Found") _T("Yes(&Y)")},
		{_T("Windows 安全"), _T("安装(&I)")},
		{_T("Windows Security"), _T("Installation(&I)")}
	};

	BOOL CALLBACK EnumWindowsProc( HWND hWnd, LPARAM lParam )
	{
		BOOL bFindFlat = FALSE;
		TCHAR tszCaption[MAX_PATH] = {0};
		TCHAR *ptszPos = NULL;
		TCHAR *ptszChr = (TCHAR*) lParam;
		EnumReturn *pER = (EnumReturn*) lParam;

		::GetWindowText( hWnd , tszCaption, MAX_PATH );
		ptszPos = wcsstr( tszCaption, pER->tszName );
		if ( ptszPos )
			SendMessage( hWnd, BM_CLICK, 0, 0 );
		else
			SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(0x14B7, BN_CLICKED), NULL);

		return TRUE;
	}

	inline CCloseWHQL::CCloseWHQL()
		:m_hThread(NULL)
	{

	}


	inline CCloseWHQL::~CCloseWHQL()
	{
		EndCloseThread();
	}

	inline BOOL CCloseWHQL::StartCloseThread()
	{
		m_hThread = ::CreateThread( NULL, 0 , (LPTHREAD_START_ROUTINE)_CloseThread, NULL, 0, NULL );
		return m_hThread != NULL;
	}

	DWORD WINAPI CCloseWHQL::_CloseThread( LPVOID lpParameter )
	{
		BOOL	bRetCode = FALSE;
		HWND	hWnd = NULL;
		EnumReturn enumReturn;

		while ( true )
		{
			ZeroMemory( &enumReturn, sizeof(enumReturn) );

			for ( int nIndex = 0; nIndex < 8; nIndex++ )
			{
				hWnd = ::FindWindow( _T("#32770"), g_pszWindow[nIndex][0]);
				if ( hWnd )
				{
					enumReturn.tszName = g_pszWindow[nIndex][1];
					enumReturn.bRetCode = FALSE;
					EnumChildWindows( hWnd, EnumWindowsProc, (LPARAM)&enumReturn );
				}
			}
			::Sleep( 100 );
		}

		return true;
	}

	inline void CCloseWHQL::EndCloseThread()
	{
		if ( m_hThread )
		{
			::TerminateThread(m_hThread, 0);
			::CloseHandle(m_hThread);
			m_hThread = NULL;
		}
	}
}



#endif