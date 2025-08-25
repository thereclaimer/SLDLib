############################
# OUTPUT
############################

$Script:OutFiles = [PSCustomObject]@{
    Obj = 'SLD.Win32.obj'
    Pdb = 'SLD.Win32.pdb'
    Lib = 'SLD.Win32.lib'
}

$Script:BatFiles = [PSCustomObject]@{
    Debug   = 'scripts\SLD.Build.Debug.bat'
    Release = 'scripts\SLD.Build.Release.bat'
}

$Script:BuildTools = [PSCustomObject]@{
    Compiler = "cl.exe"
    Library  = "lib.exe" 
}

$Script:OutDirsDebug = [PSCustomObject]@{
    Bin = 'build\debug\bin'
    Obj = 'build\debug\obj'
    Lib = 'build\debug\lib'
}

$Script:OutDirsRelease = [PSCustomObject]@{
    Bin = 'build\release\bin'
    Obj = 'build\release\obj'
    Lib = 'build\release\lib'
}

$Script:OutFilesDebug = [PSCustomObject]@{
    Obj = "$($Script:OutDirsDebug.Obj)\$($Script:OutFiles.Obj)"
    Pdb = "$($Script:OutDirsDebug.Pdb)\$($Script:OutFiles.Pdb)"
    Lib = "$($Script:OutDirsDebug.Lib)\$($Script:OutFiles.Lib)"
}

############################
# COMPILE
############################

$Script:CompileArgsDebug = [PSCustomObject]@{
    In      = "src\sld.cpp"
    Out     = "/Fo:$($Script:OutFilesDebug.Obj)"
    Include = @(
        "/Iinclude"
        "/Isrc"
        "/Isrc\core"
        "/Isrc\memory"
        "/Isrc\os"
        "/Isrc\string"
        "/Isrc\win32"
        "/Ivcpkg_installed\x64-windows\include"
    ) -join " "
    Flags = @(
        "/nologo"             # startup banner disabled
        "/c"                  # compile without linking
        "/MD"                 # link against multithreaded runtime library (MSVCRT.dll)
        "/Z7"                 # generate debug info
        "/EHs-"               # disable exception handling
        "/std:c++17"          # c++17 standard mode
        "/Od"                 # disable optimizations
        "/D_HAS_EXCEPTIONS=0" # disable exceptions for STL and CRT
    ) -join " "    
}

$Script:CompileExpressionDebug = @(
    $Script:BuildTools.Compiler
    $Script:CompileArgsDebug.In
    $Script:CompileArgsDebug.Out
    $Script:CompileArgsDebug.Include
    $Script:CompileArgsDebug.Flags
) -join " "

############################
# LIBRARY
############################

$Script:LibraryArgsDebug = [PSCustomObject]@{
    In    = $Script:OutFilesDebug.Obj
    Out   = "/OUT:$($Script:OutFilesDebug.Lib)"
    Flags = "/nologo"
}

$Script:LibraryExpressionDebug = @(
    $Script:BuildTools.Library
    $Script:DebugLibraryArgs.In
    $Script:DebugLibraryArgs.Out
    $Script:DebugLibraryArgs.Flags
) -join " "

############################
# BUILD SCRIPT
############################

$Script:BuildScriptDebug = @"
@echo off

pushd ..

@set dir_bin=    $($Script:OutDirsDebug.Bin)
@set dir_lib=    $($Script:OutDirsDebug.Lib)
@set dir_obj=    $($Script:OutDirsDebug.Obj)

@set cl_in=      $($Script:CompileArgsDebug.In)
@set cl_out=     $($Script:CompileArgsDebug.Out)
@set cl_include= $($Script:CompileArgsDebug.Include)
@set cl_flags=   $($Script:CompileArgsDebug.Flags)

@set lib_in=     $($Script:LibraryArgsDebug.In)
@set lib_out=    $($Script:LibraryArgsDebug.Out)
@set lib_flags=  $($Script:LibraryArgsDebug.Flags)

@set cmd_cl=     $($Script:BuildTools.Compiler)  %cl_in%  %cl_out%  %cl_include%  %cl_flags%
@set cmd_lib=    $($Script:BuildTools.Library) %lib_flags% %lib_in% %lib_out%

IF NOT EXIST %dir_bin% mkdir %dir_bin%
IF NOT EXIST %dir_lib% mkdir %dir_lib%
IF NOT EXIST %dir_obj% mkdir %dir_obj%

call %cmd_cl%
call %cmd_link%

popd
"@

############################
# FUNCTIONS
############################

function Invoke-SLDBuildDebug {

    Push-Location (Join-Path $PSScriptRoot '..')

    Export-SLDOutDirsDebug

    Invoke-Expression $Script:ClDebugCommand
    Invoke-Expression $Script:LibCommand

    Pop-Location
}

function Export-SLDBuildDebug {

    Push-Location (Join-Path $PSScriptRoot '..')

    New-Item    -Path $Script:BatFiles.Debug -ItemType File -Force
    Set-Content -Path $Script:BatFiles.Debug -Value    $Script:BuildScriptDebug

    Pop-Location
}

function Export-SLDOutDirsDebug {

	if (!(Test-Path -Path $Script:OutDirsDebug.Bin)) { New-Item -ItemType Directory -Path $Script:OutDirsDebug.Bin }
	if (!(Test-Path -Path $Script:OutDirsDebug.Obj)) { New-Item -ItemType Directory -Path $Script:OutDirsDebug.Obj }
	if (!(Test-Path -Path $Script:OutDirsDebug.Lib)) { New-Item -ItemType Directory -Path $Script:OutDirsDebug.Lib }
}

function Export-SLDOutDirsRelease {

	if (!(Test-Path -Path $Script:OutDirsRelease.Bin)) { New-Item -ItemType Directory -Path $Script:OutDirsRelease.Bin }
	if (!(Test-Path -Path $Script:OutDirsRelease.Obj)) { New-Item -ItemType Directory -Path $Script:OutDirsRelease.Obj }
	if (!(Test-Path -Path $Script:OutDirsRelease.Lib)) { New-Item -ItemType Directory -Path $Script:OutDirsRelease.Lib }
}

function Install-SLDDependencies {

    $vcpkg = "vcpkg install"

    Invoke-Expression $vcpkg
}

Export-ModuleMember -Function Invoke-SLDBuildDebug
Export-ModuleMember -Function Export-SLDBuildDebug
Export-ModuleMember -Function Export-SLDOutDirsDebug
Export-ModuleMember -Function Export-SLDOutDirsRelease
Export-ModuleMember -Function Install-SLDDependencies