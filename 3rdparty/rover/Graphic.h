#ifndef _ROVERLIB_GRAPHIC_H_
#define _ROVERLIB_GRAPHIC_H_

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;


namespace roverlib
{



	
	//逐像素混合
	


	void DrawPic(HDC hDC, LPCTSTR lpFileName);

	// 截取图像的一部分
	HBITMAP GetPartBitmap(HDC hSrcDC, RECT* rect);

	void FillSolidRect(HDC srcDC, LPCRECT lpRect, COLORREF clr);
	BOOL ImageFromIDResource(UINT resourceID, LPCTSTR resourceType, Image*& image);


	class CGdiPlusBitmap
	{
	public:
		Gdiplus::Bitmap* m_pBitmap;

	public:
		CGdiPlusBitmap()							{ m_pBitmap = NULL; }
		CGdiPlusBitmap(LPCWSTR pFile)				{ m_pBitmap = NULL; Load(pFile); }
		virtual ~CGdiPlusBitmap()					{ Empty(); }

		void Empty()								
		{ 
			delete m_pBitmap; 
			m_pBitmap = NULL; 
		}

		bool Load(LPCWSTR pFile)
		{
			Empty();
			m_pBitmap = Gdiplus::Bitmap::FromFile(pFile);
			return m_pBitmap->GetLastStatus() == Gdiplus::Ok;
		}

		operator Gdiplus::Bitmap*() const { return m_pBitmap; }
	};


	class CGdiPlusBitmapResource : public CGdiPlusBitmap
	{
	protected:
		HGLOBAL m_hBuffer;

	public:
		CGdiPlusBitmapResource()					{ m_hBuffer = NULL; }
		CGdiPlusBitmapResource(LPCTSTR pName, LPCTSTR pType = RT_RCDATA, HMODULE hInst = NULL)
		{ m_hBuffer = NULL; Load(pName, pType, hInst); }
		CGdiPlusBitmapResource(UINT id, LPCTSTR pType = RT_RCDATA, HMODULE hInst = NULL)
		{ m_hBuffer = NULL; Load(id, pType, hInst); }
		CGdiPlusBitmapResource(UINT id, UINT type, HMODULE hInst = NULL)
		{ m_hBuffer = NULL; Load(id, type, hInst); }
		virtual ~CGdiPlusBitmapResource()			{ Empty(); }

		void Empty();

		bool Load(LPCTSTR pName, LPCTSTR pType = RT_RCDATA, HMODULE hInst = NULL);
		bool Load(UINT id, LPCTSTR pType = RT_RCDATA, HMODULE hInst = NULL)
		{ return Load(MAKEINTRESOURCE(id), pType, hInst); }
		bool Load(UINT id, UINT type, HMODULE hInst = NULL)
		{ return Load(MAKEINTRESOURCE(id), MAKEINTRESOURCE(type), hInst); }
	};



}

namespace roverlib
{
	inline void DrawPic(HDC hDC, LPCTSTR lpFileName)
	{
		//
		// 将图片通过流方式显示在DC中
		//

		HANDLE hFile = CreateFile(		//打开文件
			lpFileName,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		DWORD dwFileSize = GetFileSize(hFile, NULL);		//得到文件的大小
		DWORD dwReadSize = 0;		//读取的字节数
		HGLOBAL hImageMemory = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);		//分配堆
		void* pImageMemory = GlobalLock(hImageMemory);	//锁定堆，并且得到堆指针
		/*
		 * 如果直接传入的是二进制内容，则通过memcpy_s将内容拷贝到pImageMemory当中即可
		 * 比如memcpy_s(pImageMemory, dwBuffSize, pBuff, dwBuffSize);
		 */
		ReadFile(		//读取文件
			hFile,
			pImageMemory,
			dwFileSize,
			&dwReadSize,
			NULL);
		GlobalUnlock(hImageMemory);		//解除锁定

		IStream* pIStream = NULL;
		CreateStreamOnHGlobal(hImageMemory, FALSE, &pIStream);	//得到IStream
		Image img(pIStream);		//将流加载到Image对象当中
		Graphics g(hDC);
		g.DrawImage(&img, 0, 0, img.GetWidth(), img.GetHeight());		//绘制图片

		GlobalFree(hImageMemory);		//释放堆空间
		pIStream->Release();		//释放IStream
	}

	inline HBITMAP GetPartBitmap(HDC hSrcDC, RECT* rect)
	{
		HBITMAP hBitmap, hOldBitmap;
		HDC hMemDC;
		int width, height;
		width = abs(rect->right-rect->left);
		height = abs(rect->bottom-rect->top);
		hMemDC = CreateCompatibleDC(hSrcDC);
		hBitmap = CreateCompatibleBitmap(hSrcDC, width, height);
		hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

		int left = rect->left>rect->right?rect->right:rect->left;
		int top = rect->top>rect->bottom?rect->bottom:rect->top;

		BitBlt(hMemDC, 0, 0, width, height, hSrcDC, left, top, SRCCOPY);

		hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);

		return hBitmap;
	}



	inline void CGdiPlusBitmapResource::Empty()
	{
		CGdiPlusBitmap::Empty();
		if (m_hBuffer)
		{
			::GlobalUnlock(m_hBuffer);
			::GlobalFree(m_hBuffer);
			m_hBuffer = NULL;
		} 
	}

	inline bool CGdiPlusBitmapResource::Load(LPCTSTR pName, LPCTSTR pType, HMODULE hInst)
	{
		Empty();

		HRSRC hResource = ::FindResource(hInst, pName, pType);
		if (!hResource)
			return false;

		DWORD imageSize = ::SizeofResource(hInst, hResource);
		if (!imageSize)
			return false;

		const void* pResourceData = ::LockResource(::LoadResource(hInst, hResource));
		if (!pResourceData)
			return false;

		m_hBuffer  = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);
		if (m_hBuffer)
		{
			void* pBuffer = ::GlobalLock(m_hBuffer);
			if (pBuffer)
			{
				CopyMemory(pBuffer, pResourceData, imageSize);

				IStream* pStream = NULL;
				if (::CreateStreamOnHGlobal(m_hBuffer, FALSE, &pStream) == S_OK)
				{
					m_pBitmap = Gdiplus::Bitmap::FromStream(pStream);
					pStream->Release();
					if (m_pBitmap)
					{ 
						if (m_pBitmap->GetLastStatus() == Gdiplus::Ok)
							return true;

						delete m_pBitmap;
						m_pBitmap = NULL;
					}
				}
				::GlobalUnlock(m_hBuffer);
			}
			::GlobalFree(m_hBuffer);
			m_hBuffer = NULL;
		}
		return false;
	}

	inline void FillSolidRect( HDC srcDC, LPCRECT lpRect, COLORREF clr )
	{
		::SetBkColor(srcDC, clr);
		::ExtTextOut(srcDC, 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL);
	}

	inline BOOL ImageFromIDResource(UINT resourceID, LPCTSTR resourceType, Image*& image)
	{
		HINSTANCE hInst = ModuleHelper::GetResourceInstance();
		HRSRC hRsrc = ::FindResource(hInst,MAKEINTRESOURCE(resourceID),resourceType);
		if (hRsrc == NULL)
			return FALSE;
		// load resource into memory
		DWORD len = SizeofResource(hInst, hRsrc);
		BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
		if (lpRsrc == NULL)
			return FALSE;
		// Allocate global memory on which to create stream
		HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
		BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
		memcpy(pmem,lpRsrc,len);
		IStream* pstm;
		CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);
		// load from stream
		image = Gdiplus::Image::FromStream(pstm);
		// free/release stuff
		GlobalUnlock(m_hMem);
		GlobalFree(m_hMem);
		pstm->Release();
		FreeResource(lpRsrc);
		return TRUE;
	}

	
	inline int ExtractIconCount(HINSTANCE HInstance, const std::wstring& FileName)
	{
		return (int)::ExtractIconW(HInstance, FileName.c_str(), 0xFFFFFFFF);
	}


	inline HICON BitmapToIcon(HBITMAP Bitmap, int cx, int cy)
	{
		HICON Result;
		HIMAGELIST	ImgList;
		int I;
		ImgList = ImageList_Create(cx, cy, ILC_COLOR, 1, 1);
		I = ImageList_Add(ImgList, Bitmap, 0);
		Result = ImageList_GetIcon(ImgList, I, ILD_NORMAL);
		ImageList_Destroy(ImgList);
	}	

	inline HICON BitmapToIcon(HBITMAP Bitmap, HBITMAP Mask, int cx, int cy)
	{
		HICON Result;
		HIMAGELIST	ImgList;
		int I;
		ImgList = ImageList_Create(cx, cy, ILC_COLOR, 1, 1);
		I = ImageList_Add(ImgList, Bitmap, Mask);
		Result = ImageList_GetIcon(ImgList, I, ILD_TRANSPARENT);
		ImageList_Destroy(ImgList);
	}

	inline HBITMAP IconToBitmap(HICON Icon)
	{
		HBITMAP Result;
		ICONINFO IconInfo = {0};
		if (GetIconInfo(Icon, &IconInfo))
		{
			DeleteObject(IconInfo.hbmMask);
			Result = IconInfo.hbmColor;
		}
		return Result;
	}

	inline IPicture* GetIconExec(const std::wstring& FileName, int IconIndex, bool UseLargeIcon)
	{
		HICON Largeicon;
		HICON Smallicon;
		HICON Selfhandle;
		PICTDESC pic;
		IPicture* Result;	

		memset(&pic, 0, sizeof(pic));

		if (ExtractIconExW(FileName.c_str(), IconIndex, &Largeicon, &Smallicon, 1))
		{
			if (UseLargeIcon)
				Selfhandle = Largeicon;
			else
				Selfhandle = Smallicon;			

			pic.cbSizeofstruct = sizeof(pic);
			pic.picType = PICTYPE_ICON;
			pic.icon.hicon = Selfhandle;

			OleCreatePictureIndirect(&pic, IID_IPicture, 1, reinterpret_cast<void**>(Result));	
			::DestroyIcon(Smallicon);
			::DestroyIcon(Largeicon);
		}

		return Result;		
	}

	/************************************************************************/
	/* RGB                                                                  */
	/************************************************************************/
	//inline COLORREF RGB(BYTE r, BYTE g, BYTE b)
	//{
	//	return (r | (g << 8) | (b << 16));
	//}

	//BYTE GetRValue(DWORD rgb)
	//{
	//	return BYTE(rgb);
	//}

	//BYTE GetGValue(DWORD rgb)
	//{
	//	return BYTE(rgb >> 8);
	//}	

	//BYTE GetBValue(DWORD rgb)
	//{
	//	return BYTE(rgb >> 16);
	//}





}


#endif