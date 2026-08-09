<#
Incremental build, no clean, fast compile
#>
Write-Host "`n[BUILD] Incremental compile hello_world"
west build -b stm32f4_disco 

if($LASTEXITCODE -ne 0){
    Write-Host "[ERROR] build failed" -ForegroundColor Red
    exit $LASTEXITCODE
}
Write-Host "[OK] incremental build done" -ForegroundColor Green