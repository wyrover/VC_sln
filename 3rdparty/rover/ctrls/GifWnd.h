#ifndef _ROVERCTRLS_GIFWND_H_
#define _ROVERCTRLS_GIFWND_H_

#include <atlcrack.h>
#include <GdiPlus.h>
using namespace Gdiplus;

namespace roverlib {
	namespace ctrls {

		/* GIF 控件 */

		typedef CWinTraits<WS_VISIBLE | WS_CHILD, 0> GifTraits;  //控制窗口样式

		class GifWnd : public CWindowImpl<GifWnd, CStatic, GifTraits>  //（此处继承CStatic就可以拖动窗口（当窗口响应了NCHITTEST消息的时候）了）
		{
		public:
			GifWnd()  //初始化gdi+    这种对gdi+环境的初始化和释放在整个程序做一次就行了。因此，可考虑封装到一个类中，此处是为了简便。
			{
				GdiplusStartupInput in;
				GdiplusStartup(&token,&in,NULL);
			}
			~GifWnd()  //释放gdi+
			{
				GdiplusShutdown(token);
			}
			void Load(const wchar_t* path)  //Image的参数要求的是unicode
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
			void Start()  //在Start之前，OnPaint已经绘制了第0帧
			{
				long pause = ((long *) pro_item->value)[frame_pos];  //获取第0帧的间隔时间
				pause = pause <= 5 ? 100 : pause * 10;  //这个是为了让gif显示速度适中，原因参考上面链接中原作者的回答
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
				img->SelectActiveFrame(&guid,frame_pos);  //指针移到下一帧
				Invalidate();
				KillTimer(1212);
				long pause = ((long *) pro_item->value)[frame_pos];
				pause = pause <= 5 ? 100 : pause * 10;
				SetTimer(1212,pause,NULL);  //每次间隔时间不一样，我记得CPictureEx类中设置成100了。
			}
			void OnPaint(CDCHandle)
			{
				CPaintDC dc(m_hWnd);
				// WTL的双缓存类，析构时自动绘制到dc上。这个是解决闪烁的
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