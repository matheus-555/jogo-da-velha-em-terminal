@echo off
cls
gcc.exe -Wall lib/velha/velha.c main.c -o main.exe -lm
pause
.\main.exe