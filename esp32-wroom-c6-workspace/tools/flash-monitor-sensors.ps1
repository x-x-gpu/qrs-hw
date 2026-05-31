param(
    [string] $Port,
    [switch] $NoMonitor
)

$ErrorActionPreference = "Stop"

$ProjectRoot = (Resolve-Path -LiteralPath (Join-Path $PSScriptRoot "..")).Path
$IdfWrapper = Join-Path $ProjectRoot "tools\idf.ps1"

function Get-PortNameFromFriendlyName {
    param([string] $FriendlyName)

    if ($FriendlyName -match "\((COM\d+)\)") {
        return $Matches[1]
    }

    return $null
}

function Get-AutoSerialPort {
    $availablePorts = @([System.IO.Ports.SerialPort]::GetPortNames())
    if ($availablePorts.Count -eq 0) {
        throw "No serial ports are currently openable. Reconnect the ESP32-C6 board and check the USB cable."
    }

    $portDevices = @(Get-PnpDevice -Class Ports -ErrorAction SilentlyContinue |
        Where-Object { $_.Status -eq "OK" })

    $preferredDevices = @(
        $portDevices | Where-Object { $_.InstanceId -like "USB\VID_303A&PID_1001*" },
        $portDevices | Where-Object { $_.InstanceId -like "USB\VID_1A86&PID_55D3*" },
        $portDevices | Where-Object { $_.FriendlyName -like "*CH343*" }
    )

    foreach ($device in $preferredDevices) {
        if ($null -eq $device) {
            continue
        }

        $candidate = Get-PortNameFromFriendlyName $device.FriendlyName
        if ($candidate -and ($availablePorts -contains $candidate)) {
            return $candidate
        }
    }

    if ($availablePorts.Count -eq 1) {
        return $availablePorts[0]
    }

    throw "Multiple serial ports found: $($availablePorts -join ', '). Re-run with -Port COMx."
}

if (-not $Port) {
    $Port = Get-AutoSerialPort
}

Write-Host "Project: $ProjectRoot"
Write-Host "Serial port: $Port"
Write-Host "Expected sensor wiring:"
Write-Host "  BH1750/GY-302: SDA=GPIO6, SCL=GPIO7, VCC=3V3, GND=GND"
Write-Host "  AM2302/DHT22:  DATA=GPIO11, VCC=3V3, GND=GND"
Write-Host "  HC-SR501:      OUT=GPIO10, VCC=5V/VBUS, GND=GND"

Push-Location $ProjectRoot
try {
    & $IdfWrapper build

    if ($NoMonitor) {
        & $IdfWrapper -p $Port flash
    } else {
        Write-Host "Starting flash + monitor. Press Ctrl+] to exit the ESP-IDF monitor."
        & $IdfWrapper -p $Port flash monitor
    }
} finally {
    Pop-Location
}
