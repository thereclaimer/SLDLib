$dir_build_bin = $PSScriptRoot + "\..\build\debug\bin\"
$dir_build_lib = $PSScriptRoot + "\..\build\debug\lib\"
$dir_build_obj = $PSScriptRoot + "\..\build\debug\obj\"
$dir_libraries = $PSScriptRoot + "\..\libraries\"

if (!(Test-Path -Path $dir_build_bin)) { New-Item -ItemType Directory -Path $dir_build_bin }
if (!(Test-Path -Path $dir_build_lib)) { New-Item -ItemType Directory -Path $dir_build_lib }
if (!(Test-Path -Path $dir_build_obj)) { New-Item -ItemType Directory -Path $dir_build_obj }

$flags_cl = @(
    ($dir_libraries + "core\src\sld.cpp"),
    ("/Fo:" + $dir_build_obj),
    ("/I"   + $dir_libraries + "core\include"),
    ("/I"   + $dir_libraries + "ds\include"),
    ("/I"   + $dir_libraries + "math\include"),
    ("/I"   + $dir_libraries + "memory\include"),
    ("/I"   + $dir_libraries + "os\include"),
    ("/I"   + $dir_libraries + "string\include"),
    ("/I"   + $dir_libraries + "win32\include"),
    "/nologo",
    "/c",
    "/MD",
    "/Z7",
    "/EHsc",
    "/std:c++17",
    "/Od"
)

$flags_link = @(
    "/nologo",
    ("/OUT:" + $dir_build_lib + "SLDLib.lib")
    ($dir_build_obj + "sld.obj")
)

$cmd_cl  = "cl.exe "  + ($flags_cl   -join " ")
$cmd_lib = "lib.exe " + ($flags_link -join " ")

Invoke-Expression $cmd_cl
Invoke-Expression $cmd_lib