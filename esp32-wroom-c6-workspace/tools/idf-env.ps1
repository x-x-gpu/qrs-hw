$ErrorActionPreference = "Stop"

$IdfPath = "D:\esp-idf\v5.5.1\esp-idf"
$IdfPythonPath = "C:\Users\Xiao\.espressif\tools\idf-python\3.11.2"

if (-not (Test-Path -LiteralPath (Join-Path $IdfPath "export.ps1"))) {
    throw "ESP-IDF export.ps1 was not found at $IdfPath"
}

if (-not (Test-Path -LiteralPath (Join-Path $IdfPythonPath "python.exe"))) {
    throw "ESP-IDF Python was not found at $IdfPythonPath"
}

$pathParts = $env:Path -split ";"
if ($pathParts -notcontains $IdfPythonPath) {
    $env:Path = "$IdfPythonPath;$env:Path"
}

. (Join-Path $IdfPath "export.ps1")
