# Build script for 802.11 Frame Parser

$Compiler = "E:\Tools\msys2\ucrt64\bin\gcc.exe"

$Flags = @(
    "-Wall",
    "-Wextra",
    "-I", "include"
)

$Sources = @(
    "src/frame_parser.c",
    "app/main.c"
)

Write-Host "Building 802.11 Frame Parser..."

& $Compiler @Flags @Sources -o frame_parser.exe

if ($LASTEXITCODE -eq 0) {
    Write-Host "Build successful!" -ForegroundColor Green
}
else {
    Write-Host "Build failed!" -ForegroundColor Red
    exit 1
}