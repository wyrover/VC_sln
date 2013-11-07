// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
using namespace Gdiplus;
using namespace roverlib;
using namespace boost::python;

class CMainDlg : public CDialogImpl<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

	BEGIN_MSG_MAP(CMainDlg)
		COMMAND_ID_HANDLER(IDC_BUTTON1, OnButton1)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
		SetIcon(hIconSmall, FALSE);

		return TRUE;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: Add validation code 
		EndDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}
	
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{		
		WTL::CPaintDC dc(m_hWnd);
		Graphics g(dc.m_hDC);
		DrawPic(dc.m_hDC, L"img\\png-0070.png");		

		return 0;
	}

	void DoPaint(Graphics& g)
	{		
		CRect rect;
		GetClientRect(&rect);

		Image im(L"img\\png-0001.png");
		g.DrawImage(&im, 0, 0, im.GetWidth(), im.GetHeight());			
	
		std::wstring message;
		message = L"adsfadsf";
		::TextOutW(g.GetHDC(), rect.Width()/2, rect.Height()/2, message.c_str(), message.length());
		
	
		
	}
	LRESULT OnButton1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		std::cout << BIN2HEX("1111") << std::endl;
		std::cout << HEX2BIN("FFFA") << std::endl;


		object mainModule;//main模块
		object mainNamespace;//main命名空间
		try
		{
			mainModule = import("__main__");//导入__main__模块
			mainNamespace = mainModule.attr("__dict__");

			//直接执行
			exec("import os", mainNamespace, mainNamespace);
			exec("print os.getcwd()", mainNamespace, mainNamespace);

			//间接执行
			object mod = mainModule.attr("os");
			object foo = mod.attr("getcwd");
			string dir = extract<string>(foo());
			cout<<"cur dir:"<<dir<<endl;

			mod = mod.attr("path");
			foo = mod.attr("isfile");
			bool r = extract<bool>(foo("testPython.cpp"));
			cout<<"file exsit:"<<r<<endl;


			boost::python::str extractpy = "Noname4.py";		
			object extractModule = exec_file(extractpy, mainNamespace, mainNamespace);
			
			object test = mainNamespace["test"];
			test();



		}
		catch(...)
		{
			if (PyErr_Occurred())
				PyErr_Print();
		}

	
		




		return 0;
	}

};
