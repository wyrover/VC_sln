#ifndef _ROVERLIB_STRUTILS_H_
#define _ROVERLIB_STRUTILS_H_

#include <string>
#include <algorithm> 
#include <iostream>
#include <sstream>
#include <iomanip>
#include <Lmcons.h>

namespace roverlib
{
	
	LPWSTR		Ansi2Unicode(LPCSTR lpStr, DWORD nMaxLen = 0);
	LPSTR		Unicode2Ansi(LPCWSTR lpStr, DWORD nMaxLen = 0);
	LPSTR		ToUTF8Str(LPCSTR lpStr);
	LPSTR		ToUTF8Str(LPCWSTR lpStr, DWORD nMaxLen = 0);
	LPSTR		UTF8ToAnsi(LPCSTR lpStr);
	std::wstring ConvertToWide(LPCSTR str, int codepage);
	std::string ConvertToAscii(LPCWSTR str, int codepage);
	std::string ConvertWideToAscii(LPCWSTR str);
	std::wstring ConvertAsciiToWide(LPCSTR str);
	std::wstring ConvertUTF8ToWide(LPCSTR str);
	std::string ConvertWideToUTF8(LPCWSTR str);
	std::string ConvertAsciiToUTF8(LPCSTR str, int codepage);
	long AnsiToBstr(const std::string& stIn, BSTR& bsOut);
	long BstrToAnsi(const BSTR bsIn, char*& pcOut);
	std::string trim(const std::string& str);
	std::string& trim(std::string& s);
	std::string toLower(const std::string& str);
	std::string toupper(const std::string& str);
	std::string trimleft(const std::string& str);
	std::string trimright(const std::string& str);
	bool startswith(const std::string& str, const std::string& substr);
	bool endswith(const std::string& str, const std::string& substr);
	bool equalsIgnoreCase(const std::string& str1, const std::string& str2);
	template<class T> T parseString(const std::string& str);
	template<class T> T parseHexString(const std::string& str);
	template<bool> bool parseString(const std::string& str);

	template<class T> std::string toString(const T& value);
	template<class T> std::string toHexString(const T& value, int width = 0);
	std::string toString(const bool& value);



	BOOL IsGuid(LPCTSTR szGuid);	
	BOOL SameGuids(const GUID &guid1, const GUID &guid2);
	BOOL GuidIsNull(const GUID &guid);
	BOOL GuidFromString(LPCTSTR szGuid, GUID &guid);
	BOOL GuidToString(const GUID &guid, CString &sGuid);
	CString GetComputerName();
	CString GetUserName();
	size_t GetCharNum(LPCTSTR lpszText);
	DWORD MBCStoUNICODE(const char *szMbcs, wchar_t *szUnicode);
	DWORD UNICODEtoMBCS(const wchar_t *szUnicode, char *szMbcs);
	void Split(CString source, std::vector<CString>& dest, CString division);
	void TestCStringFormat();
	void TestCStringTostring();
	void CalcMD5(PVOID lpBuff, DWORD dwSize, LPBYTE lpMd5);
	BOOL CalcFileMD5(LPCTSTR lpFilePath, LPBYTE lpMD5);
	std::wstring GetSysDateString();
	CStringW CStrA2CStrW(const CStringA &cstrSrcA);
	CStringA CStrW2CStrA(const CStringW &cstrSrcW);
	string wstring_to_string(const wstring& ws);
	wstring string_to_wstring(const string& s);
}


namespace roverlib 
{	

	LPWSTR Ansi2Unicode(LPCSTR lpStr,  DWORD nMaxLen)
	{
		LPWSTR lpResult = NULL;

		size_t nLen = strlen(lpStr);
		DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, lpStr, (DWORD)nLen, NULL, 0);

		lpResult = new WCHAR[max(dwNum, nMaxLen) + 1];
		if ( lpResult != NULL )
		{
			lpResult[0] = 0;
			MultiByteToWideChar(CP_ACP, 0, lpStr, (DWORD)nLen, lpResult, dwNum);
			lpResult[dwNum] = 0;
		}

		return lpResult;
	}

	LPSTR Unicode2Ansi(LPCWSTR lpStr, DWORD nMaxLen)
	{
		LPSTR lpResult = NULL;

		size_t nLen = wcslen(lpStr);
		DWORD dwNum = WideCharToMultiByte(CP_ACP, 0, lpStr, (DWORD)nLen, NULL, 0, NULL, NULL);

		lpResult = new CHAR[max(dwNum, nMaxLen) + 1];
		if ( lpResult != NULL )
		{
			lpResult[0] = 0;
			WideCharToMultiByte(CP_ACP, 0, lpStr, (DWORD)nLen, lpResult, dwNum, NULL, NULL);
			lpResult[dwNum] = 0;
		}

		return lpResult;
	}

	LPSTR ToUTF8Str(LPCSTR lpStr)
	{
		LPSTR lpResult = NULL;

		LPWSTR lpTemp = Ansi2Unicode(lpStr);
		if ( lpTemp != NULL )
		{
			lpResult = ToUTF8Str(lpTemp);

			delete lpTemp;
			lpTemp = NULL;
		}

		return lpResult;
	}

	LPSTR ToUTF8Str(LPCWSTR lpStr, DWORD nMaxLen)
	{
		LPSTR lpResult = NULL;

		size_t nLen = wcslen(lpStr);
		DWORD dwNum = WideCharToMultiByte(CP_UTF8, 0, lpStr, (DWORD)nLen, NULL, 0, NULL, NULL);

		lpResult = new CHAR[max(dwNum, nMaxLen) + 1];
		if ( lpResult != NULL )
		{
			lpResult[0] = 0;
			WideCharToMultiByte(CP_UTF8, 0, lpStr, (DWORD)nLen, lpResult, dwNum, NULL, NULL);
			lpResult[dwNum] = 0;
		}

		return lpResult;
	}

	LPSTR UTF8ToAnsi(LPCSTR lpStr)
	{
		LPSTR lpResult = NULL;

		// utf8 -> unicode -> ansi
		LPWSTR lpTemp = Ansi2Unicode(lpStr);
		if ( lpTemp != NULL )
		{
			size_t nLen = wcslen(lpTemp);
			DWORD dwNum = WideCharToMultiByte(CP_ACP, 0, lpTemp, (DWORD)nLen, NULL, 0, NULL, NULL);

			lpResult = new CHAR[dwNum + 1];
			if ( lpResult != NULL )
			{
				lpResult[0] = 0;
				WideCharToMultiByte(CP_ACP, 0, lpTemp, (DWORD)nLen, lpResult, dwNum, NULL, NULL);
				lpResult[dwNum] = 0;
			}

			delete lpTemp;
			lpTemp = NULL;
		}

		return lpResult;
	}




	std::wstring ConvertToWide(LPCSTR str, int codepage)
	{
		std::wstring szWide;

		if (str && *str)
		{
			int strLen = (int)strlen(str);
			int bufLen = MultiByteToWideChar(codepage, 0, str, strLen, NULL, 0);
			if (bufLen > 0)
			{
				szWide.resize(bufLen);
				MultiByteToWideChar(codepage, 0, str, strLen, &szWide[0], bufLen);
			}
		}
		return szWide;
	}

	std::string ConvertToAscii(LPCWSTR str, int codepage)
	{
		std::string szAscii;

		if (str && *str)
		{
			int strLen = (int)wcslen(str);
			int bufLen = WideCharToMultiByte(codepage, 0, str, strLen, NULL, 0, NULL, NULL);
			if (bufLen > 0)
			{
				szAscii.resize(bufLen);
				WideCharToMultiByte(codepage, 0, str, strLen, &szAscii[0], bufLen, NULL, NULL);
			}
		}
		return szAscii;
	}

	std::string ConvertWideToAscii(LPCWSTR str)
	{
		return ConvertToAscii(str, CP_ACP);
	}

	std::wstring ConvertAsciiToWide(LPCSTR str)
	{
		return ConvertToWide(str, CP_ACP);
	}

	std::wstring ConvertUTF8ToWide(LPCSTR str)
	{
		return ConvertToWide(str, CP_UTF8);
	}

	std::string ConvertWideToUTF8(LPCWSTR str)
	{
		return ConvertToAscii(str, CP_UTF8);
	}

	std::string ConvertAsciiToUTF8(LPCSTR str, int codepage)
	{
		std::string szUTF8;

		if (str && *str)
		{
			std::wstring wide = ConvertToWide(str, codepage);
			if (!wide.empty())
			{
				szUTF8.swap(ConvertWideToUTF8(wide.c_str()));
			}
		}
		return szUTF8;
	}

	//
	//      AnsiToBstr() allocates a BSTR. Free it with ::SysFreeString().
	//
	long AnsiToBstr(const std::string& stIn, BSTR& bsOut)
	{
		if( !stIn.empty() )
		{
			size_t nWideChars = ::MultiByteToWideChar(CP_ACP, 0, stIn.c_str(), (int)stIn.length(), 0, 0);
			if( !nWideChars )
				return ::GetLastError();

			bsOut = ::SysAllocStringLen(0, (unsigned int)nWideChars);
			size_t lRet = ::MultiByteToWideChar(CP_ACP, 0, stIn.c_str(), (int)stIn.length(), bsOut, (int)nWideChars);
			if( !lRet )
			{
				lRet = ::GetLastError();
				::SysFreeString(bsOut);
				bsOut = 0;
				return (long)lRet;
			}
		}
		else
		{
			size_t lRet = ::MultiByteToWideChar(CP_ACP, 0, "", 0, bsOut, 0);
			// bsOut = ::SysAllocString("");
		}
		return 0;
	}

	//
	//      BstrToAnsi() allocates a char*. Free it with delete[]
	//
	long BstrToAnsi(const BSTR bsIn, char*& pcOut)
	{
		if( bsIn )
		{
			long nAnsiChars = ::WideCharToMultiByte(CP_ACP, 0, bsIn, -1, 0, 0, 0, 0);
			if( !nAnsiChars )
				return ::GetLastError();

			pcOut = new char[nAnsiChars]; // nAnsiChars includes space for the terminating zero.
			long lRet = ::WideCharToMultiByte(CP_ACP, 0, bsIn, -1, pcOut, nAnsiChars, 0, 0);
			if( !lRet )
			{
				lRet = ::GetLastError();
				delete []pcOut;
				pcOut = 0;
				return lRet;
			}
		}
		else
		{
			pcOut = new char[1];
			pcOut[0];
		}
		return 0;
	}

	std::string trim(const std::string& str)
	{
		std::string retval = str;
		retval.erase(0, retval.find_first_not_of(" /t/n/r")).erase(retval.find_last_not_of(" /t/n/r") + 1);
		return retval;
	}	

	std::string& trim(std::string& s) 
	{
		if (s.empty()) 
		{
			return s;
		}

		std::string::iterator c;
		// Erase whitespace before the string
		for (c = s.begin(); c != s.end() && iswspace(*c++););
		s.erase(s.begin(), --c);

		// Erase whitespace after the string
		for (c = s.end(); c != s.begin() && iswspace(*--c););
		s.erase(++c, s.end());

		return s;
	}


	BOOL IsGuid(LPCTSTR szGuid)
	{
		GUID guid;

		return GuidFromString(szGuid, guid);
	}

	BOOL SameGuids(const GUID &guid1, const GUID &guid2) 
	{ 
		return (memcmp(& guid1, & guid2, sizeof(GUID)) == 0); 
	}

	BOOL GuidIsNull(const GUID &guid)
	{
		static GUID NULLGUID = {0};

		return SameGuids(guid, NULLGUID);
	}

	void NullGuid(GUID &guid) 
	{ 
		ZeroMemory(& guid, sizeof(guid)); 
	}


	BOOL GuidFromString( LPCTSTR szGuid, GUID & guid )
	{

#pragma comment(lib, "Rpcrt4.lib")

		RPC_STATUS rpcs = UuidFromString((RPC_WSTR)szGuid, & guid);

		if (rpcs != RPC_S_OK)
		{
			NullGuid(guid);
			return FALSE;
		}

		return TRUE;
	}

	BOOL GuidToString(const GUID &guid, CString &sGuid)
	{

#pragma comment(lib, "Rpcrt4.lib")

		RPC_WSTR pszGuid;

		if (RPC_S_OK == UuidToString((UUID *)&guid, & pszGuid))
			sGuid = (LPCWSTR)pszGuid;
		else
			sGuid.Empty();

		RpcStringFree(&pszGuid);

		return !sGuid.IsEmpty();
	}


	CString GetComputerName()
	{
		static CString sMachine;

		if (sMachine.IsEmpty())
		{
			DWORD LEN = MAX_COMPUTERNAME_LENGTH + 1;

			::GetComputerName(sMachine.GetBuffer(LEN), &LEN);

			sMachine.ReleaseBuffer();
		}

		return sMachine;
	}

	CString GetUserName()
	{
		static CString sUser;

		if (sUser.IsEmpty())
		{
			DWORD LEN = UNLEN + 1;

			::GetUserName(sUser.GetBuffer(LEN), &LEN);

			sUser.ReleaseBuffer();
		}

		return sUser;
	}


	/** 
	* 函数：GetCharNum
	* 描述：获取字符串中字符的个数
	* 返回：字符个数
	*/
	size_t GetCharNum(LPCTSTR lpszText)
	{
		size_t nRet = 0;

#ifdef _UNICODE

		nRet = wcslen(lpszText);

#else

		int nLen = strlen(lpszText);	// 获取参数占用的字节数
		if (nLen == 0)
		{
			return 0;
		}

		LPWSTR lpszBuff = new WCHAR[nLen];
		if (lpszBuff == NULL)
		{
			return 0;
		}

		USES_CONVERSION;

		wcscpy(lpszBuff, A2W(lpszText));

		nRet = wcslen(lpszBuff);

		delete[] lpszBuff;

#endif

		return nRet;
	}


	/***************************************************
	函数功能: 多字节转换为宽字符
	输入: szMbcs 多字节类型字符串
	输出: szUnicode 宽字符类型字符串
	返回值: 字符串转换为宽字符后,字符的个数
	***************************************************/
	DWORD MBCStoUNICODE(const char *szMbcs, wchar_t *szUnicode)
	{
		DWORD dwMinSize = MultiByteToWideChar (CP_ACP, 0, szMbcs, -1, NULL, 0);
		wchar_t *pWchar = new wchar_t[dwMinSize];
		MultiByteToWideChar (CP_ACP, 0, szMbcs, -1, pWchar , dwMinSize); 
		wcsncpy(szUnicode, pWchar, dwMinSize);
		*(szUnicode + wcslen(szUnicode)) = '/0';

		delete [] pWchar; 
		return dwMinSize;
	}
	/***************************************************
	函数功能: 宽字符转换为多字节
	输入: szUnicode 宽字符类型字符串
	输出: szMbcs 多字节类型字符串
	返回值: 字符串转换为多字节后,占用的字节数
	***************************************************/
	DWORD UNICODEtoMBCS(const wchar_t *szUnicode, char *szMbcs)
	{
		DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, szUnicode, -1, NULL, 0, NULL, FALSE);
		char* pChar = new char[dwNum];

		WideCharToMultiByte(CP_OEMCP, NULL, szUnicode, -1, pChar, dwNum, NULL, FALSE);
		strncpy(szMbcs, pChar, dwNum);

		*(szMbcs + strlen(szMbcs)) = '\0';

		delete [] pChar;
		return dwNum;
	}
	
	void TestCStringFormat()
	{
		// 格式化%本身
		CString strText;
		strText.Format(_T("25%c"), 0x25);

		// __int64 为%I64d

		// unsigned __int64 为%I64u
	}

	void TestCStringTostring()
	{
		// CString -> std::string
		CStringA strMfc = "test"; 
		std::string strStl; 
		strStl = strMfc.GetBuffer(0);

		// unicode
		CStringW strw = L"test";
		CStringA stra(strw.GetBuffer(0));
		strw.ReleaseBuffer();
		std::string imgpath = stra.GetBuffer(0);
		stra.ReleaseBuffer();


		// std::string -> CString
		CStringA strMfc2;
		std::string strStl2 = "test"; 
		strMfc2 = strStl2.c_str(); 


		// CString::Compare
		CString s1(_T("abc"));
		CString s2(_T("abd"));		
		ATLASSERT(s1.Compare(s2) == -1);
	}

	void Split( CString source, std::vector<CString>& dest, CString division )
	{		
		dest.clear();
		int pos = 0;
		int pre_pos = 0;
		while( -1 != pos ){
			pre_pos = pos;
			pos = source.Find(division,(pos+1));			
			dest.push_back(source.Mid(pre_pos,(pos-pre_pos)));
		}
	}

	std::string toLower(const std::string& str)
	{
		std::string retval = str;
/*		std::transform(retval.begin(), retval.end(), retval.begin(), tolower);*/
		return retval;
	}

	std::string toupper(const std::string& str)
	{
		std::string retval = str;
/*		std::transform(retval.begin(), retval.end(), retval.begin(), toupper);*/
		return retval;
	}

	std::string trimleft(const std::string& str)
	{
		std::string retval = str;
		retval.erase(0, retval.find_first_not_of(" /t/n/r"));
		return retval;
	}

	std::string trimright(const std::string& str)
	{
		std::string retval = str;
		retval.erase(retval.find_last_not_of(" /t/n/r") + 1);
		return retval;
	}

	bool startswith(const std::string& str, const std::string& substr )
	{
		return str.find(substr) == 0;
	}

	bool endswith(const std::string& str, const std::string& substr)
	{
		return str.rfind(substr) == (str.length() - substr.length());
	}

	bool equalsIgnoreCase(const std::string& str1, const std::string& str2) 
	{
		return toLower(str1) == toLower(str2);
	}



	template<class T> T parseString(const std::string& str) {
		T value;
		std::istringstream iss(str);
		iss >> value;
		return value;
	}

	template<class T> T parseHexString(const std::string& str) {
		T value;
		std::istringstream iss(str);
		iss >> hex >> value;
		return value;
	}

	template<bool>
	bool parseString(const std::string& str) {
		bool value;
		std::istringstream iss(str);
		iss >> boolalpha >> value;
		return value;
	}

	template<class T> std::string toString(const T& value) {
		std::ostringstream oss;
		oss << value;
		return oss.str();
	}

	std::string toString(const bool& value) 
	{
		std::ostringstream oss;
		/*oss << boolalpha << value;*/
		return oss.str();
	}

	template<class T> std::string toHexString(const T& value, int width) 
	{
		std::ostringstream oss;
		oss << hex;
		if (width > 0) {
			oss << setw(width) << setfill('0');
		}
		oss << value;
		return oss.str();
	}


	std::wstring GetSysDateString()
	{
		SYSTEMTIME sysTime = {0};
		wchar_t szDate[MAX_PATH] = {0};

		::GetSystemTime(&sysTime);
		::swprintf_s(szDate, L"%u-%02u-%u", sysTime.wYear, sysTime.wMonth, sysTime.wDay );
		return std::wstring(szDate);
	}

	
	CStringW CStrA2CStrW(const CStringA &cstrSrcA)
	{
		int len = MultiByteToWideChar(CP_ACP, 0, LPCSTR(cstrSrcA), -1, NULL, 0);
		wchar_t *wstr = new wchar_t[len];
		memset(wstr, 0, len*sizeof(wchar_t));
		MultiByteToWideChar(CP_ACP, 0, LPCSTR(cstrSrcA), -1, wstr, len);
		CStringW cstrDestW = wstr;
		delete[] wstr;

		return cstrDestW;
	}

	CStringA CStrW2CStrA(const CStringW &cstrSrcW)
	{
		int len = WideCharToMultiByte(CP_ACP, 0, LPCWSTR(cstrSrcW), -1, NULL, 0, NULL, NULL);
		char *str = new char[len];
		memset(str, 0, len);
		WideCharToMultiByte(CP_ACP, 0, LPCWSTR(cstrSrcW), -1, str, len, NULL, NULL);
		CStringA cstrDestA = str;
		delete[] str;

		return cstrDestA;
	}


	string wstring_to_string(const wstring& ws)
	{
		string curLocale = setlocale(LC_ALL, NULL);   
		setlocale(LC_ALL, "chs");
		const wchar_t*  _Source;
		size_t         size_target;

		string         result;
		_Source        = ws.c_str();
		size_target    = 2 * ws.size() + 1;

		int n = wcstombs(  NULL,_Source, 0  );
		if (n<=0)
		{
			return string("");
		}
		result.resize( n);

		n = wcstombs( (char *)(result.c_str()),_Source,n+1);

		setlocale(LC_ALL, curLocale.c_str());
		return result;
	}

	wstring string_to_wstring(const string& s)
	{
		string curLocale = setlocale(LC_ALL, NULL);   
		const char     *_Source  ;
		wstring        result ;
		int            n ;
		setlocale(LC_ALL, "chs");
		_Source       = s.c_str();

		n  =  mbstowcs(NULL,_Source,0 );
		if(n<=0)
		{
			return wstring(L"");
		}
		result.resize(n);
		mbstowcs( ( wchar_t *)( result.c_str()) ,_Source, n+1);

		setlocale(LC_ALL, curLocale.c_str());
		return result;
	}



}


#endif
