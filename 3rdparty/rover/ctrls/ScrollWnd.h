#ifndef _ROVERCTRLS_SCROLLWND_H_
#define _ROVERCTRLS_SCROLLWND_H_

#include <atlcrack.h>
#include <GdiPlus.h>
using namespace Gdiplus;

namespace roverlib {
	namespace ctrls {

		class ScrollWnd : public CWindowImpl<ScrollWnd,CWindow>  //MFC中直接继承CWnd，WTL的精髓就是模板
		{
		private:
			const int LINE;  //定义移动的距离
		public:
			BEGIN_MSG_MAP(ScrollWnd)
				MSG_WM_VSCROLL(OnVScroll)
				MSG_WM_HSCROLL(OnHScroll)
			END_MSG_MAP()
		public:
			ScrollWnd() : LINE(12) {  }
			void OnVScroll(UINT nSBCode,UINT,CScrollBar)
			{
				SCROLLINFO sif = { sizeof(SCROLLINFO),SIF_ALL };
				GetScrollInfo(SB_VERT,&sif);

				int oldPos = sif.nPos;
				switch(nSBCode)
				{
				case SB_TOP:	sif.nPos = sif.nMin;	break;
				case SB_BOTTOM:	sif.nPos = sif.nMax;	break;
				case SB_LINEUP:		sif.nPos -= LINE;	break;
				case SB_LINEDOWN:	sif.nPos += LINE;	break;
				case SB_PAGEUP:		sif.nPos -= sif.nPage;	break;
				case SB_PAGEDOWN:	sif.nPos += sif.nPage;	break;
				case SB_THUMBTRACK:	sif.nPos = sif.nTrackPos;	break;
				default:	break;
				}
				if(sif.nPos > sif.nMax)
					sif.nPos = sif.nMax;
				else if(sif.nPos < sif.nMin)
					sif.nPos = sif.nMin;
				sif.fMask = SIF_POS;
				SetScrollInfo(SB_VERT,&sif);

				GetScrollInfo(SB_VERT,&sif);
				if(sif.nPos != oldPos)
					ScrollWindow(0,oldPos - sif.nPos);	//这里是重点
			}
			void OnHScroll(UINT nSBCode,UINT,CScrollBar)
			{
				SCROLLINFO sif = { sizeof(SCROLLINFO),SIF_ALL };
				GetScrollInfo(SB_HORZ,&sif);

				int oldPos = sif.nPos;
				switch(nSBCode)
				{
				case SB_LEFT:	sif.nPos = sif.nMin;	break;
				case SB_RIGHT:	sif.nPos = sif.nMax;	break;
				case SB_LINELEFT:	sif.nPos -= LINE;	break;
				case SB_LINERIGHT:	sif.nPos += LINE;	break;
				case SB_PAGELEFT:	sif.nPos -= sif.nPage;	break;
				case SB_PAGERIGHT:	sif.nPos += sif.nPage;	break;
				case SB_THUMBTRACK:	sif.nPos = sif.nTrackPos;	break;
				default:	break;
				}
				if(sif.nPos > sif.nMax)
					sif.nPos = sif.nMax;
				else if(sif.nPos < sif.nMin)
					sif.nPos = sif.nMin;
				sif.fMask = SIF_POS;
				SetScrollInfo(SB_HORZ,&sif);

				GetScrollInfo(SB_HORZ,&sif);
				if(sif.nPos != oldPos)
					ScrollWindow(oldPos - sif.nPos,0);
			}
			void ResetThumbHeight(HWND wndChild)
			{
				SCROLLINFO sif = { sizeof(SCROLLINFO),SIF_ALL }, sif2 = sif;
				GetScrollInfo(SB_VERT,&sif);	GetScrollInfo(SB_HORZ,&sif2);

				CRect rc_client,rc_child;
				::GetWindowRect(wndChild,&rc_child);
				GetClientRect(&rc_client);

				if(rc_client.Height() >= rc_child.Height())	//不需要垂直滚动条的情况
				{
					ShowScrollBar(SB_VERT,FALSE);
					::SetWindowPos(wndChild,NULL,0,0,rc_client.Width(),rc_child.Height(),SWP_NOMOVE);
					return;
				}

				if(rc_client.Width() >= rc_child.Width())	//不需要水平滚动条的情况
				{
					ShowScrollBar(SB_HORZ,FALSE);
					::SetWindowPos(wndChild,NULL,0,0,rc_client.Width(),rc_child.Height(),SWP_NOMOVE);
					return;
				}

				sif.nPage = rc_client.Height();		sif2.nPage = rc_client.Width();
				sif.nMin = 0;						sif2.nMin = 0;
				sif.nMax = rc_child.Height();		sif2.nMax = rc_child.Width();
				sif.nPos = 0;						sif2.nPos = 0;

				SetScrollInfo(SB_VERT,&sif);		SetScrollInfo(SB_HORZ,&sif2);
			}
		};

	}
}

#endif