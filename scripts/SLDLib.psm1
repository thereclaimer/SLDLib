$Script:BatFiles = [PSCustomObject]@{
    Debug   = Join-Path $PSScriptRoot '..\scripts\build-debug.bat'
    Release = Join-Path $PSScriptRoot '..\scripts\build-release.bat'
}

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

$Script:ClIncludeFlags = @(
    "/I" + (Join-Path $PSScriptRoot '..\include'),
    "/I" + (Join-Path $PSScriptRoot '..\src'),
    "/I" + (Join-Path $PSScriptRoot '..\src\core'),
    "/I" + (Join-Path $PSScriptRoot '..\src\memory'),
    "/I" + (Join-Path $PSScriptRoot '..\src\string'),
    "/I" + (Join-Path $PSScriptRoot '..\src\win32'),
    "/I" + (Join-Path $PSScriptRoot '..\vcpkg_installed\x64-windows\include')
)

$Script:ClDebugFlags = @(
    "/Fo:" + $Script:DebugOutDirs.Obj + "\SLD.Win32.obj", # obj output
    "/nologo",                                            # startup banner disabled
    "/c",                                                 # compile without linking
    "/MD",                                                # link against multithreaded runtime library (MSVCRT.dll)
    "/Z7",                                                # generate debug info
    "/EHs-",                                              # disable exception handling
    "/std:c++17",                                         # c++17 standard mode
    "/Od",                                                # disable optimizations
    "/D_HAS_EXCEPTIONS=0"                                 # disable exceptions for STL and CRT
)

$Script:ClSourceFile         = Join-Path $PSScriptRoot '..\src\sld.cpp'
$Script:ClIncludeFlagsString = $Script:ClIncludeFlags -join " "
$Script:ClDebugFlagsString   = $Script:ClDebugFlags   -join " "
$Script:ClDebugCommand       = @(
    $Script:Commands.Cl,
    $Script:ClSourceFile,
    $Script:ClIncludeFlagsString,
    $Script:ClDebugFlagsString
) -join " " 

$Script:LibFlags = @(
    "/nologo",
    ("/OUT:" + $Script:DebugOutDirs.Lib + "\SLD.Win32.lib"),
    ($Script:DebugOutDirs.Obj + "\SLD.Win32.obj")
) -join " "

$Script:LibCommand = @(
    $Script:Commands.Lib,
    $Script:LibFlags
) -join " "

function Invoke-DebugBuild {

    Write-Output $Script:ClDebugCommand
    Write-Output $Script:LibCommand

    Invoke-Expression $Script:ClDebugCommand
    Invoke-Expression $Script:LibCommand
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

function Export-BatchScripts {

    # create files
    if (Test-Path $Script:BatFiles.Debug)   { Remove-Item $Script:BatFiles.Debug   -Force }
    if (Test-Path $Script:BatFiles.Release) { Remove-Item $Script:BatFiles.Release -Force }
    New-Item $Script:BatFiles.Debug
    New-Item $Script:BatFiles.Release

    # build debug
    ("@echo off")                               | Out-File -Encoding ASCII -Append -FilePath $Script:BatFiles.Debug
    ("")                                        | Out-File -Encoding ASCII -Append -FilePath $Script:BatFiles.Debug
    ("@set cmd_cl="   + $Script:ClDebugCommand) | Out-File -Encoding ASCII -Append -FilePath $Script:BatFiles.Debug
    ("@set cmd_lib="  + $Script:LibCommand)     | Out-File -Encoding ASCII -Append -FilePath $Script:BatFiles.Debug
    ("")                                        | Out-File -Encoding ASCII -Append -FilePath $Script:BatFiles.Debug
    ("call %cmd_cl%")                           | Out-File -Encoding ASCII -Append -FilePath $Script:BatFiles.Debug
    ("call %cmd_lib%")                          | Out-File -Encoding ASCII -Append -FilePath $Script:BatFiles.Debug

}

function Install-Dependencies {

    $vcpkg = "vcpkg install"

    Invoke-Expression $vcpkg
}

Export-ModuleMember -Function Install-Dependencies
Export-ModuleMember -Function Invoke-DebugBuild
Export-ModuleMember -Function Export-DebugOutDirs
Export-ModuleMember -Function Export-ReleaseOutDirs
Export-ModuleMember -Function Export-BatchScripts