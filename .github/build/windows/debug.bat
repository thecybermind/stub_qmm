for /f %%x in (games.lst) do (
         msbuild .\msvc\stub_qmm.vcxproj /p:Configuration=Debug-%%x /p:Platform=x86
         msbuild .\msvc\stub_qmm.vcxproj /p:Configuration=Debug-%%x /p:Platform=x64
       )

msbuild .\msvc\stub_qmm.vcxproj /p:Configuration=Debug-Q2R /p:Platform=x64
