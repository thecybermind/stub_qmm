for %%x in (COD11MP CODMP CODUOMP JAMP JASP JK2MP JK2SP MOHAA MOHBT MOHSH Q3A QUAKE2 RTCWMP RTCWSP SIN SOF2MP STEF2 STVOYHM STVOYSP WET) do (
         msbuild .\msvc\stub_qmm.vcxproj /p:Configuration=Release-%%x /p:Platform=x86
         msbuild .\msvc\stub_qmm.vcxproj /p:Configuration=Release-%%x /p:Platform=x64
       )

msbuild .\msvc\stub_qmm.vcxproj /p:Configuration=Release-Q2R /p:Platform=x64
