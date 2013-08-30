echo "zlib-1.2.8"
cd %~dp0
nmake -f win32/Makefile.msc zlib.lib
copy "zlib.lib" "../../lib/win32d/zlib_d.lib"
copy "zlib.lib" "../../lib/win32/zlib.lib"
nmake -f win32\Makefile.msc clean