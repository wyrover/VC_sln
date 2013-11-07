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
	
	std::string toOctalString(unsigned int n);
	int strcasecmp(const char* s1, const char* s2);
	int strncasecmp(const char* s1, const char* s2, size_t count);
	int vsnprintf(char* buffer, size_t size, const char* format, va_list arguments);
	int vswprintf(wchar_t* buffer, size_t size, const wchar_t* format, va_list arguments);


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
	void trimleft(std::string& toTrim);
	std::string trimright(const std::string& str);
	void trimright(std::string& toTrim);
	bool startswith(const std::string& str, const std::string& substr);
	bool endswith(const std::string& str, const std::string& substr);
	bool endswith(const std::wstring& str, const std::wstring& substr);
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
	
	size_t GetCharNum(LPCTSTR lpszText);
	DWORD MBCStoUNICODE(const char *szMbcs, wchar_t *szUnicode);
	DWORD UNICODEtoMBCS(const wchar_t *szUnicode, char *szMbcs);
	std::wstring string2wstring(const std::string & rString, UINT codepage);
	std::string wstring2string(const std::wstring & rwString, UINT codepage);
	void Split(CString source, std::vector<CString>& dest, CString division);
	void TestCStringFormat();
	void TestCStringTostring();
	void CalcMD5(PVOID lpBuff, DWORD dwSize, LPBYTE lpMd5);
	BOOL CalcFileMD5(LPCTSTR lpFilePath, LPBYTE lpMD5);
	std::wstring GetSysDateString();
	CStringW CStrA2CStrW(const CStringA &cstrSrcA);
	CStringA CStrW2CStrA(const CStringW &cstrSrcW);
	std::string wstring_to_string(const std::wstring& ws);
	std::wstring string_to_wstring(const std::string& s);

	

	class StringUtil
	{
	public:
		static const std::string sEmpty;
		static const std::wstring sWEmpty;
		static const char* getNotNullString(const char* strValue)
		{
			return (strValue?strValue:sEmpty.c_str());
		}

		static const wchar_t* getNotNullString(const wchar_t* strValue)
		{
			return (strValue?strValue:sWEmpty.c_str());
		}


		template<typename StringPtrType>
		static bool NullOrEmpty(StringPtrType strPtr)
		{
			return !(strPtr && strPtr[0]);
		}

		template<typename StringPtrType>
		static bool NotNullOrEmpty(StringPtrType strPtr)
		{
			return (strPtr && strPtr[0]);
		}

		template<typename StringType>
		static void Trim(StringType& toTrim)
		{
			TrimRight(toTrim);
			TrimLeft(toTrim);
		}

		template<typename StringType>
		static void TrimLeft(StringType& toTrim)
		{
			while (::isspace(toTrim[0]))
				toTrim.erase(toTrim.begin(), toTrim.begin() + 1);
		}

		template<typename StringType>
		static void TrimRight(StringType& toTrim)
		{
			while (::isspace(toTrim[toTrim.length() - 1]))
				toTrim.erase(toTrim.length() - 1);
		}

		template<typename StringType>
		static void replace(StringType& strBig, const StringType& strSrc, const StringType& strDst)
		{
			typedef typename StringType::size_type size_type;
			size_type pos = 0;
			size_type srcLen = strSrc.size();
			size_type dstLen = strDst.size();

			while((pos=strBig.find(strSrc, pos)) != StringType::npos)
			{
				strBig.replace(pos, srcLen, strDst);
				pos += dstLen;
			}
		}
	};

	__declspec(selectany) const std::wstring StringUtil::sWEmpty = L"";
	__declspec(selectany) const std::string StringUtil::sEmpty = "";
}


namespace roverlib 
{	
	inline std::string toOctalString(unsigned int n)
	{
		std::string s;
		s.resize(32);
		std::string::size_type charPos = 32;
		const int radix = 1 << 3;
		int mask = radix - 1;
		do
		{
			s[--charPos] = '0' + (n & mask);
			n >>= 3;
		}
		while(n != 0);

		return std::string(s, charPos, (32 - charPos));
	}



	inline int strcasecmp(const char* s1, const char* s2)
	{
		return _stricmp(s1, s2);
	}

	inline int strncasecmp(const char* s1, const char* s2, size_t count) 
	{
		return _strnicmp(s1, s2, count);
	}

	inline int vsnprintf(char* buffer, size_t size,	const char* format, va_list arguments) 
	{
		int length = vsnprintf_s(buffer, size, size - 1, format, arguments);
		if (length < 0)
			return _vscprintf(format, arguments);
		return length;
	}

	inline int vswprintf(wchar_t* buffer, size_t size, const wchar_t* format, va_list arguments) 
	{
		int length = _vsnwprintf_s(buffer, size, size - 1, format, arguments);
		if (length < 0)
			return _vscwprintf(format, arguments);
		return length;
	}

	inline int snprintf(char* buffer, size_t size, const char* format, ...) 
	{
		va_list arguments;
		va_start(arguments, format);
		int result = vsnprintf(buffer, size, format, arguments);
		va_end(arguments);
		return result;
	}

	inline int swprintf(wchar_t* buffer, size_t size, const wchar_t* format, ...) 
	{
		va_list arguments;
		va_start(arguments, format);
		int result = vswprintf(buffer, size, format, arguments);
		va_end(arguments);
		return result;
	}

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
		std::string Result = str;
		Result.erase(0, Result.find_first_not_of(" /t/n/r")).erase(Result.find_last_not_of(" /t/n/r") + 1);
		return Result;
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

	std::wstring GetComputerName()
	{
		static std::wstring Result;
		if (Result.empty())
		{
			DWORD LEN = MAX_COMPUTERNAME_LENGTH + 1;
			wchar_t Buffer[MAX_COMPUTERNAME_LENGTH + 1];
			::GetComputerNameW(Buffer, &LEN);
			Result = Buffer;
		}
		return Result;
	}

	std::wstring GetUserName()
	{
		static std::wstring Result;
		if (Result.empty())
		{
			DWORD LEN = UNLEN + 1;
			wchar_t Buffer[UNLEN + 1];
			::GetUserNameW(Buffer, &LEN);
			Result = Buffer;
		}
		return Result;
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
		*(szUnicode + wcslen(szUnicode)) = '\0';

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


	std::wstring string2wstring(const std::string & rString, UINT codepage)
	{
		int len = MultiByteToWideChar(codepage, 0, rString.c_str(), -1, NULL, 0);
		if(len > 0)
		{		
			std::vector<wchar_t> vw(len);
			MultiByteToWideChar(codepage, 0, rString.c_str(), -1, &vw[0], len);
			return &vw[0];
		}
		else
			return L"";
	}

	std::string wstring2string(const std::wstring & rwString, UINT codepage)
	{
		int len = WideCharToMultiByte(codepage, 0, rwString.c_str(), -1, NULL, 0, NULL, NULL);
		if(len > 0)
		{		
			std::vector<char> vw(len);
			WideCharToMultiByte(codepage, 0, rwString.c_str(), -1, &vw[0], len, NULL, NULL);
			return &vw[0];
		}
		else
			return "";
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

	inline std::string toLower(const std::string& str)
	{
		std::string Result = str;
		std::transform(Result.begin(), Result.end(), Result.begin(), ::tolower);
		return Result;
	}

	inline void toLower(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	}
	

	inline std::string toUpper(const std::string& str)
	{
		std::string Result = str;
		std::transform(Result.begin(), Result.end(), Result.begin(), ::toupper);
		return Result;
	}

	inline void toUpper(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	}


	inline void replaceFirst(std::string& str, const std::string& ths, const std::string& with)
	{
		size_t start_pos = str.find(ths);
		if(start_pos != std::string::npos)
		{
			str.replace(start_pos, ths.length(), with);
		}
	}

	inline std::string replaceFirstCopy(const std::string& str, const std::string& ths, const std::string& with)
	{
		std::string Result = str;
		replaceFirst(Result, ths, with);
		return Result;
	}


	inline void replaceAll(std::string& str, const std::string& ths, const std::string& with)
	{
		if(ths.empty())
			return;
		size_t start_pos = 0;
		while((start_pos = str.find(ths, start_pos)) != std::string::npos) {
			str.replace(start_pos, ths.length(), with);
			start_pos += with.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
		}
	}

	inline std::string replaceAllCopy(const std::string& str, const std::string& ths, const std::string& with)
	{
		std::string Result = str;
		replaceAll(Result, ths, with);
		return Result;
	}

	inline void eraseAll(std::string& str, const std::string& ths)
	{
		if(ths.empty())
			return;
		size_t start_pos = 0;
		while((start_pos = str.find(ths)) != std::string::npos) {
			str.erase(start_pos, ths.length());
		}
	}

	inline void replaceLast(std::string& str, const std::string& ths, const std::string& with)
	{
		size_t start_pos = str.find_last_of(ths);
		if(start_pos != std::string::npos)
		{
			str.replace(start_pos, ths.length(), with);
		}
	}

	inline std::string replaceLastCopy(const std::string& str, const std::string& ths, const std::string& with)
	{
		std::string Result = str;
		replaceLast(Result, ths, with);
		return Result;
	}



	inline void splitInternal(std::vector<std::string>& output, const std::string& input, const std::string& delimiter)
	{
		size_t start = 0;

		std::string temp = input;
		start = temp.find(delimiter);
		while(start != std::string::npos)
		{
			if(start!=0)
			{
				output.push_back(temp.substr(0, start));
			}
			else
			{
				output.push_back("");
			}
			if(temp.length()>start+delimiter.length())
			{
				temp = temp.substr(start+delimiter.length());
				start = temp.find(delimiter);
			}
			else
			{
				temp = temp.substr(start);
				break;
			}
		}
		replaceFirst(temp, delimiter, "");
		output.push_back(temp);
	}

	


	inline void split(std::vector<std::string> &output, const std::string& input, const std::string& delimiter)
	{
		output.clear();
		splitInternal(output, input, delimiter);
	}

	inline std::vector<std::string> split(const std::string& input, const std::string& delimiter)
	{
		std::vector<std::string> output;
		split(output, input, delimiter);
		return output;

	}

	

	inline void split(std::vector<std::string>& output, const std::string& input, std::vector<std::string> delimiters)
	{
		output.clear();
		output.push_back(input);
		for (int var = 0; var < (int)delimiters.size(); ++var) {
			std::vector<std::string> output1;
			for (int var1 = 0; var1 < (int)output.size(); ++var1) {
				splitInternal(output1, output.at(var1), delimiters.at(var));
			}
			output.swap(output1);
		}
	}

	inline std::vector<std::string> split(const std::string& input, std::vector<std::string> delimiters)
	{
		std::vector<std::string> output;
		split(output, input, delimiters);
		return output;
	}


	inline std::string joinString(const std::vector<std::string>& values, const std::string& delimiter)
	{
		std::ostringstream out;
		for(unsigned int i = 0; i < values.size(); i++)
		{
			if(i != 0)
			{
				out << delimiter;
			}
			out << values[i];
		}
		return out.str();
	}


	inline std::string removeWhitespace(const std::string& S)
	{
		std::string Result;
		for(unsigned int i = 0; i < S.length(); ++ i)
		{
			if(!isspace(static_cast<unsigned char>(S[i])))
			{
				Result += S[i];
			}
		}
		return Result;
	}




	inline std::string trimleft(const std::string& str)
	{
		std::string Result = str;
		Result.erase(0, Result.find_first_not_of(" /t/n/r"));
		return Result;
	}


	inline void trimleft(std::string& toTrim)
	{
		while (::isspace(toTrim[0]))
			toTrim.erase(toTrim.begin(), toTrim.begin() + 1);
	}


	inline std::string trimright(const std::string& str)
	{
		std::string retval = str;
		retval.erase(retval.find_last_not_of(" /t/n/r") + 1);
		return retval;
	}


	inline void trimright(std::string& toTrim)
	{
		while (::isspace(toTrim[toTrim.length() - 1]))
			toTrim.erase(toTrim.length() - 1);
	}


	inline bool startswith(const std::string& str, const std::string& substr )
	{
		return str.find(substr) == 0;
	}

	inline bool endswith(const std::string& str, const std::string& substr)
	{
		return str.rfind(substr) == (str.length() - substr.length());
	}

	inline bool endswith(const std::wstring& str, const std::wstring& substr)
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


	std::string wstring_to_string(const std::wstring& ws)
	{
		std::string curLocale = setlocale(LC_ALL, NULL);   
		setlocale(LC_ALL, "chs");
		const wchar_t*  _Source;
		size_t         size_target;

		std::string         result;
		_Source        = ws.c_str();
		size_target    = 2 * ws.size() + 1;

		int n = wcstombs(  NULL,_Source, 0  );
		if (n<=0)
		{
			return std::string("");
		}
		result.resize( n);

		n = wcstombs( (char *)(result.c_str()),_Source,n+1);

		setlocale(LC_ALL, curLocale.c_str());
		return result;
	}

	std::wstring string_to_wstring(const std::string& s)
	{
		std::string curLocale = setlocale(LC_ALL, NULL);   
		const char     *_Source  ;
		std::wstring        result ;
		int            n ;
		setlocale(LC_ALL, "chs");
		_Source       = s.c_str();

		n  =  mbstowcs(NULL,_Source,0 );
		if(n<=0)
		{
			return std::wstring(L"");
		}
		result.resize(n);
		mbstowcs( ( wchar_t *)( result.c_str()) ,_Source, n+1);

		setlocale(LC_ALL, curLocale.c_str());
		return result;
	}


	namespace utility
	{

		inline void trim(std::string& _str, bool _left = true, bool _right = true)
		{
			if (_right) _str.erase(_str.find_last_not_of(" \t\r") + 1);
			if (_left) _str.erase(0, _str.find_first_not_of(" \t\r"));
		}

		// 泻芯薪胁械褉褌懈褉芯胁邪薪懈械 胁 褋褌褉芯泻褍
		template<typename T>
		inline std::string toString (T p)
		{
			std::ostringstream stream;
			stream << p;
			return stream.str();
		}

		inline const std::string& toString (const std::string& _value)
		{
			return _value;
		}

		template<typename T1,  typename T2>
		inline std::string toString (T1 p1, T2 p2)
		{
			std::ostringstream stream;
			stream << p1 << p2;
			return stream.str();
		}

		template<typename T1,  typename T2,  typename T3>
		inline std::string toString (T1 p1, T2 p2, T3 p3)
		{
			std::ostringstream stream;
			stream << p1 << p2 << p3;
			return stream.str();
		}

		template<typename T1,  typename T2,  typename T3, typename T4>
		inline std::string toString (T1 p1, T2 p2, T3 p3, T4 p4)
		{
			std::ostringstream stream;
			stream << p1 << p2 << p3 << p4;
			return stream.str();
		}

		template<typename T1,  typename T2,  typename T3, typename T4, typename T5>
		inline std::string toString (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
		{
			std::ostringstream stream;
			stream << p1 << p2 << p3 << p4 << p5;
			return stream.str();
		}

		template<typename T1,  typename T2,  typename T3, typename T4, typename T5, typename T6>
		inline std::string toString (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
		{
			std::ostringstream stream;
			stream << p1 << p2 << p3 << p4 << p5 << p6;
			return stream.str();
		}

		template<typename T1,  typename T2,  typename T3, typename T4, typename T5, typename T6, typename T7>
		inline std::string toString (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
		{
			std::ostringstream stream;
			stream << p1 << p2 << p3 << p4 << p5 << p6 << p7;
			return stream.str();
		}

		template<typename T1,  typename T2,  typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
		inline std::string toString (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
		{
			std::ostringstream stream;
			stream << p1 << p2 << p3 << p4 << p5 << p6 << p7 << p8;
			return stream.str();
		}

		template<typename T1,  typename T2,  typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
		inline std::string toString (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9)
		{
			std::ostringstream stream;
			stream << p1 << p2 << p3 << p4 << p5 << p6 << p7 << p8 << p9;
			return stream.str();
		}

		template<>
		inline std::string toString<bool> (bool _value)
		{
			return _value ? "true" : "false";
		}


		// 褍褌懈谢懈褌褘 写谢褟 锌邪褉褋懈薪谐邪
		template<typename T>
		inline T parseValue( const std::string& _value )
		{
			std::istringstream stream(_value);
			T result;
			stream >> result;
			if (stream.fail())
				return T();
			else
			{
				int item = stream.get();
				while (item != -1)
				{
					if (item != ' ' && item != '\t')
						return T();
					item = stream.get();
				}
			}
			return result;
		}

		// 芯褌写械谢褜薪邪褟 懈屑锌谢械屑械薪褌邪褑懈褟 锌芯写 bool
		template<>
		inline bool parseValue(const std::string& _value)
		{
			if (_value == "True" || _value == "true" || _value == "1")
				return true;
			return false;
		}

		// 芯褌写械谢褜薪邪褟 懈屑锌谢械屑械薪褌邪褑懈褟 锌芯写 char
		template<>
		inline char parseValue(const std::string& _value)
		{
			return (char)parseValue<short>(_value);
		}

		// 芯褌写械谢褜薪邪褟 懈屑锌谢械屑械薪褌邪褑懈褟 锌芯写 unsigned char
		template<>
		inline unsigned char parseValue(const std::string& _value)
		{
			return (unsigned char)parseValue<unsigned short>(_value);
		}


		inline short parseShort(const std::string& _value)
		{
			return parseValue<short>(_value);
		}

		inline unsigned short parseUShort(const std::string& _value)
		{
			return parseValue<unsigned short>(_value);
		}

		inline int parseInt(const std::string& _value)
		{
			return parseValue<int>(_value);
		}

		inline unsigned int parseUInt(const std::string& _value)
		{
			return parseValue<unsigned int>(_value);
		}

		inline size_t parseSizeT(const std::string& _value)
		{
			return parseValue<size_t>(_value);
		}

		inline float parseFloat(const std::string& _value)
		{
			return parseValue<float>(_value);
		}

		inline double parseDouble(const std::string& _value)
		{
			return parseValue<double>(_value);
		}

		inline bool parseBool(const std::string& _value)
		{
			return parseValue<bool>(_value);
		}

		inline char parseChar(const std::string& _value)
		{
			return parseValue<char>(_value);
		}

		inline unsigned char parseUChar(const std::string& _value)
		{
			return parseValue<unsigned char>(_value);
		}

		// 写谢褟 锌邪褉褋懈薪谐邪 褋谢芯卸薪褘褏 褌懈锌芯胁, 褋芯褋褌芯褟褖懈褏 懈蟹 锌褉芯褋褌褘褏
		template<typename T1, typename T2>
		inline T1 parseValueEx2(const std::string& _value)
		{
			T2 p1, p2;
			std::istringstream stream(_value);
			stream >> p1 >> p2;
			if (stream.fail())
				return T1();
			else
			{
				int item = stream.get();
				while (item != -1)
				{
					if (item != ' ' && item != '\t')
						return T1();
					item = stream.get();
				}
			}
			return T1(p1, p2);
		}

		template<typename T1, typename T2>
		inline T1 parseValueEx3(const std::string& _value)
		{
			T2 p1, p2, p3;
			std::istringstream stream(_value);
			stream >> p1 >> p2 >> p3;
			if (stream.fail())
				return T1();
			else
			{
				int item = stream.get();
				while (item != -1)
				{
					if (item != ' ' && item != '\t')
						return T1();
					item = stream.get();
				}
			}
			return T1(p1, p2, p3);
		}

		template<typename T1, typename T2>
		inline T1 parseValueEx4(const std::string& _value)
		{
			T2 p1, p2, p3, p4;
			std::istringstream stream(_value);
			stream >> p1 >> p2 >> p3 >> p4;
			if (stream.fail())
				return T1();
			else
			{
				int item = stream.get();
				while (item != -1)
				{
					if (item != ' ' && item != '\t')
						return T1();
					item = stream.get();
				}
			}
			return T1(p1, p2, p3, p4);
		}

		namespace templates
		{
			template<typename Type>
			inline void split(std::vector<Type>& _ret, const Type& _source, const Type& _delims)
			{
				size_t start = _source.find_first_not_of(_delims);
				while (start != _source.npos)
				{
					size_t end = _source.find_first_of(_delims, start);
					if (end != _source.npos)
						_ret.push_back(_source.substr(start, end - start));
					else
					{
						_ret.push_back(_source.substr(start));
						break;
					}
					start = _source.find_first_not_of(_delims, end + 1);
				}
			}
		} // namespace templates

		inline std::vector<std::string> split(const std::string& _source, const std::string& _delims = "\t\n ")
		{
			std::vector<std::string> result;
			templates::split<std::string>(result, _source, _delims);
			return result;
		}

		template<typename T1, typename T2, typename T3, typename T4>
		inline bool parseComplex(const std::string& _value, T1& _p1, T2& _p2, T3& _p3, T4& _p4)
		{
			std::istringstream stream(_value);

			stream >> _p1 >> _p2 >> _p3 >> _p4;

			if (stream.fail())
				return false;
			int item = stream.get();
			while (item != -1)
			{
				if (item != ' ' && item != '\t')
					return false;
				item = stream.get();
			}

			return true;
		}

		template<typename T1, typename T2, typename T3>
		inline bool parseComplex(const std::string& _value, T1& _p1, T2& _p2, T3& _p3)
		{
			std::istringstream stream(_value);

			stream >> _p1 >> _p2 >> _p3;

			if (stream.fail())
				return false;
			int item = stream.get();
			while (item != -1)
			{
				if (item != ' ' && item != '\t')
					return false;
				item = stream.get();
			}

			return true;
		}

		template<typename T1, typename T2>
		inline bool parseComplex(const std::string& _value, T1& _p1, T2& _p2)
		{
			std::istringstream stream(_value);

			stream >> _p1 >> _p2;

			if (stream.fail())
				return false;
			int item = stream.get();
			while (item != -1)
			{
				if (item != ' ' && item != '\t')
					return false;
				item = stream.get();
			}

			return true;
		}

		template<typename T1>
		inline bool parseComplex(const std::string& _value, T1& _p1)
		{
			std::istringstream stream(_value);

			stream >> _p1;

			if (stream.fail())
				return false;
			int item = stream.get();
			while (item != -1)
			{
				if (item != ' ' && item != '\t')
					return false;
				item = stream.get();
			}

			return true;
		}

		template<>
		inline bool parseComplex<bool>(const std::string& _value, bool& _p1)
		{
			std::string value(_value);
			trim(value);
			if ((value == "True") || (value == "true") || (value == "1"))
			{
				_p1 = true;
				return true;
			}
			else if ((value == "False") || (value == "false") || (value == "0"))
			{
				_p1 = false;
				return true;
			}

			return false;
		}

		inline bool startWith(const std::string& _source, const std::string& _value)
		{
			size_t count = _value.size();
			if (_source.size() < count)
				return false;
			for (size_t index = 0; index < count; ++ index)
			{
				if (_source[index] != _value[index])
					return false;
			}
			return true;
		}

		inline bool endWith(const std::string& _source, const std::string& _value)
		{
			size_t count = _value.size();
			if (_source.size() < count)
				return false;
			size_t offset = _source.size() - count;
			for (size_t index = 0; index < count; ++ index)
			{
				if (_source[index + offset] != _value[index])
					return false;
			}
			return true;
		}

	} // namespace utility



}


#endif
