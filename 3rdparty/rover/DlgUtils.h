#ifndef _ROVERLIB_DLGUTILS_H_
#define _ROVERLIB_DLGUTILS_H_

#include <shlobj.h>

namespace roverlib
{
	void SelectFile( HWND hWnd, CString& strFilePath, LPCTSTR lpszFileFilter = NULL, LPCTSTR lpszTitle = NULL );
	void SaveFile( HWND hWnd, CString& strFilePath, LPCTSTR lpszFileFilter = NULL, LPCTSTR lpszTitle = NULL );
	void SelectDir( HWND hWnd, CString& strDirPath, LPCTSTR lpszTitle = NULL );
}

namespace roverlib
{
	void SelectFile( HWND hWnd, CString& strFilePath, LPCTSTR lpszFileFilter /*= NULL*/, LPCTSTR lpszTitle /*= NULL */ )
	{
		OPENFILENAME ofn;
		ZeroMemory( &ofn, sizeof( ofn ) );
		ofn.lStructSize			=   sizeof(OPENFILENAME);  
		ofn.hwndOwner			=   hWnd;  
		ofn.lpstrFile			=   strFilePath.GetBuffer( MAX_PATH );  
		ofn.nMaxFile			=   MAX_PATH;    
		ofn.lpstrInitialDir		=   strFilePath;  
		ofn.lpstrTitle			=	lpszTitle;
		ofn.Flags				=   OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; 
		ofn.lpstrFilter			=	lpszFileFilter;

		if( GetOpenFileName( &ofn ) )
			strFilePath.ReleaseBuffer();
		else 
			strFilePath.ReleaseBuffer( 0 );
	}

	void SaveFile( HWND hWnd, CString& strFilePath, LPCTSTR lpszFileFilter /*= NULL*/, LPCTSTR lpszTitle /*= NULL */ )
	{
		OPENFILENAME ofn;
		ZeroMemory( &ofn, sizeof( ofn ) );
		ofn.lStructSize			=   sizeof(OPENFILENAME);  
		ofn.hwndOwner			=   hWnd;  
		ofn.lpstrFile			=   strFilePath.GetBuffer( MAX_PATH );  
		ofn.nMaxFile			=   MAX_PATH;   
		ofn.lpstrTitle			=	lpszTitle;
		ofn.lpstrInitialDir		=   strFilePath;  
		ofn.lpstrFilter			=	lpszFileFilter;

		if( GetSaveFileName( &ofn ) )
			strFilePath.ReleaseBuffer();
		else 
			strFilePath.ReleaseBuffer( 0 );
	}

	void SelectDir( HWND hWnd, CString& strDirPath, LPCTSTR lpszTitle /*= NULL */ )
	{
		CString strDirName;
		BROWSEINFO bsn;
		ZeroMemory( &bsn, sizeof( bsn ) );
		bsn.hwndOwner			= hWnd;
		bsn.pszDisplayName		= strDirName.GetBuffer( MAX_PATH ); 
		bsn.lpszTitle			= lpszTitle;
		bsn.ulFlags				= 0; 

		LPITEMIDLIST lpItemIDList = SHBrowseForFolder( &bsn );
		if ( NULL == lpItemIDList )
		{
			strDirName.ReleaseBuffer( 0 );
			return ;
		}

		strDirName.ReleaseBuffer();

		BOOL bRet = SHGetPathFromIDList( lpItemIDList,strDirPath.GetBuffer( MAX_PATH ) ); 
		if ( bRet )
		{
			strDirPath.ReleaseBuffer();
			strDirPath.TrimRight( L"\\/" );
		}
		else
			strDirPath.ReleaseBuffer( 0 );
	}
}




#endif