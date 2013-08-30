#ifndef _ROVER_PATHUTILS_H_
#define _ROVER_PATHUTILS_H_

#include <string>
#include <vector>
#include <atlfile.h>
#include <io.h>
#include <ShlObj.h>
#include "Diagnostic.h"

namespace roverlib
{

	const wchar_t PathDelim  = L'\\';
	const wchar_t DirDelimiter = L'\\';
	const wchar_t DriveDelim = L':';
	const wchar_t PathSep    = L';';


	struct FileInfo
	{
		FileInfo(const std::wstring& _name, bool _folder) : name(_name), folder(_folder) { }
		std::wstring name;
		bool folder;
	};

	typedef std::vector<FileInfo> VectorFileInfo;
	typedef std::vector<std::wstring> VectorWString;

	bool IsPathDelimiter(const std::wstring& S, int Index);
	std::wstring IncludeTrailingBackslash(const std::wstring& S);
	std::wstring IncludeTrailingPathDelimiter(const std::wstring& S);
	std::wstring ExcludeTrailingBackslash(const std::wstring& S);
	std::wstring ExcludeTrailingPathDelimiter(const std::wstring& S);
	std::wstring ExtractFilePath(const std::wstring& FileName);

	std::wstring GetAppFileName();
	std::wstring GetAppPath();
	bool isAbsolutePath(const wchar_t* path);
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
	bool isReservedDir (const wchar_t* _fn);
	bool isParentDir (const wchar_t* _fn);
	std::wstring concatenatePath(const std::wstring& _base, const std::wstring& _name);
	void scanFolder(VectorWString& _result, const std::wstring& _folder, bool _recursive, const std::wstring& _mask, bool _fullpath);

	typedef BOOL (WINAPI *EnumerateFunc) (const wchar_t* lpFileOrPath, void* pUserData);
	void doFileEnumeration(const wchar_t* lpPath, BOOL bRecursion, BOOL bEnumFiles, EnumerateFunc pFunc, void* pUserData);

	std::wstring GetShellFolderPath(int CSIDL);
	bool FileExists(const std::wstring& FileName);
	bool DirectoryExists(const std::wstring& Directory);
	DWORD GetLargeFileSize(const std::wstring& FileName);
	
}


namespace roverlib 
{
	// Add backslash to string
	#define ADDBACKSLASH(s) if(s.Right(1)!=_T('\\')) s+=_T('\\');
	#define REMOVEBACKSLASH(s) if(s.Right(1)==_T('\\')) s=s.Left(s.GetLength()-1);


	inline bool IsPathDelimiter(const std::wstring& S, int Index)
	{
		return (Index > 0) && (Index <= S.length()) && (S[Index] == PathDelim);
	}


	inline std::wstring IncludeTrailingBackslash(const std::wstring& S)
	{
		return IncludeTrailingPathDelimiter(S);
	}

	inline std::wstring IncludeTrailingPathDelimiter(const std::wstring& S)
	{
		std::wstring Result = S;
		if (!IsPathDelimiter(Result, Result.length()))
			Result = Result + PathDelim;
		return Result;
	}

	inline std::wstring PathAddSeparator(const std::wstring& Path)
	{
		std::wstring Result = Path;
		if ((Path == L"") || (Path[Path.length()] != DirDelimiter))
			Result = Path + DirDelimiter;
	}

	inline std::wstring PathBuildRoot(int Drive)
	{
		wchar_t Result[MAX_PATH + 10] = {0};
		::PathBuildRootW(Result, Drive);
		return std::wstring(Result);
	}
	

	inline std::wstring ExcludeTrailingBackslash(const std::wstring& S)
	{
		return ExcludeTrailingPathDelimiter(S);
	}	

	inline std::wstring ExcludeTrailingPathDelimiter(const std::wstring& S)
	{
		std::wstring Result = S;
		if (IsPathDelimiter(Result, Result.length()))		
			Result.erase(Result.length() - 1);			
		return Result;
	}


	inline std::wstring ExtractFilePath(const std::wstring& FileName)
	{
		std::wstring Result = FileName;
		return Result.erase(Result.find_last_of(PathDelim) + 1);
	}


	inline std::wstring ExtractFileName(const std::wstring& FileName)
	{
		std::wstring Result = FileName;
		return Result.erase(0, Result.find_last_of(PathDelim) + 1);
	}

	inline std::wstring ExtractFileExt(const std::wstring& FileName)
	{
		std::wstring Result = FileName;
		return Result.erase(0, Result.find_last_of(L'.') + 1);
	}

	inline std::wstring ExpandFileName(const std::wstring& FileName)
	{
		wchar_t* FName;
		wchar_t Buffer[MAX_PATH] = {0};
		::GetFullPathNameW(FileName.c_str(), MAX_PATH,	Buffer, &FName);
		return std::wstring(Buffer);
	}


	inline std::wstring ChangeFileExt(const std::wstring& FileName, const std::wstring& Extension)
	{
		std::wstring Result = FileName;
		return Result.erase(Result.find_last_of(L'.') + 1) + Extension;
	}

	inline std::wstring ChangeFilePath(const std::wstring& FileName, const std::wstring& Path)
	{
		std::wstring Result = Path;
		return IncludeTrailingPathDelimiter(Result) + ExtractFileName(FileName);
	}


	inline std::wstring ExtractShortPathName(const std::wstring& FileName)
	{
		wchar_t Buffer[MAX_PATH + 1] = {0};
		GetShortPathNameW(FileName.c_str(), Buffer, MAX_PATH);
		return std::wstring(Buffer);

	}



	inline bool CreateDir(const std::wstring& Dir)
	{
		return CreateDirectoryW(Dir.c_str(), NULL) == TRUE;
	}

	inline bool ForceDirectories(std::wstring Dir)
	{
		/*
		bool Result;
		if (Dir.length() == 0)
			return false;
		Dir = ExcludeTrailingPathDelimiter(Dir);
		if ((Dir.length() < 3) || DirectoryExists(Dir) || (ExtractFilePath(Dir) == Dir)) 
			exit(0); // avoid 'xyz:\' problem.		 
		return ForceDirectories(ExtractFilePath(Dir)) && CreateDir(Dir);
		*/
		return false;
	}

	inline std::wstring GetCurrentWorkingDir()
	{
		wchar_t Buffer[MAX_PATH + 1] = {0};
		_wgetcwd(Buffer, MAX_PATH);
		return std::wstring(Buffer);		
	}


	inline bool SetCurrentDir(const std::wstring& Dir)
	{
		return ::SetCurrentDirectory(Dir.c_str()) == TRUE;
	}

	inline bool RemoveDir(const std::wstring& Dir)
	{
		return ::RemoveDirectory(Dir.c_str()) == TRUE;
	}

	inline bool DeleteFile(const std::wstring& FileName)
	{
		return ::DeleteFile(FileName.c_str()) == TRUE;
	}

	inline bool RenameFile(const std::wstring& OldName, const std::wstring& NewName)
	{
		return ::MoveFile(OldName.c_str(), NewName.c_str()) == TRUE;
	}

	inline bool DeleteFileByShell(const std::wstring& FileName)
	{
		SHFILEOPSTRUCTW Fileop = {0};
		Fileop.hwnd = NULL;
		Fileop.wFunc = FO_DELETE;
		Fileop.pFrom = FileName.c_str();
		Fileop.fFlags = FOF_ALLOWUNDO;
		return ::SHFileOperationW(&Fileop) > 0;
	}
	

	inline std::wstring CreateClassID()
	{
		GUID ClassID = {0};
		wchar_t* P;
		std::wstring Result;

		::CoCreateGuid(&ClassID);
		::StringFromCLSID(ClassID, &P);
		Result = std::wstring(P);
		::CoTaskMemFree(P);

		return Result;
	}
	

	inline std::wstring GUIDToString(const GUID& ClassID)
	{
		wchar_t* P;
		std::wstring Result;
		if (SUCCEEDED(StringFromCLSID(ClassID, &P)))
		{
			Result = std::wstring(P);
			CoTaskMemFree(P);
		}
		return Result;
	}	

	inline GUID StringToGUID(const std::wstring& S)
	{
		GUID Result;
		SUCCEEDED(CLSIDFromString((LPOLESTR)S.c_str(), &Result));
		return Result;
	}
	

	inline GUID ProgIDToClassID(const std::wstring& ProgID)
	{
		GUID Result;
		SUCCEEDED(CLSIDFromString((LPOLESTR)ProgID.c_str(), &Result));
		return Result;
	}
	

	inline std::wstring ClassIDToProgID(const GUID& ClassID)
	{
		wchar_t* P;
		std::wstring Result;
		if (SUCCEEDED(ProgIDFromCLSID(ClassID, &P)))
		{
			Result = std::wstring(P);
			CoTaskMemFree(P);
		}
		return Result;
	}


	inline int FileOpen(const std::wstring& FileName, DWORD Mode)
	{
		DWORD AccessMode[3] = {GENERIC_READ, GENERIC_WRITE, GENERIC_READ | GENERIC_WRITE};
		DWORD ShareMode[5] = {0, 0, FILE_SHARE_READ, FILE_SHARE_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE};

		int Result = -1;
		if (((Mode & 3) <= OF_READWRITE) && ((Mode & 0xF0) <= OF_SHARE_DENY_NONE))
			Result = (int)::CreateFileW(FileName.c_str(), AccessMode[Mode & 3], ShareMode[(Mode & 0xF0) >> 4], NULL, OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL, 0);

		return Result;			
	}

	

	inline int FileCreate(const std::wstring& FileName)
	{
		return (int)CreateFileW(FileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	}

	
	inline int FileRead(int Handle, BYTE* Buffer, DWORD Count)
	{
		DWORD Result;
		if (!ReadFile((HANDLE)Handle, Buffer, Count, &Result, NULL))
			Result = -1;
		return Result;
	}

	inline int FileWrite(int Handle, const BYTE* Buffer, DWORD Count)
	{
		DWORD Result;
		if (!WriteFile((HANDLE)Handle, Buffer, Count, &Result, NULL))
			Result = -1;
		return Result;
	}



	inline int FileSeek(int Handle, int Offset, int Origin)
	{
		return ::SetFilePointer((HANDLE)Handle, Offset, NULL, Origin);
	}	

	inline void FileClose(int Handle)
	{
		::CloseHandle((HANDLE)Handle);
	}

	


	inline std::wstring GetAppFileName()
	{		
		wchar_t filename[MAX_PATH + 10] = {0};
		GetModuleFileNameW((HMODULE)&__ImageBase, filename, MAX_PATH);		
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
	}

	inline bool isAbsolutePath(const wchar_t* path)
	{
		if (IsCharAlphaW(path[0]) && path[1] == ':')
			return true;

		return path[0] == '/' || path[0] == '\\';
	}

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

	inline FILE *TS_tfopen(const char* FileName, const wchar_t *mode)
	{
		wchar_t wFileName[MAX_PATH];
		MultiByteToWideChar(CP_UTF8, 0, FileName, -1, wFileName, MAX_PATH);
		return _wfopen(wFileName, mode);		
	}


	inline void LogWrite(const char* FileName, const char* text)
	{
		FILE *fp;	
		errno_t err;
		err = fopen_s(&fp, FileName, "w");
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

	inline bool ExistsLockedOrShared(const std::wstring& FileName)
	{
		WIN32_FIND_DATAW FindData = {0};
		HANDLE LHandle;
		LHandle = ::FindFirstFileW(FileName.c_str(), &FindData);
		if (LHandle != INVALID_HANDLE_VALUE) 
		{	
			::FindClose(LHandle);
			return ((FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0);
		}
		else
			return false;
	}

	

	inline bool FileExists(const std::wstring& FileName)
	{
		DWORD Code;
		DWORD LastError;
		Code = ::GetFileAttributesW(FileName.c_str());
		if (Code != -1)
		{
			return ((FILE_ATTRIBUTE_DIRECTORY & Code) == 0);
		}
		else
		{
			LastError = ::GetLastError();
			return ((LastError != ERROR_FILE_NOT_FOUND) &&
				(LastError != ERROR_PATH_NOT_FOUND) &&
				(LastError != ERROR_INVALID_NAME) && ExistsLockedOrShared(FileName));
		}

	}

	inline bool DirectoryExists(const std::wstring& Directory)
	{
		DWORD Code;
		Code = ::GetFileAttributesW(Directory.c_str());
		return ((Code != INVALID_FILE_ATTRIBUTES) && ((FILE_ATTRIBUTE_DIRECTORY & Code) != 0));
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



	inline bool isReservedDir (const wchar_t* _fn)
	{
		// if "."
		return (_fn [0] == '.' && _fn [1] == 0);
	}

	inline bool isParentDir (const wchar_t* _fn)
	{
		// if ".."
		return (_fn [0] == '.' && _fn [1] == '.' && _fn [2] == 0);
	}

	inline std::wstring concatenatePath(const std::wstring& _base, const std::wstring& _name)
	{
		if (_base.empty() || isAbsolutePath(_name.c_str()))
		{
			return _name;
		}
		else
		{
			if (endswith(_base, L"\\") || endswith(_base, L"/"))
				return _base + _name;

#ifdef _WIN32
			return _base + L'\\' + _name;
#else
			return _base + L'/' + _name;
#endif
		}
	}


	inline void getSystemFileList(VectorFileInfo& _result, const std::wstring& _folder, const std::wstring& _mask)
	{
		//FIXME add optional parameter?
		bool ms_IgnoreHidden = true;

		long lHandle, res;
		struct _wfinddata_t tagData;

		// pattern can contain a directory name, separate it from mask
		size_t pos = _mask.find_last_of(L"/\\");
		std::wstring directory;
		if (pos != _mask.npos)
			directory = _mask.substr (0, pos);

		std::wstring full_mask = concatenatePath(_folder, _mask);

		lHandle = _wfindfirst(full_mask.c_str(), &tagData);
		res = 0;
		while (lHandle != -1 && res != -1)
		{
			if (( !ms_IgnoreHidden || (tagData.attrib & _A_HIDDEN) == 0 ) &&
				!isReservedDir(tagData.name))
			{
				_result.push_back(FileInfo(concatenatePath(directory, tagData.name), (tagData.attrib & _A_SUBDIR) != 0));
			}
			res = _wfindnext( lHandle, &tagData );
		}
		// Close if we found any files
		if (lHandle != -1)
			_findclose(lHandle);

	}
	

	inline void scanFolder(VectorWString& _result, const std::wstring& _folder, bool _recursive, const std::wstring& _mask, bool _fullpath)
	{
		std::wstring folder = _folder;
		if (!folder.empty() && *folder.rbegin() != '/' && *folder.rbegin() != '\\') folder += L"/";

		VectorFileInfo result;
		getSystemFileList(result, folder, _mask);

		for (VectorFileInfo::const_iterator item = result.begin(); item != result.end(); ++item)
		{
			if (item->folder) continue;

			if (_fullpath)
				_result.push_back(folder + item->name);
			else
				_result.push_back(item->name);
		}

		if (_recursive)
		{
			getSystemFileList(result, folder, L"*");

			for (VectorFileInfo::const_iterator item = result.begin(); item != result.end(); ++item)
			{
				if (!item->folder
					|| item->name == L".."
					|| item->name == L".") continue;
				scanFolder(_result, folder + item->name, _recursive, _mask, _fullpath);
			}

		}
	}

	
	inline void doFileEnumeration(const wchar_t* lpPath, BOOL bRecursion, BOOL bEnumFiles, EnumerateFunc pFunc, void* pUserData)
	{

		static BOOL s_bUserBreak = FALSE;
		try
		{
			//-------------------------------------------------------------------------
			if(s_bUserBreak) 
				return;

			size_t len = wcslen(lpPath);
			if (lpPath==NULL || len<=0) 
				return;

			

			std::wstring path;
			path.append(lpPath);
			if (*(path.end()-1) != L'\\')
				path.append(L"\\*");


			WIN32_FIND_DATA fd;
			HANDLE hFindFile = FindFirstFileW(path.c_str(), &fd);
			if(hFindFile == INVALID_HANDLE_VALUE)
			{
				::FindClose(hFindFile); 
				return;
			}

		
			BOOL bUserReture=TRUE; 
			BOOL bIsDirectory;
			std::wstring tempPath;

			BOOL bFinish = FALSE;
			while(!bFinish)
			{
				tempPath.clear();
				tempPath.append(lpPath);
				if (*(tempPath.end()-1) != L'\\')
					tempPath.append(L"\\");
				tempPath.append(fd.cFileName);				

				bIsDirectory = ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);

				//如果是.或..
				if( bIsDirectory
					&& (wcscmp(fd.cFileName, L".") == 0 || wcscmp(fd.cFileName, L"..") == 0)) 
				{		
					
					bFinish = (FindNextFileW(hFindFile, &fd) == FALSE);
					continue;
				}

				if(pFunc && bEnumFiles!=bIsDirectory)
				{
					bUserReture = pFunc(tempPath.c_str(), pUserData);
					if(bUserReture==FALSE)
					{
						s_bUserBreak = TRUE; 
						::FindClose(hFindFile); 
						return;
					}
				}				

				if(bIsDirectory && bRecursion) //是子目录
				{
					doFileEnumeration(tempPath.c_str(), bRecursion, bEnumFiles, pFunc, pUserData);
				}

				bFinish = (FindNextFileW(hFindFile, &fd) == FALSE);
			}

			::FindClose(hFindFile);

			//-------------------------------------------------------------------------
		}
		catch(...)
		{ 
			ATLASSERT(0); 
			return; 
		}
	}


	inline std::wstring GetShellFolderPath(int CSIDL)
	{
		std::wstring retval;
		LPITEMIDLIST pID;
		wchar_t buffer[MAX_PATH + 10] = {0};
		if (SUCCEEDED(::SHGetSpecialFolderLocation(NULL, CSIDL, &pID)))
		{
			if (::SHGetPathFromIDListW(pID, buffer))
				retval = buffer;
			GlobalFree(pID);
		}
		return retval;			
	}


	inline DWORD GetLargeFileSize(const std::wstring& FileName)
	{
		DWORD dwSizeHigh;
		DWORD dwSizeLow = GetCompressedFileSize(FileName.c_str(), &dwSizeHigh);
		return dwSizeHigh;
	}
	


	inline std::wstring getFullPath(wchar_t* filePath, wchar_t* fname)
	{
		std::wstring Result = L"";
		// In Windows 8, the first parameter seems to be used for more than just the
		// path name.  For example, its numerical value can be 1.  Passing a non-valid
		// pointer to SearchPathW will cause a crash, so we need to check to see if we
		// are handed a valid pointer, and otherwise just pass NULL to SearchPathW.
		PWCHAR sanitizedFilePath = (intptr_t(filePath) < 1024) ? NULL : filePath;

		// figure out the length of the string that we need
		DWORD pathlen = SearchPathW(sanitizedFilePath, fname, L".dll", 0, NULL,
			NULL);
		if (pathlen == 0)
		{
			return Result;
		}

		wchar_t* full_fname = new wchar_t[pathlen+1];
		if (!full_fname) {			
			return Result;			
		}

		// now actually grab it
		SearchPathW(sanitizedFilePath, fname, L".dll", pathlen + 1, full_fname,
			NULL);
		Result = std::wstring(full_fname);

		delete[] full_fname;
		return Result;
	}
	

}

#endif