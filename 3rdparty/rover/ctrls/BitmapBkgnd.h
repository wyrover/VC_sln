#ifndef _ROVERLIB_CTRLS_BITMAPBKGND_H_
#define _ROVERLIB_CTRLS_BITMAPBKGND_H_

/* usage:
1.包含类的头文件；  
2.让对话框类公有继承：public CBitmapBkgnd<CMainDlg, IDB_MAIN_BKGND>;  
3.在消息映射之前：typedef CBitmapBkgnd<CMainDlg,IDB_MAIN_BKGND> CBmpBkgndBase;  
4.在消息映射内：CHAIN_MSG_MAP(CBmpBkgndBase)  
*/

namespace roverlib {
	namespace ctrls {

		template <class T, UINT uBitmapID>
		class CBitmapBkgnd : public CMessageMap
		{
		public:
			CBitmapBkgnd() { m_Bitmap.LoadBitmap(uBitmapID); }
			~CBitmapBkgnd() { m_Bitmap.DeleteObject(); }
			BEGIN_MSG_MAP(CPaintBkgnd)
				MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
			END_MSG_MAP()

			LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
			{
				T*   pT = static_cast<T*>(this);
				HDC  hDC = (HDC) wParam;
				RECT rcClient;
				pT->GetClientRect ( &rcClient );

				BITMAP bm;
				m_Bitmap.GetBitmap(&bm);
				CDC memDC;
				memDC.CreateCompatibleDC(hDC);
				HBITMAP hOldBmp = memDC.SelectBitmap(m_Bitmap);
				StretchBlt(hDC,0,0,rcClient.right - rcClient.left,rcClient.bottom - rcClient.top,memDC,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
				memDC.SelectBitmap(hOldBmp);
				memDC.DeleteDC();
				return 1;    // we painted the background
			}
		protected:
			CBitmap m_Bitmap;
		};
	}

}


#endif