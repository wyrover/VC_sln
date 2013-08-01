#ifndef _ROVERCTRLS_GIFWND_H_
#define _ROVERCTRLS_GIFWND_H_

#include <atlcrack.h>
#include <GdiPlus.h>
using namespace Gdiplus;

namespace roverlib {
	namespace ctrls {

		/* GIF �ؼ� */

		typedef CWinTraits<WS_VISIBLE | WS_CHILD, 0> GifTraits;  //���ƴ�����ʽ

		class GifWnd : public CWindowImpl<GifWnd, CStatic, GifTraits>  //���˴��̳�CStatic�Ϳ����϶����ڣ���������Ӧ��NCHITTEST��Ϣ��ʱ���ˣ�
		{
		public:
			GifWnd()  //��ʼ��gdi+    ���ֶ�gdi+�����ĳ�ʼ�����ͷ�������������һ�ξ����ˡ���ˣ��ɿ��Ƿ�װ��һ�����У��˴���Ϊ�˼�㡣
			{
				GdiplusStartupInput in;
				GdiplusStartup(&token,&in,NULL);
			}
			~GifWnd()  //�ͷ�gdi+
			{
				GdiplusShutdown(token);
			}
			void Load(const wchar_t* path)  //Image�Ĳ���Ҫ�����unicode
			{
				img = Image::FromFile(path);
				width = img->GetWidth();
				height = img->GetHeight();
				frame_pos = 0;
				IsAnimate();
			}
			void Create(HWND hWnd,CPoint pt)
			{
				__super::Create(hWnd);
				SetWindowPos(hWnd,pt.x,pt.y,width,height,SWP_NOZORDER);
			}
			void Start()  //��Start֮ǰ��OnPaint�Ѿ������˵�0֡
			{
				long pause = ((long *) pro_item->value)[frame_pos];  //��ȡ��0֡�ļ��ʱ��
				pause = pause <= 5 ? 100 : pause * 10;  //�����Ϊ����gif��ʾ�ٶ����У�ԭ��ο�����������ԭ���ߵĻش�
				SetTimer(1212,pause,NULL);
			}
		public:
			BEGIN_MSG_MAP(GifWnd)
				MSG_WM_TIMER(OnTimer)
				MSG_WM_PAINT(OnPaint)
			END_MSG_MAP()
		public:
			void OnTimer(UINT_PTR)
			{
				GUID guid = FrameDimensionTime;
				frame_pos++;
				if(frame_pos >= frame_count)
					frame_pos = 0;
				img->SelectActiveFrame(&guid,frame_pos);  //ָ���Ƶ���һ֡
				Invalidate();
				KillTimer(1212);
				long pause = ((long *) pro_item->value)[frame_pos];
				pause = pause <= 5 ? 100 : pause * 10;
				SetTimer(1212,pause,NULL);  //ÿ�μ��ʱ�䲻һ�����Ҽǵ�CPictureEx�������ó�100�ˡ�
			}
			void OnPaint(CDCHandle)
			{
				CPaintDC dc(m_hWnd);
				// WTL��˫�����࣬����ʱ�Զ����Ƶ�dc�ϡ�����ǽ����˸��
				CMemoryDC mem(dc,CRect(0,0,width,height));  
				Graphics graph(mem);
				graph.DrawImage(img,0,0,width,height);
			}
		private:
			bool IsAnimate()
			{
				UINT cn = img->GetFrameDimensionsCount();
				GUID* pGuid = new GUID[cn];
				img->GetFrameDimensionsList(pGuid,cn);
				frame_count = img->GetFrameCount(&pGuid[0]);
				int sz = img->GetPropertyItemSize(PropertyTagFrameDelay);
				pro_item = (PropertyItem *) malloc(sz);
				img->GetPropertyItem(PropertyTagFrameDelay,sz,pro_item);
				delete pGuid;
				return frame_count > 1;
			}
		private:
			int width,height;
			Image *img;
			int frame_count;
			int frame_pos;
			PropertyItem *pro_item;

			ULONG_PTR token;
		};

	}
}

#endif;