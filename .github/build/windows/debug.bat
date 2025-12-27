for %%x in (COD11MP CODMP CODUOMP JAMP JASP JK2MP JK2SP MOHAA MOHBT MOHSH Q2R Q3A QUAKE2 RTCWMP RTCWSP SIN SOF2MP STEF2 STVOYHM STVOYSP WET) do (
    msbuild .\msvc\stub_qmm.vcxproj /p:Configuration=Debug-%%x /p:Platform=x86
    msbuild .\msvc\stub_qmm.vcxproj /p:Configuration=Debug-%%x /p:Platform=x64
)

rem msbuild .\msvc\stub_qmm.vcxproj /p:Configuration=Debug-Q2R /p:Platform=x64
