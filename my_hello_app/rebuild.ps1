<#
Full rebuild: clean + --pristine compile hello_world stm32f4_disco
#>
if(Test-Path "./build"){
    Remove-Item -Recurse -Force ./build
    Write-Host "[OK] Clean build folder"
}

Write-Host "`n[REBUILD] Full compile hello_world"
west build -b stm32f4_disco 

if($LASTEXITCODE -ne 0){
    Write-Host "[ERROR] rebuild failed" -ForegroundColor Red
    exit $LASTEXITCODE
}
Write-Host "[OK] rebuild done" -ForegroundColor Green