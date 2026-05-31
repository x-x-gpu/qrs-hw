$ErrorActionPreference = "Stop"

. (Join-Path $PSScriptRoot "idf-env.ps1")

$ProjectRoot = (Resolve-Path -LiteralPath (Join-Path $PSScriptRoot "..")).Path
$WorkRoot = if ($ProjectRoot -match "[^\x00-\x7F]") {
    "C:\Users\Xiao\Desktop\esp32c6_wroom1_idf_work"
} else {
    $ProjectRoot
}

function Assert-WorkRoot {
    param([string] $Path)

    $fullPath = [System.IO.Path]::GetFullPath($Path)
    if ($fullPath -notlike "C:\Users\Xiao\Desktop\esp32c6_wroom1_idf_work*") {
        throw "Refusing to use unexpected ESP-IDF work root: $fullPath"
    }
}

function Sync-ProjectToWorkRoot {
    if ($WorkRoot -eq $ProjectRoot) {
        return
    }

    Assert-WorkRoot $WorkRoot

    if (-not (Test-Path -LiteralPath $WorkRoot)) {
        New-Item -ItemType Directory -Path $WorkRoot | Out-Null
    }

    $resolvedWorkRoot = (Resolve-Path -LiteralPath $WorkRoot).Path
    foreach ($item in Get-ChildItem -LiteralPath $resolvedWorkRoot -Force) {
        if ($item.Name -eq "build") {
            continue
        }

        if ($item.FullName -notlike "$resolvedWorkRoot\*") {
            throw "Refusing to remove unexpected work item: $($item.FullName)"
        }
        Remove-Item -LiteralPath $item.FullName -Recurse -Force
    }

    $excluded = @(".git", "build")
    foreach ($item in Get-ChildItem -LiteralPath $ProjectRoot -Force) {
        if ($excluded -contains $item.Name) {
            continue
        }
        Copy-Item -LiteralPath $item.FullName -Destination $resolvedWorkRoot -Recurse -Force
    }
}

function Sync-GeneratedFilesBack {
    if ($WorkRoot -eq $ProjectRoot) {
        return
    }

    foreach ($name in @("sdkconfig", "sdkconfig.old", "dependencies.lock")) {
        $source = Join-Path $WorkRoot $name
        if (Test-Path -LiteralPath $source) {
            Copy-Item -LiteralPath $source -Destination (Join-Path $ProjectRoot $name) -Force
        }
    }
}

Sync-ProjectToWorkRoot

$PreviousDirectory = [System.IO.Directory]::GetCurrentDirectory()
Push-Location $WorkRoot
[System.IO.Directory]::SetCurrentDirectory($WorkRoot)
try {
    idf.py @args
    $ExitCode = $LASTEXITCODE
} finally {
    [System.IO.Directory]::SetCurrentDirectory($PreviousDirectory)
    Pop-Location
    Sync-GeneratedFilesBack
}

exit $ExitCode
