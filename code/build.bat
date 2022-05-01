

@echo off

set compiler_options=/nologo /FC /TC /Zi /D_DEBUG
REM set linker_options=

IF NOT EXIST ..\..\build (mkdir ..\..\build)
pushd ..\..\build

REM TEST
REM cl %compiler_options% ..\cinterpreter\code\test.c %*

REM Application
cl %compiler_options% ..\cinterpreter\code\interpret.c %*

popd

echo Compilation finished!
