#ifndef _ROVERLIB_CTRLS_CTRLCOLOR_H_
#define _ROVERLIB_CTRLS_CTRLCOLOR_H_

namespace roverlib {
	namespace ctrls {

		template <class T, COLORREF t_crTextColor = RGB(0, 0, 0), COLORREF t_crBkgnd = RGB(255,255,255)>
		class CCtrlColor
		{
		public:
			CCtrlColor() { m_brBkgnd = CreateSolidBrush(t_crBkgnd);}
			~CCtrlColor() { DeleteObject(m_brBkgnd);}

			BEGIN_MSG_MAP(CCtrlColor)
				MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColor)
				MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColor)
			END_MSG_MAP()
			LRESULT OnCtlColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
			{
				HDC hDC = (HDC)wParam;
				HWND hWnd = (HWND)lParam;
				if(uMsg == WM_CTLCOLORDLG)
				{
					bHandled = TRUE;
					return (LRESULT)m_brBkgnd;
				}
				else if(uMsg == WM_CTLCOLORSTATIC)
				{	
					SetBkMode(hDC,TRANSPARENT);
					SetTextColor(hDC,t_crTextColor);

					bHandled = TRUE;
					return (LRESULT)m_brBkgnd;
				}
				else
					return 0;
			}

		protected:
			HBRUSH m_brBkgnd;
		};
	}
}


#endif