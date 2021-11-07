clang++ -target x86_64-pc-windows-gnu ./ans-greedy-hybrid.cpp -O3 -D LOCAL
if %ERRORLEVEL% NEQ 0 goto FAILURE

cd tools
cargo run --release --bin tester ..\a.exe < .\in\0000.txt > ..\out\0000_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0001.txt > ..\out\0001_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0002.txt > ..\out\0002_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0003.txt > ..\out\0003_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0004.txt > ..\out\0004_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0005.txt > ..\out\0005_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0006.txt > ..\out\0006_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0007.txt > ..\out\0007_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0008.txt > ..\out\0008_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0009.txt > ..\out\0009_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0010.txt > ..\out\0010_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0011.txt > ..\out\0011_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0012.txt > ..\out\0012_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0013.txt > ..\out\0013_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0014.txt > ..\out\0014_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0015.txt > ..\out\0015_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0016.txt > ..\out\0016_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0017.txt > ..\out\0017_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0018.txt > ..\out\0018_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0019.txt > ..\out\0019_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0020.txt > ..\out\0020_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0021.txt > ..\out\0021_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0022.txt > ..\out\0022_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0023.txt > ..\out\0023_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0024.txt > ..\out\0024_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0025.txt > ..\out\0025_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0026.txt > ..\out\0026_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0027.txt > ..\out\0027_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0028.txt > ..\out\0028_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0029.txt > ..\out\0029_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0030.txt > ..\out\0030_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0031.txt > ..\out\0031_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0032.txt > ..\out\0032_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0033.txt > ..\out\0033_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0034.txt > ..\out\0034_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0035.txt > ..\out\0035_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0036.txt > ..\out\0036_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0037.txt > ..\out\0037_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0038.txt > ..\out\0038_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0039.txt > ..\out\0039_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0040.txt > ..\out\0040_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0041.txt > ..\out\0041_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0042.txt > ..\out\0042_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0043.txt > ..\out\0043_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0044.txt > ..\out\0044_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0045.txt > ..\out\0045_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0046.txt > ..\out\0046_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0047.txt > ..\out\0047_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0048.txt > ..\out\0048_latest.txt 
cargo run --release --bin tester ..\a.exe < .\in\0049.txt > ..\out\0049_latest.txt 
if %ERRORLEVEL% NEQ 0 goto FAILURE

cd ..\

goto END

:FAILURE
echo ERROR STOP: %ERRORLEVEL%
pause

:END
exit /b
