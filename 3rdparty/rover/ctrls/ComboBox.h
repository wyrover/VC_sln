#ifndef _ROVERLIB_CTRLS_COMBOBOX_H_
#define _ROVERLIB_CTRLS_COMBOBOX_H_

#include <atlcrack.h>

namespace roverlib {
	namespace ctrls {

		class ComboBox : public CWindowImpl<ComboBox,CComboBox>,public COwnerDraw<ComboBox>
		{
		public:
			BEGIN_MSG_MAP(ComboBox)
				MSG_WM_ERASEBKGND(OnEraseBkgnd)
				MSG_WM_PAINT(OnPaint)
				CHAIN_MSG_MAP_ALT(COwnerDraw<ComboBox>,1)
				DEFAULT_REFLECTION_HANDLER()
			END_MSG_MAP()

			BOOL OnEraseBkgnd(CDCHandle dc)
			{	
				return TRUE;
			}
			void OnPaint(CDCHandle)	//绘制那个按钮
			{
				CPaintDC dc(m_hWnd);
				CRect rc;
				GetClientRect(&rc);
				HBRUSH hBrush = CreateSolidBrush(RGB(200,200,250));
				dc.FillRect(&rc,hBrush);

				TCHAR str[255];
				GetWindowTextW(str,255);
				if(CString(str).IsEmpty() == FALSE)
				{
					CFont font;
					font.CreatePointFont(110,L"微软雅黑");
					dc.SelectFont(font);
					dc.SetBkMode(TRANSPARENT);
					rc.left += 5;
					dc.DrawTextW(str,-1,&rc,DT_VCENTER | DT_SINGLELINE | DT_LEFT);
				}	
				DeleteObject(hBrush);
			}
			void DrawItem(LPDRAWITEMSTRUCT lpdis)
			{
				//ATLTRACE("ComboBox::DrawItem\r\n");
				int index = lpdis->itemID;
				if(index < 0)
					return;
				CDCHandle dc(lpdis->hDC);
				CRect rc = lpdis->rcItem;
				TCHAR sz[255] = { 0 };
				GetLBText(index,sz);

				if(lpdis->itemState & ODS_SELECTED)
				{
					dc.Draw3dRect(&rc,RGB(0,0,255),RGB(0,0,255));
					dc.SetTextColor(RGB(200,0,0));
				}
				else
				{
					HBRUSH hBrush = CreateSolidBrush(RGB(255,255,255));
					dc.FillRect(&rc,hBrush);
					dc.SetTextColor(RGB(0,0,0));
					DeleteObject(hBrush);
				}
				dc.SetBkMode(TRANSPARENT);
				rc.left += 5;
				dc.DrawTextW(sz,-1,&rc,DT_VCENTER | DT_SINGLELINE | DT_LEFT);		
			}
			void MeasureItem(LPMEASUREITEMSTRUCT lpmis)
			{
				//ATLTRACE("ComboBox::MeasureItem\r\n");
				if(lpmis->itemID < 0)	//是按钮
					lpmis->itemHeight = 25;	//如果是子类化，改动无效
				else
					lpmis->itemHeight = 25;
			}
		};
	}
}

#endif
