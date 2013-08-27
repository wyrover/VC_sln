// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

static int PrintLSNumber(LuaState* state)
{
	LuaStack args(state);

	// Verify it is a number and print it.
	if (args[1].IsNumber()) {
		std::cout << args[1].GetNumber() << std::endl;		
	}
	// No return values.
	return 0;
}


using namespace Gdiplus;
using namespace roverlib;

class CMainDlg : public CDialogImpl<CMainDlg>
{
private:
	LuaStateOwner state;
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


		
		state->GetGlobals().Register("PrintNumber", PrintLSNumber);

		int iret = state->DoFile("test2.lua");

		

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
		// Get a global variable:
		LuaObject sObj = state->GetGlobal("health");
		int mytest = sObj.GetInteger();
		std::cout << "init value in luascript:" << mytest << std::endl;
		

		//Update the value:
		sObj.AssignInteger(state, 50);
		//Get value again:
		mytest = sObj.GetInteger();
		std::cout << "after changed by c++:" << mytest << std::endl;
		

		//Call a function in lua:
		LuaFunction<int> Add =  state->GetGlobal("Add");
		int myret = Add(3, 4);
		std::cout << "Call add() in luascript,ret=" << myret << std::endl;
		

		return 0;
	}

};
