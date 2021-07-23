@echo off
set CFLAGS=-DSLOW=1 -DINTERNAL=1 -W4 -MT -wd4701 -wd4100 -wd4189 -wd4201 -wd4996 -nologo -Oi -Od -GR- -Gm- -Z7 -EHa 

set LDLIBS=

set LDFLAGS=-incremental:no

set OPTIMIZE=/0i /02 /fp:fast

set PROJDIR=%CD%\
set SOURCEDIR=%PROJDIR%src\
set TESTDIR=%PROJDIR%test\
set BUILDDIR=%PROJDIR%build\
set OBJDIR=%BUILDDIR%obj\

REM Clean up	
   del %BUILDDIR%*.pdb

REM Platform code
	cl %CFLAGS% /Fo:%OBJDIR% /Fd:%OBJDIR% %SOURCEDIR%main.cpp /link %LDFLAGS% %LDLIBS% /OUT:%BUILDDIR%jus.exe

REM Test code
	cl %CFLAGS% /I%SOURCEDIR% /Fo:%OBJDIR% /Fd:%OBJDIR% %TESTDIR%test.cpp /link %LDFLAGS% %LDLIBS% /OUT:%BUILDDIR%test.exe