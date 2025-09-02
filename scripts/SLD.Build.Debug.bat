@echo off

pushd ..

@set dir_bin=    build\debug\bin
@set dir_lib=    build\debug\lib
@set dir_obj=    build\debug\obj

@set cl_in=      src\sld.cpp
@set cl_out=     /Fo:build\debug\obj\SLD.Win32.obj
@set cl_include= /Iinclude /Isrc /Isrc\core /Isrc\math /Isrc\memory /Isrc\os /Isrc\string /Isrc\win32 /Ivcpkg_installed\x64-windows\include
@set cl_flags=   /nologo /c /MD /Z7 /EHs- /std:c++17 /Od /D_HAS_EXCEPTIONS=0

@set lib_in=     build\debug\obj\SLD.Win32.obj
@set lib_out=    /OUT:build\debug\lib\SLD.Win32.lib
@set lib_flags=  /nologo

@set cmd_cl=     cl.exe  %cl_in%  %cl_out%  %cl_include%  %cl_flags%
@set cmd_lib=    lib.exe %lib_flags% %lib_in% %lib_out%

IF NOT EXIST %dir_bin% mkdir %dir_bin%
IF NOT EXIST %dir_lib% mkdir %dir_lib%
IF NOT EXIST %dir_obj% mkdir %dir_obj%

call %cmd_cl%
call %cmd_lib%

popd
