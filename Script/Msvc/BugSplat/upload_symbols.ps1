# Post-build script to upload symbols to Bugsplat.
# Set post-build command line to:
#
#   powershell -ExecutionPolicy Bypass -File "$(ProjectDir)..\..\Script\Msvc\BugSplat\upload_symbols.ps1" -ProjectDir "$(ProjectDir)" -TargetDir "$(TargetDir)"


param (
    [string]$ProjectDir,
    [string]$TargetDir
)

# Parse version macros from header file
$headerPath = Join-Path $ProjectDir "..\..\..\Source\Windows\WindowsVersion.h"
if (Test-Path $headerPath) {
    $content = Get-Content $headerPath -Raw
    $major = [regex]::Match($content, '#define\s+VERSION_MAJOR\s+(\d+)').Groups[1].Value
    $minor = [regex]::Match($content, '#define\s+VERSION_MINOR\s+(\d+)').Groups[1].Value
    $patch = [regex]::Match($content, '#define\s+VERSION_REVISION\s+(\d+)').Groups[1].Value

    if (-not ($major -and $minor -and $patch)) {
        Write-Error "ERROR! Could not parse version in '$headerPath'."
        exit 1
    }

    $version = "$major.$minor.$patch"
    Write-Host "Parsed version $version from $headerPath"
} else {
    Write-Error "ERROR! Header file not found at '$headerPath'. Symbol upload aborted."
    exit 1
}

# Source BugSplat environment
$envPath = Join-Path $ProjectDir "..\..\BugSplat\env.ps1"
if (Test-Path $envPath) { 
    . $envPath 
} else {
    Write-Error "ERROR! Can't find env.ps1: '$envPath'. Symbol upload aborted."
    exit 1
}

# Execute symbol upload
#
$scriptDir = $PSScriptRoot
$uploader = Join-Path $scriptDir "symbol-upload-windows.exe"

if (-not (Test-Path $uploader)) {
    Write-Error "ERROR! Executable 'symbol-upload-windows.exe' not found in '$scriptDir'."
    exit 1
}

$cleanTarget = $TargetDir.TrimEnd('\')

& $uploader -b amju-games -a piano-fest -v $version -d $cleanTarget -f '**/*.{pdb,exe,dll}'

if ($LASTEXITCODE -ne 0) {
    Write-Error "ERROR! symbol-upload-windows.exe failed with exit code $LASTEXITCODE."
    exit $LASTEXITCODE
}

