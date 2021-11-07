clang++ -std=c++17 -Wall -Wextra -pedantic ./run-test.cpp -O3
if %ERRORLEVEL% NEQ 0 goto FAILURE

hoge
goto END

:FAILURE
echo ERROR STOP: %ERRORLEVEL%
pause

:END
exit /b
