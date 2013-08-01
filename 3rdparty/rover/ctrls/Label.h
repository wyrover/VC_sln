#ifndef _ROVERLIB_CTRLS_LABEL_H_
#define _ROVERLIB_CTRLS_LABEL_H_

namespace roverlib {
	namespace ctrls {

		template<class T, class TBase = CStatic, class TWinTraits = CControlWinTraits>
		class ATL_NO_VTABLE CLabel : public CWindowImpl<T, TBase, TWinTraits>
		{
		public:
			typedef CLabel<T, TBase, TWinTraits> thisClass;
			COLORREF m_clrText;
			COLORREF m_bkColor;
			COLORREF m_clrLine;
			BOOL m_bHorizontal;
			BOOL m_bActive;
			BOOL m_bUnderLine;
			CFont m_font;
			CBitmap m_bmp;
			CLabel(void) :
			m_clrText(RGB(104, 115, 122)),
				m_bkColor(RGB(255, 255, 255)),
				m_clrLine(RGB(104, 115, 122)),
				m_bActive(FALSE),
				m_bHorizontal(FALSE),
				m_bUnderLine(FALSE){ }
			~CLabel(void){ }
			BOOL SubclassWindow(HWND hwnd)
			{
				ATLASSERT(m_hWnd == NULL);
				ATLASSERT(::IsWindow(hwnd));
				BOOL bRet = CWindowImpl<T, TBase>::SubclassWindow(hwnd);
				if (bRet)
				{
					InitMyStatic();
				}
				return bRet;
			}
			void SetTextColor(COLORREF clrText)
			{
				m_clrText = clrText;
				if (IsWindow())
				{
					Invalidate();
				}
			}
			void SetLabelBkColor(COLORREF clrBk)
			{
				m_bkColor = clrBk;
				if (IsWindow())
				{
					Invalidate();
				}
			}
			void SetActive(BOOL bActive = FALSE)
			{
				m_bActive = bActive;
				if (IsWindow())
				{
					Invalidate();
				}
			}
			void SetHorizontalFill(BOOL bHoriz = FALSE)
			{
				m_bHorizontal = bHoriz;
				if (IsWindow())
				{
					Invalidate();
				}
			}
			void SetUnderLine(COLORREF clrLine, BOOL bUnderLine = FALSE)
			{
				m_clrLine = clrLine;
				m_bUnderLine = bUnderLine;
				if (IsWindow())
				{
					Invalidate();
				}
			}

			void InitMyStatic()
			{
				ATLASSERT(::IsWindow(m_hWnd));
				TCHAR lpszBuffer[10] = { 0 };
				if( ::GetClassName(m_hWnd, lpszBuffer, 9) )
				{
					if( ::lstrcmpi(lpszBuffer, TBase::GetWndClassName()) == 0 )
					{
						ModifyStyle(0, SS_NOTIFY);  // We need this
						DWORD dwStyle = GetStyle() & 0x000000FF;
						if( dwStyle == SS_ICON || dwStyle == SS_BLACKRECT || dwStyle == SS_GRAYRECT ||
							dwStyle == SS_WHITERECT || dwStyle == SS_BLACKFRAME || dwStyle == SS_GRAYFRAME ||
							dwStyle == SS_WHITEFRAME || dwStyle == SS_OWNERDRAW ||
							dwStyle == SS_BITMAP || dwStyle == SS_ENHMETAFILE )
						{
							ATLASSERT("Invalid static style for gradient label"==NULL);
						}
					}
				}
				CWindow wnd = GetParent();
				CFontHandle font = wnd.GetFont();
				if (!font.IsNull())
				{
					LOGFONT lf;
					font.GetLogFont(&lf);
					lf.lfWeight = FW_BOLD;
					//lf.lfUnderline = TRUE;
					m_font.CreateFontIndirect(&lf);
					SetFont(m_font);
				}
			}
			BEGIN_MSG_MAP(thisClass)
				MESSAGE_HANDLER(WM_CREATE, OnCreate)
				MESSAGE_HANDLER(WM_PAINT, OnPaint)
				MESSAGE_HANDLER(WM_PRINTCLIENT, OnPaint)
				MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
				MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
				MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
				MESSAGE_HANDLER(WM_MOUSEHOVER, OnMouseHover)
				MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
				DEFAULT_REFLECTION_HANDLER()
			END_MSG_MAP()
			LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
			{
				InitMyStatic();
				return 0;
			}
			LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
			{
				return 1;
			}
			LRESULT OnSetCursor(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
			{
				HCURSOR hcur = LoadCursor(NULL, IDC_HAND);
				::SetCursor(hcur);
				return 0;
			}
			LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
			{
				T* pt = static_cast<T*>(this);
				if (wParam != NULL)
				{
					pt->DoPaint((HDC)wParam);
				}
				else
				{
					CPaintDC dc(m_hWnd);
					pt->DoPaint((HDC)dc);
				}
				return 0;
			}
			LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& hHandled)
			{
				int xPos = LOWORD(lParam);
				int yPos = HIWORD(lParam);
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(tme);
				tme.hwndTrack = m_hWnd;
				tme.dwFlags = TME_LEAVE | TME_HOVER;
				tme.dwHoverTime = 1;
				_TrackMouseEvent(&tme);

				return 0;
			}
			LRESULT OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& hHandled)
			{
				SetUnderLine(RGB(104, 115, 122), TRUE);
				return 0;
			}
			LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& hHandled)
			{
				SetUnderLine(RGB(255, 255, 255), TRUE);
				return 0;
			}
			void DoPaint(CDCHandle dc)
			{
				int nTextLeft;
				int nTextBottom;
				TEXTMETRIC tm;
				CPen underLinePen, oldLinePen;
				HPEN hLinePen;
				RECT rcClient;
				GetClientRect(&rcClient);
				if (m_bActive)
				{
					DoBkFill(dc, rcClient);
				}
				dc.GetTextMetricsW(&tm);
				dc.SetBkMode(TRANSPARENT);
				dc.SetTextColor(m_clrText == CLR_INVALID ? ::GetSysColor(COLOR_CAPTIONTEXT) : m_clrText);
				HFONT hOldFont = dc.SelectFont(GetFont());
				int nLen = GetWindowTextLength();
				if (nLen > 0)
				{
					LPTSTR lpszText = (LPTSTR)_alloca((nLen + 1) * sizeof(TCHAR));
					if (GetWindowText(lpszText, nLen + 1))
					{
						DWORD dwStyle = GetStyle();
						int nDrawStyle = DT_LEFT;
						if (dwStyle & SS_CENTER)
						{
							nDrawStyle = DT_CENTER;
						}
						else if (dwStyle & SS_RIGHT)
						{
							nDrawStyle = DT_RIGHT;
						}
						if( dwStyle & SS_SIMPLE)
						{
							nDrawStyle = DT_VCENTER | DT_SINGLELINE;
						}

						dc.DrawText(lpszText, -1, &rcClient, nDrawStyle | DT_WORDBREAK);
					}
				}
				dc.SelectFont(hOldFont);
				if (m_bUnderLine)
				{
					//SetMyFont();
					nTextLeft = rcClient.left - 2;
					nTextBottom = rcClient.bottom - 3;
					hLinePen = underLinePen.CreatePen(PS_SOLID, 1, m_clrLine);
					oldLinePen = dc.SelectPen(hLinePen);
					dc.MoveTo(nTextLeft, nTextBottom);
					dc.LineTo(nTextLeft + tm.tmAveCharWidth * nLen * 2, nTextBottom);
					dc.SelectPen(oldLinePen);
				}
			}
			void DoBkFill(CDCHandle& dc, const RECT& rc)
			{
				int nShift = 6;
				int nSteps = 1 << nShift;
				//for (int i = 0; i < nSteps; i++)
				//{
				//	CBrush br;
				//	br.CreateSolidBrush(m_bkColor);
				//	RECT r2 = rc;
				//	if (m_bHorizontal)
				//	{
				//		r2.bottom =rc.bottom - ((i * (rc.bottom - rc.top)) >> nShift);
				//		r2.top = rc.bottom - (((i + 1) * (rc.bottom - rc.top)) >> nShift);
				//		if ((r2.bottom - r2.top) > 0)
				//		{
				//			dc.FillRect(&r2, br);
				//		}
				//	}
				//	else
				//	{
				//		r2.left = rc.left + ((i * (rc.right - rc.left)) >> nShift);
				//		r2.right = rc.left + (((i + 1) * (rc.right - rc.left)) >> nShift);
				//		if ((r2.right - r2.left) > 0)
				//		{
				//			dc.FillRect(&r2, br);
				//		}
				//	}
				//}
				//BITMAP bm;
				//m_bmp.LoadBitmap(IDB_BITMAP1);
				//m_bmp.GetBitmap(&bm);
				//CDC memDC;
				//memDC.CreateCompatibleDC(dc);
				//HBITMAP hOldBmp = memDC.SelectBitmap(m_bmp);
				//dc.BitBlt(0, 0, rc.right - rc.left, rc.bottom - rc.top, memDC, 0, 0, SRCCOPY);
				//memDC.SelectBitmap(hOldBmp);
				//memDC.DeleteDC();
			}
		};
		class CCoolLabel : public CLabel<CCoolLabel>
		{
		public:
			DECLARE_WND_CLASS(L"WTL_CoolLabel");
		};
	}
}

#endif