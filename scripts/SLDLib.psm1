$Script:BatFiles = [PSCustomObject]@{
    Debug   = '..\scripts\build-debug.bat'
    Release = '..\scripts\build-release.bat'
}

$Script:Commands = [PSCustomObject]@{
    Cl  = "cl.exe"
    Lib = "lib.exe" 
}

$Script:DebugOutDirs = [PSCustomObject]@{
    Bin = '..\build\debug\bin'
    Obj = '..\build\debug\obj'
    Lib = '..\build\debug\lib'
}

$Script:ReleaseOutDirs = [PSCustomObject]@{
    Bin = '..\build\release\bin'
    Obj = '..\build\release\obj'
    Lib = '..\build\release\lib'
}

$Script:ClIncludeFlags = @(
    '/I' + '..\include',
    '/I' + '..\src',
    '/I' + '..\src\core',
    '/I' + '..\src\memory',
    '/I' + '..\src\string',
    '/I' + '..\src\win32',
    '/I' + '..\vcpkg_installed\x64-windows\include'
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

function Invoke-SLDDebugBuild {

    Write-Output $Script:ClDebugCommand
    Write-Output $Script:LibCommand

    Invoke-Expression $Script:ClDebugCommand
    Invoke-Expression $Script:LibCommand
}

function Export-SLDDebugOutDirs {

	if (!(Test-Path -Path $Script:DebugOutDirs.Bin)) { New-Item -ItemType Directory -Path $Script:DebugOutDirs.Bin }
	if (!(Test-Path -Path $Script:DebugOutDirs.Obj)) { New-Item -ItemType Directory -Path $Script:DebugOutDirs.Obj }
	if (!(Test-Path -Path $Script:DebugOutDirs.Lib)) { New-Item -ItemType Directory -Path $Script:DebugOutDirs.Lib }
}

function Export-SLDReleaseOutDirs {

	if (!(Test-Path -Path $Script:ReleaseOutDirs.Bin)) { New-Item -ItemType Directory -Path $Script:ReleaseOutDirs.Bin }
	if (!(Test-Path -Path $Script:ReleaseOutDirs.Obj)) { New-Item -ItemType Directory -Path $Script:ReleaseOutDirs.Obj }
	if (!(Test-Path -Path $Script:ReleaseOutDirs.Lib)) { New-Item -ItemType Directory -Path $Script:ReleaseOutDirs.Lib }
}

function Export-SLDBatchScripts {

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

function Install-SLDDependencies {

    $vcpkg = "vcpkg install"

    Invoke-Expression $vcpkg
}

Export-ModuleMember -Function Install-SLDDependencies
Export-ModuleMember -Function Invoke-SLDDebugBuild
Export-ModuleMember -Function Export-SLDDebugOutDirs
Export-ModuleMember -Function Export-SLDReleaseOutDirs
Export-ModuleMember -Function Export-SLDBatchScripts