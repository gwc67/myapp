<#
Only flash existing zephyr.hex, NO compile
Use Under‑Reset mode, no software‑reset inside session
After flash: manually press board RESET button
#>

$hexFile = "build/zephyr/zephyr.hex"
if(-not (Test-Path $hexFile)){
    Write-Host "[ERROR] $hexFile not found! Please build first." -ForegroundColor Red
    exit 1
}

Write-Host "`n[FLASH] Programming $hexFile"
& "C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe" `
-c port=SWD mode=UR freq=4000 `
--download $hexFile

if($LASTEXITCODE -ne 0){
    Write-Host "[ERROR] Flash failed" -ForegroundColor Red
    exit $LASTEXITCODE
}

Write-Host "`n[OK] Flash complete! Press board RESET button to run firmware." -ForegroundColor Green
Write-Host "Serial: 115200 8N1`n"