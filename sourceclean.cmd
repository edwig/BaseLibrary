@echo off
@echo Cleaning BaseLibrary directories
@echo .

del /q /s /f *.sdf
del /q /s /f *.VC.db
rmdir /q /s .vs
rmdir /q /s lib
rmdir /q /s BaseLibrary\Debug
rmdir /q /s BaseLibrary\Release
rmdir /q /s BaseLibrary\x64
rmdir /q /s BaseLibrary\Win32
rmdir /q /s x64
rmdir /q /s Debug
rmdir /q /s Release

echo .
echo Ready cleaning
