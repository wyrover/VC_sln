@SET VSINSTALLDIR=C:\Program Files (x86)\Microsoft Visual Studio 8
@SET VCINSTALLDIR=C:\Program Files (x86)\Microsoft Visual Studio 8\VC
@SET FrameworkDir=C:\Windows\Microsoft.NET\Framework
@SET FrameworkVersion=v2.0.50727
@SET FrameworkSDKDir=C:\Program Files (x86)\Microsoft Visual Studio 8\SDK\v2.0
@if "%VSINSTALLDIR%"=="" goto error_no_VSINSTALLDIR
@if "%VCINSTALLDIR%"=="" goto error_no_VCINSTALLDIR

@echo Setting environment for using Microsoft Visual Studio 2005 x86 tools.

@rem
@rem Root of Visual Studio IDE installed files.
@rem
@set DevEnvDir=C:\Program Files (x86)\Microsoft Visual Studio 8\Common7\IDE

@set PATH=C:\Program Files (x86)\Microsoft Visual Studio 8\Common7\IDE;C:\Program Files (x86)\Microsoft Visual Studio 8\VC\BIN;C:\Program Files (x86)\Microsoft Visual Studio 8\Common7\Tools;C:\Program Files (x86)\Microsoft Visual Studio 8\Common7\Tools\bin;C:\Program Files (x86)\Microsoft Visual Studio 8\VC\PlatformSDK\bin;C:\Program Files (x86)\Microsoft Visual Studio 8\SDK\v2.0\bin;C:\Windows\Microsoft.NET\Framework\v2.0.50727;C:\Program Files (x86)\Microsoft Visual Studio 8\VC\VCPackages;%PATH%
@set INCLUDE=C:\Program Files (x86)\Microsoft Visual Studio 8\VC\ATLMFC\INCLUDE;C:\Program Files (x86)\Microsoft Visual Studio 8\VC\INCLUDE;C:\Program Files (x86)\Microsoft Visual Studio 8\VC\PlatformSDK\include;%INCLUDE%
@set LIB=C:\Program Files (x86)\Microsoft Visual Studio 8\VC\ATLMFC\LIB;C:\Program Files (x86)\Microsoft Visual Studio 8\VC\LIB;C:\Program Files (x86)\Microsoft Visual Studio 8\VC\PlatformSDK\lib;C:\Program Files (x86)\Microsoft Visual Studio 8\SDK\v2.0\lib;%LIB%
@set LIBPATH=C:\Windows\Microsoft.NET\Framework\v2.0.50727;C:\Program Files (x86)\Microsoft Visual Studio 8\VC\ATLMFC\LIB

@goto end

:error_no_VSINSTALLDIR
@echo ERROR: VSINSTALLDIR variable is not set. 
@goto end

:error_no_VCINSTALLDIR
@echo ERROR: VCINSTALLDIR variable is not set. 
@goto end

:end



@set MYCOMPILE=cl /nologo /W3 /c /D_CRT_SECURE_NO_DEPRECATE /D "WIN32"
@set MKLIB=lib /nologo


cd %~dp0
cd lua



:build MTd_DLL
del *.obj
%MYCOMPILE% /Od /MTd /D_DEBUG /DLUA_BUILD_AS_DLL l*.c 
del lua.obj luac.obj
link /DLL /out:"../../product/win32d/lua51_MTd.dll" l*.obj
%MYCOMPILE% /Od /MTd /D_DEBUG /DLUA_BUILD_AS_DLL lua.c
link /out:"../../product/win32d/lua_MTd.exe" /LIBPATH:"../../product/wind32d/" lua.obj lua51_MTd.lib
del lua.obj
%MYCOMPILE% /Od /MTd /D_DEBUG /DLUA_BUILD_AS_DLL luac.c print.c
link /out:"../../product/win32d/luac_MTd.exe" /LIBPATH:"../../product/win32d/" l*.obj print.obj

:build MT_DLL
del *.obj
%MYCOMPILE% /O2 /MT /DLUA_BUILD_AS_DLL l*.c 
del lua.obj luac.obj
link /DLL /out:"../../product/win32/lua51_MT.dll" l*.obj
%MYCOMPILE% /O2 /MT /DLUA_BUILD_AS_DLL lua.c
link /out:"../../product/win32/lua_MT.exe" /LIBPATH:"../../product/win32/" lua.obj lua51_MT.lib
del lua.obj
%MYCOMPILE% /O2 /MT /DLUA_BUILD_AS_DLL luac.c print.c
link /out:"../../product/win32/luac_MT.exe" /LIBPATH:"../../product/win32/" l*.obj print.obj




:build MTd_LIB
del *.obj
%MYCOMPILE% /Od /MTd /D_DEBUG /D_LIB l*.c 
del lua.obj luac.obj
%MKLIB% /out:"../../lib/win32d/lua51d.lib" l*.obj


:build MT_LIB
del *.obj
%MYCOMPILE% /O2 /MT /D_LIB l*.c 
del lua.obj luac.obj
%MKLIB% /out:"../../lib/win32/lua51.lib" l*.obj


del *.obj *.dll *.exp *.lib


cd ..