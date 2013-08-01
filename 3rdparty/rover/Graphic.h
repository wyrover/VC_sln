#ifndef _ROVERLIB_GRAPHIC_H_
#define _ROVERLIB_GRAPHIC_H_

using namespace Gdiplus;


namespace roverlib
{
	void DrawPic(HDC hDC, LPCTSTR lpFileName);

	// ��ȡͼ���һ����
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
		// ��ͼƬͨ������ʽ��ʾ��DC��
		//

		HANDLE hFile = CreateFile(		//���ļ�
			lpFileName,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		DWORD dwFileSize = GetFileSize(hFile, NULL);		//�õ��ļ��Ĵ�С
		DWORD dwReadSize = 0;		//��ȡ���ֽ���
		HGLOBAL hImageMemory = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);		//�����
		void* pImageMemory = GlobalLock(hImageMemory);	//�����ѣ����ҵõ���ָ��
		/*
		 * ���ֱ�Ӵ�����Ƕ��������ݣ���ͨ��memcpy_s�����ݿ�����pImageMemory���м���
		 * ����memcpy_s(pImageMemory, dwBuffSize, pBuff, dwBuffSize);
		 */
		ReadFile(		//��ȡ�ļ�
			hFile,
			pImageMemory,
			dwFileSize,
			&dwReadSize,
			NULL);
		GlobalUnlock(hImageMemory);		//�������

		IStream* pIStream = NULL;
		CreateStreamOnHGlobal(hImageMemory, FALSE, &pIStream);	//�õ�IStream
		Image img(pIStream);		//�������ص�Image������
		Graphics g(hDC);
		g.DrawImage(&img, 0, 0, img.GetWidth(), img.GetHeight());		//����ͼƬ

		GlobalFree(hImageMemory);		//�ͷŶѿռ�
		pIStream->Release();		//�ͷ�IStream
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

}


#endif