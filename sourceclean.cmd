@echo off
@echo Cleaning BaseLibrary directories
@echo .
rmdir /q /s .vs
rmdir /q /s lib
rmdir /q /s bin
rmdir /q /s BaseLibrary\x64
rmdir /q /s BaseLibrary\Win32
rmdir /q /s x64
rmdir /q /s Debug
rmdir /q /s DebugUnicode
rmdir /q /s Release
rmdir /q /s ReleaseUnicode
rmdir /q /s TestFilter\x64
rmdir /q /s TestFilter\Win32
rmdir /q /s TestRedirect\x64
rmdir /q /s TestRedirect\Win32
rmdir /q /s UnitTests\x64
rmdir /q /s UnitTests\Win32
rmdir /q /s BaseLibrary-cppcheck-build-dir
echo .
echo Ready cleaning
