@echo off
call "C:\Program Files\Microsoft Visual C++ Build Tools\vcbuildtools.bat" x86
cd C:\Programming_practice     
set compilerflags=/EHsc
cl %compilerflags% WordNumberClient.cpp
cl %compilerflags% WordNumberServer.cpp