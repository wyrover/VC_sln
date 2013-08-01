#ifndef _ROVERCTRLS_IMAGECTRL_H_
#define _ROVERCTRLS_IMAGECTRL_H_

#include <atlcrack.h>
#include <GdiPlus.h>
using namespace Gdiplus;

namespace roverlib {
	namespace ctrls {

		class ImageCtrl : public CWindowImpl<ImageCtrl>
		{
		private:
			CString m_strImagePath;
		public:
			BEGIN_MSG_MAP(ImageCtrl)
				MSG_WM_PAINT(OnPaint)
			END_MSG_MAP()
		public:
			ImageCtrl()
			{
				m_strImagePath = L"";
			}
			void DrawImage(CString str_image_path)
			{
				m_strImagePath = str_image_path;
				RedrawWindow();
			}
			void OnPaint(CDCHandle)
			{	
				CPaintDC dc(m_hWnd);
				if(m_strImagePath.IsEmpty())
					return;
				CRect rc;
				GetClientRect(&rc);
				CMemoryDC mem(dc,CRect(0,0,rc.Width(),rc.Height()));				
				Graphics g(mem);
				Image img(m_strImagePath);
				g.DrawImage(&img,0,0,rc.Width(),rc.Height());	
			}
		};

	}
}

#endif