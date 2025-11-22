<#
 Bootstrap build for Windows (PowerShell)
 - Ensures VCPKG_ROOT (clones local .vcpkg if needed)
 - Configures CMake with the Windows preset
 - Builds the selected configuration
#>
param(
    [ValidateSet('Debug','Release')]
    [string]$Config = 'Debug'
)

$ErrorActionPreference = 'Stop'

# Resolve repo root (parent of scripts folder) and move there
$RepoRoot = Split-Path -Parent $PSScriptRoot
Set-Location -Path $RepoRoot

# Ensure VCPKG_ROOT
function Ensure-VcpkgRoot {
    param()
    $localVcpkg = Join-Path -Path $RepoRoot -ChildPath '.vcpkg'

    if (-not $env:VCPKG_ROOT -or -not (Test-Path (Join-Path -Path $env:VCPKG_ROOT -ChildPath 'scripts/buildsystems/vcpkg.cmake'))) {
        if (-not (Test-Path -Path $localVcpkg)) {
            Write-Host 'Cloning vcpkg locally into .vcpkg ...' -ForegroundColor Cyan
            git clone 'https://github.com/microsoft/vcpkg.git' "$localVcpkg" | Out-Null
            & (Join-Path -Path $localVcpkg -ChildPath 'bootstrap-vcpkg.bat') | Out-Null
        }
        $env:VCPKG_ROOT = (Resolve-Path -Path $localVcpkg).Path
    }
}

Ensure-VcpkgRoot
Write-Host "Using VCPKG_ROOT=$env:VCPKG_ROOT" -ForegroundColor Yellow

Write-Host 'Configuring (x64-windows) ...' -ForegroundColor Cyan
cmake --preset x64-windows | Out-Host

Write-Host 'Building ...' -ForegroundColor Cyan
cmake --build --preset x64-windows --config $Config | Out-Host
