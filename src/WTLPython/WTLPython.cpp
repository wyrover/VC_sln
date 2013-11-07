// WTLPython.cpp : main source file for WTLPython.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>


#include "resource.h"

#include "MainDlg.h"

using namespace Gdiplus;


CAppModule _Module;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	InitConsole();

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken; 
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);


	

	Py_Initialize ();//初始化python环境
	if(!Py_IsInitialized())
	{
		cout<<"------python初始化失败"<<endl;
		return 0;
	}

	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");

	  


	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = 0;
	// BLOCK: Run application
	{
		CMainDlg dlgMain;
		nRet = dlgMain.DoModal();
	}

	_Module.Term();


	Py_Finalize();

	GdiplusShutdown(gdiplusToken);

	::CoUninitialize();

	return nRet;
}
