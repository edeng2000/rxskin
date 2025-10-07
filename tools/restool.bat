call 7z a -tzip "..\bin\uires.zip" "..\bin\uires\*" -p"QAZwsx!@#456"
xcopy ..\bin\*.zip ..\bin\Win32\Debug\ /Y 
xcopy ..\bin\*.zip ..\bin\Win32\Release\ /Y 
xcopy ..\bin\*.zip ..\bin\x64\Debug\ /Y 
xcopy ..\bin\*.zip ..\bin\x64\Release\ /Y 
