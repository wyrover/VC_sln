#ifndef _ROVERLIB_RESUTILS_H_
#define _ROVERLIB_RESUTILS_H_

namespace roverlib
{
	bool LoadBinaryResource(int binaryId, DWORD &dwSize, LPBYTE &pBytes);

	

}

namespace roverlib
{
	bool LoadBinaryResource(int binaryId, DWORD &dwSize, LPBYTE &pBytes)
	{
		extern HINSTANCE hInst;
		HRSRC hRes = FindResource(hInst, MAKEINTRESOURCE(binaryId),
			MAKEINTRESOURCE(256));
		if(hRes)
		{
			HGLOBAL hGlob = LoadResource(hInst, hRes);
			if(hGlob)
			{
				dwSize = SizeofResource(hInst, hRes);
				pBytes = (LPBYTE)LockResource(hGlob);
				if(dwSize > 0 && pBytes)
					return true;
			}
		}

		return false;
	}
}


#endif