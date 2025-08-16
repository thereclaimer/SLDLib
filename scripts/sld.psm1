
$Script:Commands = [PSCustomObject]@{
    Cl  = "cl.exe"
    Lib = "lib.exe" 
}

$Script:DebugOutDirs = [PSCustomObject]@{
    Bin = Join-Path $PSScriptRoot '..\build\debug\bin'
    Obj = Join-Path $PSScriptRoot '..\build\debug\obj'
    Lib = Join-Path $PSScriptRoot '..\build\debug\lib'
}

$Script:ReleaseOutDirs = [PSCustomObject]@{
    Bin = Join-Path $PSScriptRoot '..\build\release\bin'
    Obj = Join-Path $PSScriptRoot '..\build\release\obj'
    Lib = Join-Path $PSScriptRoot '..\build\release\lib'
}

$Script:CLIncludeFlags = @(
    "/I" + (Join-Path $PSScriptRoot '..\include'),
    "/I" + (Join-Path $PSScriptRoot '..\src'),
    "/I" + (Join-Path $PSScriptRoot '..\src\core'),
    "/I" + (Join-Path $PSScriptRoot '..\src\memory'),
    "/I" + (Join-Path $PSScriptRoot '..\src\string'),
    "/I" + (Join-Path $PSScriptRoot '..\src\win32'),
    "/I" + (Join-Path $PSScriptRoot '..\vcpkg_installed\x64-windows')
)

$Script:CLDebugFlags = @(
    "/Fo:" + $Script:DebugOutDirs.Obj, # obj output directory 
    "/nologo",                         # startup banner disabled
    "/c",                              # compile without linking
    "/MD",                             # link against multithreaded runtime library (MSVCRT.dll)
    "/Z7",                             # generate debug info
    "/EHs-",                           # disable exception handling
    "/std:c++17",                      # c++17 standard mode
    "/Od",                             # disable optimizations
    "/D_HAS_EXCEPTIONS=0"              # disable exceptions for STL and CRT
)

$Script:CLSourceFile = "sld.cpp"

function Invoke-DebugBuild {

    $ClCommand = @(

        $Script:Commands.Cl,
        $Script:CLSourceFile,
        $Script:CLIncludeFlags -join " ",
        $Script:CLDebugFlags   -join " "
    )

    Invoke-Expression $ClCommand
}

function Export-DebugOutDirs {

	if (!(Test-Path -Path $Script:DebugOutDirs.Bin)) { New-Item -ItemType Directory -Path $Script:DebugOutDirs.Bin }
	if (!(Test-Path -Path $Script:DebugOutDirs.Obj)) { New-Item -ItemType Directory -Path $Script:DebugOutDirs.Obj }
	if (!(Test-Path -Path $Script:DebugOutDirs.Lib)) { New-Item -ItemType Directory -Path $Script:DebugOutDirs.Lib }
}

function Export-ReleaseOutDirs {

	if (!(Test-Path -Path $Script:ReleaseOutDirs.Bin)) { New-Item -ItemType Directory -Path $Script:ReleaseOutDirs.Bin }
	if (!(Test-Path -Path $Script:ReleaseOutDirs.Obj)) { New-Item -ItemType Directory -Path $Script:ReleaseOutDirs.Obj }
	if (!(Test-Path -Path $Script:ReleaseOutDirs.Lib)) { New-Item -ItemType Directory -Path $Script:ReleaseOutDirs.Lib }
}

function Install-Dependencies {

    $vcpkg = "vcpkg install"

    Invoke-Expression $vcpkg
}

Export-ModuleMember -Function Invoke-DebugBuild
Export-ModuleMember -Function Export-DebugOutDirs
Export-ModuleMember -Function Export-ReleaseOutDirs
Export-ModuleMember -Function Install-Dependencies