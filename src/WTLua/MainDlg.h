// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once



lua_State *lua;

int StackDump(lua_State* L)
{
	
	std::string strMsg;
	// 得到栈的元素个数, 栈顶的位置
	int nTop = lua_gettop(L); 

	// 输出栈顶位置
	std::cout << "The Length of stack is " << nTop << std::endl;
	 
	for (int i = 1; i <= nTop; ++i)
	{
		int t = lua_type(L, i);

		// 这里的typename是把类型的枚举变成字符串，是类型名。不是栈中的位置。
		std::cout << lua_typename(L, t) << ":";		
		switch(t)
		{
		case LUA_TNUMBER:
			std::cout << lua_tonumber(L, i);			
			break;
		case LUA_TSTRING:
			std::cout << lua_tostring(L, i);			
			break;
		case LUA_TTABLE:
			//std::cout << lua_tostring(L,i) << std::endl;			
			break;
		case LUA_TFUNCTION:
			//std::cout << lua_tostring(L,i) << std::endl;			
			break;
		case LUA_TNIL:
			std::cout << "Is NULL";
			
			break;
		case LUA_TBOOLEAN:
			strMsg = lua_toboolean(L, i) ? "true" : "false";
			std::cout << strMsg;
			
			break;
		default:
			break;
		}
		std::cout << std::endl;
		
	}
	return 0;
}


static int Msg(lua_State* L)
{
	// 测试第一个参数是否为字串形式，并取得这个字串
	const char *s1 = luaL_checkstring(L, 1);     
	StackDump(L);
	MessageBoxA(NULL, s1, "caption", MB_OK);

	// 清除栈里的这个字串
	lua_pop(lua, 1);      
	StackDump(L);

	// 把返回值压进栈里
	lua_pushlstring(L, "MsgOK!", 6);  

	// 这个返回是指返回值的个数
	return 1;
}


using namespace Gdiplus;
using namespace roverlib;

class CMainDlg : public CDialogImpl<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
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



		lua = lua_open(); 
		if (lua)
		{
			luaopen_base (lua);
			luaopen_table (lua);
			luaopen_string (lua);
			luaopen_math (lua);
			luaopen_debug (lua);
			//luaopen_io (lua);

			lua_pushcfunction(lua, Msg);
			lua_setglobal(lua, "Msg");
		}


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
		DrawPic(dc.m_hDC, L"png-0070.png");		

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
		StackDump(lua);
		luaL_dofile(lua, "test.lua"); 
		StackDump(lua);
		lua_getglobal(lua, "add");
		StackDump(lua);
		lua_pushnumber(lua, 5);
		StackDump(lua);
		lua_pushnumber(lua, 6); 
		StackDump(lua);
		if (lua_pcall(lua, 2, 1, 0) != 0)
		{
			std::cout << "lua_pcall error!" << std::endl;           
		}
		StackDump(lua);
		int d = (int)lua_tonumber(lua, -1); 
		std::cout << d << std::endl;
		StackDump(lua);
		lua_pop(lua, 1);
		StackDump(lua);


		return 0;
	}
	
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		lua_close(lua);
		return 0;
	}

};
