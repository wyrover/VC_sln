// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
using namespace Gdiplus;
using namespace roverlib;

BOOL WINAPI myEnumerateFunc(const wchar_t* lpFileOrPath, void* pUserData)
{
	const wchar_t* pdot;
	if((pdot = wcschr(lpFileOrPath, L'.')) && wcsicmp(pdot, L".mp3") == 0)
	{
		std::wcout << lpFileOrPath << std::endl;
	}
	return TRUE;
}


class CMainDlg : public CDialogImpl<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

	BEGIN_MSG_MAP(CMainDlg)
		COMMAND_ID_HANDLER(IDC_BUTTON5, OnButton5)
		COMMAND_ID_HANDLER(IDC_BUTTON4, OnButton4)
		COMMAND_ID_HANDLER(IDC_BUTTON3, OnButton3)
		COMMAND_ID_HANDLER(IDC_BUTTON2, OnButton2)
		COMMAND_ID_HANDLER(IDC_BUTTON1, OnButton1)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		//COMMAND_HANDLER(IDC_BUTTON5, BN_CLICKED, OnBnClickedButton5)
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
		system("cls");

		string str1 = "abcdefg";
		string str2 = "defg";

		if (endswith(str1, str2))
			std::cout << "endswith success." << std::endl;


		VectorFileInfo filelist;		
		getSystemFileList(filelist, GetAppPath(), L"*.*");
		VectorFileInfo::const_iterator iter;
		for (iter = filelist.begin(); iter != filelist.end(); ++iter)
		{
			std::wcout << (*iter).folder << L"-----" << (*iter).name << std::endl;
		}


		std::vector<wstring> filelist2;
		scanFolder(filelist2, GetAppPath(), true, L"*.*", true);
		std::vector<wstring>::const_iterator iter2;
		for (iter2 = filelist2.begin(); iter2 != filelist2.end(); ++iter2)
		{

			std::wcout << (*iter2) << std::endl;
		}
		
		//ROVER_ASSERT(str1 == str2, "字符串不相等");

		doFileEnumeration(L"G:\\music", TRUE, TRUE, myEnumerateFunc, NULL);


		return 0;
	}

	LRESULT OnButton2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		std::wcout << GetShellFolderPath(0x25) << std::endl;

		if (FileExists(GetAppFileName()))
			std::wcout << GetAppFileName() << std::endl;

		std::wcout << IncludeTrailingPathDelimiter(L"C:\\test\\test") << std::endl;

		std::wcout << ExtractFilePath(GetAppFileName()) << std::endl;

		std::wcout << ExtractFileName(GetAppFileName()) << std::endl;

		ForceDirectories(ExtractFilePath(GetAppFileName()) + L"test1\\test2\\test3");

		std::wcout << ChangeFileExt(L"test.mp3", L"txt") << std::endl;

		return 0;
	}

	LRESULT OnButton3(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		std::wcout << CreateClassID() << std::endl;

		std::wstring value = GetRegStringValue(L"SOFTWARE\\Microsoft\\Internet Explorer", L"svcKBFWLink", HKEY_LOCAL_MACHINE);

		std::wcout << value << std::endl;

		return 0;
	}

	LRESULT OnButton4(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		int FileHandle;
		if (!FileExists(GetAppPath() + L"test1.log"))
		{
			FileHandle = FileCreate(GetAppPath() + L"test1.log");	
			std::wstring output = L"asl的萨克记分卡技术的看法";
			WORD a = 0xfeff; // Unicode文件头			
			FileWrite(FileHandle, (const BYTE*)&a, 2);
			FileWrite(FileHandle, (const BYTE*)output.c_str(), (output.length())*sizeof(wchar_t));			
			FileClose(FileHandle);
		}


		TFileStream tmpFileStream(GetAppPath() + L"test2.log", fmCreate);
		std::wstring output2 = L"大师的发生的";
		WORD b = 0xfeff; // Unicode文件头	
		tmpFileStream.Write((const BYTE*)&b, 2);
		tmpFileStream.Write((const BYTE*)output2.c_str(), (output2.length())*sizeof(wchar_t));
		

		return 0;
	}
	LRESULT OnButton5(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		//std::wcout << roverlib::PathBuildRoot('c') << std::endl;
		return 0;
	}
	
};


