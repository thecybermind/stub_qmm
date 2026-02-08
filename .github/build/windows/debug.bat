for %%G in (COD11MP CODMP CODUOMP JAMP JASP JK2MP JK2SP MOHAA MOHBT MOHSH Q2R Q3A QUAKE2 RTCWMP RTCWSP SIN SOF2MP SOF2SP STEF2 STVOYHM STVOYSP WET) do (
    msbuild .\msvc\stub_qmm.vcxproj /p:Configuration=Debug-%%G /p:Platform=x86
	IF %ERRORLEVEL% NEQ 0 (
	exit /b 1
	goto :eof
	)
    msbuild .\msvc\stub_qmm.vcxproj /p:Configuration=Debug-%%G /p:Platform=x64
	IF %ERRORLEVEL% NEQ 0 (
	exit /b 1
	goto :eof
	)
)
