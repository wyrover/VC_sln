#ifndef _ROVERLIB_OSUTILS_H_
#define _ROVERLIB_OSUTILS_H_



#include <fcntl.h>

#include <iostream>
#include <string>
#include <sstream>
#include <TlHelp32.h>
#include <vector>
#include <tracetool/tracetool.h>
#include "Diagnostic.h"

using namespace std;

namespace roverlib
{
	BOOL IsWindows64();
	BOOL IsCurrentUserAdmin();
	LANGID GetLangID();
	LCID SetLocale2GBK(LCID id);	
	DWORD MyFindProcess(const std::wstring& processname);
	BOOL MyGetProcessPath(DWORD dwProcessId, std::wstring& path);

	// 执行一个命令提示符命令，成功则返回命令提示符的回显字符串
	BOOL ExcuteCmd(const std::wstring& cmd, std::string& value);
	void InitConsole();
	void SetConsoleColor(unsigned short ForeColor=4,unsigned short BackGroundColor=0);
	void PrintConsoleColor();
	std::vector<std::string> get_local_ipaddress();

	// 用来注册COM组件
	bool RegSvr32(const std::wstring& filename, bool bUnReg = false);
	void CreateRegKey(const std::wstring& Key, const std::wstring& ValueName, const std::wstring& Value, HKEY RootKey = HKEY_CLASSES_ROOT);
	void DeleteRegKey(const std::wstring& Key, HKEY RootKey = HKEY_CLASSES_ROOT);
	std::wstring GetRegStringValue(const std::wstring& Key, const std::wstring& ValueName, HKEY RootKey = HKEY_CLASSES_ROOT);
}

namespace roverlib 
{
	BOOL IsWindows64()
	{
		BOOL bRet = FALSE;
		static HMODULE hKernel32 = NULL;
		if( !hKernel32 )
			hKernel32 = ::LoadLibrary(TEXT("Kernel32.DLL"));
		if( !hKernel32 )
			return FALSE;

		typedef BOOL (WINAPI *FunctionIsWow64Process)(HANDLE hProcess, PBOOL Wow64Process);
		FunctionIsWow64Process pfnIsWow64Process = NULL;
		pfnIsWow64Process = (FunctionIsWow64Process)GetProcAddress(hKernel32, "IsWow64Process");

		if (NULL == pfnIsWow64Process)
			return FALSE;

		HANDLE hCurrentProcess = GetCurrentProcess();
		pfnIsWow64Process(hCurrentProcess, &bRet);
		return bRet;
	}

	BOOL IsCurrentUserAdmin()  
	{    
		HANDLE hAccessToken;    
		BYTE * InfoBuffer = new BYTE[1024];    
		PTOKEN_GROUPS ptgGroups;    
		DWORD dwInfoBufferSize;    
		PSID psidAdministrators;    
		SID_IDENTIFIER_AUTHORITY siaNtAuthority = SECURITY_NT_AUTHORITY;    

		if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&hAccessToken))    
		{    
			delete InfoBuffer;    
			return FALSE;    
		}    

		if(!GetTokenInformation(hAccessToken,TokenGroups,InfoBuffer,1024,&dwInfoBufferSize))    
		{    
			delete InfoBuffer;    
			CloseHandle(hAccessToken);    
			return FALSE;    
		}    

		CloseHandle(hAccessToken);    

		if(!AllocateAndInitializeSid(&siaNtAuthority,    
			2,    
			SECURITY_BUILTIN_DOMAIN_RID,    
			DOMAIN_ALIAS_RID_ADMINS,    
			0,0,0,0,0,0,    
			&psidAdministrators))    
		{    
			delete InfoBuffer;    
			return FALSE;    
		}    

		ptgGroups = (PTOKEN_GROUPS)InfoBuffer;    

		for(UINT i = 0; i < ptgGroups->GroupCount; i++)    
		{    
			if(EqualSid(psidAdministrators,ptgGroups->Groups[i].Sid))    
			{    
				FreeSid(psidAdministrators);    
				delete InfoBuffer;    
				return TRUE;    
			}    
		}    
		return FALSE;    
	}  

	LANGID GetLangID()
	{
		static LANGID dwLang = 0;
		if(dwLang==0)
		{
			// GetSystemDefaultLangID();
			dwLang = GetSystemDefaultUILanguage();
			if(dwLang==2057)
				dwLang = 1033;
		}
		return dwLang;
	}

	LCID SetLocale2GBK(LCID id)
	{
		LCID old = ::GetThreadLocale();
		if(id==0)
			::SetThreadLocale( 0x0804 ); 
		return old;
	}

	DWORD MyFindProcess(const std::wstring& processname)
	{
		DWORD dwResult = 0;

		HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if ( hProcSnap != INVALID_HANDLE_VALUE )
		{
			PROCESSENTRY32 pe = { 0 };
			pe.dwSize = sizeof (pe);

			BOOL bContinue = Process32FirstW(hProcSnap, &pe);
			while ( bContinue )
			{
				if ( _tcsicmp(pe.szExeFile, processname.c_str()) == 0 )
				{
					dwResult = pe.th32ProcessID;
					break;
				}

				bContinue = Process32NextW(hProcSnap, &pe);
			}

			CloseHandle(hProcSnap);
			hProcSnap = NULL;
		}

		return dwResult;
	}

	BOOL MyGetProcessPath(DWORD dwProcessId, std::wstring& path)
	{
		BOOL bResult = FALSE;

		HANDLE hModSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
		if ( hModSnap != INVALID_HANDLE_VALUE )
		{
			MODULEENTRY32 me = { 0 };
			me.dwSize = sizeof (me);

			BOOL bContinue = Module32First(hModSnap, &me);
			if ( bContinue )			
			{				
				
				wchar_t szVal[MAX_PATH] = {0};
				GetLongPathName(me.szExePath, szVal, MAX_PATH);
				path = std::wstring(reinterpret_cast<wchar_t*>(szVal));
				bResult = TRUE;
			}

			CloseHandle(hModSnap);
			hModSnap = NULL;
		}

		return bResult;
	}

	ULARGE_INTEGER MyGetProcessUniqID(HANDLE hProcess, DWORD dwProcessID)
	{
		FILETIME tmCreatetion, tmExit, tmKernel, tmUser;
		ULARGE_INTEGER r;

		if ( hProcess == NULL )
		{
			hProcess = GetCurrentProcess();
			dwProcessID = GetCurrentProcessId();
		}

		if ( !GetProcessTimes(hProcess, &tmCreatetion, &tmExit, &tmKernel, &tmUser) )
		{
			tmCreatetion.dwHighDateTime = 0;
			tmCreatetion.dwLowDateTime = 0;
		}

		r.LowPart = dwProcessID;
		r.HighPart = tmCreatetion.dwLowDateTime;

		return r;
	}

	static HWND WindowFromPoint( LONG cx, LONG cy )
	{
		POINT pt = {cx, cy};
		return WindowFromPoint(pt);
	}

	static BOOL CheckWnd(LONG x, LONG y, HWND hWnd, DWORD dwPid)
	{
		HWND hWnd2 = WindowFromPoint(x, y);
		ATLTRACE(_T("CheckWnd hWnd: %08x, %08x pt:%d,%d\n"), hWnd, hWnd2, x, y);

		if ( hWnd2 != NULL )
		{
			if ( hWnd2 == hWnd )
			{
				return TRUE;
			}

			hWnd2 = GetAncestor(hWnd2, 2);
			if ( hWnd2 == hWnd )
			{
				return TRUE;
			}

			if ( dwPid != 0 )
			{
				DWORD dwPid2;
				return (GetWindowThreadProcessId(hWnd2, &dwPid2) && dwPid2 == dwPid);
			}
		}
		return FALSE;
	}


	inline BOOL ExcuteCmd(const std::wstring& cmd, std::string& value)
	{
		
		//定义最大的接收缓冲区大小
		const DWORD MAX_BUFFER_SIZE = 10000;
		BOOL bRet = TRUE;
		DWORD dwRet = 0;

		SECURITY_ATTRIBUTES sa;
		HANDLE hReadPipe,hWritePipe;
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		try
		{
			bRet = CreatePipe(&hReadPipe,&hWritePipe,&sa,0);
			if(!bRet)
			{
				throw("CreatePipe");
			}
			si.cb = sizeof(STARTUPINFO);
			GetStartupInfo(&si);
			si.hStdError = hWritePipe;
			si.hStdOutput = hWritePipe;
			si.wShowWindow = SW_HIDE;
			si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
			bRet = CreateProcessW(NULL, (wchar_t *)cmd.c_str(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
			if(!bRet)
			{
				throw("CreateProcess");
			}
			WaitForSingleObject(pi.hProcess,INFINITE);
			
			
			char szvalue[MAX_BUFFER_SIZE] = {0};
			bRet = ReadFile(hReadPipe, szvalue, MAX_BUFFER_SIZE, &dwRet, NULL);		
			if(!bRet)
			{
				throw("ReadFile");
			}
			value = std::string(szvalue);
		}
		catch(char c[])
		{
			ostringstream message;			 
			message << "Error at " << c << " Code:" << GetLastError() << "\n";
			TTrace::Debug()->Send(message.str().c_str());		
			bRet = FALSE;
		}
		CloseHandle(hWritePipe);
		CloseHandle(hReadPipe);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return bRet;
	}

	inline void InitConsole()
	{
		
		AllocConsole(); 
		HANDLE hin = ::GetStdHandle (STD_INPUT_HANDLE );                         
		HANDLE hout = ::GetStdHandle (STD_OUTPUT_HANDLE ); 
		SetConsoleTextAttribute(hout, 0x0a);

		int hcin = _open_osfhandle ((intptr_t )hin ,_O_TEXT );
		FILE * fpin = _fdopen(hcin ,"r" );
		*stdin = *fpin ;   //stdin 就指向了文件指针
		int hcout = _open_osfhandle((intptr_t )hout ,_O_TEXT );
		FILE * fpout = _fdopen(hcout ,"wt"); 
		*stdout = *fpout ; 
		std ::ios_base ::sync_with_stdio();      // 将iostream 流同c runtime lib 的stdio 同步，标准是同步的 
		locale::global(locale(""));
		//setlocale(LC_CTYPE, "");    // MinGW gcc.
		wcout.imbue(locale(""));
		
	}

	inline void SetConsoleColor(unsigned short usForeGroundColor,unsigned short usBackGroundColor)
	{
		HANDLE hCon;

		hCon = GetStdHandle(STD_OUTPUT_HANDLE);

		if (!SetConsoleTextAttribute(hCon,usForeGroundColor|usBackGroundColor))
		{
			GetLastError();
		}
	};


	inline void PrintConsoleColor()
	{
		unsigned short usForeGroundColor=7;     // 前景色
		unsigned short usBackGroundColor=0;     // 背景色

		for (usBackGroundColor=0; usBackGroundColor<=17; usBackGroundColor++)
		{
			for (usForeGroundColor=0; usForeGroundColor<=255; usForeGroundColor++)
			{
				SetConsoleColor(usForeGroundColor,usBackGroundColor);
				std::cout<<usForeGroundColor<<','<<usBackGroundColor<<'t';
				SetConsoleColor(7,0);

				if (15 == (usForeGroundColor))
				{
					std::cout<<endl;
				}
			}
		}
	}

		
	inline vector<std::string> get_local_ipaddress()
	{
		
		//本机的IP地址列表(从本地局域网的角度来看)
		vector<string> ips;

		char hostname[200];
		if(gethostname(hostname,200) == SOCKET_ERROR)
		{
			return ips;
		}

		hostent* hptr = gethostbyname(hostname);
		if (hptr == NULL)
		{
			return ips;
		}

		if (hptr->h_addrtype==AF_INET || 
			hptr->h_addrtype==AF_INET6)
		{
			//h_addr_list竟然定义成了char FAR * FAR *，实际上是in_addr**(历史遗留)
			//Null-terminated list of addresses for the host. Addresses are returned in network byte order. 
			in_addr **ppaddr=(in_addr **)hptr->h_addr_list;
			for (;*ppaddr!=NULL;ppaddr++)
			{
				char * ip=inet_ntoa(**ppaddr);
				if (ip!=NULL)
				{
					ips.push_back(ip);
				}
			}
		}

		return ips;
	}


	inline bool RegSvr32(const std::wstring& FileName, bool bUnReg)
	{
		if (!FileExists(FileName))
		{
			return false;
		}

		bool retval = false;
		std::string purpose;
		purpose = bUnReg ? "DllUnregisterServer" : "DllRegisterServer";

		HMODULE hLib = ::LoadLibraryW(FileName.c_str());		
		void* lProcAddress = NULL;
		HANDLE lThread;
		DWORD lSuccess;
		DWORD ExitCode;
		DWORD ThreadId;
		if (hLib)
		{
			lProcAddress = GetProcAddress(hLib, purpose.c_str());
			if (lProcAddress)
			{
				lThread = ::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)lProcAddress, NULL, NULL, &ThreadId);
				if (lThread)
				{
					lSuccess = (::WaitForSingleObject(lThread, 10000) == 0);
					if (!lSuccess)
					{
						::GetExitCodeThread(lThread, &ExitCode);
						::ExitThread(ExitCode);
						retval = false;
					}
					else				
						retval = true;
					::CloseHandle(lThread)	;
				}
			}
			::FreeLibrary(hLib);			
		}
		return retval;
	}	


	inline void CreateRegKey(const std::wstring& Key, const std::wstring& ValueName, const std::wstring& Value, HKEY RootKey)
	{
		HKEY Handle;
		LONG Status;
		DWORD Disposition;
		Status = ::RegCreateKeyExW(RootKey, Key.c_str(), 0, L"", REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &Handle, &Disposition);
		if (Status == 0)
		{
			Status = ::RegSetValueExW(Handle, ValueName.c_str(), 0, REG_SZ, (const BYTE*)Value.c_str(), (Value.length() + 1) * sizeof(wchar_t));
			::RegCloseKey(Handle);
		}
		if (Status != 0)
			ROVER_EXCEPT("Error creating system registry entry");

	}
	

	inline void DeleteRegKey(const std::wstring& Key, HKEY RootKey)
	{
		::RegDeleteKeyW(RootKey, Key.c_str());
	}
	

	inline std::wstring GetRegStringValue(const std::wstring& Key, const std::wstring& ValueName, HKEY RootKey)
	{
		HKEY RegKey;
		DWORD Size;
		std::wstring Result = L"";
		if (::RegOpenKeyW(RootKey, Key.c_str(), &RegKey) == ERROR_SUCCESS)
		{		
			wchar_t Buffer[256] = {0};
			Size = 256 * sizeof(wchar_t);
			if (::RegQueryValueExW(RegKey, ValueName.c_str(), NULL, NULL, PBYTE(Buffer), &Size) == ERROR_SUCCESS)
			{
				Result = std::wstring(Buffer);
			}
			::RegCloseKey(RegKey);
		}

		return Result;

	}	

	inline void RegisterAsService(const std::wstring& ClassID, const std::wstring& ServiceName)
	{
		CreateRegKey(L"AppID\\" + ClassID, L"LocalService", ServiceName);
		CreateRegKey(L"CLSID\\" + ClassID, L"AppID", ClassID);
	}

	inline void AddToRecentDocs(const std::wstring& FileName)
	{
		::SHAddToRecentDocs(SHARD_PATH, FileName.c_str());
	}


	inline bool LoadModule(HMODULE& Module, const std::wstring& FileName)
	{
		bool Result;
		if (Module == NULL)
		{
			Module = LoadLibraryW(FileName.c_str());
			Result = (Module != NULL);
		}
		return Result;
	}


	inline bool LoadModuleEx(HMODULE& Module, const std::wstring& FileName, DWORD Flags)
	{
		bool Result;
		if (Module == NULL)
		{
			Module = LoadLibraryExW(FileName.c_str(), 0, Flags);
			Result = (Module != NULL);
		}
		return Result;
	}

	inline void UnloadModule(HMODULE& Module)
	{
		if (Module != NULL)
		{
			::FreeLibrary(Module);
			Module = NULL;
		}
	}


	inline void* GetModuleSymbol(const HMODULE& Module, const std::string& SymbolName)
	{
		void* Result = NULL;
		if (Module != NULL)
			Result = GetProcAddress(Module, SymbolName.c_str());
		return Result;
	}

	inline void* GetModuleSymbolEx(const HMODULE& Module, const std::string& SymbolName, bool& Accu)
	{
		void* Result = NULL;
		if (Module != NULL)
		{
			Result = GetProcAddress(Module, SymbolName.c_str());
			Accu = Accu & (Result != NULL);
		}
		return Result;	
	}


	inline bool ExpandEnvironmentVar(std::wstring& Value)
	{
		bool Result;
		int R;
		wchar_t Expanded[1];
		R = ExpandEnvironmentStringsW(Value.c_str(), Expanded, 0);
		wchar_t* pExpanded = new wchar_t[R];
		memset(pExpanded, 0, R);
		Result = (ExpandEnvironmentStringsW(Value.c_str(), pExpanded, R) != 0);
		if (Result)
			Value = pExpanded;
		delete[] pExpanded;
		return Result;
	}

	inline bool GetEnvironmentVar(const std::wstring& Name, std::wstring& Value, bool Expand)
	{
		bool Result;
		DWORD R;
		R = ::GetEnvironmentVariableW(Name.c_str(), NULL, 0);
		wchar_t* pBuffer = new wchar_t[R + 1];
		memset(pBuffer, 0, R + 1);
		R = ::GetEnvironmentVariableW(Name.c_str(), pBuffer, R);
		Result = (R != 0);
		if (!Result)
			Value = L"";
		else
		{
			Value = pBuffer;
		
			if (Expand)
				ExpandEnvironmentVar(Value);
		}

		return Result;
	}


	inline bool GetEnvironmentVar(const std::wstring& Name, std::wstring& Value)
	{
		return GetEnvironmentVar(Name, Value, True);
		
	}
	
	inline bool SetEnvironmentVar(const std::wstring& Name, const std::wstring& Value)
	{
		return (::SetEnvironmentVariableW(Name.c_str(), Value.c_str()) == TRUE);
	}


	typedef struct _SHNOTIFYSTRUCT
	{
		LPITEMIDLIST dwItem1;
		LPITEMIDLIST dwItem2;
	}SHNOTIFYSTRUCT, *PSHNOTIFYSTRUCT;	
	

	inline bool SHNotify_Register(HWND hWnd, DWORD& hSHNotify, LPITEMIDLIST& pidlDesktop)
	{
		SHChangeNotifyEntry ps;
		

		if (hSHNotify == 0)
		{
			// ::SHGetSpecialFolderLocation(0, CSIDL_DESKTOP, &g_pidlDesktop);
			::SHGetSpecialFolderLocation(0, CSIDL_DRIVES, &pidlDesktop);			
			if (pidlDesktop)
			{
				ps.pidl = pidlDesktop;
				ps.fRecursive = TRUE;
				hSHNotify = ::SHChangeNotifyRegister(hWnd, SHCNF_TYPE | SHCNF_IDLIST, SHCNE_ALLEVENTS | SHCNE_INTERRUPT, 0x401, 1, &ps);
				return hSHNotify != 0;
			}
			else
			{
				::CoTaskMemFree(pidlDesktop);
			}

		}

		return false;
	}

	inline bool SHNotify_Unregister(DWORD& hSHNotify, LPITEMIDLIST& pidlDesktop)
	{
		bool Result = false;

		if (hSHNotify)
		{
			if (::SHChangeNotifyDeregister(hSHNotify))
			{
				hSHNotify = 0;
				::CoTaskMemFree(pidlDesktop);
				Result = true;
			}
		}

		return Result;
	}

}

#endif