@echo off

for /f "usebackq tokens=*" %%d in (`dir /s /b *.meta`) do (
attrib %%d +h
)
pause