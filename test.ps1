$Compiler = "E:\Tools\msys2\ucrt64\bin\gcc.exe"

$Tests = @(
    "test_frame_control",
    "test_ssid",
    "test_supported_rates",
    "test_ds_parameter_set",
    "test_beacon_ies",
    "test_beacon_fixed",
    "test_beacon_frame"
)

$Failed = 0

foreach ($Test in $Tests)
{
    Write-Host "`nRunning $Test ..." -ForegroundColor Cyan

    $Source = "tests/$Test.c"
    $Output = "$Test.exe"

    & $Compiler -Wall -Wextra -I include src/frame_parser.c $Source -o $Output

    if ($LASTEXITCODE -ne 0)
    {
        Write-Host "Compilation failed: $Test" -ForegroundColor Red
        $Failed++
        continue
    }

    & ".\$Output"

    if ($LASTEXITCODE -ne 0)
    {
        Write-Host "Test failed: $Test" -ForegroundColor Red
        $Failed++
    }
    else
    {
        Write-Host "Test passed: $Test" -ForegroundColor Green
    }
}

Write-Host "`n============================"

if ($Failed -eq 0)
{
    Write-Host "ALL TESTS PASSED!" -ForegroundColor Green
}
else
{
    Write-Host "$Failed TEST(S) FAILED!" -ForegroundColor Red
    exit 1
}