#ifndef _ROVER_PATHUTILS_H_
#define _ROVER_PATHUTILS_H_

#include <string>
#include <vector>
#include <atlfile.h>
#include <io.h>


namespace roverlib
{
	std::wstring GetAppFileName();
	std::wstring GetAppPath();
	std::wstring GetFilePath(const std::wstring& filename);
	void dividePath(const std::wstring& path, std::wstring& filename, std::wstring& ext);
	std::wstring GetFilename(const std::wstring& path);
	std::wstring GetFileExtension(const std::wstring& filepath);
	std::wstring ExtracFileName(const std::wstring& Filename);
	void LogWrite(const char* filename, const char* text);
	HICON GetIconByFileType(const std::string& ext);
	HICON MyGetFileIconHandle(const std::wstring& filepath, BOOL bSmall);
	CString GetuniqueTempName();
	bool IsFileExist(const std::wstring& filename);
	bool CreateDirectoryNested(const std::wstring&  dir);
	BOOL ForceCreateDirectory(std::wstring strDir);
	BOOL file_get_contents( LPCTSTR lpszFilename, CStringA &strA );
	BOOL file_get_contents(LPCTSTR lpszFilename, CString &strBuffer);
	BOOL file_put_contents(LPCTSTR lpszFilename, BYTE *pBuffer, INT nLen);
	BOOL file_put_contents(LPCTSTR lpszFilename, LPCTSTR lpszBuffer);
	INT64 GetFolderSize(LPCTSTR szFolder);
	INT64 file_get_size(const std::wstring& filename);
	DWORD MyGetFileSize(LPCTSTR lpPath);
	bool DeleteDirectory(const std::wstring& path);
	std::wstring GetCurrentDirectory();
	
	
}


namespace roverlib 
{
	// Add backslash to string
	#define ADDBACKSLASH(s) if(s.Right(1)!=_T('\\')) s+=_T('\\');
	#define REMOVEBACKSLASH(s) if(s.Right(1)==_T('\\')) s=s.Left(s.GetLength()-1);


	inline std::wstring GetAppFileName()
	{		
		wchar_t filename[MAX_PATH] = {0};
		GetModuleFileNameW(NULL, filename, MAX_PATH);
		return std::wstring(filename);
	}


	inline std::wstring GetAppPath()		
	{
		std::wstring filename = GetAppFileName();
		size_t i = filename.find_last_of(L'\\');	
		std::wstring s = filename.substr(0, i);	
		if (*(s.end()-1) != L'\\')
			s.append(L"\\");
		
		return s;
	};

	inline std::wstring GetFilePath(const std::wstring& filename)
	{		
		wchar_t path[MAX_PATH];
		LPWSTR p;
		DWORD len = ::GetFullPathNameW(filename.c_str(), MAX_PATH, path, &p);
		if ( p != NULL && p != 0)
			return std::wstring(path);
		else
			return std::wstring(L'');	
	}

	inline void dividePath(const std::wstring& path, std::wstring& filename, std::wstring& ext)
	{
		wchar_t	drive[_MAX_DRIVE],
			dir  [_MAX_DIR],
			fname[_MAX_FNAME],
			fext [_MAX_EXT];

		_tsplitpath(path.c_str(), drive, dir, fname, fext);
		filename = std::wstring(fname);
		ext = std::wstring(fext);		
	}


	inline std::wstring GetFilename(const std::wstring& path)
	{
		wchar_t	drive[_MAX_DRIVE],
			dir  [_MAX_DIR],
			fname[_MAX_FNAME],
			fext [_MAX_EXT];

		_tsplitpath(path.c_str(), drive, dir, fname, fext);

		return std::wstring(fname);
	}

	inline std::wstring GetFileExtension(const std::wstring& filepath)
	{
		wchar_t  drive[_MAX_DRIVE],
			    dir  [_MAX_DIR],
			    fname[_MAX_FNAME],
				ext[_MAX_EXT];

		_tsplitpath(filepath.c_str(), drive, dir, fname, ext);
		return std::wstring(ext);		
	}


	inline CString ExtracFilePath(const CString& Filename)
	{	
		CString retval;

		// Todo::
		return retval;
	}

	inline CString ExtracFileDir(const CString& Filename)
	{	
		CString retval;

		// Todo::
		return retval;
	}

	inline std::wstring ExtracFileName(const std::wstring& Filename)
	{			
		return std::wstring(PathFindFileNameW(Filename.c_str()));		
	}

	inline CString ExtracFileExt(const CString& Filename)
	{	
		CString retval;

		// Todo::
		return retval;
	}


	inline void LogWrite(const char* filename, const char* text)
	{
		FILE *fp;	
		errno_t err;
		err = fopen_s(&fp, filename, "w");
		if (err == 0)
		{
			fprintf(fp, text);
			fclose(fp);
		}		
	}


	
	/*static void ClearDir(const CString &dirpath)
	{	
		CFileFind finder;
		CString dir = dirpath;
		if (dir.Right(1)!="\\") 
			dir += "\\";
		dir += "*.*";

		BOOL bfile = finder.FindFile(dir);
		while (bfile)
		{
			bfile = finder.FindNextFile();
			if (finder.IsDirectory() && !finder.IsDots())   
				ClearDir(finder.GetFilePath());
			else if (!finder.IsDots() && !finder.IsDirectory())
				DeleteFile(finder.GetFilePath());
		}
		finder.Close();
	}*/

	HICON GetIconByFileType(const std::wstring& ext)
	{	
	
		std::wstring DummyFileName = L"foo.";
		DummyFileName.append(ext);

		DWORD_PTR dwRet;
		SHFILEINFO shfi = {0};
		
		dwRet = SHGetFileInfoW(DummyFileName.c_str(),
			FILE_ATTRIBUTE_NORMAL,
			&shfi,
			sizeof(shfi),
			SHGFI_ICON|SHGFI_USEFILEATTRIBUTES);

		return SUCCEEDED(dwRet) ? shfi.hIcon : NULL;		
	}

	HICON MyGetFileIconHandle(const std::wstring& filepath, BOOL bSmall)
	{
		
		SHFILEINFO sfi = { 0 };
		DWORD_PTR dwRet;

		DWORD dwFlag = SHGFI_ICON | SHGFI_USEFILEATTRIBUTES;
		if ( bSmall )
		{
			dwFlag |= SHGFI_SMALLICON;
		}

		dwRet = SHGetFileInfoW(filepath.c_str(), FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(SHFILEINFO), dwFlag);

		return SUCCEEDED(dwRet) ? sfi.hIcon : NULL;		
	}


	inline CString GetuniqueTempName()
	{
		//Get the temporary files directory.
		CString TempPath;
		DWORD dwResult = ::GetTempPath(MAX_PATH, TempPath.GetBuffer(MAX_PATH));
		TempPath.ReleaseBuffer();		

		//Create a unique temporary file.		
		CString tempFile;		
		UINT nResult = GetTempFileName((LPCTSTR)TempPath, _T("~ex"), 0, tempFile.GetBuffer(MAX_PATH));		
		tempFile.ReleaseBuffer();

		return tempFile;
	}

	bool IsFileExist(const std::wstring& filename)
	{
		bool bRet = false;
		

		WIN32_FIND_DATA fd = {0};
		HANDLE hFile = FindFirstFile(filename.c_str(), &fd);
		if(hFile != INVALID_HANDLE_VALUE)
		{
			FindClose(hFile);
			if( !(fd.dwFileAttributes &  FILE_ATTRIBUTE_DIRECTORY) )
				bRet = true;
		}
		return bRet;
	}

	template<size_t nSize>
	void ModifyPathSpec( TCHAR (&szDst)[nSize], BOOL  bAddSpec )
	{
		size_t nLen = _tcslen( szDst );
		ATLASSERT( nLen > 0 );
		TCHAR  ch  = szDst[ nLen - 1 ];
		if( ( ch == _T('\\') ) || ( ch == _T('/') ) )
		{
			if( !bAddSpec )
			{
				while( ch==_T('\\') || ch==_T('/') )
				{
					szDst[ nLen - 1 ] = _T('\0');
					-- nLen;
					ch = szDst[ nLen - 1];
				}
			}
		}
		else
		{
			if( bAddSpec )
			{
				szDst[ nLen ] = _T('\\');
				szDst[ nLen + 1 ] = _T('\0');
			}
		}
	}

	bool CreateDirectoryNested(const std::wstring&  dir)
	{
		if( ::PathIsDirectoryW(dir.c_str())) 
			return true;

		if(dir.length() >= MAX_PATH)
			return false;

		wchar_t   szPreDir[ MAX_PATH ];
		_tcscpy_s( szPreDir, dir.c_str() );
		//确保路径末尾没有反斜杠
		ModifyPathSpec( szPreDir, FALSE );

		//获取上级目录
		BOOL  bGetPreDir  = ::PathRemoveFileSpec( szPreDir );
		if( !bGetPreDir ) return false;

		//如果上级目录不存在,则递归创建上级目录
		if( !::PathIsDirectory( szPreDir ) )
		{
			CreateDirectoryNested( szPreDir );
		}

		return SUCCEEDED(::CreateDirectory(dir.c_str(), NULL)) ? true : false;
	}


	inline BOOL ForceCreateDirectory(std::wstring strDir)
	{
		using namespace std;

		BOOL bRet = FALSE;
		//确保以"\"结尾，以创建最后一个目录
		if(strDir.find_last_of(L"\\") != strDir.length()-1)
		{
			strDir.append(L"\\");
		}
		vector<wstring> dirs;		//存放每一层目录
		wstring temp;			//临时变量，存放目录字符串
		for(size_t i=0; i != strDir.length(); ++i)
		{
			if(strDir[i] != L'\\')
			{
				temp += strDir[i];
			}
			else
			{
				dirs.push_back(temp);
				temp += L'\\';
			}
		}
		//遍历vector,创建目录
		for(vector<wstring>::const_iterator iter=dirs.begin(); iter!=dirs.end(); ++iter)
		{
			bRet = CreateDirectory((*iter).c_str(), NULL);
		}
		return bRet;
	}

	BOOL file_get_contents( LPCTSTR lpszFilename, CStringA &strA )
	{
		CAtlFile file;
		if( FAILED( file.Create(lpszFilename, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING) ) )
			return FALSE;

		BOOL bRet = FALSE;
		do 
		{
			ULONGLONG filesize = 0;
			if( FAILED( file.GetSize(filesize) ) ) 
				break;

			strA = "";
			if(filesize>0)
			{
				file.Read( strA.GetBuffer((int)filesize), (DWORD)filesize );
				strA.ReleaseBuffer((int)filesize);
			}
			bRet = TRUE;
		} while (FALSE);
		file.Close();
		return bRet;
	}

	BOOL file_get_contents(LPCTSTR lpszFilename, CString &strBuffer)
	{
		CStringA strA;
		BOOL bRet = file_get_contents(lpszFilename, strA);

		USES_CONVERSION;
		strBuffer = CA2CT( strA );		
		return bRet;
	}

	BOOL file_put_contents(LPCTSTR lpszFilename, BYTE *pBuffer, INT nLen)
	{
		CAtlFile file;
		if( FAILED( file.Create(lpszFilename, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, CREATE_ALWAYS) ) )
			return FALSE;

		file.Write( pBuffer, nLen );
		file.Close();
		return TRUE;
	}

	BOOL file_put_contents(LPCTSTR lpszFilename, LPCTSTR lpszBuffer)
	{
		CStringA strA;
		USES_CONVERSION;
		strA = CT2CA( lpszBuffer );
		return file_put_contents(lpszFilename, (BYTE*)strA.GetString(), strA.GetLength());
	}

	INT64 GetFolderSize(LPCTSTR szFolder)
	{
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = INVALID_HANDLE_VALUE;

		CString strFolder(szFolder);
		strFolder.Append( _T("\\*") );

		INT64 folderSize = 0;
		hFind = FindFirstFile(strFolder, &FindFileData);
		if (hFind != INVALID_HANDLE_VALUE) 
		{
			_tprintf (_T("First file name is %s\n"), FindFileData.cFileName);
			do 
			{
				_tprintf (_T("Next file name is %s\n"), FindFileData.cFileName);

				if( _tcsicmp(_T("."), FindFileData.cFileName)==0 
					|| _tcsicmp(_T(".."), FindFileData.cFileName)==0 )
				{
					continue;
				}
				if( FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
				{
					CString strResult = szFolder;
					LPCTSTR pszResult;
					LPCTSTR pchLast;
					pszResult = strResult;
					pchLast = _tcsdec( pszResult, pszResult+strResult.GetLength() );
					ATLASSERT(pchLast!=NULL);
					if ((*pchLast != _T('\\')) && (*pchLast != _T('/')))
						strResult += _T('\\');
					strResult += FindFileData.cFileName;

					folderSize += GetFolderSize( strResult );
				}
				else
				{
					ULARGE_INTEGER nFileSize;
					nFileSize.LowPart = FindFileData.nFileSizeLow;
					nFileSize.HighPart = FindFileData.nFileSizeHigh;
					folderSize += nFileSize.QuadPart;
				}

			} while (FindNextFile(hFind, &FindFileData) != 0);
			FindClose(hFind);
		}
		return folderSize;
	}


	INT64 file_get_size(const std::wstring& filename)
	{
		CAtlFile file;
		if( FAILED( file.Create(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING) ) )
			return 0;

		ULONGLONG filesize = 0;
		file.GetSize(filesize);
		return filesize;
	}

	DWORD MyGetFileSize(LPCTSTR lpPath)
	{
		WIN32_FILE_ATTRIBUTE_DATA wfd;

		if ( GetFileAttributesEx(lpPath, GetFileExInfoStandard, &wfd) )
		{
			return (wfd.nFileSizeHigh != 0) ? 0xffffffff : wfd.nFileSizeLow;
		}

		return 0;
	}

	bool DeleteDirectory(const std::wstring& path)
	{
		_wfinddata_t fileInfo;			

		std::wstring localDir;
		localDir = path;
		localDir.append(L"\\*");	
		std::wstring filepath;

		long handle = _wfindfirst(localDir.c_str(),&fileInfo);
		int index=0;
		if (handle==-1)
		{
			return false;
		}
		else
		{
			do 
			{		
				filepath = path;
				filepath.append(L"\\");
				filepath.append(fileInfo.name);

				if (fileInfo.attrib&_A_SUBDIR)
				{
					if (!_tcscmp(fileInfo.name,_T("."))||!_tcscmp(fileInfo.name,_T("..")))
					{
						continue;
					}					

					DeleteDirectory(filepath);
				}
				else
				{
					DeleteFile(filepath.c_str());
				}

			} while(_tfindnext(handle, &fileInfo) == 0);
			RemoveDirectory(path.c_str());
		}
		_findclose(handle);
		return true;
	}

	std::wstring GetCurrentDirectory()
	{
		wchar_t buffer[MAX_PATH];
		::GetCurrentDirectoryW(MAX_PATH, buffer);
		return std::wstring(buffer);
	}


	

}

#endif