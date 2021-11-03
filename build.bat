clang++ -target x86_64-pc-windows-gnu ./ans.cpp -O3
if %ERRORLEVEL% NEQ 0 goto FAILURE

cd tools
cargo run --release --bin tester ..\a.exe < .\in\0000.txt > ..\out\0000_latest.txt 
if %ERRORLEVEL% NEQ 0 goto FAILURE

cd ..\

goto END

:FAILURE
echo ERROR STOP: %ERRORLEVEL%
pause

:END
exit /b
