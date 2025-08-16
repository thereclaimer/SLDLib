@echo off

@set cmd_cl=cl.exe C:\Users\sldav\source\repos\SLDLib\scripts\..\src\sld.cpp /IC:\Users\sldav\source\repos\SLDLib\scripts\..\include /IC:\Users\sldav\source\repos\SLDLib\scripts\..\src /IC:\Users\sldav\source\repos\SLDLib\scripts\..\src\core /IC:\Users\sldav\source\repos\SLDLib\scripts\..\src\memory /IC:\Users\sldav\source\repos\SLDLib\scripts\..\src\string /IC:\Users\sldav\source\repos\SLDLib\scripts\..\src\win32 /IC:\Users\sldav\source\repos\SLDLib\scripts\..\vcpkg_installed\x64-windows /Fo:C:\Users\sldav\source\repos\SLDLib\scripts\..\build\debug\obj\SLD.Win32.obj /nologo /c /MD /Z7 /EHs- /std:c++17 /Od /D_HAS_EXCEPTIONS=0
@set cmd_lib=lib.exe /nologo /OUT:C:\Users\sldav\source\repos\SLDLib\scripts\..\build\debug\lib\SLD.Win32.lib C:\Users\sldav\source\repos\SLDLib\scripts\..\build\debug\obj\SLD.Win32.obj

call %cmd_cl%
call %cmd_lib%
