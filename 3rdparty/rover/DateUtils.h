#pragma once

namespace roverlib 
{
	class DateUtils
	{
	public:
		static void FormatTimeString(LPTSTR lpszBuf,SYSTEMTIME * pTime)
		{
			wsprintf(lpszBuf,_T("%02d:%02d:%02d"),pTime->wHour,pTime->wMinute,pTime->wSecond);
		}
	public:
		DateUtils(void);
		~DateUtils(void);
	};

}
